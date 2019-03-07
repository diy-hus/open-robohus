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
		
	void idle();
	
	void pickUp(float, float, float);
	
	void drop();
	
	void setServo(int, int, int step = 10);
		};

#endif // ARMCONTROL_H

