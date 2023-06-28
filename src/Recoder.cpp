
#include "Recoder.h"


static constexpr uint8_t  RECODER_DEBOUNCE_TIME             {200};
static constexpr uint16_t RECODER_P_AND_H_TIMEOUT          {2000};
static constexpr uint16_t DEFAULT_PRESS_AND_HOLD_THRESHOLD {1000};

// good thing for tab completion!
static constexpr uint8_t INTERNAL_RECODER_START     {0x0};
static constexpr uint8_t INTERNAL_RECODER_CW_FINAL  {0x1};
static constexpr uint8_t INTERNAL_RECODER_CW_BEGIN  {0x2};
static constexpr uint8_t INTERNAL_RECODER_CW_NEXT   {0x3};
static constexpr uint8_t INTERNAL_RECODER_CCW_BEGIN {0x4};
static constexpr uint8_t INTERNAL_RECODER_CCW_FINAL {0x5};
static constexpr uint8_t INTERNAL_RECODER_CCW_NEXT  {0x6};


// Good thing this library is GPL-3!
// We can straight up lift the (great) code to read movement by
// Ben Buxton (gh:buxtronix/arduino/tree/master/libraries/Rotary)
constexpr uint8_t ttable[7][4] = {
  {INTERNAL_RECODER_START, INTERNAL_RECODER_CW_BEGIN,
      INTERNAL_RECODER_CCW_BEGIN, INTERNAL_RECODER_START},
  {INTERNAL_RECODER_CW_NEXT, INTERNAL_RECODER_START,
      INTERNAL_RECODER_CW_FINAL,
      INTERNAL_RECODER_START | static_cast<uint8_t>(REEvent::RECODER_CW)},
  {INTERNAL_RECODER_CW_NEXT, INTERNAL_RECODER_CW_BEGIN,
      INTERNAL_RECODER_START, INTERNAL_RECODER_START},
  {INTERNAL_RECODER_CW_NEXT, INTERNAL_RECODER_CW_BEGIN,
      INTERNAL_RECODER_CW_FINAL, INTERNAL_RECODER_START},
  {INTERNAL_RECODER_CCW_NEXT, INTERNAL_RECODER_START,
      INTERNAL_RECODER_CCW_BEGIN, INTERNAL_RECODER_START},
  {INTERNAL_RECODER_CCW_NEXT, INTERNAL_RECODER_CCW_FINAL,
      INTERNAL_RECODER_START,
      INTERNAL_RECODER_START | static_cast<uint8_t>(REEvent::RECODER_CCW)},
  {INTERNAL_RECODER_CCW_NEXT, INTERNAL_RECODER_CCW_FINAL,
      INTERNAL_RECODER_CCW_BEGIN, INTERNAL_RECODER_START},
};


Recoder::Recoder(const uint8_t _clk, const uint8_t _dt,
                 const uint8_t _sw)
    : Recoder(_clk, _dt, _sw, DEFAULT_PRESS_AND_HOLD_THRESHOLD) {
}

Recoder::Recoder(const uint8_t _clk, const uint8_t _dt,
                 const uint8_t _sw,
                 const uint16_t _hold_threshold)
    : clk {_clk},
      dt  {_dt},
      sw  {_sw},
      state {INTERNAL_RECODER_START},
      hold_threshold {_hold_threshold},
      held {0},
      last_press {0},
      last_press_and_hold {0},
      press_pending_p {false},
      suppress_next_event_p {false} {
    pinMode(clk, INPUT);
    pinMode(dt,  INPUT);
    pinMode(sw,  INPUT_PULLUP);
}

REEvent Recoder::readButton() {
    const uint32_t now = millis();
    // If the button is pressed...
    if (digitalRead(sw) == LOW) {
        // and it's been held down without pause...
        if (held) {
            // and it's been held down long enough...
            if ((now - held) >= hold_threshold) {
                held = 0;
                last_press = now;
                last_press_and_hold = now;
                press_pending_p = false;
                suppress_next_event_p = true;
                return REEvent::RECODER_PRESS_AND_HOLD;
            }
            // but we haven't yet reached "press and hold" threshold...
            return REEvent::RECODER_NIL;
        }
        // and if it HASN'T been held down
        else {
            // and the time since the last button press is sufficient
            // (to rule out bouncing)
            if ((now - last_press) >= RECODER_DEBOUNCE_TIME &&
                (now - last_press_and_hold) >= RECODER_P_AND_H_TIMEOUT &&
                !suppress_next_event_p) {
                last_press = now;
                held = now;
                press_pending_p = true;
            }
            // otherwise, we just gotta wait
            return REEvent::RECODER_NIL;
        }
    }
    // but, if it _HASN'T_ been pressed...
    else {
        held = 0;
        last_press_and_hold = 0;
        suppress_next_event_p = false;
        // and there is a press event pending...
        if (press_pending_p) {
            press_pending_p = false;
            return REEvent::RECODER_PRESS;
        }
        return REEvent::RECODER_NIL;
    }
}

// Thanks, Ben Buxton!
REEvent Recoder::readMovement() {
    const uint8_t pinstate = (digitalRead(dt) << 1) | digitalRead(clk);
    state = ttable[state & 0xf][pinstate];
    return static_cast<REEvent>(state & 0x30);
}

