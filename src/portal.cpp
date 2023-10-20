#include "define.h"
#include "GyverPortal.h"
GyverPortal portal;

void build();
void buildAP();
void actionAP(GyverPortal &p);
void action(GyverPortal &p);

void portal_start_AP(void)
{
  // запускаем портал
  portal.attachBuild(build);
  // portal.start(WIFI_AP);
  portal.start();
  portal.attach(action);
}

void portal_start(void)
{
  portal.attachBuild(build);
  portal.attach(action);
  portal.start("esp");
}

void portal_tick(void)
{
  portal.tick();
}

// конструктор страницы
void build()
{
  GP.BUILD_BEGIN();
  GP.THEME(GP_LIGHT);

  GP.TITLE(ESP_TITLE);
  GP.HR();

  // GP.NAV_TABS_LINKS("/,/wifi");
  GP_MAKE_BLOCK_TAB(
      "WIFI",

      GP.FORM_BEGIN("/login");

      GP_MAKE_BOX(
          GP.LABEL("SSID");
          GP.TEXT("lg", "Login", lp.ssid););

      GP_MAKE_BOX(
          GP.LABEL("Пароль");
          GP.TEXT("ps", "Password", lp.pass););

      GP.SUBMIT("Сохранить");

      GP.FORM_END();

  );

  // GP.LABEL("Настройка");

  GP_MAKE_BLOCK_TAB(
      "Настройка",
      // ┌────────────────────────────────────────────────────┐
      GP.LABEL("IP адресс клиента (192.168.0.101)"); // │

      GP_MAKE_BOX(
          GP.TEXT("ipclient", "ipclient", ipchar);     // │
          GP.BUTTON_MINI("saveipclient", "Сохранить"); // │
      );
      
      // GP.BREAK(); // │
      // └────────────────────────────────────────────────────┘
      // ┌────────────────────────────────────────────────────┐
      GP.LABEL("Serial2 битрейт"); // │
      GP_MAKE_BOX(
          GP.NUMBER("bitrate", "bitrate", Serial2Bitrate); // │
          GP.BUTTON_MINI("saveBR", "Сохранить");           // │
      );
      // └────────────────────────────────────────────────────┘
      // ┌────────────────────────────────────────────────────┐
      // GP.BREAK();                                 // │
      GP.LABEL("Таймаут новой строки ms"); // │
      GP_MAKE_BOX(
          GP.NUMBER("timeout", "timeout", timeout); // │
          GP.BUTTON_MINI("saveTimeout", "Сохранить"););
      // └────────────────────────────────────────────────────┘
      // ┌────────────────────────────────────────────────────┐
      GP.BREAK(); // │

      GP_MAKE_BOX(
          GP.LABEL("Использовать broadcast");
          GP.CHECK("сheck_broadcast", broadcast););

      GP_MAKE_BOX(
          GP.LABEL("Эхо на Serial");    // │
          GP.CHECK("сheck_echo", echo); // │
      );
      // └────────────────────────────────────────────────────┘
  );

  GP.BLOCK_BEGIN();
  GP.BUTTON("rst", "ESP32 reset");
  GP.BLOCK_END();

  GP.BUILD_END();
}

void action(GyverPortal &p)
{
  int valNum = 99;
  String valText;

  // ┌────────────────────────────────────────────────────┐
  //   Изменение адреса IP клиента                      //│
  if (p.click("ipclient"))
  {                                         // │
    valText = portal.getString("ipclient"); // │
    Serial.print("ipclient: ");             // │
    Serial.println(valText);                // │
    valText.toCharArray(ipchar, 20, 0);     // │
  }                                         // │
    // │
  // Кнопка сохранить в EEPROM IP клиента               //│
  if (p.click("saveipclient"))
  {                                                  // │
    Serial.println("Сохранить в EEPROM IP клиента"); // │
    EEPROM.writeString(EEPROM_ADRESS_IP_CLIENT, ipchar);
    EEPROM.commit(); // │
  }                  // │
  // └────────────────────────────────────────────────────┘
  // ┌────────────────────────────────────────────────────┐
  // Броадкас пакеты
  if (portal.click("сheck_broadcast"))
  {
    bool valSwitch = portal.getCheck("сheck_broadcast");
    Serial.print("Switch: ");
    Serial.println(valSwitch);
    Serial.println("Сохранить броадкаст");
    EEPROM.writeInt(EEPROM_ADRESS_broadcast, valSwitch);
    EEPROM.commit();
    broadcast = valSwitch;
  }
  // └────────────────────────────────────────────────────┘

  if (portal.click("сheck_echo"))
  {
    bool valSwitch = portal.getCheck("сheck_echo");
    Serial.print("Switch: ");
    Serial.println(valSwitch);
    Serial.println("Сохранить Эхо");
    EEPROM.writeInt(EEPROM_ADRESS_echo, valSwitch); // сохраняем
    EEPROM.commit();                                // записываем
    echo = valSwitch;
  }

  // Изменение битрейда
  if (p.click("bitrate"))
  {
    valNum = portal.getInt("bitrate");
    Serial.print("bitrate: ");
    Serial.println(valNum);
    Serial2Bitrate = valNum;
  }

  // Изменение Таймаута
  if (p.click("timeout"))
  {
    valNum = portal.getInt("timeout");
    Serial.print("timeout: ");
    Serial.println(valNum);
    timeout = valNum;
  }

  // Кнопка сохранить в EEPROM битрейт
  if (p.click("saveBR"))
  {
    Serial.println("Сохранить битрейт");
    EEPROM.writeInt(EEPROM_ADRESS_Serial2Bitrate, Serial2Bitrate); // сохраняем
    EEPROM.commit();                                               // записываем
  }

  // Кнопка сохранить в EEPROM таймаут
  if (p.click("saveTimeout"))
  {
    Serial.println("Сохранить Таймаут");
    EEPROM.writeInt(EEPROM_ADRESS_timeout, timeout); // сохраняем
    EEPROM.commit();                                 // записываем
  }

  if (p.click("rst"))
  {
    Serial.println("Перезагрузка");
    ESP.restart();
  }

  if (p.form("/login"))
  {
    Serial.println("Сохранить login в EEPROM");
    p.copyStr("lg", lp.ssid); // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp); // сохраняем
    EEPROM.commit();   // записываем
  }
}

// конструктор страницы
void buildAP()
{
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

  GP.FORM_BEGIN("/login");
  GP.TEXT("lg", "Login", lp.ssid);
  GP.BREAK();
  GP.TEXT("ps", "Password", lp.pass);
  GP.SUBMIT("Сохранить");
  GP.FORM_END();

  GP.BUILD_END();
}

void actionAP(GyverPortal &p)
{
  if (p.form("/login"))
  {                           // кнопка нажата
    p.copyStr("lg", lp.ssid); // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);       // сохраняем
    EEPROM.commit();         // записываем
    WiFi.softAPdisconnect(); // отключаем AP
    ESP.restart();           // перезагрузка
  }
}
