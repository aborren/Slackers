#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>

ZumoBuzzer SLKBuzzer;
Pushbutton SLKButton(ZUMO_BUTTON);
ZumoMotors SLKMotors;
ZumoReflectanceSensorArray SLKReflectanceSensors;

int lastError = 0;

const int MAX_SPEED = 300;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  SLKButton.waitForButton();
  SLKReflectanceSensors.init();
  customCalibration();
}

void loop() {
  unsigned int sensors[6];

  int position = SLKReflectanceSensors.readLine(sensors);

  int error = position - 2500;

  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;

  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > MAX_SPEED)
    m1Speed = MAX_SPEED;
  if (m2Speed > MAX_SPEED)
    m2Speed = MAX_SPEED;
  
  SLKMotors.setSpeeds(m1Speed, m2Speed);
  
}

void customCalibration()
{
  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      SLKMotors.setSpeeds(-200, 200);
    else
      SLKMotors.setSpeeds(200, -200);
    SLKReflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  SLKMotors.setSpeeds(0,0);
}
 
