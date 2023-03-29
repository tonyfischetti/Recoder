
#include <stdint.h>
#include <Arduino.h>
#include <elapsedMillis.h>
#include <Recoder.h>


#define CHECK_RE_EVERY 30
#define PRESS_AND_HOLD_THRESHOLD 1500

// arduino uno
#define RE_CLK       13
#define RE_DT_LAG    12
#define RE_SW_BUTTON 11


elapsedMillis re_timer;

Recoder recoder(RE_CLK, RE_DT_LAG, RE_SW_BUTTON);
//Recoder recoder(RE_CLK, RE_DT_LAG, RE_SW_BUTTON, PRESS_AND_HOLD_THRESHOLD);


void update_re() {
    uint8_t mres = recoder.readMovement();
    uint8_t bres = recoder.readButton();

    // movement
    switch (mres) {
        case RECODER_CW:
            Serial.println("clockwise");
            return;
        case RECODER_CCW:
            Serial.println("counter clockwise");
            return;
    }

    // button
    switch (bres) {
        case RECODER_PRESS:
            Serial.println("sw button press!");
            break;
        case RECODER_PRESS_AND_HOLD:
            Serial.println("pressed _AND_ held");
            break;
    }
}



void update_all_devices() {
    if (re_timer >= CHECK_RE_EVERY) {
        update_re();
    }
}


void setup() {
    Serial.begin(115200);
    Serial.println(F("started serial"));
}


void loop() {
    update_all_devices();
}


