/*
 * Project Zwillingsbaeren
 * Description: Dual Garage Door Opener
 * Author: CWD <cwd@lazybaer.com>
 * Date: 20180725
 */
  const int EXECUTE_DELAY=750;

  int openDoor(String doorId);
  int closeDoor(String doorId);

  int controlPins[]={ D0, D1 };
  int doorStates[]={ 0, 0 };
  int doorLastStates[]={ 0, 0 };

  int statusLED = D7;


 void setup() {
     Serial.begin(9600);
     //iGarageState01=iLastState01=0;
     Spark.function("openDoor",openDoor);
     Spark.function("closeDoor",closeDoor);
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
/*     String s=((iGarageState01==0)?"closed":"open");

     if(iLastState01!=iGarageState01){
         Spark.publish("state",s);
         Serial.println(s);
     }

     iLastState01=iGarageState01;
     delay(1000); */
 }

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

 int operate(String doorId, int state) {
   int dId=doorId.toInt()-1; //CWD-- Door Ids should be cardinal. -1 should arrise when it can't convert right

   if((dId==-1)||(dId>arraySize(controlPins))) { //CWD-- kick out on bad indices
     return -1;
   }

   if(doorLastStates[dId]==state) { //CWD-- kick out if we're already at that state
     return -2;
   }

   doorLastStates[dId]=state;
   executeDoor(controlPins[dId]);
   return doorLastStates[dId];
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
