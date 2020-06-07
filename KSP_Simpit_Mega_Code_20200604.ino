// Sets up the Arduino Mega to handle the analog joysticks, Action Groups, switches and buttons
#include <KerbalSimpit.h>
#include <KerbalSimpitMessageTypes.h>
#include <PayloadStructs.h>
#include <ezButton.h> // loads ezButton library for button debounce
#include <Rotary.h>

rotationMessage myRotation;
translationMessage myTranslation;

const int ROT_X = A0; // assigns rotation joystick X-axis to pin Analog 0
const int ROT_Y = A1; // assigns rotation joystick Y-axis to pin Analog 1
const int ROT_Z = A2; // assigns rotation joystick Z-axis to pin Analog 2
const int TRANS_X = A3; // assigns translation joystick X-axis to pin Analog 3
const int TRANS_Y = A4; // assigns translation joystick Y-axis to pin Analog 4
const int TRANS_Z = A5; // assigns translation joystick Z-axis to pin Analog 5
const int STAGE_BTN = 2; // assigns the Staging button to pin 2
const int RCS_LED = 3; // assigns the RCS indicator LED to pin 3
const int BRAKES_LED = 4; // assigns the Brakes indicator LED to pin 4
const int GEAR_LED = 5; // assigns the Landing Gear indicator LED to pin 5
const int LIGHTS_LED = 6; // assigns the Lights indicator LED to pin 6
const int SAS_LED = 7; // assigns the SAS indicator LED to pin 7
const int THROT_CLK = 22; // assigns the Throttle rotary encoder CLK output ("A") to pin 22
const int THROT_DT = 23; // assigns the Throttle rotary encoder DT output ("B") to pin 23
const int THROT_BTN = 24; // assigns the Throttle rotary encoder Switch output to pin 24
const int THROT_CUT = 25; // assigns the Throttle Cut button output to pin 25
const int SAS_CLK = 32; // assigns the SAS rotary encoder CLK output ("A") to pin 32
const int SAS_DT = 33; // assigns the SAS rotary encoder DT output ("B") to pin 33
const int SAS_SWITCH = 34; // assigns the SAS switch output to pin 34
const int RCS_SWITCH = 35; // assigns the RCS switch output to pin 35
const int BRAKES_SWITCH = 36; // assigns the Brakes switch output to pin 36
const int GEAR_SWITCH = 37; // assigns the Gear switch output to pin 37
const int LIGHTS_SWITCH = 38; // assigns the Lights switch output to pin 38
const int ABORT_BTN = 39; // assigns the Abort switch output to pin 38
const int AG_01 = 41; // assigns the Action Group 1 switch output to pin 41
const int AG_02 = 42; // assigns the Action Group 2 switch output to pin 42
const int AG_03 = 43; // assigns the Action Group 3 switch output to pin 43
const int AG_04 = 44; // assigns the Action Group 4 switch output to pin 44
const int AG_05 = 45; // assigns the Action Group 5 switch output to pin 45
const int AG_06 = 46; // assigns the Action Group 6 switch output to pin 46
const int AG_07 = 47; // assigns the Action Group 7 switch output to pin 47
const int AG_08 = 48; // assigns the Action Group 8 switch output to pin 48
const int AG_09 = 49; // assigns the Action Group 9 switch output to pin 49
const int AG_10 = 50; // assigns the Action Group 10 switch output to pin 50

int sas_Counter = 1; // initializes SAS Mode counter value variable at 1
int currentSASStateCLK;
int lastSASStateCLK;
int throt_Counter = 0; // initializes Throttle counter value variable at 0
int currentThrotStateCLK;
int lastThrotStateCLK;
int rot_X_Read;
int rot_Y_Read;
int rot_Z_Read;
int rot_X_Mapped;
int rot_Y_Mapped;
int rot_Z_Mapped;
int trans_X_Read;
int trans_Y_Read;
int trans_Z_Read;
int trans_X_Mapped;
int trans_Y_Mapped;
int trans_Z_Mapped;
int debounce_Time = 25;

KerbalSimpit mySimpit(Serial);

