#include "motorcontrol.h"

MotorControl::MotorControl()
{
}
/*
Initial Pusle and direction for motor.
Using soft PWM, any hardpin can be use
P,I,D factor can be change in here by active trackbar in config file
*/

void MotorControl::init()
{
    softPwmCreate(Config::sPWM1_1, 0, 100);
    softPwmCreate(Config::sPWM1_2, 0, 100);
    softPwmCreate(Config::sPWM2_1, 0, 100);
    softPwmCreate(Config::sPWM2_2, 0, 100);

    bP = cvRound(Config::KP * 100);
    bI = cvRound(Config::KI * 100);
    bD = cvRound(Config::KD * 100);
    
    if (Config::DISPLAY_PID_CONFIG){
        cvCreateTrackbar("kP", "Origin", &bP, 5000);
        cvCreateTrackbar("kI", "Origin", &bI, 1000);
        cvCreateTrackbar("kD", "Origin", &bD, 1000);
    }
}

MotorControl::~MotorControl()
{
    stop();
}
/*
we need pull_up with 1.2kOhm to Vcc from motor pin
*/
void MotorControl::stop()
{
    softPwmWrite(Config::sPWM1_1, 0);
    softPwmWrite(Config::sPWM1_2, 0);
    softPwmWrite(Config::sPWM2_1, 0);
    softPwmWrite(Config::sPWM2_2, 0);
}

/*
to keep moving robot forward when detect ball, f is limitation factor, f will be decrease if robot closer the ball 
PID affect to error from robot to center of ball
*/
void MotorControl::move_forward(float error, float f)
{

    kP = bP / 100.0;
    kI = bI / 100.0;
    kD = bD / 100.0;
    int change = pidCalculate(error);

    left_forward(Config::VELOCITY * f + change);
    right_forward(Config::VELOCITY * f - change);
}
/*
to keep moving robot forward when start.
PID affect to angle error 
*/
void MotorControl::move_forward(float error)
{
    kP = bP / 100.0;
    kI = bI / 100.0;
    kD = bD / 100.0;
    int change = pidCalculate(error);

    left_forward(Config::MAX_VELOCITY + change);
    right_forward(Config::MAX_VELOCITY - change);
}
/*
to drive robot backward when it picked up the ball
*/

void MotorControl::move_back(float val)
{
    left_back(Config::VELOCITY - val * Config::VELOCITY / 2);
    right_back(Config::VELOCITY + val * Config::VELOCITY / 2);
}

int MotorControl::pidCalculate(float error)
{
    float Pout = kP * error;

    integral += error;
    
    if (integral > 1) integral = 1;
    else if (integral < -1) integral = -1;
    float Iout = kI * integral;

    float Dout = kD * (error - pre_error);

    int output = round(Pout + Iout + Dout);

    output = middle(output, min, max);

    pre_error = error;

    return output;
}

void MotorControl::left_forward(int val)
{
    softPwmWrite(Config::sPWM1_1, val);
    softPwmWrite(Config::sPWM1_2, 0);
}

void MotorControl::right_forward(int val)
{
    softPwmWrite(Config::sPWM2_1, val);
    softPwmWrite(Config::sPWM2_2, 0);
}

void MotorControl::left_back(int val)
{
    softPwmWrite(Config::sPWM1_1, 0);
    softPwmWrite(Config::sPWM1_2, val);
}

void MotorControl::right_back(int val)
{
    softPwmWrite(Config::sPWM2_1, 0);
    softPwmWrite(Config::sPWM2_2, val);
}

int MotorControl::middle(int value, int min, int max)
{
    if( value > max )
        value = max;
    else if( value < min )
        value = min;
    return value;
}
