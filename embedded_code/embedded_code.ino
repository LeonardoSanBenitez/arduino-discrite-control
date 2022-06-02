
// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define DEBUG                         true
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define USE_TIMER_1                   true
#define USE_TIMER_2                   true
#define USING_LOOP_TEST               false
#define SAMPLE_RATE_MS                5
#define REFERENCE_RATE_MS             100
#define ANALOG_IN_PIN                 A0
#define ANALOG_OUT_PIN                9

#if (DEBUG)
volatile int i = 0; // iteration counter
#endif

volatile float reference = 1;




#include "TimerInterrupt.h"


void control_law()
{
  static bool state = false;
  static float last_measurement = 0; // already in volts
  static float error_n_minus_2 = 0;
  static float error_n_minus_1 = 0;
  static float error_n = 0;
  static float u_n_minus_2 = 0;
  static float u_n_minus_1 = 0;
  static float u = 0;

  state = !state;

  //#if (DEBUG)
  //Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
  //#endif

  
  // Read
  // between 0 and 1023, 0 to 5V
  last_measurement = analogRead(ANALOG_IN_PIN) * (5.0 / 1023.0);

  error_n_minus_2 = error_n_minus_1;
  error_n_minus_1 = error_n;
  error_n = last_measurement - reference;
  u_n_minus_2 = u_n_minus_1;
  u_n_minus_1 = u;
  
  // Calculate
  //u[n] = 0.9329*erro[n] - 0.9243*erro[n-1] + 0.4899*erro[n-2] + 1.17*u[n-1] - 0.1696*u[n-2]
  u = 0.9329*error_n - 0.9243*error_n_minus_1 + 0.4899*error_n_minus_2 + 1.17*u_n_minus_1 - 0.1696*u_n_minus_2;
  
  
  // Write
  // between 0 and 255, 0 to 5V (mean voltage)
  // 490 Hz (pins 4 and 13: 980 Hz)
  // PWM pins: 2 - 13, 44 - 46
  analogWrite(ANALOG_OUT_PIN, u * 255/5); 
  digitalWrite(LED_BUILTIN, state);
  //Serial.println("> eq");
  i++;
}

void change_reference(){
  if (reference > 1.25){
    reference = 1;
  } else {
    reference = 1.5;
  }

  #if (DEBUG)
  //delay(1000); //1s
  Serial.print(i);
  Serial.print("\tRef=");
  Serial.print(reference);
  Serial.print("\n");


  i = 0;
  #endif
}

void setup()
{ 
  pinMode(ANALOG_IN_PIN, INPUT);
  pinMode(ANALOG_OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  ITimer1.init();
  ITimer2.init();

  /*******************/
  /* Configure Timer */ 
  if (ITimer1.attachInterruptInterval(SAMPLE_RATE_MS, control_law)){
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  } else{
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
  }

  if (ITimer2.attachInterruptInterval(REFERENCE_RATE_MS, change_reference)){
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  } else{
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
  }
}

void loop()
{
}