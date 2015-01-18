#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "espmissingincludes.h"
#include "wifi.h"
//Drop the soft AP if we are a valid client on the network.
static void ICACHE_FLASH_ATTR wifiCheckCb(void *arg) {
	int x=wifi_get_opmode();
        
	if (x!=1) {
            x=wifi_station_get_connect_status();
            os_printf("WiFi State Update: %d - looking for %d (got IP)\n", x, STATION_GOT_IP);
            
            if (x==STATION_GOT_IP) {
                os_printf("Dropping soft AP...");
            	//Go to STA mode. This needs a reset, so do that.
            	wifi_set_opmode(1);
            	system_restart();
            } else {
            	os_printf("Connect fail. Not going into STA-only mode.\n");
            }
            
	}
}

void ICACHE_FLASH_ATTR wifiCheck(void){
    static ETSTimer wifiTimer;
    os_timer_setfn(&wifiTimer, wifiCheckCb, NULL);
    os_timer_arm(&wifiTimer, 5000, 0);
}