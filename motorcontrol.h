#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <softPwm.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

using namespace std;
using namespace cv;

class MotorControl
{
public:
    MotorControl();
    ~MotorControl();
    void init();
    void stop();
    void move_forward(float, float);
    void move_forward(float);
    void move_back(float);
    
protected:

    int bP;
    int bI;
    int bD;

    float kP;
    float kI;
    float kD;

    float max = Config::VELOCITY;
    float min = -Config::VELOCITY;

    float integral = 0;
    float pre_error = 0;
    float pre_veloc_error = 0;

    int pidCalculate(float);
    void left_forward(int val);
    void right_forward(int val);
    void left_back(int val);
    void right_back(int val);
    int middle(int value, int min, int max);
};

#endif // MOTORCONTROL_H
