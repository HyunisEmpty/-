#include <Servo.h>

// Arduino pin assignment
#define PIN_POTENTIOMETER 0 // A0번 핀 정의
#define PIN_LED 9           // 핀 9번 LED 정의 
#define PIN_SERVO 10

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100.0   // 최소거리 정의
#define _DIST_MAX 250.0   // 최대거리 정의
#define _EMA_ALPHA 0.5    // EMA 필터 알파값 정의

#define LOOP_INTERVAL 50  // Loop Interval (unit: msec)
float dist_prev = _DIST_MAX;
float dist_ema;

Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;

  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;
  
  a_value = analogRead(PIN_POTENTIOMETER);
  
  // Read IR Sensor value !!!
  // Convert IR sensor value into distance !!!
  float dist = (6762.0/(a_value-9)-4.0)*10.0;
  
  // we need distance range filter here !!!
  if ( dist < _DIST_MIN) {
    dist = _DIST_MIN - 10.0;
    dist = dist_prev;
    digitalWrite(PIN_LED, 1);  
  } else if( dist > _DIST_MAX){
    // 최고값 정의
    dist = _DIST_MAX + 10.0; 
    dist = dist_prev;
    digitalWrite(PIN_LED, 0);
  } else{
    dist_prev = dist;
    digitalWrite(PIN_LED, 0);
  }
  
  // we need EMA filter here !!!
  dist_ema = _EMA_ALPHA * dist + (1 - _EMA_ALPHA) * dist_ema;

  if(dist_ema <= _DIST_MIN){
    digitalWrite(PIN_LED, 1);
    myservo.writeMicroseconds(_DUTY_MIN);
  } else if(dist_ema >= _DIST_MAX){
    digitalWrite(PIN_LED, 1);
    myservo.writeMicroseconds(_DUTY_MAX); // LED OFF
  } else {
    digitalWrite(PIN_LED, 0);
    duty = 1846/150 * (dist - 100) + 533;
    myservo.writeMicroseconds(duty);
  }
  

  // map distance into duty
  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:"); Serial.print(_DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX:"); Serial.print(_DIST_MAX);
  Serial.println("");
  
}
