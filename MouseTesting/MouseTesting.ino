x#include "DRV8834.h"
#include "MMouse.h" 

//LEFT DRIVERSTEP 11 DIR 10
//RIGHT DRIVER STEP 9 DIR 8
DRV8834    left(_STEPSINREV, 10, 11, ENABLE, M0, M1);
DRV8834    right(_STEPSINREV, 8, 9, ENABLE, M0, M1);
MMouse grigoras(left, right);

void setup() {
grigoras.setupMMouse();  // put your setup code here, to run once:

}

void loop() {/*
  left.begin(200);
  right.begin(200);
  left.enable();
  left.setMicrostep(32);
for (int i = 0; i < 10000; i++) {
  left.move(-25);
  right.move(25);
  }
  delay(1000);*/

  grigoras.testIRSensors();
  delay(2);
}
