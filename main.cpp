#include <msp430.h>
#include <stdint.h>

#include "BoardLL.h"
#include "Motor.h"
#include "SoftI2C.h"
#include "MXC62320.h"

BoardLL Board;
Motor Engine;
SoftI2C IIC;
MXC62320 Accelerometer(&IIC);

void main(void) {
    Board.Init();
    Engine.Init();
    Accelerometer.Init();

    volatile int16_t x,y;
    while(1){
        Accelerometer.ReadData();
        x = Accelerometer.GetAccelXmG();
        y = Accelerometer.GetAccelYmG();

        Board.SetRearRightLight(y > 100);
        Board.SetRearLeftLight(y < -100);

        Board.SetFrontLeftLight(x < -100);
        Board.SetFrontRightLight(x > 100);
        for(volatile long i=20; i>0; i--);
    }
}
