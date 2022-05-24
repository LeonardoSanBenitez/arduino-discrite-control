#include <Ticker.h>

volatile byte state = LOW;
const float sample_rate_ms = 0.4666;
const int analog_in_pin = A0;
const int analog_out_pin = D1;
int i = 0; // iteration counter
int last_measurement = 0;

void control_equation(){
  timer1_write(sample_rate_ms*5000);


  state = !state;

  // Read
  last_measurement = analogRead(analog_in_pin);

  // Calculate
  // TODO
  
  // Write
  // The PWM seems to interfere with our timer...
  // analogWrite(analog_out_pin, 128); // 0 to 255? Or 0 to 1023?
  digitalWrite(LED_BUILTIN, state);
  //Serial.println("> eq");
  i++;
  
}


void setup() {
  pinMode(analog_in_pin, INPUT);
  pinMode(analog_out_pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  /*******************/
  /* Configure Timer */ 
  timer1_attachInterrupt(control_equation);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(sample_rate_ms*5000);
}

// the loop function runs over and over again forever
void loop() {
  delay(1000); //1s
  Serial.println(i);
  i = 0;
}
