#include "uMyo.h"

uMyo::uMyo(void)
{

	device_count = 0;
	nx.x = 1; ny.x = 0; nz.x = 0; //for angles calculation
	nx.y = 0; ny.y = 1; nz.y = 0;
	nx.z = 0; ny.z = 0; nz.z = 1;
}

void uMyo::update(){
	uint8_t pack[32];
	radio.readPack(pack);
	parsePack(pack);
}

uint8_t uMyo::idToIdx(uint32_t id)
{
	uint32_t ms = to_us_since_boot(absolute_time_t());
	//remove devices that are not sending data for more than 5 seconds
	for(uint8_t u = 0; u < device_count; u++)
	{
		if(ms - devices[u].last_data_time > 5000)
		{
			for(int u1 = u+1; u1 < device_count; u1++)
			{
				devices[u1-1] = devices[u1];
			}
			device_count--;
		}
	}
	//check if device is already in the list and if so update last_data_time and return its index
	for(uint8_t u = 0; u < device_count; u++)
		if(id == devices[u].id)
		{
			devices[u].last_data_time = ms;
			return u;
		}
	
	//if not in the list, add it if there is space left and return its index
	if(device_count < MAX_UMYO_DEVICES)
	{
		uint8_t u = device_count;
		devices[u].id = id;
		devices[u].last_data_time = ms;
		device_count++;
		return u;
	}
	//if no space left, return 0xFF
	return 0;
}

void uMyo::parsePack(uint8_t rf_pack[32])
{
	uint8_t *in_pack = rf_pack+1; //ignore 1st byte

	uint8_t data_id = in_pack[0];
	uint8_t message_length = in_pack[1];
	uint8_t chk = in_pack[30];
	uint8_t sum = 0;
	for(int x = 0; x < 30; x++)
	{
		sum += in_pack[x];
	}
	if(sum != chk)
	{
		return;
	}
	uint8_t u1 = in_pack[2];//32-bit unit ID, unique for every uECG device
	uint8_t u2 = in_pack[3];
	uint8_t u3 = in_pack[4];
	uint8_t u4 = in_pack[5];
	uint32_t id = (u1<<24) | (u2<<16) | (u3<<8) | u4;
	uint8_t u = idToIdx(id);
	devices[u].last_data_id = data_id;

	uint8_t ppos = 6;
	devices[u].batt_mv = 2000 + 10 * in_pack[ppos++];
	int muscle_avg = 0;
	for(uint8_t x = 0; x < 4; x++)
	{
		if(x == 0)
		{
			devices[u].cur_spectrum[x] = in_pack[ppos]<<8;
			uint16_t muscle_avg = in_pack[ppos+1];
			devices[u].device_avg_muscle_level = (muscle_avg*muscle_avg)>>3;
		}
		else
			devices[u].cur_spectrum[x] = (in_pack[ppos]<<8)|in_pack[ppos+1];
		ppos += 2;
	}
	int16_t w, x, y, z;
	w = (in_pack[ppos]<<8)|in_pack[ppos+1]; ppos += 2;
	x = in_pack[ppos++]<<8;
	y = in_pack[ppos++]<<8;
	z = in_pack[ppos++]<<8;
	devices[u].Qsg.w = 0.00003125*(float)w;
	devices[u].Qsg.x = 0.00003125*(float)x;
	devices[u].Qsg.y = 0.00003125*(float)y;
	devices[u].Qsg.z = 0.00003125*(float)z;
	q_renorm(&devices[u].Qsg);

	devices[u].raw_data[0] = (in_pack[ppos]<<8)|in_pack[ppos+1]; ppos += 2;
	uint8_t sign_byte = in_pack[ppos+7];
	for(uint8_t x = 0; x < 7; x++)
	{
		if(sign_byte & 1<<(6-x)) devices[u].raw_data[1+x] = devices[u].raw_data[0] - in_pack[ppos++];
		else devices[u].raw_data[1+x] = devices[u].raw_data[0] + in_pack[ppos++];
	}
}
uint8_t uMyo::getDeviceCount()
{
	return device_count;
}
int uMyo::getBattery(uint8_t devidx)
{
	if(devidx < device_count) return devices[devidx].batt_mv;
	return 0;
}
uint32_t uMyo::getID(uint8_t devidx)
{
	if(devidx < device_count) return devices[devidx].id;
	return 0;
}
uint8_t uMyo::getDataID(uint8_t devidx)
{
	if(devidx < device_count) return devices[devidx].last_data_id;
	return 0;
}
float uMyo::getMuscleLevel(uint8_t devidx)
{
	if(devidx >= device_count) return 0;
	float lvl = devices[devidx].cur_spectrum[2] + 2*devices[devidx].cur_spectrum[3];
	return lvl;
}
float uMyo::getAverageMuscleLevel(uint8_t devidx)
{
	if(devidx >= device_count) return 0;
	return devices[devidx].device_avg_muscle_level;
}
void uMyo::getSpectrum(uint8_t devidx, float *spectrum)
{
	if(devidx >= device_count) return;
	for(uint8_t x = 0; x < 4; x++) spectrum[x] = devices[devidx].cur_spectrum[x];
}
void uMyo::getRawData(uint8_t devidx, int16_t *data)
{
	if(devidx >= device_count) return;
	for(uint8_t x = 0; x < 8; x++) data[x] = devices[devidx].raw_data[x];
}
float uMyo::getPitch(uint8_t devidx)
{
	if(devidx >= device_count) return 0;
	sV nzg = nz;
	sQ Qgs;
	Qgs.w = devices[devidx].Qsg.w;
	Qgs.x = -devices[devidx].Qsg.x;
	Qgs.y = -devices[devidx].Qsg.y;
	Qgs.z = -devices[devidx].Qsg.z;
	rotate_v(&Qgs, &nzg);
	return acos_f(v_dot(&nzg, &ny)) + M_PI;
}
float uMyo::getRoll(uint8_t devidx)
{
	if(devidx >= device_count) return 0;
	sV nzg = nz;
	sQ Qgs;
	Qgs.w = devices[devidx].Qsg.w;
	Qgs.x = -devices[devidx].Qsg.x;
	Qgs.y = -devices[devidx].Qsg.y;
	Qgs.z = -devices[devidx].Qsg.z;
	rotate_v(&Qgs, &nzg);
	return atan2_f(nzg.z, nzg.x) + M_PI;
}
float uMyo::getYaw(uint8_t devidx)
{
	if(devidx >= device_count) return 0;
	sV nyr = ny;
	rotate_v(&devices[devidx].Qsg, &nyr);
	return atan2_f(nyr.y, nyr.x) + M_PI;
}


