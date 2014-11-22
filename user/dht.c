
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include "ets_sys.h"
#include "osapi.h"
#include "espmissingincludes.h"
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "gpio.h"
#include "dht.h"

#define MAXTIMINGS 10000
#define BREAKTIME 20



enum sensor_type SENSOR;

static inline float scale_humidity(int* data){
	if(SENSOR == SENSOR_DHT11){
		return data[0];
	}else{
		float humidity = data[0] * 256 + data[1];
		return humidity/= 10;
	}
}

static inline float scale_temperature(int* data){
	if(SENSOR == SENSOR_DHT11){
		return data[2];
	}else{
		float temperature = data[2] & 0x7f;
		temperature *= 256;
		temperature += data[3];
		temperature /= 10;
		if(data[2] & 0x80)
			temperature *= -1;
		return temperature;
	}
}

  struct sensor_reading* ICACHE_FLASH_ATTR
readDHT(void)
{
	static struct sensor_reading r;
    int counter = 0;
    int laststate = 1;
    int i = 0;
    int bits_in = 0;
	//int bitidx = 0;
    //int bits[250];

    int data[100];

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	//Wake up device, 250ms of high
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(250000);
	
	//Hold low for 20ms
    GPIO_OUTPUT_SET(2, 0);
    os_delay_us(20000);
	
	//High for 40ms
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(40);
	
	//Set pin to input with pullup
    GPIO_DIS_OUTPUT(2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);


    // wait for pin to drop?
    while (GPIO_INPUT_GET(2) == 1 && i<100000) {
          os_delay_us(1);
          i++;
    }

	//Give up if pin didn't go low
	if(i==100000){
		goto fail;
	}

    // read data!
    for (i = 0; i < MAXTIMINGS; i++) {
		//Count high time (in approx us)
		counter = 0;
        while ( GPIO_INPUT_GET(2) == laststate) {
			counter++;
			os_delay_us(1);
            if (counter == 1000)
                break;
        }
        laststate = GPIO_INPUT_GET(2);

		if (counter == 1000) break;

		//store data after 3 reads
		if ((i>3) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[bits_in/8] <<= 1;
            if (counter > BREAKTIME)
                data[bits_in/8] |= 1;
            bits_in++;
        }
    }
	
	if(bits_in < 40){
		goto fail;
	}
	
	int checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
	if (data[4] != checksum) {
		os_printf("Checksum was incorrect after %d bits. Expected %d but got %d", bits_in, data[4], checksum);
		goto fail;
	}
	
	r.temperature = scale_temperature(data);
	r.humidity = scale_humidity(data);
	os_printf("Temp =  %d *C, Hum = %d %%\n", (int)(r.temperature*100), (int)(r.humidity*100));
	
	r.success = 1;
	return &r;
fail:
	r.success = 0;
	return &r;
}



void ICACHE_FLASH_ATTR DHT(void) {
	 readDHT();
}


void DHTInit(enum sensor_type sensor_type) {
	SENSOR = sensor_type;
    //Set GPIO2 to output mode for DHT22
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
}
