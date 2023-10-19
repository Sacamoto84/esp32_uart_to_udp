#include "define.h"
#include "AsyncUDP.h"
#include <WiFi.h>

//Mark: UDP-------------------------------------------------------
AsyncUDP udp;
//-----------------------------------------------------------------
void parsePacket(AsyncUDPPacket packet);

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {

  pinMode(EXT_RESET, OUTPUT_OPEN_DRAIN);
  digitalWrite(EXT_RESET, 1); 

  EEPROM.begin(100); 
  Serial.begin(SerialBitrate);
  Serial.println(APP_TITLE);

  EEPROM_READ_Serial2Bitrate(); //Прочитать из EEPROM битрейт для Serial2
  //
  const uart_config_t uart_config = {
         .baud_rate = Serial2Bitrate, // Скорость передачи
         .data_bits = UART_DATA_8_BITS, // Биты данных
         .parity = UART_PARITY_DISABLE, // проверка четности
         .stop_bits = UART_STOP_BITS_1, // Стоповые биты
         //.flow_ctrl = UART_HW_FLOWCTRL_RTS, // управление потоком
         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // управление потоком
         .rx_flow_ctrl_thresh = 122, // Аппаратный порог RTS 122
  };

  uart_driver_install(UART_NUM_2, SERIAL2_SIZE_RX , 0, 0, NULL, 0);
  uart_param_config(UART_NUM_2, &uart_config);
  //uart_set_pin(UART_NUM_2, TXD2, RXD2, RTS2, UART_PIN_NO_CHANGE);
  uart_set_pin(UART_NUM_2, TXD2, RXD2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_flush_input (UART_NUM_2);

  //
  //EXT_reset();

  oled_begin();

  EEPROM_READ_ip_client();      //Читаем из EEPROM адресс клиента -> ipchar
  EEPROM_READ_timeout();   //Прочитать из EEPROM таймаут
  EEPROM_READ_echo();      //Прочитать из EEPROM echo
  EEPROM_READ_broadcast(); //Прочитать из EEPROM broadcast

  EEPROM.get(0, lp);  
  Serial.print(">EEPROM READ SSID:");  Serial.println(lp.ssid);  Serial.print(">EEPROM READ PASS:"); Serial.println(lp.pass);

  //Подключение к Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(lp.ssid, lp.pass);
  
  //
  char str[64];
  //oled.setCursor(0, 0);  oled.print(APP_TITLE);
  //sprintf(str, "SSID: %s", lp.ssid);   
  //oled.setCursor(0, 10);  oled.print(str);
  //oled.setCursor(0, 20);  sprintf(str, "PASS: %s", lp.pass); oled.print(str);
  //oled.setCursor(0, 34);  oled.print("Connecting to STA");   oled.setCursor(0, 48);
  //

  int count  = 0;
  bool needAP = false; //Если сети нет создаем точку доступа
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    //oled.print(".");
    //oled.display();

    count++;
    if (count>20)
    {
      //Сеть не найдена
      Serial.println("Wifi STA не найдена");
      needAP = true; //Нужно создать точку доступа
      break; //Выходим
    }
  }
   
  //Создаем точку доступа
  if (needAP)
  {
    //oled.clearDisplay();
    //oled.setCursor(0, 0);
    //oled.setTextSize(2);
    //oled.print("AP Mode");
    //oled.setCursor(0, 32);
    //oled.print("SSID: AP32");

    //oled.display();

    // запускаем точку доступа
    WiFi.mode(WIFI_AP);
    WiFi.softAP("AP ESP32");

    // запускаем портал
    portal_start();

    // работа портала
    while (true){portal_tick();}  
  }
  Serial.println();
  Serial.print("Connected! Local IP: ");
  Serial.println(WiFi.localIP());

  //подключаем конструктор и запускаем
  portal_start();

if(udp.listen(8889)) {
     Serial.println("udp.listen(8889)");
     udp.onPacket(parsePacket);
  }

 if (broadcast == 0)
 {
    mString <32> s;
    s += APP_TITLE;
    IPAddress ipclient;
    ipclient.fromString(ipchar);
    udp.writeTo((uint8_t* )s.c_str(), s.length() , ipclient , 8888, TCPIP_ADAPTER_IF_MAX);      
  }
  else
    udp.broadcastTo(APP_TITLE, 8888);

  xTaskCreatePinnedToCore(
  TaskReadUART,     /* Функция для задачи */
  "TaskReadUART",   /* Имя задачи */
  10000,            /* Размер стека */
  NULL,             /* Параметр задачи */
  0,                /* Приоритет */
  &Task1,           /* Выполняемая операция */
  1);               /* Номер ядра, на котором она должна выполняться */

  oled_refresh();

}

// Определяем callback функцию обработки пакета
void parsePacket(AsyncUDPPacket packet)
{
  Serial.println(">Принят Пакет");
  if ( strstr((const char*)packet.data(), "Reset") )
  {
     Serial.println(">☝ Найден Reset");
     uart_flush_input(UART_NUM_2);
     all_TX_to_UDP = 0;
     EXT_reset();
  }
}

void loop() {
  while(1)
  {
    portal_tick();   
    oled_refresh();
    delay(100);
  }
}
