
#pragma once

#include <stdint.h>
#include <Arduino.h>


enum class REEvent : uint8_t {
    RECODER_NIL             = 0x0,
    RECODER_CW              = 0x10,
    RECODER_CCW             = 0x20,
    RECODER_PRESS           = 0x02,
    RECODER_PRESS_AND_HOLD  = 0x03
};


class Recoder {
    public:
        Recoder(const uint8_t, const uint8_t, const uint8_t);
        Recoder(const uint8_t, const uint8_t, const uint8_t, const uint16_t);
        REEvent readButton();
        REEvent readMovement();
    private:
        uint8_t  clk;
        uint8_t  dt;
        uint8_t  sw;
        uint8_t  state;
        uint16_t hold_threshold;
        uint32_t held;
        uint32_t last_press;
        uint32_t last_press_and_hold;
        bool press_pending_p;
        bool suppress_next_event_p;
};

