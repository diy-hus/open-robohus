Setting wifi using network-manager commandline
Install PCD8544 Library
Git UISystem and open-roboHUS
edit excuteable Fruitpicker path in UISystem
add UISystem auto start to bashrc
add @lxterminal to /etc/xdg/lxsession/LXDE-pi/autostart
-----------
Using Servo file to calibrate center of arm
set permission for Servo file to 777
sudo ./Servo
PCM pin <-> Pulse
500 < Pulse < 2500
First, we should using 1500 pulse
zero point to config file
----------
int Config::SERVO1 = 19;
int Config::SERVO2 = 18;
int Config::SERVO3 = 13;
int Config::SERVO4 = 12;
