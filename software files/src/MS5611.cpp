#include "MS5611.h"

/**
 * @brief Performs initialization routines.
 *
 * @param osr Oversampling rate for MS5611.
 * @return A boolean value indicating whether the initialization was successful or not.
 *
 * This function initializes the MS5611 sensor and performs necessary initialization routines.
 * First, it starts the Wire library and sends a reset signal to the sensor. Then, it sets
 * the specified oversampling rate and waits for a certain period of time. Finally, it
 * retrieves calibration data and returns a boolean value indicating the success of the operation.
 */
bool MS5611::begin(MS5611_osr osr) {
    Wire.begin();

    performReset();

    setOversampling(osr);

    delay(100);

    getCalibrationData();

    return true;
}

MS5611::MS5611() {
    
}

/**
 * @brief Sets the oversampling rate for the MS5611 sensor.
 *
 * @param osr Oversampling rate to be set.
 *
 * This function sets the oversampling rate for the MS5611 sensor based on the provided parameter.
 * The oversampling rate determines the number of samples taken by the sensor for each measurement,
 * affecting the resolution and accuracy of the readings. The function uses a switch statement to
 * determine the appropriate counter value based on the oversampling rate parameter. The counter
 * value is then assigned to the `counter` member variable and the `userOversamplingRate` member
 * variable is updated with the provided oversampling rate.
 */
void MS5611::setOversampling(MS5611_osr osr) {
    switch (osr)
    {
    case ULTRA_LOW_POWER:
    counter = 1;
        break;
    case LOW_POWER:
    counter = 2;
        break;
    case STANDARD:
    counter = 3;
        break;
    case HIGH_RES:
    counter = 5;
        break;
    case ULTRA_HIGH_RES:
    counter = 10;
        break;
    default:
        break;
    }
    userOversamplingRate = osr;
}

/**
 * @brief Retrieves the current oversampling rate set for the MS5611 sensor.
 *
 * @return The current oversampling rate as an MS5611_osr enum value.
 *
 * This function returns the current oversampling rate that has been set for the MS5611 sensor.
 * The oversampling rate determines the number of samples taken by the sensor for each measurement,
 * affecting the resolution and accuracy of the readings. The function simply returns the value of
 * the `userOversamplingRate` member variable, which holds the current oversampling rate.
 */
uint8_t MS5611::getOversampling(void) {
    return static_cast<uint8_t>(userOversamplingRate);
}

/**
 * @brief Performs a reset operation on the MS5611 sensor.
 *
 * This function sends a reset signal to the MS5611 sensor to perform a reset operation.
 * It starts a transmission using the Wire library to the MS5611 sensor's address and writes
 * the reset command (MS5611_RESET). Finally, it ends the transmission.
 */
void MS5611::performReset(void) {
    Wire.beginTransmission(MS5611_ADDRESS);
    Wire.write(MS5611_RESET);
    Wire.endTransmission();
}

/**
 * @brief Retrieves calibration data from the MS5611 sensor.
 *
 * This function retrieves calibration data from the MS5611 sensor by reading the
 * appropriate registers. It uses a for loop to iterate over the calibration data
 * registers and reads 16 bits of data from each register. The retrieved data is then
 * stored in the `filterCoefficient` array, which holds the calibration coefficients
 * used for sensor measurements.
 */ 
void MS5611::getCalibrationData(void) {
    for(uint8_t offset = 0; offset < 6; offset++) {
        filterCoefficient[offset] = readRegister16(MS5611_READ_PROM + (offset * 2));
    }
}

/**
 * @brief Reads the raw temperature value from the MS5611 sensor.
 *
 * @return The raw temperature value as a 32-bit unsigned integer.
 *
 * This function reads the raw temperature value from the MS5611 sensor. It starts a transmission
 * using the Wire library to the MS5611 sensor's address and writes the conversion command (MS5611_CONV_D2)
 * along with the user-specified oversampling rate. It then ends the transmission and waits for a certain
 * delay based on the oversampling rate. Finally, it reads a 24-bit value from the ADC read register
 * (MS5611_ADC_READ) and returns it as a 32-bit unsigned integer.
 */
