/*
 * Project Zwillingsbaeren
 * Description:
 * Author:
 * Date:
 */

 int openDoor(String command);
 int closeDoor(String command);
 int controlPin=D0;
 int relayPower=A0;

 int iGarageState=0;
 int iLastState;

 void setup() {
     Serial.begin(9600);
     iGarageState=iLastState=0;
     Spark.function("openDoor",openDoor);
     Spark.function("closeDoor",closeDoor);
     Spark.variable("state", &iGarageState, INT);

     pinMode(controlPin, OUTPUT);
     pinMode(relayPower, OUTPUT);

     digitalWrite(controlPin, LOW);
     digitalWrite(relayPower, HIGH);
 }

 void loop() {
     String s=((iGarageState==0)?"closed":"open");

     if(iLastState!=iGarageState){
         Spark.publish("state",s);
         Serial.println(s);
     }

     iLastState=iGarageState;
     delay(1000);
 }

 int openDoor(String command){
     //CWD-- execute open
     iGarageState=1;
     digitalWrite(controlPin,HIGH);
     delay(750);
     digitalWrite(controlPin,LOW);
     return iGarageState;
 }

 int closeDoor(String command){
     //CWD-- execute close
     iGarageState=-1;
     digitalWrite(controlPin,HIGH);
     delay(750);
     digitalWrite(controlPin,LOW);
     return iGarageState;
 }
