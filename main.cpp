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

enum class Blink {
    NONE, LEFT, RIGHT
};

enum class SpeedChange{
    NONE, ACCELERATING, DECCELERATING
};

static volatile bool blinkLedState = true;

void InitBlinkTimer() {
    TA1CTL = TASSEL_2 | ID__2 | MC_2 | TAIE;
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_ISR(){
    volatile uint8_t dummy = TA1IV;
    blinkLedState = !blinkLedState;
}

void SetLEDs(Blink blink, SpeedChange speed){
    //state machine
    switch(blink){
    case Blink::NONE:
        if(speed == SpeedChange::ACCELERATING) {
            Board.SetFrontLeftLight(1);
            Board.SetFrontRightLight(1);
            Board.SetRearLeftLight(0);
            Board.SetRearRightLight(0);
        } else if(speed == SpeedChange::DECCELERATING) {
            Board.SetFrontLeftLight(0);
            Board.SetFrontRightLight(0);
            Board.SetRearLeftLight(1);
            Board.SetRearRightLight(1);
        } else {
            Board.SetFrontLeftLight(0);
            Board.SetFrontRightLight(0);
            Board.SetRearLeftLight(0);
            Board.SetRearRightLight(0);
        }
        break;

    case Blink::LEFT:
        if(speed == SpeedChange::ACCELERATING) {
            Board.SetRearLeftLight(0);
            Board.SetFrontLeftLight(blinkLedState);
            Board.SetFrontRightLight(1);
            Board.SetRearRightLight(0);
        } else if(speed == SpeedChange::DECCELERATING) {
            Board.SetRearLeftLight(0);
            Board.SetFrontLeftLight(blinkLedState);
            Board.SetFrontRightLight(0);
            Board.SetRearRightLight(1);
        } else {
            Board.SetRearLeftLight(0);
            Board.SetFrontLeftLight(blinkLedState);
            Board.SetFrontRightLight(0);
            Board.SetRearRightLight(0);
        }
        break;

    case Blink::RIGHT:
        if(speed == SpeedChange::ACCELERATING) {
            Board.SetRearRightLight(0);
            Board.SetFrontRightLight(blinkLedState);
            Board.SetFrontLeftLight(1);
            Board.SetRearLeftLight(0);
        } else if(speed == SpeedChange::DECCELERATING) {
            Board.SetRearRightLight(0);
            Board.SetFrontRightLight(blinkLedState);
            Board.SetFrontLeftLight(0);
            Board.SetRearLeftLight(1);
        } else {
            Board.SetRearRightLight(0);
            Board.SetFrontRightLight(blinkLedState);
            Board.SetFrontLeftLight(0);
            Board.SetRearLeftLight(0);
        }
        break;
    }
}

void main(void) {
    static const uint16_t MIN_SPEED = 360;
    static const uint16_t MAX_SPEED = 395;
    static const int MAX_ACCELERATION = 250;

    float currentSpeed = MIN_SPEED;

    Board.Init();
    Engine.Init();
    Accelerometer.Init();

    Board.SetRearLeftLight(1);
    Board.SetRearRightLight(1); //rear on
    for(volatile long i=100000; i>0; i--); //wait
    Board.SetFrontLeftLight(1);
    Board.SetFrontRightLight(1); //all on
    Accelerometer.CalibrateZero(50, 1000); //calibrate for zero offset
    Board.SetFrontLeftLight(0);
    Board.SetFrontRightLight(0);
    Board.SetRearLeftLight(0);
    Board.SetRearRightLight(0); //all off

    InitBlinkTimer();
    __enable_interrupt();
    Blink blinker = Blink::NONE;
    SpeedChange speedChange = SpeedChange::NONE;

    while(1) {
        Accelerometer.ReadData();
        int16_t currentY = Accelerometer.GetAccelYmG();
        if(currentY < -300) blinker = Blink::LEFT;
        else if(currentY > 300) blinker = Blink::RIGHT;
        if((currentY > -50 && blinker == Blink::LEFT) || (currentY < 50 && blinker == Blink::RIGHT)) blinker = Blink::NONE;

        speedChange = SpeedChange::NONE;
        if(abs(currentY) < (MAX_ACCELERATION-100)) { //in allowed boundaries of side acceleration
            if(currentSpeed < MAX_SPEED){ //accelerate if not at max speed
                currentSpeed += 0.4;
                speedChange = SpeedChange::ACCELERATING;
            }
        }
        else if(abs(currentY) >= MAX_ACCELERATION){ //road curve
            if(currentSpeed > MIN_SPEED) {
                speedChange = SpeedChange::DECCELERATING;
                currentSpeed = MIN_SPEED;
            }
        }

        Engine.Forward(currentSpeed);
        SetLEDs(blinker, speedChange);
    }
}
