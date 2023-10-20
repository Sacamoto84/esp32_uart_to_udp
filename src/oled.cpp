#include "define.h"
#include <Wire.h>
#include "WiFi.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "model.h"

extern ListUI listUI;

#define TFT_GREY 0x7BEF
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
TFT_eSprite face = TFT_eSprite(&tft);

#define FACE_W 240
#define FACE_H 16
void facePrint(int y, const char *str);

void facePrint(int y, const char *str)
{
  face.fillSprite(0);
  face.setCursor(0, 0);
  face.drawString(str, 0, 0);
  face.pushSprite(0, y, TFT_TRANSPARENT);
}

void facePrint(int y, int x0, int x1, const char *str0, const char *str1)
{
  face.fillSprite(TFT_BLACK);
  face.drawString(str0, x0, 0);
  face.drawString(str1, x1, 0);
  face.pushSprite(0, y, TFT_TRANSPARENT);
}

void oled_begin(void)
{
  tft.init(); // initialize a ST7735S chip
  tft.setRotation(1);
  tft.fillScreen(0);
  tft.setCursor(0, 0, 2);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);

  face.createSprite(FACE_W, FACE_H);
  face.loadFont(AA_FONT_ROBOMONO15);
}

void oled_refresh(void)
{
  char str[64];
  face.setTextColor(TFT_WHITE, TFT_BLACK);
  facePrint(0, 0, 80, "IP:", WiFi.localIP().toString().c_str());
  if (needAP)
    facePrint(14, 0, 80, "CLIENT:", "192.168.4.2");
  else
    facePrint(14, 0, 80, "CLIENT:", ipchar);

  sprintf(str, "%d", Serial2Bitrate);
  facePrint(28, 0, 80, "BITRATE:", str);
  sprintf(str, "E:%d B:%d TX:%lu", echo, broadcast, all_TX_to_UDP);
  facePrint(42, str);
  face.setTextColor(TFT_GREEN, TFT_BLACK);
  for (int i = 0; i < MAX_L; i++)
  {
    int index = (listUI.size + i - (MAX_L - 1)) % MAX_L;
    facePrint(60 + 15 * i, listUI.list[index].c_str());
  }
  tft.drawFastHLine(0, 59, 240, TFT_RED);
  tft.drawFastHLine(0, 239, 240, TFT_RED);
}
