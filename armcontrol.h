#ifndef ARMCONTROL_H
#define ARMCONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>
#include <iostream>
#include <cmath>

#include "config.h"

using namespace std;

class ArmControl
{
public:
    ArmControl();

    void originState();

    void pickUp(float, float, float);

    void drop();

    void setServo(int, float, float step = 5.0f);

    void release();

};

#endif // ARMCONTROL_H

