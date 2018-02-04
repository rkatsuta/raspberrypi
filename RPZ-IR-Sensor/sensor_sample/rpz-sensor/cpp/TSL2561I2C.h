//---------------------------------
// 2017/6/14

#ifndef TSL2561I2C_H
#define TSL2561I2C_H



//---------------------------------
// Class

class TSL2561I2C{
private:
	//---------------------------------
	// Variable

	// I2C
	int i2c;

public:
	//---------------------------------
	// Constant

	// ADC Gain
	static const uint8_t againLow = 0;
	static const uint8_t againHigh = 1;

	// ADC Integration time
	static const uint8_t atime13ms = 0;
	static const uint8_t atime101ms = 1;
	static const uint8_t atime402ms = 2;


	//---------------------------------
	// Variable

	// Measurement result
	uint16_t ch0;
	uint16_t ch1;
	float lux;
	uint8_t again;
	uint8_t atime;


	//---------------------------------
	// Function
	TSL2561I2C(int i2cAdr);
	uint8_t readAddress(uint8_t addr);
	void readAddress(uint8_t addr, uint8_t*data, uint8_t length);
	void writeAddress(uint8_t addr, uint8_t data);
	void clearInt();
	bool idRead();
	void timing(uint8_t again, uint8_t atime);
	void alsIntegration();
	bool measSingle();
	void calcLux();
	void printMeas();
	void printRegister();
};

#endif

