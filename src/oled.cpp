#include "define.h"
#include <Wire.h>
#include "WiFi.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "model.h"

#include "MicrosoftSansSerif24.h"
#include "NotoSansBold15.h"
#include "RoboMono_Regular_15.h"

extern ListUI listUI;

#define AA_FONT_SMALL MicrosoftSansSerif18

#define AA_FONT_SMALL15 NotoSansBold15
#define AA_FONT_ROBOMONO15 RoboMonoRegular15

#define TFT_GREY 0x7BEF
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
TFT_eSprite face = TFT_eSprite(&tft);

#define FACE_W 240
#define FACE_H 16
void facePrint(int y, const char *str);

void facePrint(int y, const char *str)
{
  face.fillSprite(TFT_BLACK);
  face.setTextColor(TFT_WHITE, TFT_BLACK);
  face.setCursor(0, 0);
  face.drawString(str, 0, 0);
  face.pushSprite(0, y, TFT_TRANSPARENT);
}

void facePrint(int y, int x0, int x1, const char *str0, const char *str1)
{
  face.fillSprite(TFT_BLACK);
  face.setTextColor(TFT_WHITE, TFT_BLACK);
  // face.setCursor(0, 0);
  face.drawString(str0, x0, 0);
  face.drawString(str1, x1, 0);
  face.pushSprite(0, y, TFT_TRANSPARENT);
}

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
  tft.loadFont(AA_FONT_ROBOMONO15); // Must load the font first
  tft.println("Hello Креведко 123");

  face.createSprite(FACE_W, FACE_H);
  face.loadFont(AA_FONT_ROBOMONO15); // Must load the font first
}

void oled_refresh(void)
{
  char str[64];

  // tft.fillScreen(0);

  tft.setCursor(0, 0);
  // sprintf(str, "SSID: %s\n", lp.ssid);
  facePrint(0, 0, 100, "SSID:", lp.ssid);
  facePrint(16, 0, 100, "PASS:", lp.pass);
  facePrint(32, 0, 100, "IP:", WiFi.localIP().toString().c_str());
  facePrint(48, 0, 100, "CLIENT:", ipchar);
  sprintf(str, "%d", Serial2Bitrate); 
  facePrint(64, 0, 100, "BITRATE:", ipchar);
  sprintf(str, "Echo %d  Broadcast %d", echo, broadcast);
  facePrint(80, str);
  sprintf(str,"TX:%lu", all_TX_to_UDP);
  facePrint(96, str);

  // facePrint(120, const char* str");
  // facePrint(120+16, "const char* str");

  for (int i = 0; i < MAX_L; i++)
  {
    int index = (listUI.size + i - (MAX_L - 1)) % MAX_L;
    // face.println(listUI.list[index].c_str());
    facePrint(120 + 15 * i, listUI.list[index].c_str());
  }

  // //sprintf(str, "Status");
  // //oled.setCursor(0,56);
  // //oled.print(str);

  // oled.display();
}
