This code is part of wifi_connect.c in the ESPRESSIF library
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
