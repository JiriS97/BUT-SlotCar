/*
 * Motor.cpp
 *
 *  Created on: 16 Nov 2019
 *      Author: jiris
 */

#include "Motor.h"

void Motor::Init(){
    Neutral();

    P8DIR |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5); //outputs
    P8SEL |= 1 << 2; //timer AF on P8.2

    TA0CCR0 = 1000 - 1; //period
    TA0CCR2 = 0; //duty cycle
    TA0CCTL2 = OUTMOD_7; //reset/set
    TA0CTL = TASSEL_2 | MC_1; //start, count up to CCR0
}

void Motor::Forward(uint16_t speed) {
    TA0CCR2 = speed; //duty cycle
    P8OUT = 0xC; //EN=H, D1=L, IN1=H, IN2=L
}

void Motor::Backward(uint16_t speed) {
    TA0CCR2 = speed; //duty cycle
    P8OUT = 0x6; //EN=H, D1=L, IN1=L, IN2=H
}

void Motor::Break() {
    P8OUT = 0x4; //EN=H, D1=L, IN1=L, IN2=L
}

void Motor::Neutral() {
    P8OUT = 0x26; //EN=H, D1=H, IN1=L, IN2=H
}
