#include "ballpicker.h"

BallPicker::BallPicker(): capture(VideoCapture(0)),
                        frame(Size(WIDTH, HEIGHT))
{
	init();
}

int BallPicker::start()
{
    digitalWrite(Config::BL, HIGH);
    delay(250);
    digitalWrite(Config::BL, LOW);
    delay(250);
    digitalWrite(Config::BL, HIGH);
    delay(250);
    digitalWrite(Config::BL, LOW);
	while(true){
		char key = waitKey(1);
		
		if (key == 'q') break;
        if (key == 'r') started = true;
		
        if(digitalRead (Config::BTN2) == LOW)
		{
            started = true;
            if (!motor.move(0, duration)) {
                motor.stop();
                exit(0);
            }
            motor.stop();
            delay(500);
            if (!motor.rotateTo(angle * 100 / 180)) {
                motor.stop();
                exit(0);
            }
            motor.stop();
            motor.reset();
            frameSkip = 20;
		}

        if(digitalRead (Config::BTN3) == LOW)
        {
            delay(250);
            return 1;
        }

        if(digitalRead (Config::BTN1) == LOW)
        {
            arm.pickUp(0, 25, 15);
        }

        timer.update();
        motor.update();
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
		
        if (started){
			if (frameSkip > 0) frameSkip--;
            if (frameSkip == 0) {
                detect.detectBall(src, ballColor);
                process(detect.getBallList());
            }
		}

        if (Config::DISPLAY_STREAM) {
            int fps = cvRound(1.0 / timer.getDeltaTime());
            putText(src, to_string(fps), Point2f(5,15), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0));
            imshow( "Origin", src);
        }
    }
    capture.release();
    return 0;
}

bool BallPicker::isStart()
{
    return run;
}

void BallPicker::setZone(int zone)
{
    this->zone = zone;
}

void BallPicker::setAngle(int angle)
{
    this->angle = angle;
}

void BallPicker::setDuration(int duration)
{
    this->duration = duration;
}

void BallPicker::setColor(int color)
{
    this->ballColor = color;
}

void BallPicker::setOffset(float x, float y)
{
    this->offsetX = x;
    this->offsetY = y;
}

void BallPicker::init()
{
    namedWindow( "Origin", WINDOW_NORMAL );

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

    pinMode (Config::BL, OUTPUT);

    motor.setCamera(&capture, frame);
	
	motor.init();
	
	arm.originState();
}

void BallPicker::process(const vector<Vec3f> &ballList)
{
    float x, y;
    float radius = 0;
    if (preError == 0)
    {
        for( size_t i = 0; i < ballList.size(); i++ )
        {
            int yPos = HEIGHT / 2 - ballList[i][1];
            float distance = calcDistance(ballList[i][2], yPos);
            float height = calcHeight(distance, yPos);
            if (height < 5) continue;

            if (ballList[i][2] > radius){
                x = ballList[i][0];
                y = ballList[i][1];
                radius = ballList[i][2];
                preError = (float) (ballList[i][0] - WIDTH / 2) / (WIDTH / 2);
            }
        }
    }
    else
    {
        float nearest_error = -1;
        for( size_t i = 0; i < ballList.size(); i++ )
        {
            float error = (float) (ballList[i][0] - WIDTH / 2) / (WIDTH / 2);
            int yPos = HEIGHT / 2 - ballList[i][1];
            float distance = calcDistance(ballList[i][2], yPos);
            float height = calcHeight(distance, yPos);
            if (height < 5) continue;

            if (abs(preError - error) < abs(preError - nearest_error)){
                x = ballList[i][0];
                y = ballList[i][1];
                radius = ballList[i][2];
                nearest_error = error;
            }
        }
        if (nearest_error == -1) {
            preError = 0;
            return;
        }
        else preError = nearest_error;
    }

    if (ballList.size() > 0 && radius != 0) {
        line(src, Point(WIDTH / 2, HEIGHT / 2), Point(cvRound(x), cvRound(y)), Scalar(0,255,0), 2, 4, 0);
        float dstDistance = 28.0f;
        int yPos = HEIGHT / 2 - y;
        float distance = calcDistance(radius, yPos);
        float height = calcHeight(distance, yPos);
        float error = (float) (x - WIDTH / 2) / (WIDTH / 2);

        cout << error << " " << distance << "  " << height << endl;

        if (distance > dstDistance && Config::MOTOR) {
            float x = dstDistance / distance;
            x = 1 - pow(x, 2);
            motor.move_forward(error / x, x);
        }
        else {
            motor.stop();
            if (!isPickUp) {
                delay(500);
                frameSkip = 5;
                isPickUp = true;
                return;
            }
            if (Config::ARM){
                delay(100);
                arm.pickUp(error, distance + offsetX, height + offsetY);
                delay(100);
                arm.release();
             if (!motor.move_back(2)) {
                    motor.stop();
                    exit(0);
                }
                delay(100);
                motor.reset();

                if (zone == 0)
                {
                    if (!motor.rotateTo(-180)) {
                        motor.stop();
                        exit(0);
                    }
                }
                else
                {
                    if (!motor.rotateTo(180)) {
                        motor.stop();
                        exit(0);
                    }
                }

                delay(500);
                arm.drop();
                delay(100);
                if (!motor.rotateTo(0)) {
                    motor.stop();
                    exit(0);
                }
                frameSkip = 10;
                isPickUp = false;
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

float BallPicker::calcDistance(float radius, int yPos)
{
    static float f = 148.0f;
    static float width = 4.0f;
    float angle = atan((float) yPos / (HEIGHT / 2) * tan(24.4 * DEG_TO_RAD));

    radius -= 17.2864 * pow(angle, 2) - 0.9397 * angle; // camera calibrate

    float distance = f * width / radius * cos(angle + 8 * DEG_TO_RAD);
    return distance;
}

float BallPicker::calcHeight(float distance, int yPos)
{
    float angle = atan((float) yPos / (HEIGHT / 2) * tan(24.4 * DEG_TO_RAD)) + 8 * DEG_TO_RAD;
    float height = distance * tan(angle) + 10 + 4 * tan(angle);
    return height;
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