uint32_t MS5611::readRawTemperature(void) {
    Wire.beginTransmission(MS5611_ADDRESS);
    Wire.write(MS5611_CONV_D2 + userOversamplingRate);
    Wire.endTransmission();
    delay(counter);
    return readRegister24(MS5611_ADC_READ);
}

/**
 * @brief Reads the raw pressure value from the MS5611 sensor.
 *
 * @return The raw pressure value as a 32-bit unsigned integer.
 *
 * This function reads the raw pressure value from the MS5611 sensor. It starts a transmission
 * using the Wire library to the MS5611 sensor's address and writes the conversion command (MS5611_CONV_D1)
 * along with the user-specified oversampling rate. It then ends the transmission and waits for a certain
 * delay based on the oversampling rate. Finally, it reads a 24-bit value from the ADC read register
 * (MS5611_ADC_READ) and returns it as a 32-bit unsigned integer.
 */
uint32_t MS5611::readRawPressure(void) {
    Wire.beginTransmission(MS5611_ADDRESS);
    Wire.write(MS5611_CONV_D1 + userOversamplingRate);
    Wire.endTransmission();
    delay(counter);
    return readRegister24(MS5611_ADC_READ);
}

/**
 * @brief Reads the temperature from the MS5611 sensor.
 *
 * @param compensation Flag to enable temperature compensation.
 * @return The temperature value as a double.
 *
 * This function reads the temperature from the MS5611 sensor. It first reads the raw temperature value
 * using the `readRawTemperature` function and calculates the temperature difference (dT) by subtracting
 * a scaled coefficient from the raw temperature. It then calculates the temperature using a formula that
 * involves the coefficient and the temperature difference. If compensation is enabled, it checks if the
 * temperature is less than 2000 and calculates an additional temperature correction (temperature2). Finally,
 * it subtracts the temperature correction from the calculated temperature and returns the result as a double,
 * scaled by 100.
 */
double MS5611::readTemperature(bool compensation) {
    uint32_t D2 = readRawTemperature();
    int32_t dT = D2 - (uint32_t)filterCoefficient[4] * 256;

    int32_t temperature = 2000 + ((int64_t) dT * filterCoefficient[5]) / 8388608;

    temperature2 = 0;
    if(compensation) {
        if(temperature2 < 2000) {
            temperature2 = (dT * dT) / pow(2, 31);
        }
    }
    temperature = temperature - temperature2;

    return ((double)temperature/100);
}

/**
 * @brief Reads the pressure from the MS5611 sensor.
 *
 * @param compensation Flag to enable pressure compensation.
 * @return The pressure value as a 32-bit signed integer.
 *
 * This function reads the pressure from the MS5611 sensor. It first reads the raw pressure value using the
 * `readRawPressure` function. It then reads the raw temperature value using the `readRawTemperature` function
 * and calculates the temperature difference (dT) by subtracting a scaled coefficient from the raw temperature.
 * It calculates the offset and sensitivity using formulas that involve the coefficients and the temperature
 * difference. If compensation is enabled, it calculates additional offset and sensitivity corrections (offset2
 * and sensitivity2) based on the temperature. It subtracts the corrections from the offset and sensitivity.
 * Finally, it calculates the pressure using a formula that involves the raw pressure, sensitivity, and offset,
 * and returns the result as a 32-bit signed integer.
 */
int32_t MS5611::readPressure(bool compensation) {
    uint32_t D1 = readRawPressure(); // D1 is a variable used for pressure measurement

    uint32_t D2 = readRawTemperature(); // D2 is a variable used for temperature measurement
    int32_t dT = D2 - (uint32_t)filterCoefficient[4] * 256;

    int64_t offset = (int64_t)filterCoefficient[1] * 65536 + (int64_t)filterCoefficient[3] * dT / 128;
    int64_t sensitivity = (int64_t)filterCoefficient[0] * 32768 + (int64_t)filterCoefficient[2] * dT / 256;

    if(compensation) {
        int32_t temperature = 2000 + ((int64_t) dT * filterCoefficient[5]) / 8388608;

        offset2 = 0;
        sensitivity2 = 0;

        if(temperature < 2000) {
            offset2 = 5 * ((temperature - 2000) * (temperature - 2000)) / 2;
	        sensitivity2 = 5 * ((temperature - 2000) * (temperature - 2000)) / 4;
        }
        if(temperature < -1500) {
        	offset2 = offset2 + 7 * ((temperature + 1500) * (temperature + 1500));
	        sensitivity2 = sensitivity2 + 11 * ((temperature + 1500) * (temperature + 1500)) / 2;
        }
        offset = offset - offset2;
	    sensitivity = sensitivity - sensitivity2;
    }
    
    uint32_t pres = (D1 * sensitivity / 2097152 - offset) / 32768;
    return pres;
}

