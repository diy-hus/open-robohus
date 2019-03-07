#ifndef DETECTBALL_H
#define DETECTBALL_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "config.h"

using namespace std;
using namespace cv;

class DetectBall
{
public:
    DetectBall();

    void detectBall(const Mat &src, int color);

    vector<Vec3f> getBallList() const;

protected:

    Mat preprocess(const Mat &src, int color);
    void findContour(const Mat &regions);
    Mat morphological(const Mat &image);

    vector<Vec3f> ballList;
};

#endif // DETECTBALL_H