ezButton buttonSTAGE(STAGE_BTN);
ezButton buttonTHROT(THROT_BTN);
ezButton buttonTHROT_CUT(THROT_CUT);
ezButton buttonSAS(SAS_SWITCH);
ezButton buttonRCS(RCS_SWITCH);
ezButton buttonBRAKES(BRAKES_SWITCH);
ezButton buttonGEAR(GEAR_SWITCH);
ezButton buttonLIGHTS(LIGHTS_SWITCH);
ezButton buttonABORT(ABORT_BTN);
ezButton buttonAG_01(AG_01);
ezButton buttonAG_02(AG_02);
ezButton buttonAG_03(AG_03);
ezButton buttonAG_04(AG_04);
ezButton buttonAG_05(AG_05);
ezButton buttonAG_06(AG_06);
ezButton buttonAG_07(AG_07);
ezButton buttonAG_08(AG_08);
ezButton buttonAG_09(AG_09);
ezButton buttonAG_10(AG_10);

Rotary throtRotary = Rotary(THROT_DT, THROT_CLK);
Rotary sasRotary = Rotary(SAS_DT, SAS_CLK);

void setup() {
  Serial.begin(230400); // begins the serial connection to the computer through USB
  pinMode(ROT_X, INPUT); // defines inputs and outputs on Arduino pins
  pinMode(ROT_Y, INPUT);
  pinMode(ROT_Z, INPUT);
  pinMode(TRANS_X, INPUT);
  pinMode(TRANS_Y, INPUT);
  pinMode(TRANS_Z, INPUT);
  pinMode(STAGE_BTN, INPUT_PULLUP);
  pinMode(SAS_LED, OUTPUT); 
  pinMode(RCS_LED, OUTPUT);
  pinMode(BRAKES_LED, OUTPUT);
  pinMode(GEAR_LED, OUTPUT);
  pinMode(LIGHTS_LED, OUTPUT); 
  pinMode(SAS_CLK, INPUT);
  pinMode(SAS_DT, INPUT);
  pinMode(SAS_SWITCH, INPUT);
  pinMode(THROT_CLK, INPUT);
  pinMode(THROT_DT, INPUT);
  pinMode(THROT_BTN, INPUT);
  pinMode(SAS_SWITCH, INPUT_PULLUP);
  pinMode(RCS_SWITCH, INPUT_PULLUP);
  pinMode(BRAKES_SWITCH, INPUT_PULLUP);
  pinMode(GEAR_SWITCH, INPUT_PULLUP);
  pinMode(LIGHTS_SWITCH, INPUT_PULLUP);
  pinMode(ABORT_BTN, INPUT_PULLUP);
  pinMode(AG_01, INPUT_PULLUP);
  pinMode(AG_02, INPUT_PULLUP);
  pinMode(AG_03, INPUT_PULLUP);
  pinMode(AG_04, INPUT_PULLUP);
  pinMode(AG_05, INPUT_PULLUP);
  pinMode(AG_06, INPUT_PULLUP);
  pinMode(AG_07, INPUT_PULLUP);
  pinMode(AG_08, INPUT_PULLUP);
  pinMode(AG_09, INPUT_PULLUP);
  pinMode(AG_10, INPUT_PULLUP);  

  buttonSTAGE.setDebounceTime(debounce_Time); // sets debounce times for buttons
  buttonTHROT.setDebounceTime(debounce_Time);
  buttonTHROT_CUT.setDebounceTime(debounce_Time);
  buttonSAS.setDebounceTime(debounce_Time);
  buttonRCS.setDebounceTime(debounce_Time);
  buttonBRAKES.setDebounceTime(debounce_Time);
  buttonGEAR.setDebounceTime(debounce_Time);
  buttonLIGHTS.setDebounceTime(debounce_Time);
  buttonABORT.setDebounceTime(debounce_Time);
  buttonAG_01.setDebounceTime(debounce_Time);
  buttonAG_02.setDebounceTime(debounce_Time);
  buttonAG_03.setDebounceTime(debounce_Time);
  buttonAG_04.setDebounceTime(debounce_Time);
  buttonAG_05.setDebounceTime(debounce_Time);
  buttonAG_06.setDebounceTime(debounce_Time);
  buttonAG_07.setDebounceTime(debounce_Time);
  buttonAG_08.setDebounceTime(debounce_Time);
  buttonAG_09.setDebounceTime(debounce_Time);
  buttonAG_10.setDebounceTime(debounce_Time);

  digitalWrite(SAS_LED, HIGH); // turns on all the LEDs while the handshake process is happening
  digitalWrite(RCS_LED, HIGH);
  digitalWrite(BRAKES_LED, HIGH);
  digitalWrite(GEAR_LED, HIGH);
  digitalWrite(LIGHTS_LED, HIGH);
  while (!mySimpit.init()) { // initializes (handshakes) with Simpit mod
    delay(100);
  }
  digitalWrite(SAS_LED, LOW); // turns off all the LEDs once the handshake process is complete
  digitalWrite(RCS_LED, LOW);
  digitalWrite(BRAKES_LED, LOW);
  digitalWrite(GEAR_LED, LOW);
  digitalWrite(LIGHTS_LED, LOW);

  mySimpit.inboundHandler(messageHandler); // declares the message handler to read incoming messages from Simpit mod
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE); // subscribes to the Action Status message channel
  mySimpit.registerChannel(ROTATION_MESSAGE); // subscribes to the Rotation message channel
  mySimpit.registerChannel(TRANSLATION_MESSAGE); // subscribes to the Translation message channel
}