/**
 * @brief Calculates the altitude based on the pressure and sea level pressure.
 *
 * @param pressure The current pressure value.
 * @param seaLevelPressure The sea level pressure value.
 * @return The altitude value as a double.
 *
 * This function calculates the altitude based on the pressure and sea level pressure. It uses the barometric
 * formula to estimate the altitude. The formula involves dividing the current pressure by the sea level
 * pressure, raising it to a power of 0.1902949, subtracting the result from 1, and multiplying it by a factor
 * of 44330.0. The calculated altitude is then returned as a double.
 */
double MS5611::getAltitude(double pressure, double seaLevelPressure) {
    return (44330.0f * (1.0f - pow((double)pressure / (double)seaLevelPressure, 0.1902949f)));
}

/**
 * @brief Calculates the sea level pressure based on the current pressure and altitude.
 *
 * @param pressure The current pressure value.
 * @param altitude The current altitude value.
 * @return The sea level pressure value as a double.
 *
 * This function calculates the sea level pressure based on the current pressure and altitude. It uses the barometric
 * formula to estimate the sea level pressure. The formula involves dividing the current pressure by the result of
 * subtracting the current altitude divided by a factor of 44330.0 from 1, raising the result to a power of 5.255, and
 * multiplying it by the current pressure. The calculated sea level pressure is then returned as a double.
 */
double MS5611::getSeaLevel(double pressure, double altitude) {
    return ((double)pressure / pow(1.0f - ((double)altitude / 44330.0f), 5.255f));
}

/**
 * @brief Reads a 16-bit register value from the MS5611 sensor.
 *
 * @param reg The register address to read from.
 * @return The register value as a 16-bit unsigned integer.
 *
 * This function reads a 16-bit register value from the MS5611 sensor. It begins by initiating a transmission to the
 * MS5611 sensor using the I2C communication protocol. It sends the register address to read from. After ending the
 * transmission, it requests 2 bytes of data from the MS5611 sensor. It then reads the high byte and low byte of the
 * register value from the Wire buffer. Finally, it combines the high byte and low byte to form the 16-bit register
 * value and returns it as a 16-bit unsigned integer.
 */
uint16_t MS5611::readRegister16(uint8_t reg) {
    uint16_t value;
    Wire.beginTransmission(MS5611_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(MS5611_ADDRESS, 2);

    uint8_t valueHigh = Wire.read();
    uint8_t valueLow = Wire.read();
// if value = 0 it will be false
    value = (valueHigh << 8) | valueLow;
    return value;
}

/**
 * @brief Reads a 24-bit register value from the MS5611 sensor.
 *
 * @param reg The register address to read from.
 * @return The register value as a 24-bit unsigned integer.
 *
 * This function reads a 24-bit register value from the MS5611 sensor. It begins by initiating a transmission to the
 * MS5611 sensor using the I2C communication protocol. It sends the register address to read from. After ending the
 * transmission, it requests 3 bytes of data from the MS5611 sensor. It then reads the extra byte, high byte, and low
 * byte of the register value from the Wire buffer. Finally, it combines the bytes to form the 24-bit register value
 * and returns it as a 24-bit unsigned integer.
 */
uint32_t MS5611::readRegister24(uint8_t reg) {
    uint32_t value;
    Wire.beginTransmission(MS5611_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(MS5611_ADDRESS, 2);

    uint8_t valueXbyte = Wire.read();
    uint8_t valueHigh = Wire.read();
    uint8_t valueLow = Wire.read();

    value = ((int32_t)valueXbyte << 16) | ((int32_t)valueHigh << 8) | valueLow;
    return value;
}