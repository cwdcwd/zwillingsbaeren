/*
 * Project Zwillingsbaeren
 * Description: Dual Garage Door Opener
 * Author: CWD <cwd@lazybaer.com>
 * Date: 20180725
 */
 #include "HC-SR04.h"

// CWD-- CONSTANTS
  const int DOOR1 = 0;
  const int DOOR2 = 1;
  const int DOOR_OPEN = 1;
  const int DOOR_CLOSED = 0;
  const int DOOR_ERROR = -1;
  const int DOOR_ALREADY_SET = -2;
  const int EXECUTE_DELAY=750;
  const float SENSOR_DISTANCE = 5.0f;

// CWD-- CLOUD FUNCS
  int openDoor(String doorId);
  int closeDoor(String doorId);
  int checkDoorState(String doorId);

// CWD-- GLOBALS
  int controlPins[]={ D0, D1 };
  int doorStates[]={ 0, 0 };
  int doorLastStates[]={ 0, 0 };

  int statusLED = D7;

  // CWD-- trigger / echo pins for HC_SR04 distance sensor
  const int triggerPins[] = { A0, A1 };
  const int echoPins[] = { D4, D5 };

  HC_SR04 rangeFinder01 = HC_SR04(triggerPins[DOOR1], echoPins[DOOR1]);
  HC_SR04 rangeFinder02 = HC_SR04(triggerPins[DOOR2], echoPins[DOOR2]);

// CWD-- STANDARD SET UP & LOOP
 void setup() {
  Serial.begin(9600);
  rangeFinder01.init();
  rangeFinder02.init();
  Spark.function("openDoor", openDoor);
  Spark.function("closeDoor", closeDoor);
  Spark.function("checkDoorState", checkDoorState);

  Spark.variable("state01", &doorStates[DOOR1], INT);
  Spark.variable("state02", &doorStates[DOOR2], INT);

  pinMode(controlPins[DOOR1], OUTPUT);
  pinMode(controlPins[DOOR2], OUTPUT);
  pinMode(statusLED, OUTPUT);

  digitalWrite(controlPins[DOOR1], HIGH);
  digitalWrite(controlPins[DOOR2], HIGH);
  digitalWrite(statusLED, LOW);
 }

 void loop() {
   checkDoorState("1");
   checkDoorState("2");
   delay(500);
 }

 // CWD-- CLOUD EXPOSED FUNCS
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


// CWD-- HELPER AND OPERATOR FUNCS
 int operate(String doorId, int state) {
  int dId=doorId.toInt() - 1; //CWD-- Door Ids should be cardinal. -1 should arrise when it can't convert right

  if((dId == -1) || (dId > arraySize(controlPins))) { //CWD-- kick out on bad indices
    return DOOR_ERROR;
  }

  if(doorLastStates[dId] == state) { //CWD-- kick out if we're already at that state
    return DOOR_ALREADY_SET;
  }

  // doorLastStates[dId]=state;
  executeDoor(controlPins[dId]);
  return doorLastStates[dId];
 }

 int checkDoorState(String doorId){
   int dId=doorId.toInt() - 1; //CWD-- Door Ids should be cardinal. -1 should arrise when it can't convert right

   if((dId == -1)||(dId > arraySize(controlPins))) { //CWD-- kick out on bad indices
     return DOOR_ERROR;
   }

   doorLastStates[dId] = doorStates[dId];
   float f = measureSensor(dId);
   doorStates[dId] = DOOR_CLOSED;

   if(f > SENSOR_DISTANCE) {
     doorStates[dId] = DOOR_OPEN;
   }

   return doorStates[dId];
 }

float measureSensor(int dId) {
  float inch = -1;
  unsigned long start = micros();

  if(dId == DOOR1) {
    inch = rangeFinder01.distInch();
  } else if(dId == DOOR2) {
    inch = rangeFinder02.distInch();
  } else {
    return DOOR_ERROR;
  }

  unsigned long calcTime = micros() - start;
  Serial.printf("DoorId %i: Range finding duration: %lu | Distance in inches: %.2f\n", dId, calcTime, inch);
  return inch;
}

 int executeDoor(int controlPin) {
  Serial.println(controlPins[DOOR1]);
  Serial.println(controlPins[DOOR2]);
  Serial.print("executing Door on ");
  Serial.println(controlPin);
  digitalWrite(controlPin,LOW);
  digitalWrite(statusLED, HIGH);
  delay(EXECUTE_DELAY);
  digitalWrite(controlPin,HIGH);
  digitalWrite(statusLED, LOW);
  return 1;
 }
