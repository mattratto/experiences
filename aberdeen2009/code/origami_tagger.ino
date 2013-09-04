#include <Servo.h>
#include <IRremote.h> ////the library assigns pin 3 to the IR LED

Servo myservo;  // create servo object to control a servo
int pos = 1;    // variable to store the servo position
int IR_power = 12;    //set pin 12 to power IR receiver without breadboard
IRrecv irrecv(7);
IRsend irsend;
decode_results results;

void setup() {
myservo.attach(9);  // attaches the servo on pin 9 to the servo object
pinMode(IR_power, OUTPUT);
digitalWrite(IR_power, HIGH);     //set this HIGH to power IR Receiver
irrecv.enableIRIn(); // Start the receiver
Serial.begin(9600);
Serial.println(“Origami Tagger started!”);
}

void loop() {
if(button.isPressed()){
//for (int i = 0; i < 3; i++) {
irsend.sendSony(0xa90, 12); // Sony TV power code
delay(200);
//}
}
myservo.write(pos);

if (irrecv.decode(&results)) {
Serial.println(results.value);
/* Uncomment for more specific remote control
if (results.value>0) {
pos=pos+30;
Serial.print(“Pos”);
Serial.println(pos);
if (pos>180) {
pos=1;
}
} */
pos=pos+30;
Serial.print(“Pos”);
Serial.println(pos);
if (pos>180) {
pos=1;
}
delay(100);
irrecv.resume(); // Receive the next value
}
}
