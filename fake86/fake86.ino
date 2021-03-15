#include "cpu8086.h"
#include "outputserial.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init86();
}

void loop() {
  // put your main code here, to run repeatedly:
  exec86(100);
}
