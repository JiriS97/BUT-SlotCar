/*
 * BoardLL.h
 *
 *  Created on: 16 Nov 2019
 *      Author: jiris
 */

#ifndef BOARDLL_H_
#define BOARDLL_H_

#include <msp430.h>
#include <stdint.h>

class BoardLL{
public:
    void Init();

    void SetFrontLeftLight(bool on);
    void ToggleFrontLeftLight();

    void SetFrontRightLight(bool on);
    void ToggleFrontRightLight();

    void SetRearLeftLight(bool on);
    void ToggleRearLeftLight();

    void SetRearRightLight(bool on);
    void ToggleRearRightLight();
};

#endif /* BOARDLL_H_ */
