
// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define DEBUG                         true
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define USE_TIMER_3                   true
#define USE_TIMER_4                   true
#define USING_LOOP_TEST               false
#define SAMPLE_RATE_MS                5
#define REFERENCE_RATE_MS             100
#define ANALOG_IN_PIN                 A0
#define DEBUG_OUT_PIN                 8
#define ANALOG_OUT_PIN                9
#define STATE_SPACE                   false // true = ss, false = transfer function

#if (STATE_SPACE)
  #define K0 -1 //K[0,0]
  #define K1 -1 //K[0,1]
  #define ki -1

  #define A_0_0 -1 //A[0, 0]
  #define A_1_0 -1 //A[1, 0]
  #define A_0_1 -1 //A[0, 1]
  #define A_1_1 -1 //A[1, 1]
  #define B_0 -1   //B[0, 0]
  #define B_1 -1   //B[1, 0]

  #define C_0 //C[0, 0]
  #define C_1 //C[0, 1]
  #define D 0
  #define Ke_0 -1 //Ke[0,0]
  #define Ke_1 -1 //Ke[1,0]
#else
  #define P1 0.9329312978
  #define P2 0.9242767625
  #define P3 0.4898917071
  #define P4 1.169605738
  #define P5 0.1696057384
#endif


//---------------------------------------------//
//----  No changes are needed bellow here  ----//
//---------------------------------------------//

#if (DEBUG)
volatile int i = 0; // iteration counter
#endif

volatile float reference = 1;




#include "TimerInterrupt.h"


