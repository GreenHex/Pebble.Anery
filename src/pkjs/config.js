//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

module.exports = [
  {
    "type": "heading",
    "defaultValue": "Anery"
  },
  {
    "type": "text",
    "defaultValue": 'Configure your watch app settings. Weather icons are from <a href="http://forecastfont.iconvau.lt">Forecast Font.</a>'
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Watch Settings"
      },
      {
        "type": "select",
        "messageKey": "ANALOG_HANDS_STYLE",
        "defaultValue": '0',
        "label": "Hands Style",
        "options": [
          { 
            "label": "Contemporary",
            "value": '0'
          },
          { 
            "label": "Spiffy GS",
            "value": '1'
          },
          { 
            "label": "SBGE001",
            "value": '2'
          }
        ]
      },
      {
        "type": "slider",
        "messageKey": "ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS",
        "defaultValue": '20',
        "label": "Seconds Display (seconds)",
        "description": "Number of seconds to display seconds hand after activation by tapping or shaking watch. Set value to '0' to disable.",
        "min": '0',
        "max": '60',
        "step": '5'
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_DATE",
        "label": "Show Date",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_BATTERY_GAUGE",
        "label": "Show Battery Status",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "capabilities": [ "HEALTH" ],
        "messageKey": "SHOW_HEALTH",
        "label": "Show Health Information",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Chime settings"
      },
      {
        "type": "select",
        "messageKey": "CHIME_INTERVAL",
        "defaultValue": '0',
        "label": "Chime Interval",
        "options": [
          { 
            "label": "Off", 
            "value": '0'
          },
          { 
            "label": "Every Hour",
            "value": '1'
          },
          { 
            "label": "Every Half Hour",
            "value": '2'
          },
          { 
            "label": "Every Twenty Minutes",
            "value": '3'
          },
          { 
            "label": "Every Quarter Hour",
            "value": '4'
          },
          { 
            "label": "Every Ten Minutes",
            "value": '6'
          },
          { 
            "label": "Every Five Minutes",
            "value": '12'
          },
          { 
            "label": "Every Three Minutes",
            "value": '20'
          },
          { 
            "label": "Every Two Minutes",
            "value": '30'
          },
          { 
            "label": "Every Minute",
            "value": '60'
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "CHIME_START_TIME",
        "defaultValue": '0',
        "label": "Chime Start Time",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "select",
        "messageKey": "CHIME_END_TIME",
        "defaultValue": '23',
        "label": "Chime End Time",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "checkboxgroup",
        "messageKey": "CHIME_ON_DAYS",
        "label": "Chime On Days",
        "defaultValue": [ true, true, true, true, true, true, true ],
        "options": [ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" ]
      },
      {
        "type": "slider",
        "messageKey": "CHIME_OFFSET",
        "defaultValue": '0',
        "label": "Chime Offset (minutes)",
        "description": "Number of minutes earlier to chime.",
        "min": '0',
        "max": '5',
        "step": '1'
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "WEATHER DISPLAY"
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_WEATHER",
        "label": "Show Weather",
        "defaultValue": false
      },
      {
        "type": "input",
        "messageKey": "WEATHER_OWM_API_KEY",
        "defaultValue": "",
        "label": "OpenWeatherMap API Key",
        "description": 'Weather forecast on the Pebble will not work without a valid API key. You can get your free API key from <a href="http://openweathermap.org">openweathermap.org.</a>',  
        "attributes": {
          "placeholder": "eg: a64e1f53a22fcccc25458ea5e0b2daeb",
          "limit": '32',
          "type": "text"
        }
      },
      {
        "type": "select",
        "messageKey": "WEATHER_TEMPERATURE_UNITS",
        "defaultValue": '0',
        "label": "Temperature Units",
        "options": [
          { 
          "label": "Celsius", 
          "value": '0'
          },
          { 
          "label": "Fahrenheit",
          "value": '1'
          },
          { 
          "label": "Kelvin",
          "value": '2'
          }
        ]
      },
      {
        "type": "slider",
        "messageKey": "WEATHER_UPDATE_INTERVAL",
        "defaultValue": '30',
        "label": "Update Interval (minutes)",
        "description": "Time interval between weather updates.",
        "min": '10',
        "max": '60',
        "step": '5'
      },
      {
        "type": "select",
        "messageKey": "WEATHER_UPDATE_START_TIME",
        "defaultValue": '0',
        "label": "Start Updates At",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "select",
        "messageKey": "WEATHER_UPDATE_END_TIME",
        "defaultValue": '23',
        "label": "Stop Updates At",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "checkboxgroup",
        "messageKey": "WEATHER_UPDATE_ON_DAYS",
        "label": "Update On Days",
        "defaultValue": [ true, true, true, true, true, true, true ],
        "options": [ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" ]
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings and Exit"
  }
];
