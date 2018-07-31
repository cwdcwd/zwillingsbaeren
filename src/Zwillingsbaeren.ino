/*
 * Project Zwillingsbaeren
 * Description: Dual Garage Door Opener
 * Author: CWD <cwd@lazybaer.com>
 * Date: 20180725
 */
 #include "HC-SR04.h"

// CWD-- CONSTANTS
  const int EXECUTE_DELAY=750;
  const float SENSOR_DISTANCE = 5.0f;

  int openDoor(String doorId);
  int closeDoor(String doorId);
  int checkDoorState(String doorId);

  int controlPins[]={ D0, D1 };
  int doorStates[]={ 0, 0 };
  int doorLastStates[]={ 0, 0 };

  int statusLED = D7;

  // trigger / echo pins
  const int triggerPins[] = { A0, A1 };
  const int echoPins[] = { D4, D5 };
  HC_SR04 rangeFinder01 = HC_SR04(triggerPins[0], echoPins[0]);
  HC_SR04 rangeFinder02 = HC_SR04(triggerPins[1], echoPins[1]);

// CWD-- STANDARD SET UP & LOOP
 void setup() {
  Serial.begin(9600);
  rangeFinder01.init();
  rangeFinder02.init();
  Spark.function("openDoor", openDoor);
  Spark.function("closeDoor", closeDoor);
  Spark.function("checkDoorState", checkDoorState);

  Spark.variable("state01", &doorStates[0], INT);
  Spark.variable("state02", &doorStates[1], INT);

  pinMode(controlPins[0], OUTPUT);
  pinMode(controlPins[1], OUTPUT);
  pinMode(statusLED, OUTPUT);

  digitalWrite(controlPins[0], HIGH);
  digitalWrite(controlPins[1], HIGH);
  digitalWrite(statusLED, LOW);
 }

 void loop() {
   checkDoorState("1");
   checkDoorState("2");
   delay(500);
 }

 // CWD-- cloud exposed funcs
 int openDoor(String doorId){
  //CWD-- execute open
  Spark.publish("operation", "open:"+doorId);
  return operate(doorId, 1);
 }

 int closeDoor(String doorId){
  //CWD-- execute close
  Spark.publish("operation", "close:"+doorId);
  return operate(doorId, 0);
 }


// CWD-- helper and operator funcs
 int operate(String doorId, int state) {
  int dId=doorId.toInt()-1; //CWD-- Door Ids should be cardinal. -1 should arrise when it can't convert right

  if((dId==-1)||(dId>arraySize(controlPins))) { //CWD-- kick out on bad indices
    return -1;
  }

  if(doorLastStates[dId]==state) { //CWD-- kick out if we're already at that state
    return -2;
  }

  // doorLastStates[dId]=state;
  executeDoor(controlPins[dId]);
  return doorLastStates[dId];
 }

 int checkDoorState(String doorId){
   int dId=doorId.toInt()-1; //CWD-- Door Ids should be cardinal. -1 should arrise when it can't convert right

   if((dId==-1)||(dId>arraySize(controlPins))) { //CWD-- kick out on bad indices
     return -1;
   }

   if(dId > arraySize(doorLastStates)) {
     return -1;
   }

   doorLastStates[dId] = doorStates[dId];
   float f = measureSensor(dId);
   doorStates[dId] = 0;

   if(f > SENSOR_DISTANCE) {
     doorStates[dId] = 1;
   }

   return doorStates[dId];
 }

float measureSensor(int dId) {
  float inch = -1;
  unsigned long start = micros();

  if(dId == 0) {
    inch = rangeFinder01.distInch();
  } else if(dId == 1) {
    inch = rangeFinder02.distInch();
  } else {
    return -1;
  }

  unsigned long calcTime = micros() - start;
  Serial.printf("Range finding duration: %lu | Distance in inches: %.2f\n", calcTime, inch);
  return inch;
}

 int executeDoor(int controlPin) {
  Serial.println(controlPins[0]);
  Serial.println(controlPins[1]);
  Serial.print("executing Door on ");
  Serial.println(controlPin);
  digitalWrite(controlPin,LOW);
  digitalWrite(statusLED, HIGH);
  delay(EXECUTE_DELAY);
  digitalWrite(controlPin,HIGH);
  digitalWrite(statusLED, LOW);
  return 1;
 }
