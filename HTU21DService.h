#ifndef HTU21D_SERVICE_H
#define HTU21D_SERVICE_H

#include <Wire.h>
#include <Adafruit_HTU21DF.h>

/**
 * HTU21D Temperature and Humidity Sensor Service
 * 
 * This service provides indoor temperature and humidity readings from the HTU21D sensor.
 * It measures HOME/INDOOR climate conditions where the device is located.
 * 
 * IMPORTANT: This sensor measures INDOOR conditions (your home climate).
 *            The weather API provides OUTDOOR forecast data.
 *            Together, you get complete indoor + outdoor monitoring!
 * 
 * Features:
 * - Indoor temperature reading in Celsius (±0.3°C accuracy)
 * - Indoor relative humidity reading in percentage (±2% RH accuracy)
 * - Automatic sensor detection and initialization
 * - Error handling for sensor communication failures
 * - Cached readings to avoid excessive I2C communication
 * - Calculated dew point and heat index for home comfort analysis
 * - Comfort level indicator based on temperature and humidity
 * 
 * Use cases:
 * - Monitor home comfort levels
 * - Track bedroom climate for better sleep
 * - Prevent mold by monitoring humidity
 * - Optimize heating/cooling efficiency
 * - Plant care and humidity management
 */

class HTU21DService {
public:
  HTU21DService() 
    : m_sensorFound(false),
      m_temperature(0.0f),
      m_humidity(0.0f),
      m_lastReadTime(0),
      m_readInterval(2000) // Read every 2 seconds minimum
  {}

  /**
   * Initialize the HTU21D sensor
   * Returns: true if sensor found and initialized, false otherwise
   */
  bool begin() {
    m_sensorFound = m_htu.begin();
    
    if (m_sensorFound) {
      Serial.println("HTU21D sensor found and initialized");
      // Perform initial reading
      update();
    } else {
      Serial.println("HTU21D sensor not found!");
      Serial.println("Check wiring:");
      Serial.println("  VCC -> 3.3V");
      Serial.println("  GND -> GND");
      Serial.println("  SDA -> GPIO0 (D3)");
      Serial.println("  SCL -> GPIO2 (D4)");
    }
    
    return m_sensorFound;
  }

  /**
   * Check if sensor is available and working
   */
  bool isAvailable() const {
    return m_sensorFound;
  }

  /**
   * Update sensor readings (respects minimum read interval)
   * Returns: true if reading was successful
   */
  bool update() {
    unsigned long currentTime = millis();
    
    // Check if enough time has passed since last reading
    if (currentTime - m_lastReadTime < m_readInterval) {
      return true; // Return cached values
    }

    if (!m_sensorFound) {
      return false;
    }

    // Read temperature and humidity
    float newTemp = m_htu.readTemperature();
    float newHumidity = m_htu.readHumidity();

    // Validate readings (HTU21D returns NaN on error)
    if (isnan(newTemp) || isnan(newHumidity)) {
      Serial.println("Failed to read from HTU21D sensor!");
      return false;
    }

    // Check for reasonable values
    if (newTemp < -40 || newTemp > 85 || newHumidity < 0 || newHumidity > 100) {
      Serial.println("HTU21D reading out of range!");
      return false;
    }

    // Update cached values
    m_temperature = newTemp;
    m_humidity = newHumidity;
    m_lastReadTime = currentTime;

    return true;
  }

  /**
   * Force an immediate sensor reading (ignores read interval)
   */
  bool forceUpdate() {
    m_lastReadTime = 0; // Reset timer
    return update();
  }

  /**
   * Get temperature in Celsius
   */
  float getTemperature() const {
    return m_temperature;
  }

  /**
   * Get temperature in Fahrenheit
   */
  float getTemperatureFahrenheit() const {
    return (m_temperature * 9.0f / 5.0f) + 32.0f;
  }

  /**
   * Get relative humidity (0-100%)
   */
  float getHumidity() const {
    return m_humidity;
  }

  /**
   * Calculate heat index in Celsius
   * Based on temperature and humidity
   */
  float getHeatIndex() const {
    if (!m_sensorFound) return m_temperature;
    
    float tempF = getTemperatureFahrenheit();
    float hi = calculateHeatIndex(tempF, m_humidity);
    return (hi - 32.0f) * 5.0f / 9.0f; // Convert back to Celsius
  }

  /**
   * Calculate dew point in Celsius
   * The temperature at which water vapor condenses
   */
  float getDewPoint() const {
    if (!m_sensorFound) return 0.0f;
    
    // Magnus formula approximation
    float a = 17.27;
    float b = 237.7;
    float alpha = ((a * m_temperature) / (b + m_temperature)) + log(m_humidity / 100.0);
    float dewPoint = (b * alpha) / (a - alpha);
    
    return dewPoint;
  }

  /**
   * Get comfort level based on temperature and humidity
   * Returns: string describing comfort level
   */
  String getComfortLevel() const {
    if (!m_sensorFound) return "N/A";
    
    float heatIndex = getHeatIndex();
    
    if (heatIndex < 15) return "Cold";
    if (heatIndex < 20) return "Cool";
    if (heatIndex < 24) return "Comfortable";
    if (heatIndex < 27) return "Warm";
    if (heatIndex < 32) return "Hot";
    if (heatIndex < 41) return "Very Hot";
    return "Extreme";
  }

  /**
   * Set minimum interval between sensor readings (in milliseconds)
   * Default: 2000ms (2 seconds)
   */
  void setReadInterval(unsigned long intervalMs) {
    m_readInterval = intervalMs;
  }

  /**
   * Print current readings to Serial
   */
  void printReadings() const {
    if (!m_sensorFound) {
      Serial.println("HTU21D: Sensor not available");
      return;
    }

    Serial.println("=== HTU21D Sensor Readings ===");
    Serial.print("Temperature: ");
    Serial.print(m_temperature, 1);
    Serial.print("°C (");
    Serial.print(getTemperatureFahrenheit(), 1);
    Serial.println("°F)");
    
    Serial.print("Humidity: ");
    Serial.print(m_humidity, 1);
    Serial.println("%");
    
    Serial.print("Dew Point: ");
    Serial.print(getDewPoint(), 1);
    Serial.println("°C");
    
    Serial.print("Heat Index: ");
    Serial.print(getHeatIndex(), 1);
    Serial.println("°C");
    
    Serial.print("Comfort Level: ");
    Serial.println(getComfortLevel());
    Serial.println("==============================");
  }

private:
  Adafruit_HTU21DF m_htu;
  bool m_sensorFound;
  float m_temperature;
  float m_humidity;
  unsigned long m_lastReadTime;
  unsigned long m_readInterval;

  /**
   * Calculate heat index in Fahrenheit
   * Rothfusz regression equation
   */
  float calculateHeatIndex(float tempF, float humidity) const {
    // Simple formula for temperatures below 80°F
    if (tempF < 80) {
      return tempF;
    }

    // Rothfusz regression
    float hi = -42.379 
             + 2.04901523 * tempF 
             + 10.14333127 * humidity 
             - 0.22475541 * tempF * humidity 
             - 0.00683783 * tempF * tempF 
             - 0.05481717 * humidity * humidity 
             + 0.00122874 * tempF * tempF * humidity 
             + 0.00085282 * tempF * humidity * humidity 
             - 0.00000199 * tempF * tempF * humidity * humidity;

    return hi;
  }
};

#endif
