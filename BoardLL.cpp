/*
 * BoardLL.cpp
 *
 *  Created on: 16 Nov 2019
 *      Author: jiris
 */

#include "BoardLL.h"

void BoardLL::Init(){
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    SetFrontLeftLight(0);
    SetFrontRightLight(0);
    SetRearLeftLight(0);
    SetRearRightLight(0);

    P1DIR |= (1 << 2) | (1 << 3);             // P1.2, P1.3 output
    P4DIR |= (1 << 2);                        // P4.2 output
    P3DIR |= (1 << 6);                        // P3.6 output
}

void BoardLL::SetFrontRightLight(bool on) {
    if(on) P1OUT |= 1 << 3;
    else P1OUT &= ~(1 << 3);
}
void BoardLL::ToggleFrontRightLight() {
    P1OUT ^= 1 << 3;
}

void BoardLL::SetFrontLeftLight(bool on) {
    if(on) P1OUT |= 1 << 2;
    else P1OUT &= ~(1 << 2);
}
void BoardLL::ToggleFrontLeftLight() {
    P1OUT ^= 1 << 2;
}

void BoardLL::SetRearRightLight(bool on) {
    if(on) P3OUT |= 1 << 6;
    else P3OUT &= ~(1 << 6);
}
void BoardLL::ToggleRearRightLight() {
    P3OUT ^= 1 << 6;
}

void BoardLL::SetRearLeftLight(bool on) {
    if(on) P4OUT |= 1 << 2;
    else P4OUT &= ~(1 << 2);
}
void BoardLL::ToggleRearLeftLight() {
    P4OUT ^= 1 << 2;
}

