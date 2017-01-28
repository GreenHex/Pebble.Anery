//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

module.exports = function( minified ) {
  var clayConfig = this;
  //  var _ = minified._;
  //  var $ = minified.$;
  //  var HTML = minified.HTML;

  function show( showItems ) {
    showItems.map( function( item ) {
      clayConfig.getItemByMessageKey( item ).show();
    });
  }

  function hide( hideItems ) {
    hideItems.map( function( item ) {
      clayConfig.getItemByMessageKey( item ).hide();
    });
  }
  
  var toggle_chime_settings_visibility = function() {
    var showItems;
    var hideItems;
    
    if ( this.get() == '0' ) { // no chime
      showItems = [ ];
      hideItems = [ 'CHIME_START_TIME', 'CHIME_END_TIME', 'CHIME_ON_DAYS', 'CHIME_OFFSET' ];
    } else { 
      showItems = [ 'CHIME_START_TIME', 'CHIME_END_TIME', 'CHIME_ON_DAYS', 'CHIME_OFFSET' ];
      hideItems = [ ];
    }
    show( showItems );
    hide( hideItems );
  };
  
  var toggle_weather_visibility = function() {
    var showItems;
    var hideItems;
    
    if ( this.get() == '1' ) { // weather
      showItems = [ 'WEATHER_TEMPERATURE_UNITS', 'WEATHER_OWM_API_KEY', 'WEATHER_UPDATE_INTERVAL', 'WEATHER_UPDATE_START_TIME', 'WEATHER_UPDATE_END_TIME', 'WEATHER_UPDATE_ON_DAYS' ];
      hideItems = [ ];
    } else { // nothing
      showItems = [ ];
      hideItems = [ 'WEATHER_TEMPERATURE_UNITS', 'WEATHER_OWM_API_KEY', 'WEATHER_UPDATE_INTERVAL', 'WEATHER_UPDATE_START_TIME', 'WEATHER_UPDATE_END_TIME', 'WEATHER_UPDATE_ON_DAYS' ]; 
    }
    show( showItems );
    hide( hideItems );
  };
  
  var toggle_battery_gauge_settings_visibility = function() {
    var showItems;
    var hideItems;
    
    if ( this.get() == '2' ) { // weather
      showItems = [ ];
      hideItems = [ 'ANALOG_SHOW_SECONDS_TICKS', 'SHOW_BATTERY_GAUGE' ];
    } else { // nothing
      showItems = [ 'ANALOG_SHOW_SECONDS_TICKS', 'SHOW_BATTERY_GAUGE' ];
      hideItems = [ ]; 
    }
    show( showItems );
    hide( hideItems );
  };

  clayConfig.on( clayConfig.EVENTS.AFTER_BUILD, function() {
    var chimeSettings = clayConfig.getItemByMessageKey( 'CHIME_INTERVAL' );
    toggle_chime_settings_visibility.call( chimeSettings );
    chimeSettings.on( 'change', toggle_chime_settings_visibility );

    var showWeather = clayConfig.getItemByMessageKey( 'SHOW_WEATHER' );
    toggle_weather_visibility.call( showWeather );
    showWeather.on( 'change', toggle_weather_visibility );
    
    var showBatteryGauge = clayConfig.getItemByMessageKey( 'ANALOG_HANDS_STYLE' );
    toggle_battery_gauge_settings_visibility.call( showBatteryGauge );
    showBatteryGauge.on( 'change', toggle_battery_gauge_settings_visibility );
    
  });
};
