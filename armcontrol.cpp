#include "armcontrol.h"

#define RAD_TO_DEG 180 / 3.14159265359

int run = 1;

int inRange(int value, int min = 500, int max = 2400);

int inRange(int value, int min, int max)
{
    if (value > max) value = max;
    else if (value < min) value = min;
    return value;
}

int angle2Pulse(float angle) { // deg
    return round(-10 * angle + 2400);
}

float pulse2Angle(int pulse) {
    return -0.1 * pulse + 240;
}

void stop(int signum)
{
    run = 0;
    gpioServo(Config::SERVO1, 0);
    gpioServo(Config::SERVO2, 0);
    gpioServo(Config::SERVO3, 0);
    gpioTerminate();
}

ArmControl::ArmControl()
{
    if (gpioInitialise() < 0) return;
    gpioSetSignalFunc(SIGINT, stop);
}

void ArmControl::originState()
{
    gpioServo(Config::SERVO1, 900);
    time_sleep(0.5);

    gpioServo(Config::SERVO2, 1300);
    time_sleep(0.5);

    gpioServo(Config::SERVO3, 700);
    time_sleep(0.5);

    gpioServo(Config::SERVO4, 1500);
    time_sleep(0.5);
}

void ArmControl::pickUp(float error, float b, float a)
{	
    a -= 1; // distance from origin arm to camera
    b -= 12; // height

    a *= 10; // cm to mm
    b *= 10;
    double servo1Angle = 2 * (atan2(270*a - sqrt(-pow(a, 4) - 2*pow(a, 2)*pow(b, 2) + 79668*pow(a, 2) - pow(b, 4) +79668 * pow(b, 2) - 11451456), pow(a, 2) + pow(b, 2) + 270 * b - 3384));
    double servo2Angle = 2 * (atan2(294*b - sqrt(-pow(a, 4) - 2*pow(a, 2)*pow(b, 2) + 79668*pow(a, 2) - pow(b, 4) +79668 * pow(b, 2) - 11451456), pow(a, 2) + pow(b, 2) + 294 * a + 3384));
    servo1Angle = pulse2Angle(Config::SERVO1_ORIG) - servo1Angle * RAD_TO_DEG;
    servo2Angle = pulse2Angle(Config::SERVO2_ORIG) - servo2Angle * RAD_TO_DEG;

    setServo(Config::SERVO1, 120);
    setServo(Config::SERVO4, 90 - error * 80);
    time_sleep(0.5);

    time_sleep(0.2);

    setServo(Config::SERVO2, servo2Angle);
    time_sleep(0.2);

    setServo(Config::SERVO3, 100);
    time_sleep(0.2);

    setServo(Config::SERVO1, servo1Angle, 2.0f);
    time_sleep(0.2);

    setServo(Config::SERVO3, 180);
    time_sleep(0.2);
}

void ArmControl::drop()
{
    setServo(Config::SERVO1, 140, 10);
    time_sleep(0.2);
    setServo(Config::SERVO4, 0);
    time_sleep(0.5);
    setServo(Config::SERVO3, 60, 0);
    time_sleep(0.5);
    setServo(Config::SERVO3, 175, 0);
    time_sleep(0.5);
    setServo(Config::SERVO4, 90);
    time_sleep(0.5);
    setServo(Config::SERVO2, 110);
    time_sleep(0.5);
}

void ArmControl::setServo(int servo, float angle, float step)
{
    if (step != 0) {
        while (abs(gpioGetServoPulsewidth(servo) - angle2Pulse(angle)) > step * 10 / 2)
        {
            if (gpioGetServoPulsewidth(servo) > angle2Pulse(angle))
            {
                gpioServo(servo, gpioGetServoPulsewidth(servo) - step * 10);
            } else {
                gpioServo(servo, gpioGetServoPulsewidth(servo) + step * 10);
            }
            time_sleep(0.02);
        }
    }
    gpioServo(servo, angle2Pulse(angle));
}
