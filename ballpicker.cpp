#include "ballpicker.h"

BallPicker::BallPicker(): capture(VideoCapture(0)),
                        frame(Size(WIDTH, HEIGHT))
{
	init();
}

int BallPicker::start()
{
    float findBallTime = duration;
	while(true){
		char key = waitKey(1);
		
		if (key == 'q') break;
        if (key == 'r') ballColor = Config::RED;
        if (key == 'g') ballColor = Config::GREEN;
        if (key == 'b') ballColor = Config::BLUE;
		
        if(digitalRead (Config::BTN2) == LOW)
		{
            stated = true;
            sensor.calibrateMPU9250();
            delay(250);
		}

        if(digitalRead (Config::BTN3) == LOW)
        {
            delay(250);
            return 1;
        }

        timer.update();
        capture >> src;
        src.convertTo(src, -1, Config::ALPHA / 100.0f, 0);
        resize(src, src, frame);

        if (Config::DISPLAY_COLOR_THRESHOLD)
        {
            Mat imgHSV, imgThresholded;
            cvtColor(src, imgHSV, COLOR_BGR2HSV);
            if (low[0] > high[0])
            {
                Mat imgThresholded1, imgThresholded2;
                inRange(imgHSV, Scalar(low[0], low[1], low[2]), Scalar(179, high[1], high[2]), imgThresholded1);
                inRange(imgHSV, Scalar(0, low[1], low[2]), Scalar(high[0], high[1], high[2]), imgThresholded2);
                bitwise_or(imgThresholded1, imgThresholded2, imgThresholded);
            }
            else inRange(imgHSV, Scalar(low[0], low[1], low[2]), Scalar(high[0], high[1], high[2]), imgThresholded);

            imshow("Threshold", imgThresholded);
        }
		
        if (stated){
			if (frameSkip > 0) frameSkip--;
            if (mode == PICK_BALL)
            {
                detect.detectBall(src, ballColor);
                process(detect.getBallList());
            }
            else
            {
                sensor.updateGyro();
                float error = sensor.angleGyro - angle;
                findBallTime -= timer.getDeltaTime();
                if (findBallTime <= 0) mode = PICK_BALL;
                error = - error / 5.0f;
                cout << error << endl;
                motor.move_forward(error, 1);
            }
		}

        int fps = cvRound(1.0 / timer.getDeltaTime());
        putText(src, to_string(fps), Point2f(5,15), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0));
        imshow( "Origin", src);
    }
    capture.release();
    return 0;
}

bool BallPicker::isStart()
{
    return run;
}

void BallPicker::setAngle(int angle)
{
    this->angle = angle;
}

void BallPicker::setDuration(int duration)
{
    this->duration = duration;
}

void BallPicker::init()
{
    sensor.initMPU9250();

	namedWindow( "Origin", CV_WINDOW_AUTOSIZE );

    if (Config::DISPLAY_COLOR_THRESHOLD)
    {
        namedWindow( "Threshold", CV_WINDOW_AUTOSIZE );

        cvCreateTrackbar("LowH", "Threshold", &low[0], 179);
        cvCreateTrackbar("HighH", "Threshold", &high[0], 179);

        cvCreateTrackbar("LowS", "Threshold", &low[1], 255);
        cvCreateTrackbar("HighS", "Threshold", &high[1], 255);

        cvCreateTrackbar("LowV", "Threshold", &low[2], 255);
        cvCreateTrackbar("HighV", "Threshold", &high[2], 255);
    }
    
    pinMode (Config::BTN1, INPUT);
	pullUpDnControl (Config::BTN1, PUD_UP) ;
	
	pinMode (Config::BTN2, INPUT);
	pullUpDnControl (Config::BTN2, PUD_UP) ;
	
	pinMode (Config::BTN3, INPUT);
	pullUpDnControl (Config::BTN3, PUD_UP) ;

	pinMode (Config::BTN4, INPUT);
	pullUpDnControl (Config::BTN4, PUD_UP) ;
	
	motor.init();
	
	arm.idle();
}

void BallPicker::process(const vector<Vec3f> &ballList)
{
    float x, y;
    float max_radius = 0;
    if (preError == 0)
    {
        for( size_t i = 0; i < ballList.size(); i++ )
        {
            if (ballList[i][2] > max_radius){
                x = ballList[i][0];
                y = ballList[i][1];
                max_radius = ballList[i][2];
            }
        }
    }
    else
    {
        float nearest_error = -1;
        for( size_t i = 0; i < ballList.size(); i++ )
        {
            float error = (float) (ballList[i][0] - WIDTH / 2) / (WIDTH / 2);
            if (abs(preError - error) < abs(preError - nearest_error)){
                x = ballList[i][0];
                y = ballList[i][1];
                max_radius = ballList[i][2];
                nearest_error = error;
            }
        }
        if (nearest_error == -1) preError = 0;
    }

    if (ballList.size() > 0) {
        line(src, Point(WIDTH / 2, HEIGHT / 2), Point(cvRound(x), cvRound(y)), Scalar(0,255,0), 2, 4, 0);

        float dstDistance = 25.0f;

		float error = (float) (x - WIDTH / 2) / (WIDTH / 2);
        float yPos = HEIGHT / 2 - y;
        float distance = calcDistance(max_radius, yPos);

        if (yPos < 0) dstDistance = 20.0f;

        if (distance > dstDistance && Config::MOTOR) {
            float x = dstDistance / distance;
            x = 1 - pow(x, 5);
            motor.move_forward(error, x);
        }
        else {
            motor.stop();
            if (frameSkip == 0 && Config::ARM){
                delay(100);
                arm.pickUp(error, yPos, distance);
                delay(100);
                motor.move_back(0);
                delay(2500);
                motor.stop();
                delay(100);
                arm.drop();
                delay(100);
                frameSkip = 10;
            }
        }
    } else {
        motor.stop();
    }
	
	if (Config::DISPLAY_STREAM){
		line(src, Point(WIDTH / 2 - 5, HEIGHT / 2), Point(WIDTH / 2 + 5, HEIGHT/ 2), Scalar(255,255,255), 2, 8, 0);
		line(src, Point(WIDTH / 2, HEIGHT / 2 - 5), Point(WIDTH / 2, HEIGHT/ 2 + 5), Scalar(255,255,255), 2, 8, 0);

        drawBall(ballList);
    }
}

float BallPicker::calcDistance(float radius, float high)
{
    static float f = 141.25f;
    static float height = 4.0f;
    return f * height / radius;
}

void BallPicker::drawBall(const vector<Vec3f> &balls)
{
	for( size_t i = 0; i < balls.size(); i++ )
    {
        Point center(cvRound(balls[i][0]), cvRound(balls[i][1]));
        int radius = cvRound(balls[i][2]);

        circle(src, center, 3, Scalar(255,0,0), -1, 8, 0 );
        circle(src, center, radius, Scalar(0,0,255), 2, 8, 0 );
    }
    
}
