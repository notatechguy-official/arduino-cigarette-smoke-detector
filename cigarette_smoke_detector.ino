#include <Servo.h>

Servo servo;
int timer = 0;
bool smokeDetected = false;

void setup() {
  Serial.begin(9600);

  servo.attach(8); /* Servo connected at digital pin 8 */
  servo.write(0); /* Reset servo's position to 0 */
  
  delay(1000);
}

/* This loop will run for every 1 second */
void loop() {
  servo.write(0);

  /* Read the Parts-per-million (PPM) value of MQ135 Air Quality Sensor from analog pin 0 */
  int gasPPM = analogRead(0);

  /* 
   *  Generally, if the indoor PPM is above 190, the air quality = moderate.
   *  This is my threshold for detecting cigarette smoke. Adjust this to suit your own use case.
   */
  if (gasPPM > 190 && smokeDetected == false) {
    /* 
     *  If PPM > 190 (smoke is detected currently) AND smoke has never been detected for the past 5 minutes, tilt the servo to 25 degree angle.
     *  The servo will switch on the standing fan's power.
     */
    servo.write(25);
    smokeDetected = true;
  }
  
  /* Start the timer when smoke is detected, so that we can reset the smoke detection status after 5 minutes. */
  if (smokeDetected) {   
    timer++;
  }

  /* 
   *  Once the timer has reached 300 seconds (5 minutes), reset the smoke detection status and timer,
   *  so that the servo can be re-activated if a smoke has been detected again.
   *  This is to prevent the servo from activating for every second if the PPM remains bad.
   */
  if (timer >= 300) {
    smokeDetected = false;
    timer = 0;
  }
  
  delay(1000);
}
