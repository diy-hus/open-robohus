#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <softPwm.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "mpu9250.h"
#include "timer.h"

using namespace std;
using namespace cv;

class MotorControl
{
public:
    MotorControl();
    ~MotorControl();
    void init();
    void stop();
    void update();
    void reset();
    void move_forward(float, float, int velocity = Config::VELOCITY);
    void move(float, float);
    void rotateTo(float angle);
    void move_back(float);
    
protected:

    MPU9250 sensor;

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

    int pidCalculate(float, float kP, float kI, float kD);
    void left_forward(int val);
    void right_forward(int val);
    void left_back(int val);
    void right_back(int val);
    int middle(int value, int min, int max);
};

#endif // MOTORCONTROL_H
