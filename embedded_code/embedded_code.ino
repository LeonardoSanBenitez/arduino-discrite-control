
// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define DEBUG         true
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define USE_TIMER_1     true
#define USING_LOOP_TEST       false

#define SAMPLE_RATE_MS    5


#define ANALOG_IN_PIN A0
#define ANALOG_OUT_PIN 9

#if (DEBUG)
int i = 0; // iteration counter
#endif


#include "TimerInterrupt.h"


void control_law()
{
  static bool state = false;
  static int last_measurement = 0;

  state = !state;

  //#if (DEBUG)
  //Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
  //#endif

  // Read
  last_measurement = analogRead(ANALOG_IN_PIN);

  // Calculate
  // TODO
  
  // Write
  // The PWM seems to interfere with our timer...
  analogWrite(ANALOG_OUT_PIN, 128); // 0 to 255? Or 0 to 1023?
  digitalWrite(LED_BUILTIN, state);
  //Serial.println("> eq");
  i++;
}


void setup()
{ 
  pinMode(ANALOG_IN_PIN, INPUT);
  pinMode(ANALOG_OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  ITimer1.init();

  /*******************/
  /* Configure Timer */ 
  if (ITimer1.attachInterruptInterval(SAMPLE_RATE_MS, control_law))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{
  #if (DEBUG)
  delay(1000); //1s
  Serial.println(i);
  i = 0;
  #endif
}