#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <Arduino.h>

// 16x16 bitmaps (monochrome) for SSD1306 OLED Display
// Each icon is designed to be clear and recognizable at small size

// ============ DAY ICONS ============

// SUNNY (Clear sky - day) - ID: 2
// Sun with rays
const uint8_t ICON_SUNNY[] PROGMEM = {
  0x04, 0x20,  // Row 1:    .....X.....X....
  0x0C, 0x30,  // Row 2:    ....XX....XX....
  0x00, 0x00,  // Row 3:    ................
  0x23, 0xC4,  // Row 4:    ..X...XXXX...X..
  0x31, 0x8C,  // Row 5:    ..XX...XX...XX..
  0x18, 0x18,  // Row 6:    ...XX......XX...
  0x0F, 0xF0,  // Row 7:    ....XXXXXXXX....
  0x0F, 0xF0,  // Row 8:    ....XXXXXXXX....
  0x0F, 0xF0,  // Row 9:    ....XXXXXXXX....
  0x0F, 0xF0,  // Row 10:   ....XXXXXXXX....
  0x18, 0x18,  // Row 11:   ...XX......XX...
  0x31, 0x8C,  // Row 12:   ..XX...XX...XX..
  0x23, 0xC4,  // Row 13:   ..X...XXXX...X..
  0x00, 0x00,  // Row 14:   ................
  0x0C, 0x30,  // Row 15:   ....XX....XX....
  0x04, 0x20   // Row 16:   .....X.....X....
};

