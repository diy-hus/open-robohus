#include "detectball.h"

DetectBall::DetectBall() {}

void DetectBall::detectBall(const Mat &src, int color)
{
    Mat regions = preprocess(src, color);

    findContour(regions);
}

Mat DetectBall::preprocess(const Mat &src, int color)
{
    Mat imgHSV, imgThresholded;
    cvtColor(src, imgHSV, COLOR_BGR2HSV);

    int *low = Config::lowRed;
    int *high = Config::highRed;

    if (color == Config::RED)
    {
        low = Config::lowRed;
        high = Config::highRed;
    }
    else if (color == Config::BLUE)
    {
        low = Config::lowBlue;
        high = Config::highBlue;
    }
    else
    {
        low = Config::lowGreen;
        high = Config::highGreen;
    }

    if (low[0] > high[0]) {
        Mat imgThresholded1, imgThresholded2;
        inRange(imgHSV, Scalar(low[0], low[1], low[2]), Scalar(179, high[1], high[2]), imgThresholded1);
        inRange(imgHSV, Scalar(0, low[1], low[2]), Scalar(high[0], high[1], high[2]), imgThresholded2);
        bitwise_or(imgThresholded1, imgThresholded2, imgThresholded);
    }
    else
    {
        inRange(imgHSV, Scalar(low[0], low[1], low[2]), Scalar(high[0], high[1], high[2]), imgThresholded);
    }

    imgThresholded = morphological(imgThresholded);

    return imgThresholded;
}

void DetectBall::findContour(const Mat &image)
{
    ballList.clear();
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// Find contours
    findContours( image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }

    for( int i = 0; i < contours.size(); i++ )
    {
        if (radius[i] < image.rows/4)
        {
            if (contourArea(Mat(contours[i])) > radius[i] * radius[i] * M_PI * 0.6)
            {
                ballList.push_back(Vec3f(center[i].x, center[i].y, radius[i]));
            }
        }
    }
}

Mat DetectBall::morphological(const Mat &image)
{
    Mat imgThresholded;

    //morphological opening (remove small objects from the foreground)
    morphologyEx( image, imgThresholded, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //fill small holes in the foreground
    Mat im_floodfill = imgThresholded.clone();
    floodFill(im_floodfill, cv::Point(0,0), Scalar(255));

    Mat im_floodfill_inv;
    bitwise_not(im_floodfill, im_floodfill_inv);

    imgThresholded = (imgThresholded | im_floodfill_inv);

    return imgThresholded;
}

vector<Vec3f> DetectBall::getBallList() const
{
    return ballList;
}




