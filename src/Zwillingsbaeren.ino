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
  int doorStates[2];
  int doorLastStates[2];

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
     int cp=0;

     // check door state first to see if it's open. bail if so

     if(doorId=="1") {
       cp=controlPins[0];
     } else if(doorId=="2") {
       cp=controlPins[1];
     } else {
       return -1;
     }

     return executeDoor(cp);
 }

 int closeDoor(String doorId){
   //CWD-- execute close
   Spark.publish("operation", "close:"+doorId);
   int cp=0;

   // check door state first to see if it's closed. bail if so
String state = ""; //CWD-- TODO: record state
   if(state == "") {
     if(doorId == "1") {
       cp=controlPins[0];
     } else if(doorId=="2") {
       cp=controlPins[1];
     } else {
       Spark.publish("operation", "failed to open:"+doorId);
       return -1;
     }

     return executeDoor(cp);
  }
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
