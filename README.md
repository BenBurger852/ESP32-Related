# ESP32-Related
ESP32 Get IP Address
This file is related to the ESP32 ESPRESSIF ESP-IDF platform
I found it very difficult to get the assigned IPV4 address once the ESP connected to an AP
So after some testing this is what I came up with and it works 100% all the time
- Look for ESP-IDF file wifi_connect.c
- Then look for the function: static void example_handler_on_sta_got_ip(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data)......
- Replace the function with this code:
//----------------------------------------------------
char tempIPAddress[16]; // address can be 16 chars ie: 192.168.123.123...
static bool wifiConnected = false;
//=============================================================================
static void example_handler_on_sta_got_ip(void *arg, esp_event_base_t event_base,
                                          int32_t event_id, void *event_data)
{
    s_retry_num = 0;
    
    wifiConnected = true;
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    if (!example_is_our_netif(EXAMPLE_NETIF_DESC_STA, event->esp_netif))
    {
        return;
    }
    ESP_LOGI(TAG, "Got IPv4 event: Interfacexxx \"%s\" address: " IPSTR, esp_netif_get_desc(event->esp_netif), IP2STR(&event->ip_info.ip));

    uint8_t x1 = esp_ip4_addr1(&event->ip_info.ip);
    uint8_t x2 = esp_ip4_addr2(&event->ip_info.ip);
    uint8_t x3 = esp_ip4_addr3(&event->ip_info.ip);
    uint8_t x4 = esp_ip4_addr4(&event->ip_info.ip);

    //temprarily store new IP address
    sprintf(tempIPAddress,"%d.%d.%d.%d",x1,x2,x3,x4);
    
    //cPrintf("IP=:%s",localMfsParms[0].IPAddress);
    if (s_semph_get_ip_addrs)
    {
        xSemaphoreGive(s_semph_get_ip_addrs);
    }
    else
    {
        ESP_LOGI(TAG, "- IPv4 address: " IPSTR ",", IP2STR(&event->ip_info.ip));
    }
}
.......................
bool getIpStatus(void)
{
    return  wifiConnected;
}
- Then simply reference this buffer in any other file as
and voila after receiving and IP address you will have a buffer with the correct IP address
-------------------------------------------------------------------------------------------
in my Main.c I have this piece of code
extern char tempIPAddress[16];
extern bool getIpStatus(void);
...............................
 while (wifiState == false && ipRetryCount++ < 20)
  {
      wifiState = getIpStatus();
      cPrintf("Waiting for IP:%d\r\n", ipRetryCount);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // stop watchdog TO
  }

  if (ipRetryCount >= 20)
  {
      esp_restart();
  }
  else
  {
      cPrintf("%s\r\n", tempIPAddress); // get new IP addr and copy to structure          
  }
      .........................................
***cPrintf is my own version of printf... but printf also works
- The code will loop for 20 seconds and timeout or if it gets an IP address simply continue

Hope it help someone out there
