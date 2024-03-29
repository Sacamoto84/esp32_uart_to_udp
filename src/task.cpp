#include "define.h"
#include "model.h"

extern AsyncUDP udp;

ListUI listUI;

void TaskReadUART(void *parameter)
{

#define BUF_SIZE 2048
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);

  int length = 0;

  int rxBytes = 0;

  for (;;)
  {

    memset(buf, 0, BUF_SIZE);

    // uart_get_buffered_data_len(UART_NUM_2, (size_t*)&length); //Получить количество данных в буффере
    // Serial.println(length);

    int rxBytes = uart_read_bytes(UART_NUM_2, buf, 1024, 5);

    if ((rxBytes > 0) && (rxBytes != -1) && (buf[0] != 0))
    {
      all_TX_to_UDP += rxBytes;

      for (int i = 0; i < rxBytes; i++)
        listUI.put(buf[i]);

      // Serial.print("✅ !TaskReadUART>> RAW buf >>");
      // Serial.println(buf);

      if (echo)
      {
        Serial.print("🌐 >>UDP_tx_task>>Эхо>>");
        Serial.println(buf);
      }

      if (broadcast == 0)
      {
        
        //ipclient.fromString(ipchar);

        udp.writeTo((uint8_t *)buf, strlen(buf), ipclient, 8888, TCPIP_ADAPTER_IF_MAX);
      }
      else
        udp.broadcastTo(buf, 8888);
    }
    else
      delay(1);
  }
}
