#include "armcontrol.h"

int run = 1;

int inRange(int value, int min = 500, int max = 2400);

int inRange(int value, int min, int max)
{
	if (value > max) value = max;
	else if (value < min) value = min;
	return value;
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

void ArmControl::idle()
{
	gpioServo(Config::SERVO1, 900);
	time_sleep(0.5);
	
	gpioServo(Config::SERVO2, 2000);
	time_sleep(0.5);

    gpioServo(Config::SERVO3, 900);
	time_sleep(0.5);
	
	gpioServo(Config::SERVO4, 1500);
	time_sleep(0.5);
}

void ArmControl::pickUp(float error, float height, float distance)
{	
    cout << "Height: " << height << ", Distance: " << distance << endl;
    gpioServo(Config::SERVO4, inRange(1500 + error * 800));
    time_sleep(0.5);

    gpioServo(Config::SERVO1, 1100);
    time_sleep(0.2);

    float value2 = 1705.5 + 4.253 * height + 0.017 * height * height;

    setServo(Config::SERVO2, inRange(round(value2), 1300, 2300));
    time_sleep(0.2);

    gpioServo(Config::SERVO3, 1800);
    time_sleep(0.2);

    setServo(Config::SERVO1, 1450);
    time_sleep(0.2);

    gpioServo(Config::SERVO3, 900);
    time_sleep(0.2);
}

void ArmControl::drop()
{
	setServo(Config::SERVO1, 1000, 20);
	time_sleep(0.2);
    gpioServo(Config::SERVO4, 2400);
    time_sleep(0.5);
	gpioServo(Config::SERVO3, 1800);
	time_sleep(0.5);
    gpioServo(Config::SERVO3, 900);
    time_sleep(0.5);
    gpioServo(Config::SERVO4, 1500);
    time_sleep(0.5);
	setServo(Config::SERVO2, 2000);
	time_sleep(0.5);
}

void ArmControl::setServo(int servo, int pulsewidth, int step)
{
	while (abs(gpioGetServoPulsewidth(servo) - pulsewidth) > step / 2 + 1)
	{
		if (gpioGetServoPulsewidth(servo) > pulsewidth)
		{
			gpioServo(servo, gpioGetServoPulsewidth(servo) - step);
		} else {
			gpioServo(servo, gpioGetServoPulsewidth(servo) + step);
		}
		time_sleep(0.02);
	}
}
