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
int tps0Min = 267;
int tps0Max = 927;
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

  apsMOE = (apsMax - apsMin) / *.05;
//  tpsMOE = (tps1Min - tps1Max) / MARGIN_OF_ERROR;
  tpsMOE = 1024 *.05;

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.begin(9600);
}
void loop() {
  aps0 = analogRead(APS0PIN);
  aps1 = analogRead(APS1PIN);
  tps0 = analogRead(TPS0PIN);
  tps1 = analogRead(TPS1PIN);

  int pedalPos = getAPSval(aps0, aps1);
  int throtPos = getTPSval(tps0, tps1);

  if ( pedalPos > throtPos) {
    pwmOutput = outputMax;
  } else if (throtPos > pedalPos) {
    pwmOutput = pedalPos * .9;
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
    newAps = 0;         //has an oopsie happened?
    newAps = outputMin;
  } else if (p0 < pClosedThrsh) {
    newAps = outputMin;
  } else if (p0 > pWotThrsh) {
    newAps = outputMax;
  } else {
    newAps = map(p1, apsMin, apsMax, outputMin, outputMax);
  }

  return newAps;
}

int getTPSval(int t0, int t1) {
  int newTp;

  //a crude calibration of the default values
  if (t0 < tps0Min) {
    tps0Min = t0;
  } else if (t0 > tps0Max) {
    tps0Max = t0;
  }

  if ((t1 + t0) < (1024 - tpsMOE)) {
    tOopsie = tOopsie + 1;
  } else {
    newTp = map(t0, tps0Min, tps0Max, outputMin, outputMax);
  }

  return newTp;
}

