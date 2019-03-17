#include <iostream>
#include <stdlib.h>

#include "ballpicker.h"

using namespace std;

int main(int argc, char *argv[])
{
    setenv("WIRINGPI_GPIOMEM", "1", 1);
    wiringPiSetup();

    int angle = 0;
    int duration = 7;
    int ballColor = Config::RED;

    if (argc == 4) {
        angle = atoi(argv[1]);
        duration = atoi(argv[2]);
        ballColor = atoi(argv[3]);
    }

    cout << "Angle   : " << angle << endl;
    cout << "Duration: " << duration << endl;
    cout << "Color   : " << ballColor << endl;

    BallPicker ballPicker;
    ballPicker.setAngle(angle);
    ballPicker.setDuration(duration);
    ballPicker.setColor(ballColor);

    int result = ballPicker.start();
    return result;
}
