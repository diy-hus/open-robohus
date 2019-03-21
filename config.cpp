#include "config.h"

// Camera Debug

bool Config::DISPLAY_STREAM = true;

bool Config::DISPLAY_COLOR_THRESHOLD = false;

bool Config::DISPLAY_PID_CONFIG = false; 

int Config::BL = 30;

// Active

bool Config::MOTOR = true;
bool Config::ARM = true;

// Servo Origin
int Config::SERVO1_ORIG = 1400;
int Config::SERVO2_ORIG = 1325;
int Config::SERVO3_ORIG = 600;
int Config::SERVO4_ORIG = 1500;

// Color Threshold

int Config::lowRed[] = {160, 50, 50};
int Config::highRed[] = {5, 255, 255};

int Config::lowGreen[] = {40, 105, 60};
int Config::highGreen[] = {70, 255, 255};

int Config::lowBlue[] = {80, 60, 15};
int Config::highBlue[] = {125, 255, 255};

// Contrast, velocity, ...

float Config::ALPHA = 100.0f;

float Config::VELOCITY = 50.0f;

float Config::MAX_VELOCITY = 80.0f;
	
float Config::KP1 = 20.0f;
float Config::KI1 = 0.0f;
float Config::KD1 = 5.0f;

float Config::KP2 = 7.0f;
float Config::KI2 = 1.0f;
float Config::KD2 = 5.0f;

// Button Pin

int Config::BTN1 = 7;

int Config::BTN2 = 0;

int Config::BTN3 = 21;

int Config::BTN4 = 22;

// SoftPWM Pin

int Config::sPWM2_1 = 16;

int Config::sPWM2_2 = 6;

int Config::sPWM1_1 = 31;

int Config::sPWM1_2 = 11;

// Servo Pin

int Config::SERVO1 = 19;

int Config::SERVO2 = 18;

int Config::SERVO3 = 13;

int Config::SERVO4 = 12;
