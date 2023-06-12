#pragma once
#include "quat_math.h"
#include "pico/stdlib.h"
#include "radio.h"
#define MAX_UMYO_DEVICES 12

struct uMyo_data
{
	uint32_t id;
	int batt_mv;
	uint8_t last_data_id;
	uint16_t device_avg_muscle_level;
	int16_t cur_spectrum[4];
	int16_t raw_data[8];
	uint32_t last_data_time;
	sQ Qsg;
	void operator=(uMyo_data d2)
	{
		id = d2.id;
		batt_mv = d2.batt_mv;
		last_data_id = d2.last_data_id;
		device_avg_muscle_level = d2.device_avg_muscle_level;
		for(int x = 0; x < 4; x++) cur_spectrum[x] = d2.cur_spectrum[x];
		for(int x = 0; x < 8; x++) raw_data[x] = d2.raw_data[x];
		last_data_time = d2.last_data_time;
		Qsg = d2.Qsg;
	}
};

class uMyo
{
private:
	uMyo_data devices[MAX_UMYO_DEVICES];
	sV nx, ny, nz;
	int device_count;
	uint8_t idToIdx(uint32_t id);
    Radio radio = Radio();
public:
	uMyo(void);
	void update();
	void parsePack(uint8_t pack[32]);
	uint8_t getDeviceCount();
	int getBattery(uint8_t devidx);
	uint32_t getID(uint8_t devidx);
	uint8_t getDataID(uint8_t devidx);
	float getMuscleLevel(uint8_t devidx);
	float getAverageMuscleLevel(uint8_t devidx);
	void getSpectrum(uint8_t devidx, float *spectrum);
	void getRawData(uint8_t devidx, int16_t *data);
	float getPitch(uint8_t devidx);
	float getRoll(uint8_t devidx);
	float getYaw(uint8_t devidx);
};


