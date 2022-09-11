#include "define.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "WiFi.h"

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void oled_begin(void)
{
// initialize OLED display with I2C address 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  oled.setTextColor(SSD1306_WHITE);
  oled.clearDisplay();
  oled.display();
  
  oled.setTextSize(1);
}

void oled_refresh(void)
{
  char str[64];

  oled.clearDisplay();

  sprintf(str, "SSID: %s", lp.ssid);   
  oled.setCursor(0,0);
  oled.print(str);
  
  sprintf(str, "PASS: %s", lp.pass);   
  oled.setCursor(0,8);
  oled.print(str);

  sprintf(str, "IP:   %s", WiFi.localIP().toString()); 
  oled.setCursor(0,16);
  oled.print(str);

  sprintf(str, "Client: %s", ipchar); 
  oled.setCursor(0,24);
  oled.print(str);

  sprintf(str, "Bitrate: %d", Serial2Bitrate); 
  oled.setCursor(0,32);
  oled.print(str);

  sprintf(str, "Echo %d  Broadcast %d", echo, broadcast); 
  oled.setCursor(0,40);
  oled.print(str);

  sprintf(str, "TX:%lu", all_TX_to_UDP); 
  oled.setCursor(0,48);
  oled.print(str);

  //sprintf(str, "Status"); 
  //oled.setCursor(0,56);
  //oled.print(str);

  oled.display();
}