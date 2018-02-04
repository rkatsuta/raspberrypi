//---------------------------------
// 2017/6/14

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "TSL2561I2C.h"


//---------------------------------
// Public Class Function

TSL2561I2C::TSL2561I2C(int i2cAdr){
	ch0 = 0;
	ch1 = 0;
	lux = 0;
	again = againHigh;
	atime = atime402ms;

	// Open I2C
	i2c = open("/dev/i2c-1",O_RDWR);
	if(i2c<0){
		return;
	}
	
	// Set I2C Address
	if(ioctl(i2c, I2C_SLAVE, i2cAdr)<0){
		close(i2c);
		return;
	}

}


// Read 1Byte from register
uint8_t TSL2561I2C::readAddress(uint8_t addr){
	uint8_t buf[1];
	buf[0] = addr | 0x80;
	if(write(i2c,buf,1) == 1){
		read(i2c,buf,1);
	}
	return buf[0];
}

// Read length Byte from register
void TSL2561I2C::readAddress(uint8_t addr, uint8_t*data, uint8_t length){
	uint8_t buf[1];
	buf[0] = addr|0x80;
	if(write(i2c,buf,1) == 1){
		read(i2c,data,length);
	}
	return;
}

// Write 1Byte to register
void TSL2561I2C::writeAddress(uint8_t addr, uint8_t data){
	uint8_t buf[2];
	buf[0]=addr|0x80;
	buf[1]=data;
	write(i2c,buf,2);
	return;
}


// Clear ALS and no persist ALS interrupt
void TSL2561I2C::clearInt(){
	uint8_t buf[1];
	buf[0]=0xC0;
	write(i2c,buf,1);
	return;
}

// ID Read
//  Return true if ok
bool TSL2561I2C::idRead(){
	uint8_t id = readAddress(0xA);
	//printf("ID : %X\n",id);
	id = id>>4;

	// TSL2561 ID
	if(id == 0x1 || id == 0x5){
		return true;
	}
	return false;
}


// Set gain and atime
void TSL2561I2C::timing(uint8_t again, uint8_t atime){
	writeAddress(0x1, (again<<4) | atime);
}


// Start ALS integrateion (measurement) with fixed again and atime
void TSL2561I2C::alsIntegration(){
	uint8_t data[4];
	writeAddress(0x0, 0x0);	// Power OFF
	timing(again, atime);
	writeAddress(0x0, 0x3);	// Power ON
	switch(atime){
	case atime13ms:
		usleep(20000);
		break;
	case atime101ms:
		usleep(120000);
		break;
	case atime402ms:
		usleep(430000);
		break;
	}
	readAddress(0xC, data, 4);
	writeAddress(0x0, 0x0);	// Power OFF
	ch0 = data[1]<<8 | data[0];
	ch1 = data[3]<<8 | data[2];
}


// One time measurement
//  Return true if success
//  update ch0, ch1 and lux
//  again and atime automatically changed
bool TSL2561I2C::measSingle(){
	if(idRead()==false){
		return false;
	}
	
	again = againHigh;
	atime = atime101ms;

	while(1){
		alsIntegration();
		if(ch0 > 37000 || ch1 > 37000){
			if(again == againHigh && atime == atime101ms){
				again = againLow;
			}else{
				break;
			}
		}else if(ch0 < 300 || ch1 < 300){
			if(again == againHigh && atime == atime101ms){
				atime = atime402ms;
			}else{
				break;
			}
		}else{
			break;
		}
	}
	calcLux();
	return true;
}

// Calculate lux from ch0, ch1, atime and again
void TSL2561I2C::calcLux(){
	float fAtime;
	float fAgain;

	switch (atime){
	case atime13ms:
		fAtime = 13.7;
		break;
	case atime101ms:
		fAtime = 101;
		break;
	case atime402ms:
		fAtime = 402;
		break;
	}

	switch (again)
	{
	case againLow:
		fAgain = 1;
		break;
	case againHigh:
		fAgain = 16;
		break;
	}
	
	float ratio = (float)ch1 / (float)ch0;
	
	// T, FN, CL package
	if(ratio <= 0.5){
		lux = 0.0304*ch0 - (0.062*ch0 * pow(ratio,1.4));
	}else if(ratio <= 0.61){
		lux = 0.0224*ch0 - 0.031*ch1;
	}else if(ratio <= 0.8){
		lux = 0.0128*ch0 - 0.0153*ch1;
	}else if(ratio <=1.3){
		lux = 0.00146*ch0 - 0.00112*ch1;
	}else{
		lux = 0;
	}

	lux = lux * 16/fAgain * 402/fAtime;
}

// Print Register value
void TSL2561I2C::printRegister(){
	printf("Register Data\n");
	switch (atime){
	case atime13ms:
		printf(" ADC Time : 13ms\n");
		break;
	case atime101ms:
		printf(" ADC Time : 101ms\n");
		break;
	case atime402ms:
		printf(" ADC Time : 402ms\n");
		break;
	}
	switch (again)
	{
	case againLow:
		printf(" ADC Gain : Low\n");
		break;
	case againHigh:
		printf(" ADC Gain : High\n");
		break;
	}
	printf(" ch0 : 0x%x\n", ch0);
	printf(" ch1 : 0x%x\n", ch1);
}

// Print Measurement data
void TSL2561I2C::printMeas(){
	printf(" Lux : %.1f\n", lux);
}



















