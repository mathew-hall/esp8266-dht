/*
Some random cgi routines.
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "cgi.h"
#include "io.h"
#include "dht.h"
#include "ds18b20.h"
#include "espmissingincludes.h"

//cause I can't be bothered to write an ioGetLed()
static char currLedState=0;

//Cgi that turns the LED on or off according to the 'led' param in the GET data
int ICACHE_FLASH_ATTR cgiLed(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "led", buff, sizeof(buff));
	if (len!=0) {
		currLedState=atoi(buff);
		ioLed(currLedState);
	}

	httpdRedirect(connData, "led.tpl");
	return HTTPD_CGI_DONE;
}



//Template code for the led page.
void ICACHE_FLASH_ATTR tplLed(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "ledstate")==0) {
		if (currLedState) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
	}
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}

static long hitCounter=0;

//Template code for the counter on the index page.
void ICACHE_FLASH_ATTR tplCounter(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	if (os_strcmp(token, "counter")==0) {
		hitCounter++;
		os_sprintf(buff, "%ld", hitCounter);
	}
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}


//Cgi that reads the SPI flash. Assumes 512KByte flash.
int ICACHE_FLASH_ATTR cgiReadFlash(HttpdConnData *connData) {
	int *pos=(int *)&connData->cgiData;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (*pos==0) {
		os_printf("Start flash download.\n");
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "application/bin");
		httpdEndHeaders(connData);
		*pos=0x40200000;
		return HTTPD_CGI_MORE;
	}
	espconn_sent(connData->conn, (uint8 *)(*pos), 1024);
	*pos+=1024;
	if (*pos>=0x40200000+(512*1024)) return HTTPD_CGI_DONE; else return HTTPD_CGI_MORE;
}

//Template code for the DHT 22 page.
void ICACHE_FLASH_ATTR tplDHT(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	struct sensor_reading* r = readDHT(0);
	float lastTemp=r->temperature;
	float lastHum=r->humidity;
	
	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "temperature")==0) {
			os_sprintf(buff, "%d.%d", (int)(lastTemp),(int)((lastTemp - (int)lastTemp)*100) );		
	}
	if (os_strcmp(token, "humidity")==0) {
			os_sprintf(buff, "%d.%d", (int)(lastHum),(int)((lastHum - (int)lastHum)*100) );		
	}
	if(os_strcmp(token, "sensor_present") == 0){
		os_sprintf(buff, r->success?"is":"isn't");
	}
        r = readDS18B20();
        lastTemp=r->temperature;
        if(os_strcmp(token, "ds_temperature") == 0){
            if(r->success){
                os_sprintf(buff,"%d.%d", (int)(lastTemp),(int)((lastTemp - (int)lastTemp)*100));
            }else{
                os_sprintf(buff, "NA");
            }
        }
	
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}
