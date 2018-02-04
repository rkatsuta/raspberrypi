
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "BME280I2C.h"
#include "TSL2561I2C.h"


//---------------------------------
// Constant
#define STRLENGTH 256

//---------------------------------
// Function

int main(int argc, char *argv[]){
	int opt;
	char logPath[STRLENGTH];
	bool optLog = false;
	bool optDetail = false;

	while(-1!=(opt=getopt(argc, argv, "vl:"))){
		switch(opt){
		case 'v':
			optDetail = true;
			break;
		case 'l':
			strcpy(logPath, optarg);
			optLog = true;
			break;
		default:
			printf("Unknown option %c\n", opt);
			return 1;
		}
	}

	// Measurement start
	BME280I2C bme280Ch1(0x76);
	BME280I2C bme280Ch2(0x77);
	TSL2561I2C tsl2561(0x29);

	bme280Ch1.meas();
	bme280Ch2.meas();
	tsl2561.atime = TSL2561I2C::atime402ms;
	tsl2561.again = TSL2561I2C::againHigh;
	bool tsl2561result = tsl2561.measSingle();

	// Both ch1 and ch2 not available
	if((!bme280Ch1.getResult()) && (!bme280Ch2.getResult()) && (!tsl2561result)){
		printf("No Sensor Available\n");
	}

	// Print result
	if(bme280Ch1.getResult()){
		printf("BME280 (0x76)\n");

		if(optDetail){
			bme280Ch1.printCalibration();
			bme280Ch1.printRegister();
			printf("Measured Data\n");
		}
		bme280Ch1.printMeas();
		printf("\n");
	}

	if(bme280Ch2.getResult()){
		printf("BME280 (0x77)\n");

		if(optDetail){
			bme280Ch2.printCalibration();
			bme280Ch2.printRegister();
			printf("Measured Data\n");
		}
		bme280Ch2.printMeas();
		printf("\n");
	}
	
	if(tsl2561result){
		printf("TSL2561\n");
		if(optDetail){
			tsl2561.printRegister();
			printf("Measured Data\n");
		}
		tsl2561.printMeas();
	}

	// Log date and time
	if(optLog){
		time_t timer=time(NULL);
		struct tm*local = localtime(&timer);
		struct stat s;
		bool logExist = false;
		if(stat(logPath, &s)==0){
			logExist = true;
		}
		FILE*fOut = fopen(logPath, "a");

		if(NULL == fOut){
			printf("Failed to open log file\n");
			return 1;
		}

		// Write header
		if(!logExist){
			fprintf(fOut, "Time, Temp ch1, Temp ch2, Pressure ch1, Pressure ch2, Humidity ch1, Humidity ch2, Lux\n");
		}

		// Time
		fprintf(fOut, "%d/%02d/%02d ", local->tm_year+1900, local->tm_mon+1, local->tm_mday);
		fprintf(fOut, "%02d:%02d, ", local->tm_hour, local->tm_min);

		// Temp
		if(bme280Ch1.getResult()){
			fprintf(fOut, "%.1f,", bme280Ch1.getMeas(BME280I2C::selectTemp));
		}else{
			fprintf(fOut, ",");
		}
		if(bme280Ch2.getResult()){
			fprintf(fOut, "%.1f,", bme280Ch2.getMeas(BME280I2C::selectTemp));
		}else{
			fprintf(fOut, ",");
		}

		// Pressure
		if(bme280Ch1.getResult()){
			fprintf(fOut, "%.2f,", bme280Ch1.getMeas(BME280I2C::selectPressure));
		}else{
			fprintf(fOut, ",");
		}
		if(bme280Ch2.getResult()){
			fprintf(fOut, "%.2f,", bme280Ch2.getMeas(BME280I2C::selectPressure));
		}else{
			fprintf(fOut, ",");
		}

		// Humidity
		if(bme280Ch1.getResult()){
			fprintf(fOut, "%.1f,", bme280Ch1.getMeas(BME280I2C::selectHumidity));
		}else{
			fprintf(fOut, ",");
		}
		if(bme280Ch2.getResult()){
			fprintf(fOut, "%.1f,", bme280Ch2.getMeas(BME280I2C::selectHumidity));
		}else{
			fprintf(fOut, ",");
		}

		// Lux
		if(tsl2561result){
			fprintf(fOut, "%.1f\n", tsl2561.lux);
		}else{
			fprintf(fOut, "\n");
		}
		fclose(fOut);
	}

	return 0;
}
