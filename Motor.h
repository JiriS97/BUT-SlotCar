/*
 * Motor.h
 *
 *  Created on: 16 Nov 2019
 *      Author: jiris
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <msp430.h>
#include <stdint.h>

class Motor {
public:
    void Init();

    void Forward(uint16_t speed) ;
    void Backward(uint16_t speed);
    void Break();
    void Neutral();
};

#endif /* MOTOR_H_ */
