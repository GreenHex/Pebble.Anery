//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

var Clay = require( 'pebble-clay' );
var clayConfig = require( './config' );
var clayManipulator = require( './config_manipulator' );
var clay = new Clay( clayConfig, clayManipulator, { autoHandleEvents: false } );
var MESSAGE_KEYS = require( 'message_keys' );
var COLOURS = require( './pebble_colours' );

var DEBUG = 0;

var REQUEST_TYPES = {
  REQUEST_UNDEFINED : 0,
  REQUEST_CONFIG : 1,
  REQUEST_WEATHER : 2
};
Object.freeze( REQUEST_TYPES );

// clay should be able to give these, but settings are stored locally to enable sending configuration settings on call.
// But need to hook into clay, to get rid of all this.
var local_config_settings = [
  // clock
  MESSAGE_KEYS.ANALOG_HANDS_STYLE,
  MESSAGE_KEYS.ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS,
  MESSAGE_KEYS.ANALOG_SHOW_SECONDS_TICKS,
  MESSAGE_KEYS.SHOW_DATE,
  MESSAGE_KEYS.SHOW_BATTERY_GAUGE,
  MESSAGE_KEYS.SHOW_HEALTH,
  // chime
  MESSAGE_KEYS.CHIME_INTERVAL,
  MESSAGE_KEYS.CHIME_START_TIME,
  MESSAGE_KEYS.CHIME_END_TIME,
  MESSAGE_KEYS.CHIME_ON_DAYS, MESSAGE_KEYS.CHIME_ON_DAYS + 1, MESSAGE_KEYS.CHIME_ON_DAYS + 2,
  MESSAGE_KEYS.CHIME_ON_DAYS + 3, MESSAGE_KEYS.CHIME_ON_DAYS + 4, MESSAGE_KEYS.CHIME_ON_DAYS + 5,
  MESSAGE_KEYS.CHIME_ON_DAYS + 6,
  MESSAGE_KEYS.CHIME_OFFSET,
  // weather
  MESSAGE_KEYS.SHOW_WEATHER,
  MESSAGE_KEYS.WEATHER_UPDATE_INTERVAL,
  MESSAGE_KEYS.WEATHER_UPDATE_START_TIME, 
  MESSAGE_KEYS.WEATHER_UPDATE_END_TIME,
  MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS, MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS + 1, MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS + 2,
  MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS + 3, MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS + 4, MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS + 5,
  MESSAGE_KEYS.WEATHER_UPDATE_ON_DAYS + 6,
  // misc
  MESSAGE_KEYS.WEATHER_TEMPERATURE_UNITS,
  MESSAGE_KEYS.WEATHER_OWM_API_KEY
];


var xhrRequest = function ( url, type, callback ) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback( this.responseText );
  };
  xhr.open( type, url );
  xhr.send();
};

function locationSuccess( pos ) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + localStorage.getItem( MESSAGE_KEYS.WEATHER_OWM_API_KEY );

  if (DEBUG) console.log( "index.js: locationSuccess(): " + url );

  // Send request to OpenWeatherMap
  xhrRequest( url, 'GET', 
             function( responseText ) {

               var json;

               try {
                 json = JSON.parse( responseText );
               } catch ( err ) {
                 if (DEBUG) console.log( 'index.js: locationSuccess(): Error parsing responseText, invalid JSON data.' );
                 return;
               }

               if (DEBUG) console.log( "index.js: locationSuccess(): " + JSON.stringify(json) );

               var weather_temperature = "";
               if ( json.cod == 200 ) { // success

                 weather_temperature = [  Math.round( json.main.temp - 273.15 ) + "°C",
                            Math.round( json.main.temp * 9/5 - 459.67 ) + "°F",
                            Math.round( json.main.temp ) + " K"
                           ][ localStorage.getItem( MESSAGE_KEYS.WEATHER_TEMPERATURE_UNITS ) ];

               } else { // error
                 if (DEBUG) console.log( 'index.js: locationSuccess(): XMLHttpRequest returned error: ' + json.cod + ": " + json.message );
                 weather_temperature  = json.cod + ": " + json.message;
               }

               var dictionary = {
                 "WEATHER_TEMPERATURE_TXT": weather_temperature,
                 "WEATHER_ICON_ID": COLOURS.getWeatherIconIDFromID( json.weather[0].id )
               };
               
               if (DEBUG) console.log( "index.js: locationSuccess(): AFTER DICTIONARY" + JSON.stringify( dictionary ) );
               sendDictionaryToPebble( dictionary );
             });
}

function locationError( err ) {
  if (DEBUG) console.log( "index.js: Error requesting location!" );
}

function getWeather() {
  if (DEBUG) console.log( "index.js: getWeather()." );
  if ( !localStorage.getItem( MESSAGE_KEYS.WEATHER_OWM_API_KEY ) ) return;

  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

function sendDictionaryToPebble( dictionary ) {
  Pebble.sendAppMessage( dictionary,
                        function(e) {
                          if (DEBUG) console.log( "index.js: sendDictionaryToPebble(): Message sent to Pebble successfully. " + JSON.stringify( dictionary ) );
                        },
                        function(e) {
                          if (DEBUG) console.log( "index.js: sendDictionaryToPebble(): Error sending message to Pebble. " + JSON.stringify( e ) );
                        });
}

/// Send configuration data
function sendConfig() {
  var dictionary = {};
  local_config_settings.map( function ( item ) {
    dictionary[item] = localStorage.getItem( item );
    if (DEBUG) console.log( "index.js: sendConfig(): " + item + ": " + dictionary[item] );
  });
  sendDictionaryToPebble( dictionary );
}

// Listen for when the watchface is opened
Pebble.addEventListener( 'ready', 
                        function(e) {
                          if (DEBUG) console.log( "index.js: addEventListener( ready ): PebbleKit JS ready." );
                          sendConfig();
                        });

// Listen for when an AppMessage is received
Pebble.addEventListener( 'appmessage',
                        function(e) {
                          if (DEBUG) console.log( "index.js: addEventListener( appmessage ): AppMessage received: " + JSON.stringify( e.payload ) );
                          var dict = e.payload;
                          if( dict.REQUEST ) {
                            [ sendConfig, getWeather ][ dict.REQUEST - 1 ]();
                          }
                        });

/////// Pebble/clay stuff
Pebble.addEventListener( 'showConfiguration', function( e ) {
  Pebble.openURL( clay.generateUrl() );
});

Pebble.addEventListener( 'webviewclosed', function( e ) {
  if ( e && !e.response ) {
    return;
  }
  var dictionary = clay.getSettings( e.response );
  // if (DEBUG) console.log( "index.js/clay: " + JSON.stringify( dictionary ) );
  // save for later...
  local_config_settings.map( function( item ) {
    localStorage.setItem( item, dictionary[ item ] );
    if (DEBUG) console.log( "index.js/clay: " + item + " " + localStorage.getItem( item ) );
  });
  sendConfig();
});
