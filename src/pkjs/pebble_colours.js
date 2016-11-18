//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

var PBL_64_COLOURS = {
  GColorClear: 0x000000,
  GColorBlack: 0x000000,
  GColorWhite: 0xFFFFFF,
  GColorLightGray: 0xAAAAAA,
  GColorDarkGray: 0x555555,
  GColorPastelYellow: 0xFFFFAA,
  GColorIcterine: 0xFFFF55,
  GColorRajah: 0xFFAA55,
  GColorOrange: 0xFF5500,
  GColorRed: 0xFF0000,
  GColorFolly: 0xFF0055,
  GColorSunsetOrange: 0xFF5555,
  GColorMelon: 0xFFAAAA,
  GColorYellow: 0xFFFF00,
  GColorChromeYellow: 0xFFAA00,
  GColorWindsorTan: 0xAA5500,
  GColorRoseVale: 0xAA5555,
  GColorDarkCandyAppleRed: 0xAA0000,
  GColorFashionMagenta: 0xFF00AA,
  GColorBrilliantRose: 0xFF55AA,
  GColorRichBrilliantLavender: 0xFFAAFF,
  GColorBulgarianRose: 0x550000,
  GColorJazzberryJam: 0xAA0055,
  GColorMagenta: 0xFF00FF,
  GColorShockingPink: 0xFF55FF,
  GColorImperialPurple: 0x550055,
  GColorPurple: 0xAA00AA,
  GColorPurpureus: 0xAA55AA,
  GColorOxfordBlue: 0x000055,
  GColorIndigo: 0x5500AA,
  GColorVividViolet: 0xAA00FF,
  GColorLavenderIndigo: 0xAA55FF,
  GColorDukeBlue: 0x0000AA,
  GColorElectricUltramarine: 0x5500FF,
  GColorLiberty: 0x5555AA,
  GColorCobaltBlue: 0x0055AA,
  GColorCadetBlue: 0x55AAAA,
  GColorMayGreen: 0x55AA55,
  GColorIslamicGreen: 0x00AA00,
  GColorGreen: 0x00FF00,
  GColorBrightGreen: 0x55FF00,
  GColorInchworm: 0xAAFF55,
  GColorSpringBud: 0xAAFF00,
  GColorKellyGreen: 0x55AA00,
  GColorDarkGreen: 0x005500,
  GColorMidnightGreen: 0x005555,
  GColorBrass: 0xAAAA55,
  GColorArmyGreen: 0x555500,
  GColorLimerick: 0xAAAA00,
  GColorMintGreen: 0xAAFFAA,
  GColorScreaminGreen: 0x55FF55,
  GColorMalachite: 0x00FF55,
  GColorJaegerGreen: 0x00AA55,
  GColorTiffanyBlue: 0x00AAAA,
  GColorVividCerulean: 0x00AAFF,
  GColorBlue: 0x0000FF,
  GColorVeryLightBlue: 0x5555FF,
  GColorBabyBlueEyes: 0xAAAAFF,
  GColorMediumAquamarine: 0x55FFAA,
  GColorMediumSpringGreen: 0x00FFAA,
  GColorCyan: 0x00FFFF,
  GColorPictonBlue: 0x55AAFF,
  GColorBlueMoon: 0x0055FF,
  GColorElectricBlue: 0x55FFFF,
  GColorCeleste: 0xAAFFFF
};
Object.freeze( PBL_64_COLOURS );

var getWeatherIconIDFromID = function( id ) {
  switch( id ) {
    case 200: 
    case 201:
    case 202:
    case 210:
    case 211:
    case 212:
    case 221:
    case 230:
    case 231:
    case 232: return 11;
    case 300:
    case 301:
    case 302:
    case 310:
    case 311:
    case 312:
    case 313:
    case 314:
    case 321: return 9;
    case 500:
    case 501:
    case 502:
    case 503:
    case 504: return 10;
    case 511: return 13;
    case 520:
    case 521:
    case 522:
    case 531: return 9;
    case 600: 
    case 601:
    case 602:
    case 611:
    case 612:
    case 615:
    case 616:
    case 620:
    case 621:
    case 622: return 13;
    case 701: return 50;
    case 711: return 711;
    case 721: return 50;
    case 731: return 50;
    case 741: return 50;
    case 751: return 751;
    case 761: return 761;
    case 762: return 762;
    case 771: return 50;
    case 781: return 50;
    case 800: return 1;
    case 801: return 2;
    case 802: return 3;
    case 803: return 4;
    case 804: return 4;
    case 900: return 0;
    case 901: return 0;
    case 902: return 0;
    case 903: return 0;
    case 904: return 0;
    case 905: return 905;
    case 906: return 906;
    case 951: return 0;
    case 952: return 0;
    default: return 0;
  }
};

module.exports.PBL_64_COLOURS = PBL_64_COLOURS;
module.exports.getWeatherIconIDFromID = getWeatherIconIDFromID;