// MOSTLY SUNNY - ID: 3
// Sun with small cloud
const uint8_t ICON_MOSTLY_SUNNY[] PROGMEM = {
  0x04, 0x20,  // Row 1
  0x0C, 0x30,  // Row 2
  0x00, 0x00,  // Row 3
  0x23, 0xC4,  // Row 4
  0x31, 0x8C,  // Row 5
  0x18, 0x38,  // Row 6
  0x0F, 0x7C,  // Row 7
  0x0F, 0xFE,  // Row 8
  0x0F, 0xFE,  // Row 9
  0x1F, 0xFE,  // Row 10
  0x1F, 0xFC,  // Row 11
  0x00, 0x00,  // Row 12
  0x00, 0x00,  // Row 13
  0x00, 0x00,  // Row 14
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// PARTLY SUNNY - ID: 4
// Sun partially behind cloud
const uint8_t ICON_PARTLY_SUNNY[] PROGMEM = {
  0x04, 0x00,  // Row 1
  0x0C, 0x00,  // Row 2
  0x00, 0x00,  // Row 3
  0x23, 0x80,  // Row 4
  0x31, 0xE0,  // Row 5
  0x18, 0x70,  // Row 6
  0x0F, 0xF8,  // Row 7
  0x07, 0xFC,  // Row 8
  0x1F, 0xFE,  // Row 9
  0x3F, 0xFE,  // Row 10
  0x3F, 0xFE,  // Row 11
  0x3F, 0xFC,  // Row 12
  0x1F, 0xF8,  // Row 13
  0x00, 0x00,  // Row 14
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// MOSTLY CLOUDY - ID: 5
// Large cloud
const uint8_t ICON_MOSTLY_CLOUDY[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x00, 0x00,  // Row 2
  0x03, 0x80,  // Row 3
  0x07, 0xE0,  // Row 4
  0x0E, 0x70,  // Row 5
  0x1C, 0x38,  // Row 6
  0x38, 0x1C,  // Row 7
  0x70, 0x0E,  // Row 8
  0x70, 0x0E,  // Row 9
  0x78, 0x1E,  // Row 10
  0x7F, 0xFE,  // Row 11
  0x7F, 0xFE,  // Row 12
  0x3F, 0xFC,  // Row 13
  0x00, 0x00,  // Row 14
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// CLOUDY (Overcast) - ID: 6
// Multiple clouds
const uint8_t ICON_CLOUDY[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x07, 0x00,  // Row 2
  0x0F, 0xC0,  // Row 3
  0x1C, 0xE0,  // Row 4
  0x38, 0x70,  // Row 5
  0x73, 0xB8,  // Row 6
  0x67, 0xDC,  // Row 7
  0xEE, 0x0E,  // Row 8
  0xEE, 0x0E,  // Row 9
  0xFF, 0x1E,  // Row 10
  0xFF, 0xFE,  // Row 11
  0x7F, 0xFE,  // Row 12
  0x3F, 0xFC,  // Row 13
  0x00, 0x00,  // Row 14
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// RAIN (Light rain) - ID: 10
// Cloud with raindrops
const uint8_t ICON_RAIN[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x03, 0x80,  // Row 2
  0x07, 0xE0,  // Row 3
  0x0E, 0x70,  // Row 4
  0x1C, 0x38,  // Row 5
  0x3F, 0xFC,  // Row 6
  0x3F, 0xFC,  // Row 7
  0x1F, 0xF8,  // Row 8
  0x00, 0x00,  // Row 9
  0x12, 0x48,  // Row 10:  .X..X..X..X.....
  0x24, 0x90,  // Row 11:  ..X..X..X..X....
  0x09, 0x20,  // Row 12:  ....X..X..X.....
  0x12, 0x48,  // Row 13:  .X..X..X..X.....
  0x24, 0x90,  // Row 14:  ..X..X..X..X....
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// HEAVY RAIN - ID: 11
// Cloud with heavy raindrops
const uint8_t ICON_HEAVY_RAIN[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x03, 0x80,  // Row 2
  0x07, 0xE0,  // Row 3
  0x0E, 0x70,  // Row 4
  0x1C, 0x38,  // Row 5
  0x3F, 0xFC,  // Row 6
  0x3F, 0xFC,  // Row 7
  0x1F, 0xF8,  // Row 8
  0x12, 0x48,  // Row 9
  0x24, 0x90,  // Row 10
  0x49, 0x24,  // Row 11
  0x92, 0x48,  // Row 12
  0x24, 0x90,  // Row 13
  0x49, 0x24,  // Row 14
  0x12, 0x48,  // Row 15
  0x00, 0x00   // Row 16
};

// THUNDERSTORM - ID: 14, 15
// Cloud with lightning bolt
const uint8_t ICON_THUNDER[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x03, 0x80,  // Row 2
  0x07, 0xE0,  // Row 3
  0x0E, 0x70,  // Row 4
  0x1C, 0x38,  // Row 5
  0x3F, 0xFC,  // Row 6
  0x3F, 0xFC,  // Row 7
  0x1F, 0xF8,  // Row 8
  0x01, 0xE0,  // Row 9:    .......XXX......
  0x03, 0x80,  // Row 10:   ......XXX.......
  0x07, 0xF0,  // Row 11:   .....XXXXXXX....
  0x00, 0xE0,  // Row 12:   .........XXX....
  0x01, 0xC0,  // Row 13:   ........XXX.....
  0x03, 0x80,  // Row 14:   ......XXX.......
  0x02, 0x00,  // Row 15:   ......X.........
  0x00, 0x00   // Row 16
};

// SNOW (Light snow) - ID: 16
// Cloud with snowflakes
const uint8_t ICON_SNOW[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x03, 0x80,  // Row 2
  0x07, 0xE0,  // Row 3
  0x0E, 0x70,  // Row 4
  0x1C, 0x38,  // Row 5
  0x3F, 0xFC,  // Row 6
  0x3F, 0xFC,  // Row 7
  0x1F, 0xF8,  // Row 8
  0x00, 0x00,  // Row 9
  0x04, 0xA0,  // Row 10:  .....X..X.X.....
  0x11, 0x10,  // Row 11:  ...X...X...X....
  0x0A, 0xA0,  // Row 12:  ....X.X.X.X.....
  0x11, 0x10,  // Row 13:  ...X...X...X....
  0x04, 0xA0,  // Row 14:  .....X..X.X.....
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// HEAVY SNOW - ID: 17
// Cloud with heavy snowflakes
const uint8_t ICON_HEAVY_SNOW[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x03, 0x80,  // Row 2
  0x07, 0xE0,  // Row 3
  0x0E, 0x70,  // Row 4
  0x1C, 0x38,  // Row 5
  0x3F, 0xFC,  // Row 6
  0x3F, 0xFC,  // Row 7
  0x1F, 0xF8,  // Row 8
  0x04, 0xA0,  // Row 9
  0x11, 0x10,  // Row 10
  0x0A, 0xA0,  // Row 11
  0x11, 0x10,  // Row 12
  0x04, 0xA0,  // Row 13
  0x11, 0x10,  // Row 14
  0x0A, 0xA0,  // Row 15
  0x00, 0x00   // Row 16
};

// ============ NIGHT ICONS ============

// CLEAR NIGHT - ID: 26
// Crescent moon
const uint8_t ICON_CLEAR_NIGHT[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x00, 0x00,  // Row 2
  0x01, 0xC0,  // Row 3:    .......XXX......
  0x00, 0xE0,  // Row 4:    ........XXX.....
  0x00, 0x70,  // Row 5:    .........XXX....
  0x00, 0x38,  // Row 6:    ..........XXX...
  0x00, 0x1C,  // Row 7:    ...........XXX..
  0x00, 0x1C,  // Row 8:    ...........XXX..
  0x00, 0x1C,  // Row 9:    ...........XXX..
  0x00, 0x1C,  // Row 10:   ...........XXX..
  0x00, 0x38,  // Row 11:   ..........XXX...
  0x00, 0x70,  // Row 12:   .........XXX....
  0x00, 0xE0,  // Row 13:   ........XXX.....
  0x01, 0xC0,  // Row 14:   .......XXX......
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// MOSTLY CLEAR NIGHT - ID: 27
// Moon with small cloud
const uint8_t ICON_MOSTLY_CLEAR_NIGHT[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x01, 0xC0,  // Row 2
  0x00, 0xE0,  // Row 3
  0x00, 0x70,  // Row 4
  0x00, 0x38,  // Row 5
  0x00, 0x3C,  // Row 6
  0x00, 0x7E,  // Row 7
  0x00, 0xFE,  // Row 8
  0x01, 0xFE,  // Row 9
  0x01, 0xFC,  // Row 10
  0x00, 0x00,  // Row 11
  0x00, 0x00,  // Row 12
  0x00, 0x00,  // Row 13
  0x00, 0x00,  // Row 14
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// PARTLY CLEAR NIGHT - ID: 28
// Moon partially behind cloud
const uint8_t ICON_PARTLY_CLEAR_NIGHT[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x01, 0xC0,  // Row 2
  0x00, 0xE0,  // Row 3
  0x00, 0x70,  // Row 4
  0x00, 0xB8,  // Row 5
  0x01, 0xFC,  // Row 6
  0x03, 0xFE,  // Row 7
  0x07, 0xFE,  // Row 8
  0x0F, 0xFE,  // Row 9
  0x0F, 0xFC,  // Row 10
  0x0F, 0xF8,  // Row 11
  0x07, 0xF0,  // Row 12
  0x00, 0x00,  // Row 13
  0x00, 0x00,  // Row 14
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

// FOG - ID: 7, 8, 9
// Horizontal lines representing fog
const uint8_t ICON_FOG[] PROGMEM = {
  0x00, 0x00,  // Row 1
  0x00, 0x00,  // Row 2
  0x00, 0x00,  // Row 3
  0x7F, 0xFC,  // Row 4:    .XXXXXXXXXXX....
  0x00, 0x00,  // Row 5
  0x3F, 0xF8,  // Row 6:    ..XXXXXXXXXX....
  0x00, 0x00,  // Row 7
  0x7F, 0xFC,  // Row 8:    .XXXXXXXXXXX....
  0x00, 0x00,  // Row 9
  0x3F, 0xF8,  // Row 10:   ..XXXXXXXXXX....
  0x00, 0x00,  // Row 11
  0x7F, 0xFC,  // Row 12:   .XXXXXXXXXXX....
  0x00, 0x00,  // Row 13
  0x3F, 0xF8,  // Row 14:   ..XXXXXXXXXX....
  0x00, 0x00,  // Row 15
  0x00, 0x00   // Row 16
};

class WeatherIcons {
public:
  const uint8_t* getIconById(uint8_t id) {
    switch (id) {
      // Day icons
      case 2:  return ICON_SUNNY;
      case 3:  return ICON_MOSTLY_SUNNY;
      case 4:  return ICON_PARTLY_SUNNY;
      case 5:  return ICON_MOSTLY_CLOUDY;
      case 6:  return ICON_CLOUDY;
      
      // Fog
      case 7:
      case 8:
      case 9:  return ICON_FOG;
      
      // Rain
      case 10: return ICON_RAIN;
      case 11: return ICON_HEAVY_RAIN;
      
      // Thunderstorm
      case 14:
      case 15: return ICON_THUNDER;
      
      // Snow
      case 16: return ICON_SNOW;
      case 17: return ICON_HEAVY_SNOW;
      
      // Night icons
      case 26: return ICON_CLEAR_NIGHT;
      case 27: return ICON_MOSTLY_CLEAR_NIGHT;
      case 28: return ICON_PARTLY_CLEAR_NIGHT;
      
      // Default fallback
      default: return ICON_SUNNY;
    }
  }
  
  // Helper function to get icon name (useful for debugging)
  String getIconName(uint8_t id) {
    switch (id) {
      case 2:  return "Sunny";
      case 3:  return "Mostly Sunny";
      case 4:  return "Partly Sunny";
      case 5:  return "Mostly Cloudy";
      case 6:  return "Cloudy";
      case 7:
      case 8:
      case 9:  return "Fog";
      case 10: return "Light Rain";
      case 11: return "Heavy Rain";
      case 14:
      case 15: return "Thunderstorm";
      case 16: return "Light Snow";
      case 17: return "Heavy Snow";
      case 26: return "Clear Night";
      case 27: return "Mostly Clear Night";
      case 28: return "Partly Clear Night";
      default: return "Unknown";
    }
  }
};

#endif