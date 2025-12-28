# Display Layout Reference

## OLED Display Specifications

*   **Resolution**: 128x64 pixels
*   **Driver**: SSD1306
*   **Interface**: I2C
*   **Text Size 1**: 6x8 pixels per character
*   **Text Size 2**: 12x16 pixels per character

## Display Mode Layouts

### 1\. Time Mode Layout (20 seconds)

```
Pixel Layout (128x64):
┌────────────────────────────────────────────────┐  Y=0
│            Sat, 27 Dec 2024                    │  (Text size 1, centered)
│                                                │  Y=8
│                                                │
│                                                │  Y=16
│                                                │
│                14:35:42                        │  Y=25 (Text size 2, centered)
│                                                │
│                                                │  Y=41
│                                                │
│ ───────────────────────────────────────────── │  Y=50 (Line)
│                                         18.5C  │  Y=54 (Text size 1, right aligned)
└────────────────────────────────────────────────┘  Y=64

Element Positions:
- Date text: Y=5, centered, size=1
- Time text: Y=25, centered, size=2
- Separator line: Y=50, X=20 to X=108
- Temperature: Y=54, right-aligned (X=128-width-2), size=1
```

### 2\. Weather Mode Layout (10 seconds)

```
Pixel Layout (128x64):
┌────────────────────────────────────────────────┐  Y=0
│ Sat, 27 Dec 2024                         18.5C│  (Date left, Temp right)
│ ───────────────────────────────────────────── │  Y=10 (Line)
│                                                │
│                                                │  Y=15
│    ☀️         ⛅         🌧️                    │  (Icons at Y=15, 16x16px)
│                                                │
│                                                │  Y=31
│   15/22      14/20      12/18                  │  Y=33 (Temps, size=1)
│                                                │
│                                                │  Y=41
│ ───────────────────────────────────────────── │  Y=45 (Line)
│                                                │
│                  14:35:42                      │  Y=54 (Time, centered)
└────────────────────────────────────────────────┘  Y=64

Element Positions:
- Date: Y=0, X=0, size=1
- Current temp: Y=0, X=128-width, size=1
- Top line: Y=10, X=0 to X=127
- Icons: Y=15, X=6,46,86 (spacing=40px), 16x16px each
- Forecast temps: Y=33, centered under each icon, size=1
- Bottom line: Y=45, X=0 to X=127
- Current time: Y=54, centered, size=1

Icon Spacing:
- Column 1 (Today): X=6
- Column 2 (Tomorrow): X=46 (6+40)
- Column 3 (Day after): X=86 (6+80)
```

### 3\. Home Climate Mode (10 seconds) - HTU21D

```
Pixel Layout (128x64):
┌────────────────────────────────────────────────┐  Y=0
│ Home Climate                                   │  (Title, size=1)
│                                                │
│                                                │  Y=8
│          22.5°C                                │  Y=15 (Large temp, size=2)
│                                                │
│                                                │  Y=31
│          Humidity: 45%                         │  Y=35 (size=1)
│                                                │
│          Feel: Comfortable                     │  Y=45 (size=1)
│                                                │
│          14:35:42                              │  Y=55 (size=1)
└────────────────────────────────────────────────┘  Y=64

Element Positions:
- Title: Y=0, X=0, size=1
- Temperature: Y=15, X=10, size=2
- Humidity: Y=35, X=10, size=1
- Comfort: Y=45, X=10, size=1
- Time: Y=55, X=10, size=1
```

## Text Size Guidelines

### Size 1 (Default)

*   Character dimensions: 6x8 pixels
*   Characters per line: ~21 chars (128÷6)
*   Lines available: 8 lines (64÷8)
*   Good for: Normal text, labels, data

### Size 2 (Large)

*   Character dimensions: 12x16 pixels
*   Characters per line: ~10 chars (128÷12)
*   Lines available: 4 lines (64÷16)
*   Good for: Time, main temperature, emphasis

## Spacing Guidelines

### Safe Zones

*   **Top margin**: Y=0-2 (avoid text at very top)
*   **Bottom margin**: Y=62-64 (avoid text at very bottom)
*   **Side margins**: X=0-2 and X=126-128 (minimal)

### Text Spacing

*   **Line height size 1**: 8 pixels minimum (add 2-4px for readability)
*   **Line height size 2**: 16 pixels minimum
*   **Between sections**: 4-8 pixels

### Separator Lines

*   **Thickness**: 1 pixel
*   **Full width**: `display.drawLine(0, Y, 127, Y, SSD1306_WHITE)`
*   **Partial width**: Add margins, e.g., X=20 to X=108

### Icon Placement

*   Icons are 16x16 pixels
*   Allow 2px margin around icons
*   Space evenly: total\_width ÷ (num\_icons + 1)
*   For 3 icons: positions at ~32, 64, 96 or use 40px spacing

### Test Extreme Values

*   Longest date: "Wed, 30 Sep 2024"
*   Longest time: "23:59:59"
*   Longest temp: "-40.0C" or "125.0C"
*   Check with max/min values

---

**Display Resolution**: 128×64 pixels (16:8 aspect ratio)  
**Coordinate System**: (0,0) = top-left corner  
**Color**: Monochrome - pixels are ON (white) or OFF (black)