// Sets up the Arduino Leonardo to handle game-level commands through the Keyboard library
#include <Rotary.h>
#include <Keyboard.h>
#include <ezButton.h>

const int QSAVE_BTN = 2;
const int LOAD_BTN = 3;
const int PAUSE_BTN = 4;
const int WARP_CLK = 5;
const int WARP_DT = 6;
const int WARP_BTN = 7;
const int MAP_CLK = 8;
const int MAP_DT = 9;
const int MAP_BTN = 10;

int debounce_Time = 25;

ezButton buttonQSAVE(QSAVE_BTN);
ezButton buttonLOAD(LOAD_BTN);
ezButton buttonPAUSE(PAUSE_BTN);
ezButton buttonWARP(WARP_BTN);
ezButton buttonMAP(MAP_BTN);

Rotary warpRotary = Rotary(WARP_DT, WARP_CLK);
Rotary mapRotary = Rotary(MAP_DT, MAP_CLK);

void setup() {
  Serial.begin(115200);
  pinMode(QSAVE_BTN, INPUT_PULLUP);
  pinMode(LOAD_BTN, INPUT_PULLUP);
  pinMode(PAUSE_BTN, INPUT_PULLUP);
  pinMode(WARP_CLK, INPUT);
  pinMode(WARP_DT, INPUT);
  pinMode(WARP_BTN, INPUT);
  pinMode(MAP_CLK, INPUT);
  pinMode(MAP_DT, INPUT);
  pinMode(MAP_BTN, INPUT);

  buttonQSAVE.setDebounceTime(debounce_Time);
  buttonLOAD.setDebounceTime(debounce_Time);
  buttonPAUSE.setDebounceTime(debounce_Time);
  buttonWARP.setDebounceTime(debounce_Time);
  buttonMAP.setDebounceTime(debounce_Time);

  Keyboard.begin();
}

void loop() {
  buttonQSAVE.loop();
  buttonLOAD.loop();
  buttonPAUSE.loop();
  buttonWARP.loop();
  buttonMAP.loop();

  if (buttonQSAVE.isPressed()) {
    Keyboard.press(KEY_F5);
    delay(50);
    Keyboard.releaseAll();
  }

  if (buttonLOAD.isPressed()) {
    Keyboard.press(KEY_F9);
    delay(2000);
    Keyboard.releaseAll();
  }

  if (buttonPAUSE.isPressed()) {
    Keyboard.press(KEY_ESC);
    delay(100);
    Keyboard.releaseAll();
  }

  if (buttonWARP.isPressed()) {
    Keyboard.press('/');
    delay(100);
    Keyboard.releaseAll();
  }

  if (buttonMAP.isPressed()) {
    Keyboard.press('m');
    delay(100);
    Keyboard.releaseAll();
  }

  unsigned char warpResult = warpRotary.process();

  if (warpResult == DIR_CW) {
    Keyboard.press('.');
    delay(100);
    Keyboard.releaseAll();
  } else if (warpResult == DIR_CCW) {
    Keyboard.press(',');
    delay(100);
    Keyboard.releaseAll();
  }

  unsigned char mapResult = mapRotary.process();

  if (mapResult == DIR_CW) {
    Keyboard.press(KEY_TAB);
    delay(100);
    Keyboard.releaseAll();
  }

  if (mapResult == DIR_CCW) {
    Keyboard.press('`');
    delay(100);
    Keyboard.releaseAll();
  }
}

  
