#ifndef TIMECLIENT_H
#define TIMECLIENT_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

class TimeClient {
public:
  int UTC_OFFSET = 2;
  WiFiUDP ntpUDP;
  NTPClient client;
  TimeClient()
    : client(ntpUDP, "pool.ntp.org") {
    // constructor initialization list
  }
  void init() {
    client.begin();
    client.setTimeOffset(UTC_OFFSET * 3600);
  }

  String getFormattedTime() {
    client.update();
    return client.getFormattedTime();
  }

  String getDate() {
    time_t raw = client.getEpochTime();
    struct tm* ti = localtime(&raw);
    char dateBuffer[30];
    strftime(dateBuffer, sizeof(dateBuffer), "%a, %d %b %Y", ti);
    return String(dateBuffer);
  }
};

#endif