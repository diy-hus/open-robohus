#ifndef BALLPICKER_H
#define BALLPICKER_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <PCD8544.h>

#include "motorcontrol.h"
#include "detectball.h"
#include "timer.h"
#include "armcontrol.h"
#include "config.h"
#include "mpu9250.h"

using namespace std;
using namespace cv;

class BallPicker{
public:
	BallPicker();
	int start();
	bool isStart();

    void setAngle(int angle);
    void setDuration(int duration);
    void setColor(int color);
	
	enum Mode {
		SEARCH_BALL,
		PICK_BALL
	};
	
private:
	void init();
    void drawBall(const vector<Vec3f> &);
    void process(const vector<Vec3f> &);
    float calcDistance(float radius, float height);
	
	int mode = SEARCH_BALL;

    int low[3] = {0, 0, 0};
    int high[3] = {179, 255, 255};

    float preError = 0;
	
    bool stated = false;
	int ballColor = 0;
    int angle = 0;
    int duration = 10;

	bool run = false;
	int frameSkip = 0;
	
    int WIDTH = 320;
    int HEIGHT = 240;
	Size frame;

	Mat src;
	Timer timer;
	MotorControl motor;
	DetectBall detect;
	ArmControl arm;
	VideoCapture capture;
    MPU9250 sensor;
};

#endif
