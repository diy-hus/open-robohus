#include "armcontrol.h"

int run = 1;
/*
 ham inRange dung de khong che vong quay cua servo, tranh tinh trang crash
*/
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
/*
idle la ham dua cac servo cua canh tay ve vi tri goc ban dau khi khoi tao
thuong de goc ban dau la luc canh tay o vi tri vuong goc luc dau
*/

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
/*
pickUp la ham hai qua (sai so, chieu cao cua qua, khoang cach tu robot den qua)
Sai so duoc tinh tu ket qua xu ly anh, no la sai so giua tam cua qua va tam cua robot
Sai so error duoc dung de dieu khien servo 4 la sevo quay canh tay sang trai, sang phai
----
Sau khi ket qua xu ly anh chung ta se cho robot dung lai o cac distance khac nhau phu thuoc vao robot
Do chieu cao cua qua tuong ung can gap -> ta co do sau va do cao, lan luon thay doi gia tri cua xung sevo
den khi tay cham vao qua mot cach thuan loi ta duoc tuong quan do cao theo xung.
Lan luot tay doi do cao theo xung -> chon ra 3 diem thuc nghiem
Fit ham theo cac diem thuc nghiem de tim ra quy luat lay bong
Cho nay khi servo2 quay thi servo1 rat de roi vao goc chet (crash) nen dua servo1 ve goc tuong ung voi xung 1100 - thuc nghiem 
*/

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
/*
drop la ham tha qua ra khoi tay gap, luc do dua canh tay ve thang giong nhu goc ban dau, roi mo tay ra cho qua roi xuong
*/
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

/*
setServo la ham dieu khien servo theo xung can dat duoc theo cac buoc thay doi
Vi du muon servo nhan 1 xung la 1200, neu dua ngay vao xung do thi servo se chuyen ngay lap tuc den goc tuong ung -> bi giat
dua step vao no se len goc mot cach tu tu
*/
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
