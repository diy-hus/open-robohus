#ifndef CONFIG_H
#define CONFIG_H

using namespace std;
class Config
{
public:
	static bool DISPLAY_STREAM;
	
	static bool DISPLAY_COLOR_THRESHOLD;
	
	static bool DISPLAY_PID_CONFIG; 

	static float ALPHA;
	
	static float VELOCITY;
	
    static float MAX_VELOCITY;
	
	static float KP;
	
	static float KI;
	
	static float KD;	

    static bool MOTOR;

    static bool ARM;

    static const int RED = 0;
    static const int BLUE = 1;
    static const int GREEN = 2;

    static int lowRed[];
    static int highRed[];

    static int lowGreen[];
    static int highGreen[];

    static int lowBlue[];
    static int highBlue[];

    static int BTN1;
	static int BTN2;
	static int BTN3;
	static int BTN4;
	
	static int sPWM1_1;
	static int sPWM1_2;
	static int sPWM2_1;
	static int sPWM2_2;
	
	static int SERVO1;
	static int SERVO2;
	static int SERVO3;
	static int SERVO4;
};

#endif // CONFIG_H
