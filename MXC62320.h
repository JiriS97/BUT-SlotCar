/*
 * MXC62320.h
 *
 *  Created on: 16 Nov 2019
 *      Author: jiris
 */

#ifndef MXC62320_H_
#define MXC62320_H_

#include <msp430.h>
#include <stdint.h>
#include "SoftI2C.h"

class MXC62320 {
private:
    const uint8_t DEVICE_ADDR = 0x10;
    SoftI2C *i2c;

    uint8_t internalReg;
    uint16_t accelX, accelY;
    uint16_t accelXoffset = 0, accelYoffset = 0;

public:
    MXC62320(SoftI2C *bus) : i2c(bus) {;}

    void Init();
    void ReadData();

    uint8_t GetInternalReg() { return internalReg;}
    uint16_t GetRawAccelX() { return accelX;}
    uint16_t GetRawAccelY() { return accelY;}
    int16_t GetAccelXmG();
    int16_t GetAccelYmG();
    void CalibrateZero(int numMeasurements, unsigned long delay);
};

#endif /* MXC62320_H_ */
