#include <Ticker.h>

volatile byte state = LOW;
const float sample_rate_ms = 0.4666;
int i = 0; // iteration counter

void control_equation(){
  state = !state;
  digitalWrite(LED_BUILTIN, state);
  //Serial.println("> eq");
  i++;
  timer1_write(sample_rate_ms*5000);
}


void setup() {
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
  delay(1000);                       // wait for a second
  Serial.println(i);
  i = 0;
  
}
