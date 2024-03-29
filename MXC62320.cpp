/*
 * MXC62320.cpp
 *
 *  Created on: 16 Nov 2019
 *      Author: jiris
 */

#include "MXC62320.h"

void MXC62320::Init(){
    i2c->Init();
    //turn on, disable self test
    i2c->StartWrite(DEVICE_ADDR);
    i2c->Write(0x0); //value 0
    i2c->EndTransmission();
}

void MXC62320::ReadData(){
    uint8_t readData[5];
    //turn on, disable self test
    i2c->StartWrite(DEVICE_ADDR);
    i2c->Write(0x0); //value 0
    i2c->EndTransmission();
    i2c->StartRead(DEVICE_ADDR);
    i2c->ReadBuffer(readData, 5);
    i2c->EndTransmission();

    internalReg = readData[0];
    accelX = (readData[1] << 8) | readData[2];
    accelY = (readData[3] << 8) | readData[4];

    accelX -= accelXoffset;
    accelY -= accelYoffset;
}

int16_t MXC62320::GetAccelXmG() {
    int16_t tmp = accelX - 2048;
    //return tmp / 0.512f; //CCS nadava na floaty, zmetek jeden
    return 2 * tmp;
}

int16_t MXC62320::GetAccelYmG() {
    int16_t tmp = accelY - 2048;
    //return tmp / 0.512f; //CCS nadava na floaty, zmetek jeden
    return 2 * tmp;
}

void MXC62320::CalibrateZero(int numMeasurements, unsigned long delay){
    uint32_t tmpX = 0, tmpY = 0;
    for(int i=0; i<numMeasurements; i++){
        ReadData();
        tmpX += accelX;
        tmpY += accelY;
        for(volatile unsigned long t = 0; t < delay; t++);
    }
    tmpX /= numMeasurements;
    tmpY /= numMeasurements;

    accelXoffset = tmpX - 2048;
    accelYoffset = tmpY - 2048;

}