void loop() {
  mySimpit.update(); // necessary updates and loops for called functions
  buttonSTAGE.loop();
  buttonTHROT.loop();
  buttonTHROT_CUT.loop();
  buttonSAS.loop();
  buttonRCS.loop();
  buttonBRAKES.loop();
  buttonGEAR.loop();
  buttonLIGHTS.loop();
  buttonABORT.loop();
  buttonAG_01.loop();
  buttonAG_02.loop();
  buttonAG_03.loop();
  buttonAG_04.loop();
  buttonAG_05.loop();
  buttonAG_06.loop();
  buttonAG_07.loop();
  buttonAG_08.loop();
  buttonAG_09.loop();
  buttonAG_10.loop();  

  throt_Counter = constrain(throt_Counter, 0, 32767); // sets upper and lower limits for counter variables for rotary encoders
  sas_Counter = constrain(sas_Counter, 1, 10);

  rot_X_Read = analogRead(ROT_X); // takes a reading for the X-axis; from testing determined X-min = 330, X-mid = 505, X-max = 693
  if (rot_X_Read < 510 && rot_X_Read > 500) { // determines if the X-axis pot is in the middle deadzone to eliminate jitter
    rot_X_Mapped = 0;
  }
  if (rot_X_Read <= 500) { // determines if X-axis pot is in the negative portion of its motion
    rot_X_Mapped = map(rot_X_Read, 330, 500, -32768, 0); // sets the mapping for the negative portion of the axis
  }
  if (rot_X_Read >= 510) { // determined if X-axis pot is in the positive portion of its motion
    rot_X_Mapped = map(rot_X_Read, 510, 693, 0, 32767); // sets the mapping for the positive portion of the axis
  }

  rot_X_Mapped = constrain(rot_X_Mapped, -32768, 32767); // constrains the mapped value of the X-axis reading to valid results
  myRotation.mask = 2; // applies the bitmask required to only send roll information to Simpit
  myRotation.roll = rot_X_Mapped; // applies the X-axis value as the rotation roll value
  mySimpit.send(ROTATION_MESSAGE, myRotation); // sends the roll value to Simpit
  delay(10);  
  
  rot_Y_Read = analogRead(ROT_Y);
  if (rot_Y_Read < 518 && rot_Y_Read > 508) {
    rot_Y_Mapped = 0;
  }
  if (rot_Y_Read <= 508) {
    rot_Y_Mapped = map(rot_Y_Read, 344, 508, -32768, 0);
  }
  if (rot_Y_Read >= 518) {
    rot_Y_Mapped = map(rot_Y_Read, 518, 680, 0, 32767);
  }

  rot_Y_Mapped = constrain(rot_Y_Mapped, -32768, 32767);
  myRotation.mask = 1;
  myRotation.pitch = rot_Y_Mapped;
  mySimpit.send(ROTATION_MESSAGE, myRotation);
  delay(10);

  rot_Z_Read = analogRead(ROT_Z);
  if (rot_Z_Read < 520 && rot_Z_Read > 510) {
    rot_Z_Mapped = 0;
  }
  if (rot_Z_Read <= 510) {
    rot_Z_Mapped = map(rot_Z_Read, 296, 510, -32768, 0);
  }
  if (rot_Z_Read >= 520) {
    rot_Z_Mapped = map(rot_Z_Read, 520, 733, 0, 32767);
  }

  rot_Z_Mapped = constrain(rot_Z_Mapped, -32768, 32767);
  myRotation.mask = 4;
  myRotation.yaw = rot_Z_Mapped;
  mySimpit.send(ROTATION_MESSAGE, myRotation);
  delay(10);

  trans_X_Read = analogRead(TRANS_X); // takes a reading for the X-axis; from testing determined X-min = 330, X-mid = 505, X-max = 693
  if (trans_X_Read < 510 && trans_X_Read > 500) { // determines if the X-axis pot is in the middle deadzone to eliminate jitter
    trans_X_Mapped = 0;
  }
  if (trans_X_Read <= 500) { // determines if X-axis pot is in the negative portion of its motion
    trans_X_Mapped = map(trans_X_Read, 330, 500, -32768, 0); // sets the mapping for the negative portion of the axis
  }
  if (trans_X_Read >= 510) { // determined if X-axis pot is in the positive portion of its motion
    trans_X_Mapped = map(trans_X_Read, 510, 693, 0, 32767); // sets the mapping for the positive portion of the axis
  }

  trans_X_Mapped = constrain(trans_X_Mapped, -32768, 32767); // constrains the mapped value of the X-axis reading to valid results
  myTranslation.mask = 1; // applies the bitmask required to only send X information to Simpit
  myTranslation.X = trans_X_Mapped; // applies the X-axis value as the X translation value
  mySimpit.send(TRANSLATION_MESSAGE, myTranslation); // sends the x translation value to Simpit
  delay(10);  
  
  trans_Y_Read = analogRead(TRANS_Y);
  if (trans_Y_Read < 518 && trans_Y_Read > 508) {
    trans_Y_Mapped = 0;
  }
  if (trans_Y_Read <= 508) {
    trans_Y_Mapped = map(trans_Y_Read, 344, 508, -32768, 0);
  }
  if (trans_Y_Read >= 518) {
    trans_Y_Mapped = map(trans_Y_Read, 518, 680, 0, 32767);
  }

  trans_Y_Mapped = constrain(trans_Y_Mapped, -32768, 32767);
  myTranslation.mask = 2;
  myTranslation.Y = trans_Y_Mapped;
  mySimpit.send(TRANSLATION_MESSAGE, myTranslation);
  delay(10);

  trans_Z_Read = analogRead(TRANS_Z);
  if (trans_Z_Read < 520 && trans_Z_Read > 510) {
    trans_Z_Mapped = 0;
  }
  if (trans_Z_Read <= 510) {
    trans_Z_Mapped = map(trans_Z_Read, 296, 510, -32768, 0);
  }
  if (trans_Z_Read >= 520) {
    trans_Z_Mapped = map(trans_Z_Read, 520, 733, 0, 32767);
  }

  trans_Z_Mapped = constrain(trans_Z_Mapped, -32768, 32767);
  myTranslation.mask = 4;
  myTranslation.Z = trans_Z_Mapped;
  mySimpit.send(TRANSLATION_MESSAGE, myTranslation);
  delay(10);

  if (buttonSTAGE.isPressed()) {
    mySimpit.toggleAction(STAGE_ACTION);
  }

  if (buttonTHROT.isPressed()) {
    throt_Counter = 32767;
    mySimpit.send(THROTTLE_MESSAGE, throt_Counter);
  }

  if (buttonTHROT_CUT.isPressed()) {
    throt_Counter = 0;
    mySimpit.send(THROTTLE_MESSAGE, throt_Counter);
  }
  
  if (buttonSAS.isPressed()) {
    mySimpit.toggleAction(SAS_ACTION);
  }
  
  if (buttonRCS.isPressed()) { // declares action if the RCS button is pressed (toggles RCS on/off)
    mySimpit.toggleAction(RCS_ACTION);
  }
  if (buttonBRAKES.isPressed()) { // declares action if the Brakes button is pressed (toggles Brakes on/off)
    mySimpit.toggleAction(BRAKES_ACTION);
  }
  if (buttonGEAR.isPressed()) { // declares action if the Gear button is pressed (toggles Gear up/down)
    mySimpit.toggleAction(GEAR_ACTION);
  }
  if (buttonLIGHTS.isPressed()) { // declares action if the Lights button is pressed (toggles Lights on/off)
    mySimpit.toggleAction(LIGHT_ACTION);
  }

  if (buttonABORT.isPressed()) { // declares action if the Abort button is pressed (activates the Abort action group)
    mySimpit.toggleAction(ABORT_ACTION);
  }

  if (buttonAG_01.isPressed()) { // declares action if the Action Group 1 is pressed (toggles Action Group 1)
    mySimpit.toggleCAG(1); 
  }

  if (buttonAG_02.isPressed()) { // declares action if the Action Group 2 is pressed (toggles Action Group 2)
    mySimpit.toggleCAG(2); 
  }

  if (buttonAG_03.isPressed()) { // declares action if the Action Group 3 is pressed (toggles Action Group 3)
    mySimpit.toggleCAG(3); 
  }

  if (buttonAG_04.isPressed()) { // declares action if the Action Group 4 is pressed (toggles Action Group 4)
    mySimpit.toggleCAG(4); 
  }

  if (buttonAG_05.isPressed()) { // declares action if the Action Group 5 is pressed (toggles Action Group 5)
    mySimpit.toggleCAG(5); 
  }

  if (buttonAG_05.isPressed()) { // declares action if the Action Group 6 is pressed (toggles Action Group 6)
    mySimpit.toggleCAG(6); 
  }

  if (buttonAG_07.isPressed()) { // declares action if the Action Group 7 is pressed (toggles Action Group 7)
    mySimpit.toggleCAG(7); 
  }

  if (buttonAG_08.isPressed()) { // declares action if the Action Group 8 is pressed (toggles Action Group 8)
    mySimpit.toggleCAG(8); 
  }

  if (buttonAG_09.isPressed()) { // declares action if the Action Group 9 is pressed (toggles Action Group 9)
    mySimpit.toggleCAG(9); 
  }

  if (buttonAG_10.isPressed()) { // declares action if the Action Group 10 is pressed (toggles Action Group 10)
    mySimpit.toggleCAG(10); 
  }

  unsigned char throtResult = throtRotary.process();

  if (throtResult == DIR_CW && (throt_Counter <= 29487)) {
    throt_Counter = throt_Counter + 3276; // increments the Throttle counter by ~10%
    mySimpit.send(THROTTLE_MESSAGE, throt_Counter); // sends the new throttle setting to Simpit
  }
  if (throtResult == DIR_CCW && (throt_Counter >= 3276)) {
    throt_Counter = throt_Counter - 3276; // decrements the Throttle counter by ~10%
    mySimpit.send(THROTTLE_MESSAGE, throt_Counter); // sends the new throttle setting to Simpit
  }

  unsigned char sasResult = sasRotary.process();

  if (sasResult == DIR_CW) {
    sas_Counter++; // increments the SAS counter by 1
    mySimpit.send(28, (unsigned char*) &sas_Counter, 1); // sends the new SAS mode setting to Simpit 
  }

  if (sasResult == DIR_CCW) {
    sas_Counter--; // decrements the SAS Counter by 1
    mySimpit.send(28, (unsigned char*) &sas_Counter, 1); // sends the new SAS mode setting to Simpit
  }

} 
  