void control_law()
{
  digitalWrite(DEBUG_OUT_PIN, HIGH);
  static float last_measurement = 0; // already in volts
  static float u = 0;

  //#if (DEBUG)
  //Serial.print("control_law called, millis() = "); Serial.println(millis());
  //#endif

  // Read
  // between 0 and 1023, 0 to 5V
  last_measurement = analogRead(ANALOG_IN_PIN) * (5.0 / 1023.0);
  

  
  #if (STATE_SPACE)
    static float x1_n_minus_1 = 0;
    static float x2_n_minus_1 = 0;
    static float x1e_n_minus_1 = 0;
    static float x2e_n_minus_1 = 0;
    static float qsi_n_minus_1 = 0;
    static float x1_ponto_n_minus_1 = 0;
    static float x2_ponto_n_minus_1 = 0;
    static float x1e_ponto_n_minus_1 = 0;
    static float x2e_ponto_n_minus_1 = 0;
    static float erro_n_minus_1 = 0;

    x1_ponto_n_minus_1 = x1_ponto_n
    x1_n_minus_1 = x1_n
    x2_ponto_n_minus_1 = x2_ponto_n
    x2_n_minus_1 = x2_n
    x1e_ponto_n_minus_1 = x1e_ponto_n
    x1e_n_minus_1 = x1e_n
    x2e_ponto_n_minus_1 = x2e_ponto_n
    x2e_n_minus_1 = x2e_n
    erro_n_minus_1 = erro_n
    qsi_n_minus_1 = qsi_n

    x1_n =  SAMPLE_RATE_MS*x1_ponto_n_minus_1  + x1_n_minus_1;
    x2_n =  SAMPLE_RATE_MS*x2_ponto_n_minus_1  + x2_n_minus_1;
    x1e_n = SAMPLE_RATE_MS*x1e_ponto_n_minus_1 + x1e_n_minus_1;
    x2e_n = SAMPLE_RATE_MS*x2e_ponto_n_minus_1 + x2e_n_minus_1;
    qsi_n = SAMPLE_RATE_MS*erro_n_minus_1      + qsi_n_minus_1;

    u_n = -(K0*x1e_n + K1*x2e_n)+ki*qsi_n;
    x1_ponto_n = A_0_0*x1_n + A_0_1*x2_n + B_0*u_n;  
    x2_ponto_n = A_1_0*x1_n + A_1_1*x2_n + B_1*u_n; 
    ye_n = C_0*x1e_n + C_1*x2e_n + D*u_n;
    erro_n = reference - last_measurement;
    x1e_ponto_n = A_0_0*x1e_n + A_0_1*x2e_n + B_0*u_n + Ke_0*(last_measurement-ye_n);
    x2e_ponto_n = A_1_0*x1e_n + A_1_1*x2e_n + B_1*u_n + Ke_1*(last_measurement-ye_n);
  #else
    static float y_n_minus_2 = 0;
    static float y_n_minus_1 = 0;
    static float y_n = 0;
    static float error_n_minus_2 = 0;
    static float error_n_minus_1 = 0;
    static float error_n = 0;
    static float u_n_minus_2 = 0;
    static float u_n_minus_1 = 0;

    error_n_minus_2 = error_n_minus_1;
    error_n_minus_1 = error_n;
    u_n_minus_2 = u_n_minus_1;
    u_n_minus_1 = u;

    // The plant can also be simulated with:
    // 𝑦[𝑛]=+0.2964𝑢[𝑛−1]+0.238𝑢[𝑛−2]+0.9907𝑦[𝑛−1]−0.5251𝑦[𝑛−2]
    //y_n_minus_2 = y_n_minus_1;
    //y_n_minus_1 = y_n;
    //y_n = +0.2964*u_n_minus_1 +0.238*u_n_minus_2 +0.9907*y_n_minus_1 -0.5251*y_n_minus_2;
    //last_measurement = y_n;

    error_n = reference - last_measurement;
    
    // Calculate
    //𝑢[𝑛]=+0.9329312978𝑒𝑟𝑟𝑜[𝑛+0]−0.9242767625𝑒𝑟𝑟𝑜[𝑛−1]+0.4898917071𝑒𝑟𝑟𝑜[𝑛−2]+1.169605738𝑢[𝑛−1]−0.1696057384𝑢[𝑛−2]

    u = P1*error_n - P2*error_n_minus_1 + P3*error_n_minus_2 + P4*u_n_minus_1 - P5*u_n_minus_2;
  #endif
  
  
  // Clip signal
  if (u>5)
    u = 5;
  else if (u<0)
    u = 0;

  // Write
  // between 0 and 255, 0 to 5V (mean voltage)
  // 490 Hz (pins 4 and 13: 980 Hz)
  // PWM pins: 2 - 13, 44 - 46
  analogWrite(ANALOG_OUT_PIN, int(u * float(255.0/5.0))); 

  // Setando o PWM na mão:
  // https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm
  //TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  //TCCR2B = _BV(CS22);
  //OCR2A = int(u * float(255.0/5.0));
  //OCR2B = int(u * float(255.0/5.0));

  //Serial.println(u);
  //analogWrite(ANALOG_OUT_PIN, reference * 255/5); //para malha aberta
  //Serial.println(float(u*100));
  //Serial.print("last_measurement=");Serial.print(last_measurement);Serial.print("\tu=");Serial.println(u);
  //Serial.println("> eq");
  #if (DEBUG)
    i++;
  #endif
  digitalWrite(DEBUG_OUT_PIN, LOW);
}

void change_reference(){
  static bool state = false;

  if (reference > 1.25){
    reference = 1;
  } else {
    reference = 1.5;
  }
  //analogWrite(ANALOG_OUT_PIN, reference * 255/5); //para malha aberta
  

  state = !state;
  digitalWrite(LED_BUILTIN, state);

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
  pinMode(DEBUG_OUT_PIN, OUTPUT);
  pinMode(ANALOG_OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  ITimer3.init();
  ITimer4.init();

  /*******************/
  /* Configure Timer */ 
  if (ITimer3.attachInterruptInterval(SAMPLE_RATE_MS, control_law)){
    //Serial.print(F("Starting  ITimer3 OK, millis() = ")); Serial.println(millis());
  } else{
    //Serial.println(F("Can't set ITimer3. Select another freq. or timer"));
  }

  if (ITimer4.attachInterruptInterval(REFERENCE_RATE_MS, change_reference)){
    //Serial.print(F("Starting  ITimer4 OK, millis() = ")); Serial.println(millis());
  } else{
    //Serial.println(F("Can't set ITimer4. Select another freq. or timer"));
  }
}

void loop()
{
}