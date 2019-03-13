#include "config.h"

// Camera Debug

bool Config::DISPLAY_STREAM = true;

bool Config::DISPLAY_COLOR_THRESHOLD = false; //for debug HSV value of frame

bool Config::DISPLAY_PID_CONFIG = true; //we can changing P, I, D factor while robot moving

// Active

bool Config::MOTOR = true;
bool Config::ARM = true;

// Color Threshold - need calibrate before running

int Config::lowRed[] = {160, 50, 50};
int Config::highRed[] = {5, 255, 255};

int Config::lowGreen[] = {40, 105, 60};
int Config::highGreen[] = {70, 255, 255};

int Config::lowBlue[] = {80, 60, 15};
int Config::highBlue[] = {125, 255, 255};

// Contrast, velocity, ...

float Config::ALPHA = 100.0f;

float Config::VELOCITY = 50.0f; //30f initial velocity of finding ball moving

float Config::MAX_VELOCITY = 100.0f; //initial velocity of start moving
	
float Config::KP = 30.0f;
	
float Config::KI = 1.0f;
	
float Config::KD = 5.0f;

// Button Pin

int Config::BTN1 = 7;

int Config::BTN2 = 0;

int Config::BTN3 = 21;

int Config::BTN4 = 22;

// SoftPWM Pin

int Config::sPWM1_1 = 16;

int Config::sPWM1_2 = 6;

int Config::sPWM2_1 = 31;

int Config::sPWM2_2 = 11;

// Servo Pin

int Config::SERVO1 = 19;

int Config::SERVO2 = 18;

int Config::SERVO3 = 13;

int Config::SERVO4 = 12;
