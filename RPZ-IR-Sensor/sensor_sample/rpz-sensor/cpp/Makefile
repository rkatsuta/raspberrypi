
TARGET=rpz-sensor
O=main.o BME280I2C.o TSL2561I2C.o
H=BME280I2C.h TSL2561I2C.h

$(TARGET) : $(O) $(H)
	g++ -o $(TARGET) $(O)

.SUFFIXES:.o .cpp
.cpp.o:
	g++ -c $<

.PHONY:clean
clean:
	rm -f $(TARGET)
	rm -f $(O)
