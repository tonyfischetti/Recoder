# Recoder

This is a  simple rotary encoder Arduino library with 'press and hold' support.
Designed for rotary encoders with integrated push button.

### example

```{cpp}
...

// arduino uno
#define RE_CLK       13
#define RE_DT_LAG    12
#define RE_SW_BUTTON 11



Recoder recoder(RE_CLK, RE_DT_LAG, RE_SW_BUTTON);

// or Recoder recoder(RE_CLK, RE_DT_LAG, RE_SW_BUTTON,
                      1500); // with customizable 'press and hold' threshold

void update_rotary_encoder() {
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

...

```

