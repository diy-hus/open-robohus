#include "armcontrol.h"

#define RAD_TO_DEG 180 / 3.14159265359

int run = 1;

float inRange(float value, float min = 0, float max = 180);

float inRange(float value, float min, float max)
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
    gpioServo(Config::SERVO4, 0);
    gpioTerminate();
}

ArmControl::ArmControl()
{
    if (gpioInitialise() < 0) return;
    gpioSetSignalFunc(SIGINT, stop);
}

void ArmControl::originState()
{
    setServo(Config::SERVO1, pulse2Angle(Config::SERVO1_ORIG) + 37.5, 0);
    time_sleep(0.5);

    setServo(Config::SERVO2, pulse2Angle(Config::SERVO2_ORIG), 0);
    time_sleep(0.5);

    setServo(Config::SERVO3, pulse2Angle(Config::SERVO3_ORIG), 0);
    time_sleep(0.5);

    setServo(Config::SERVO4, pulse2Angle(Config::SERVO4_ORIG), 0);
    time_sleep(0.5);
}

void ArmControl::pickUp(float error, float a, float b)
{	
    a -= 3; // distance from origin arm to camera
    b -= 14; // height

    a *= 10; // cm to mm
    b *= 10;
    double servo1Angle = 2 * (atan2(270*a - sqrt(-pow(a, 4) - 2*pow(a, 2)*pow(b, 2) + 79668*pow(a, 2) - pow(b, 4) +79668 * pow(b, 2) - 11451456), pow(a, 2) + pow(b, 2) + 270 * b - 3384));
    double servo2Angle = 2 * (atan2(294*b - sqrt(-pow(a, 4) - 2*pow(a, 2)*pow(b, 2) + 79668*pow(a, 2) - pow(b, 4) +79668 * pow(b, 2) - 11451456), pow(a, 2) + pow(b, 2) + 294 * a + 3384));
    servo1Angle = pulse2Angle(Config::SERVO1_ORIG) - servo1Angle * RAD_TO_DEG;
    servo2Angle = pulse2Angle(Config::SERVO2_ORIG) - servo2Angle * RAD_TO_DEG;

    setServo(Config::SERVO1, pulse2Angle(Config::SERVO1_ORIG) + 7.5);
    time_sleep(0.2);
    setServo(Config::SERVO4, pulse2Angle(Config::SERVO4_ORIG) - error * 80);
    time_sleep(0.5);

    setServo(Config::SERVO2, servo2Angle);
    time_sleep(0.2);

    setServo(Config::SERVO3, pulse2Angle(Config::SERVO3_ORIG) - 75);
    time_sleep(0.2);

    setServo(Config::SERVO1, servo1Angle, 2.0f);
    time_sleep(0.2);

    setServo(Config::SERVO3, pulse2Angle(Config::SERVO3_ORIG));
    time_sleep(0.2);

    setServo(Config::SERVO1, pulse2Angle(Config::SERVO1_ORIG) + 37.5, 2);
    time_sleep(0.2);
}

void ArmControl::drop()
{
    setServo(Config::SERVO3, pulse2Angle(Config::SERVO3_ORIG) - 75, 0);
    time_sleep(0.5);
    setServo(Config::SERVO3, pulse2Angle(Config::SERVO3_ORIG), 0);
    time_sleep(0.5);
    setServo(Config::SERVO4, pulse2Angle(Config::SERVO4_ORIG));
    time_sleep(0.5);
    setServo(Config::SERVO2, pulse2Angle(Config::SERVO2_ORIG));
    time_sleep(0.5);
}

void ArmControl::setServo(int servo, float angle, float step)
{
    angle = inRange(angle);
    if (step != 0 && gpioGetServoPulsewidth(servo) != 0) {
        while (abs(gpioGetServoPulsewidth(servo) - angle2Pulse(angle)) > step * 10 / 2)
        {
            if (gpioGetServoPulsewidth(servo) > angle2Pulse(angle))
            {
                gpioServo(servo, round(inRange(gpioGetServoPulsewidth(servo) - step * 10, 600, 2400)));
            } else {
                gpioServo(servo, round(inRange(gpioGetServoPulsewidth(servo) + step * 10, 600, 2400)));
            }
            time_sleep(0.02);
        }
    }
    gpioServo(servo, angle2Pulse(angle));
}

void ArmControl::release()
{
  /*  setServo(Config::SERVO1, pulse2Angle(Config::SERVO1_ORIG) + 37.5, 0);
    time_sleep(0.5);

    setServo(Config::SERVO2, pulse2Angle(Config::SERVO2_ORIG), 0);
    time_sleep(0.5);

    setServo(Config::SERVO3, pulse2Angle(Config::SERVO3_ORIG), 0);
    time_sleep(0.5);

    setServo(Config::SERVO4, pulse2Angle(Config::SERVO4_ORIG), 0);
    time_sleep(0.5);
*/
    gpioServo(Config::SERVO1, 0);
    gpioServo(Config::SERVO2, 0);
    gpioServo(Config::SERVO4, 0);
}
