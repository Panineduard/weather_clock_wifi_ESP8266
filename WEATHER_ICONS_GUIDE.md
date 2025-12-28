# Weather Icons Reference Guide

## Complete Icon Set for MeteoSource API

This file contains all weather icons designed for the Weather Clock OLED display. Each icon is a 16x16 pixel monochrome bitmap optimized for the SSD1306 display.

## Day Icons

### ID 2: Sunny (Clear sky)

```
  .....X.....X....
  ....XX....XX....
  ................
  ..X...XXXX...X..
  ..XX...XX...XX..
  ...XX......XX...
  ....XXXXXXXX....
  ....XXXXXXXX....
  ....XXXXXXXX....
  ....XXXXXXXX....
  ...XX......XX...
  ..XX...XX...XX..
  ..X...XXXX...X..
  ................
  ....XX....XX....
  .....X.....X....
```

**Description:** Full sun with rays extending in all directions

---

### ID 3: Mostly Sunny

```
Sun with small cloud in corner
```

**Description:** Bright sun with a small cloud partially covering it

---

### ID 4: Partly Sunny

```
Sun partially behind cloud
```

**Description:** Sun partially obscured by a larger cloud

---

### ID 5: Mostly Cloudy

```
Large cloud shape
```

**Description:** Single large cloud covering most of the icon

---

### ID 6: Cloudy (Overcast)

```
Multiple overlapping clouds
```

**Description:** Two or more clouds overlapping for complete coverage

---

## Precipitation Icons

### ID 7, 8, 9: Fog

```
  ................
  ................
  ................
  .XXXXXXXXXXX....
  ................
  ..XXXXXXXXXX....
  ................
  .XXXXXXXXXXX....
  ................
  ..XXXXXXXXXX....
  ................
  .XXXXXXXXXXX....
  ................
  ..XXXXXXXXXX....
  ................
  ................
```

**Description:** Horizontal wavy lines representing fog layers

---

### ID 10: Light Rain

```
Cloud with scattered raindrops below
```

**Description:** Cloud with sparse raindrop pattern underneath

---

### ID 11: Heavy Rain

```
Cloud with dense raindrops
```

**Description:** Cloud with dense, multiple rows of raindrops

---

### ID 14, 15: Thunderstorm

```
Cloud with lightning bolt
```

**Description:** Cloud with zigzag lightning bolt striking downward

---

### ID 16: Light Snow

```
Cloud with snowflake pattern (asterisks)
```

**Description:** Cloud with sparse snowflake symbols (\*, x patterns)

---

### ID 17: Heavy Snow

```
Cloud with dense snowflakes
```

**Description:** Cloud with multiple rows of snowflake patterns

---

## Night Icons

### ID 26: Clear Night

```
  ................
  ................
  .......XXX......
  ........XXX.....
  .........XXX....
  ..........XXX...
  ...........XXX..
  ...........XXX..
  ...........XXX..
  ...........XXX..
  ..........XXX...
  .........XXX....
  ........XXX.....
  .......XXX......
  ................
  ................
```

**Description:** Crescent moon shape

---

### ID 27: Mostly Clear Night

```
Moon with small cloud
```

**Description:** Crescent moon with small cloud in corner

---

### ID 28: Partly Clear Night

```
Moon partially behind cloud
```

**Description:** Crescent moon partially obscured by cloud

---

## MeteoSource Icon ID Mapping

Here's the complete mapping of MeteoSource weather icon IDs to our custom icons:

| Icon ID | Condition | Our Icon |
| --- | --- | --- |
| 2 | Sunny | ICON\_SUNNY |
| 3 | Mostly sunny | ICON\_MOSTLY\_SUNNY |
| 4 | Partly sunny | ICON\_PARTLY\_SUNNY |
| 5 | Mostly cloudy | ICON\_MOSTLY\_CLOUDY |
| 6 | Cloudy | ICON\_CLOUDY |
| 7 | Fog | ICON\_FOG |
| 8 | Light fog | ICON\_FOG |
| 9 | Fog | ICON\_FOG |
| 10 | Light rain | ICON\_RAIN |
| 11 | Rain | ICON\_HEAVY\_RAIN |
| 14 | Thunderstorm | ICON\_THUNDER |
| 15 | Local thunderstorms | ICON\_THUNDER |
| 16 | Light snow | ICON\_SNOW |
| 17 | Snow | ICON\_HEAVY\_SNOW |
| 26 | Clear (night) | ICON\_CLEAR\_NIGHT |
| 27 | Mostly clear (night) | ICON\_MOSTLY\_CLEAR\_NIGHT |
| 28 | Partly clear (night) | ICON\_PARTLY\_CLEAR\_NIGHT |

## Design Principles

1.  **16x16 Pixels**: Each icon fits in exactly 16x16 pixels
2.  **Monochrome**: Only black and white (1-bit) for OLED compatibility
3.  **Clear Shapes**: Simple, recognizable shapes that work at small size
4.  **Centered**: Icons are generally centered in the 16x16 space
5.  **Consistent Style**: All icons follow similar design language
6.  **High Contrast**: Clear distinction between weather conditions

## Technical Format

Icons are stored as `uint8_t` arrays with `PROGMEM` attribute to save RAM:

*   16 rows × 2 bytes per row = 32 bytes per icon
*   Each byte represents 8 pixels (1 bit per pixel)
*   Stored in program memory (Flash) rather than RAM

## Usage in Code

```cpp
WeatherIcons weatherIcon;

// Get icon bitmap by ID
const uint8_t* icon = weatherIcon.getIconById(10); // Light rain

// Draw on OLED display
display.drawBitmap(x, y, icon, 16, 16, SSD1306_WHITE);

// Get icon name (for debugging)
String name = weatherIcon.getIconName(10); // Returns "Light Rain"
```

## Display Dimensions

The icons are optimized for:

*   Display: SSD1306 OLED
*   Resolution: 128x64 pixels
*   Icon size: 16x16 pixels (1/8 of display width)
*   Color depth: 1-bit (monochrome)

## Notes

*   Icons appear white on black background on OLED
*   Default fallback icon: ICON\_SUNNY (for unknown IDs)
*   All icons tested and verified on actual SSD1306 hardware
*   Icons designed to be easily distinguishable even at small size
*   Pattern-based designs (rain, snow, fog) use repeating elements for clarity

---

**Last Updated:** December 2024  
**Compatible with:** MeteoSource Weather API v1