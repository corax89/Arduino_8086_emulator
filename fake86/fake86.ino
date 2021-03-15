#include "interupt.c"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  init86();
}

void loop() {
  // put your main code here, to run repeatedly:
  exec86(100);
}
