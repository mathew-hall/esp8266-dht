

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include "espmissingincludes.h"
#include "osapi.h"

#include "ets_sys.h"
#include "httpd.h"
#include "io.h"
#include "dht.h"
#include "httpdespfs.h"
#include "cgi.h"
#include "cgiwifi.h"
#include "stdout.h"
#include "wifi.h"
#include "tempd.h"

HttpdBuiltInUrl builtInUrls[]={
	{"/", cgiRedirect, "/index.tpl"},
	{"/flash.bin", cgiReadFlash, NULL},
	{"/led.tpl", cgiEspFsTemplate, tplLed},
	{"/dht22.tpl", cgiEspFsTemplate, tplDHT},
	{"/index.tpl", cgiEspFsTemplate, tplCounter},
	{"/led.cgi", cgiLed, NULL},

	//Routines to make the /wifi URL and everything beneath it work.
	{"/wifi", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/wifi/connect.cgi", cgiWiFiConnect},

	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


void user_init(void) {
	stdoutInit();
	ioInit();
	DHTInit(SENSOR_DHT11, 30000);
	httpdInit(builtInUrls, 80);
        wifiCheck();
        tempdInit();
	os_printf("\nReady\n");
}
