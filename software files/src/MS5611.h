#ifndef MS5611_h
#define MS5611_h

#include "Arduino.h"
#include "Wire.h"

#define MS5611_ADDRESS 0x77

#define MS5611_ADC_READ 0x00
#define MS5611_RESET 0x1E
#define MS5611_CONV_D1 0x40
#define MS5611_CONV_D2 0x50
#define MS5611_READ_PROM 0xA2

    enum MS5611_osr {
        ULTRA_HIGH_RES   = 0x08,
        HIGH_RES         = 0x06,
        STANDARD         = 0x04,
        LOW_POWER        = 0x02,
        ULTRA_LOW_POWER  = 0x00
    };

class MS5611 {
public:
    MS5611();
    bool begin(MS5611_osr osr = HIGH_RES);
    uint32_t readRawTemperature(void);
    uint32_t readRawPressure(void);
    double readTemperature(bool compensation = false);
    int32_t readPressure(bool compensation = false);
    double getAltitude(double pressure, double seaLevelPressure = 101325);
    double getSeaLevel(double pressure, double altitude);
    void setOversampling(MS5611_osr osr);
    uint8_t getOversampling(void);
    void getCalibrationData(void);
private:
    uint16_t filterCoefficient[6];
    uint8_t counter;
    uint8_t userOversamplingRate;
    int32_t temperature2;
    int64_t offset2, sensitivity2;
    MS5611_osr osr;

    void performReset(void);


	uint16_t readRegister16(uint8_t reg);
	uint32_t readRegister24(uint8_t reg);
};

#endif