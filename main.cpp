#include <iostream>
#include <stdlib.h>

#include "ballpicker.h"

using namespace std;

int main(int argc, char *argv[])
{
    setenv("WIRINGPI_GPIOMEM", "1", 1);
    wiringPiSetup();

    int angle = 0;
    int duration = 6;
    int ballColor = Config::RED;
    float offsetX = 0;
    float offsetY = 0;

    if (argc >= 4) {
        angle = atoi(argv[1]);
        duration = atoi(argv[2]);
        ballColor = atoi(argv[3]);
    }

    if (argc == 6) {
        offsetX = atoi(argv[4]);
        offsetY = atoi(argv[5]);
    }

    cout << "Angle   : " << angle << endl;
    cout << "Duration: " << duration << endl;
    cout << "Color   : " << ballColor << endl;

    BallPicker ballPicker;
    ballPicker.setAngle(angle);
    ballPicker.setDuration(duration);
    ballPicker.setColor(ballColor);
    ballPicker.setOffset(offsetX, offsetY);

    int result = ballPicker.start();
    return result;
}
