#include "define.h"
#include <Wire.h>
#include "WiFi.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "model.h"

#include "MicrosoftSansSerif24.h"

extern ListUI listUI;

#define AA_FONT_SMALL MicrosoftSansSerif18
#define TFT_GREY 0x7BEF
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

void oled_begin(void)
{

  tft.init(); // initialize a ST7735S chip
  tft.setRotation(1);

  tft.fillScreen(127);

  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);

  tft.drawEllipse(120, 120, 100, 100, random(0xFFFF));
  // We can now plot text on screen using the "print" class
  tft.println("Hello Креведко 123");

  // tft.setTextDatum(MC_DATUM);
  //  tft.loadFont(AA_FONT_SMALL);    // Must load the font first
  //  for(int i = 0 ; i<14; i++)
  //  {
  //    tft.drawString("Логичводу данные .", 0, 17*i, 1); // Print the name of the font
  //  }

  // initialize OLED display with I2C address 0x3C
  // if (!oled.begin(SSD1306_SWITCHCAPVCC)) {
  //   Serial.println(F("failed to start SSD1306 OLED"));
  //   while (1);
  // }

  // oled.setTextColor(SSD1306_WHITE);
  // oled.clearDisplay();
  // oled.display();

  // oled.setTextSize(1);
}

void oled_refresh(void)
{
  char str[64];

  // oled.clearDisplay();

  tft.fillScreen(0);

  sprintf(str, "SSID: %s", lp.ssid);
  tft.setCursor(0, 0);
  tft.println(str);

  sprintf(str, "PASS: %s", lp.pass);
  // tft.setCursor(0,8);
  tft.println(str);

  sprintf(str, "IP:   %s", WiFi.localIP().toString());
  // tft.setCursor(0,16);
  tft.println(str);

  sprintf(str, "Client: %s", ipchar);
  // tft.setCursor(0,24);
  tft.println(str);

  sprintf(str, "Bitrate: %d", Serial2Bitrate);
  // tft.setCursor(0,32);
  tft.println(str);

  sprintf(str, "Echo %d  Broadcast %d", echo, broadcast);
  // tft.setCursor(0,40);
  tft.println(str);

  sprintf(str, "TX:%lu", all_TX_to_UDP);
  // tft.setCursor(0,48);
  tft.println(str);

  for (int i = 0; i < MAX_L; i++)
  {
    int index = (listUI.size + i - (MAX_L - 1)) % MAX_L;
    tft.println(listUI.list[index].c_str());
  }

  // //sprintf(str, "Status");
  // //oled.setCursor(0,56);
  // //oled.print(str);

  // oled.display();
}