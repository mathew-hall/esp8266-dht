
enum sensor_type{
	SENSOR_DHT11,SENSOR_DHT22
};

struct sensor_reading{
	float temperature;
	float humidity;
	BOOL success;
};


void ICACHE_FLASH_ATTR DHT(void);
struct sensor_reading * ICACHE_FLASH_ATTR readDHT(void);
void DHTInit(enum sensor_type, uint32_t polltime);