void messageHandler(byte messageType, byte msg[], byte msgSize) { // sets up the message handler to receive messages from Simpit
  switch(messageType) {
  case ACTIONSTATUS_MESSAGE: // defines the set of actions for messages coming from ACTIONSTATUS_MESSAGE
  byte actions = msg[0]; // assigns the ACTIONSTATUS_MESSAGE to the variable actions
    if (actions & SAS_ACTION) { // checks to see if SAS is turned on
      digitalWrite(SAS_LED, HIGH); // turns on the SAS LED indicator if SAS is on
    } else {
      digitalWrite(SAS_LED, LOW); // set SAS LED indicator off if SAS is off
    }
    if (actions & GEAR_ACTION) { // checks to see if Gear is down
      digitalWrite(GEAR_LED, HIGH); // turns on Gear LED indicator if gear is down
    } else {
      digitalWrite(GEAR_LED, LOW); // set the Gear indicator off if gear is up
    }
    if (actions & LIGHT_ACTION) { // checks to see if Lights are on
      digitalWrite(LIGHTS_LED, HIGH); // turns on Lights LED indicator if lights are on
    } else {
      digitalWrite(LIGHTS_LED, LOW); // set Lights indicator off if lights are off
    }
    if (actions & RCS_ACTION) { // checks to see if RCS is active
      digitalWrite(RCS_LED, HIGH); // turns on the RCS LED indicator if RCS is active
    } else {
      digitalWrite(RCS_LED, LOW); // set RCS indicator off if RCS is inactive
    }
    if (actions & BRAKES_ACTION) { // checks to see if Brakes are on
      digitalWrite(BRAKES_LED, HIGH); // turns on Brakes LED indicator if brakes are on
    } else {
      digitalWrite(BRAKES_LED, LOW); // set Brakes indicator off if brakes are off
    }
    break;
}
}
