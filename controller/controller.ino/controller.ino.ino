/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01
    by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#define enA 9
#define in1 6
#define in2 7
#define button 4

int rotDirection = 0;
int APS0PIN = A0;
int APS1PIN = A1;
int TPS0PIN = A2;
int TPS1PIN = A3;

int MARGIN_OF_ERROR = 5;
int apsMOE;
int tpsMOE;

int pOopsie;
int tOopsie;

int aps0;
int aps1;
int apsMin = 150;
int apsMax = 850;

int tps0;
int tps1;
int tps1Min = 800;
int tps1Max = 160;

int pClosedThrsh = 150;
int pWotThrsh = 830;

int outputMin = 0;
int outputMax = 210;

boolean open = true;

int wBoost;

int pwmOutput = 0;
int throttlePos = 0;
int pedalPos = 0;
int pwmOld;
//int pressed = false;
void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(APS0PIN, INPUT);
  pinMode(APS1PIN, INPUT);
  pinMode(TPS0PIN, INPUT);
  pinMode(TPS1PIN, INPUT);

  apsMOE = (apsMax - apsMin) / MARGIN_OF_ERROR;
  tpsMOE = (tps1Min - tps1Max) / MARGIN_OF_ERROR;

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.begin(9600);
}
void loop() {
  aps0 = analogRead(APS0PIN);
  aps1 = analogRead(APS1PIN);
  tps1 = analogRead(TPS1PIN);



  int pedalPos = getAPSval(aps0, aps1);


  int throtPos = getTPSval(tps1, tps1);

//    Serial.println(throtPos);

  //  int diff = pedalPos - throtPos; //not sure on this

//  Serial.println(pedalPos);

  if ( pedalPos > throtPos) {
    pwmOutput = outputMax;
  } else if (throtPos > pedalPos) {
    pwmOutput = pedalPos*.9;
  } else {
    pwmOutput = pedalPos;
  }

  analogWrite(enA, pwmOutput);
}

int getAPSval(int p0, int p1) {
  int newAps = 0;
  //a crude calibration of the default values
  if (p1 < apsMin || p0 < apsMin) {
    apsMin = min(p0, p1);
  } else if (p1 > apsMax || p0 > apsMax) {
    apsMax = max(p0, p1);
  }


  int diff = abs(p1 - p0);

  if (diff > apsMOE) {
    //    return pOopsie + 1;
    newAps = outputMin;
  } else if (p0 < pClosedThrsh) {
    newAps = outputMin;
  } else if (p0 > pWotThrsh) {
    newAps = outputMax;
  } else {
    newAps = map(p1, apsMin, apsMax, outputMin, outputMax);
  }

//  if (pedalPos != newAps) {
    //        Serial.print ("Pedal POS changed to : ");
    //        Serial.println(newAps);
    //        Serial.println(p0);
    //        Serial.println(p1);
//
//    pedalPos = newAps;
//  }

  return newAps;
}

int getTPSval(int t0, int t1) {
  int newTp;

  //a crude calibration of the default values
  if (t1 > tps1Min) {
    tps1Min = t1;
  } else if (t1 < tps1Max) {
    tps1Max = t1;
  }


  //  if ((t1 + t0) < 1024){
  //    tOopsie = tOopsie + 1;
  //  } else {
  newTp = map(t1, tps1Min, tps1Max, outputMin, outputMax);
  //  }
Serial.println(t1);
//  if (throttlePos != newTp) {
//        Serial.print("TPS Changed pos : ");
//        Serial.println(newTp);
//
//
//    throttlePos = newTp;
//  }
  
  return newTp;
}

//int getTPSdif(int t0, int t1) {
//  //a crude calibration of the default values
//  if (t1 > tps1Min) {
//    tps1Min = t1;
//  } else if (t1 < tps1Max) {
//    tps1Max = t1;
//  }
//
//  //  if ((t1 + t0) < 1024){
//  //    tOopsie = tOopsie + 1;
//  //  } else {
//  return map(t1, tps1Min, tps1Max, 0, 512);
//  //  }
//}

