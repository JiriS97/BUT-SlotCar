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

static const float THRESHOLD_COEF = 0.33f; //0...2
static const uint16_t MOTOR_SPEED = 350; //1000
static const int SCALE = 2048;
static const unsigned DATA_LEN = 4000; //40 seconds
static const unsigned WINDOW_LEN = 400; //2 seconds
int16_t collectedData[DATA_LEN];
int16_t *window = collectedData;

int32_t CalculateDivideFactorFromWindow(){
    int32_t sum = 0;
    for(int i=0; i<WINDOW_LEN; i++){
        sum += (int32_t)(window[i]) * (int32_t)(window[i]);
    }
    return sum / SCALE;
}

int32_t CalculateCorrelationForPoint(int pointIndex){
    int32_t tmp = 0;
    for(int j = 0; j < WINDOW_LEN; j++){
        tmp += (int32_t)(collectedData[pointIndex + j]) * (int32_t)(window[j]);
    }
    return tmp;
}

void main(void) {
    Board.Init();
    Engine.Init();
    Accelerometer.Init();

    Engine.Forward(MOTOR_SPEED);
    for(volatile long i=50000; i>0; i--); //pauza na dosazeni potrebne rychlosti
    Board.SetRearLeftLight(1);

    bool similaritiesFound = false;
    int32_t divFactor = 0;
    int32_t threshold = 0;
    for(int i=0; i<(DATA_LEN-WINDOW_LEN); i++){
        Board.ToggleRearLeftLight();
        Accelerometer.ReadData();
        collectedData[i] = Accelerometer.GetAccelYmG();

        if(i < WINDOW_LEN){
            volatile int32_t corrForPoint = CalculateCorrelationForPoint(WINDOW_LEN) / 123L;
        }
        else if(i == WINDOW_LEN) {
            Board.SetRearRightLight(1);
            divFactor = CalculateDivideFactorFromWindow();
            threshold = THRESHOLD_COEF * divFactor;
        }
        else if (i < 2*WINDOW_LEN){
            volatile int32_t corrForPoint = CalculateCorrelationForPoint(WINDOW_LEN) / 123L;
        }
        else if(i > 2*WINDOW_LEN){
            Board.SetFrontLeftLight(1);
            Board.SetFrontRightLight(1);
            int32_t corrForPoint = CalculateCorrelationForPoint(i-WINDOW_LEN) / divFactor;
            if(corrForPoint > threshold){
                similaritiesFound = true;
                break;
            }
        }

        //for(volatile long i=2000; i>0; i--); //10ms pauza !!!!!!!!!!!!!!!!!!
    }

    Engine.Break();
    if(similaritiesFound){
        Board.SetFrontLeftLight(1);
        Board.SetFrontRightLight(1);
    } else {
        Board.SetFrontLeftLight(0);
        Board.SetFrontRightLight(0);
    }
    while(1);

//    volatile int16_t x,y;
//    while(1){
//        Accelerometer.ReadData();
//        x = Accelerometer.GetAccelXmG();
//        y = Accelerometer.GetAccelYmG();
//
//        Board.SetRearRightLight(y > 100);
//        Board.SetRearLeftLight(y < -100);
//
//        Board.SetFrontLeftLight(x < -100);
//        Board.SetFrontRightLight(x > 100);
//        for(volatile long i=20; i>0; i--);
//    }
}
