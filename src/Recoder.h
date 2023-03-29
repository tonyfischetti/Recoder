
#include <stdint.h>
#include <Arduino.h>


// can I read from registers directly?
//   (using #ifdef to do it for multiple boards?)


#ifndef recoder_h
#define recoder_h

#define RECODER_NIL             0x0
#define RECODER_CW              0x10
#define RECODER_CCW             0x20
#define RECODER_PRESS           0x02
#define RECODER_PRESS_AND_HOLD  0x03


class Recoder {
    public:
        Recoder(uint8_t, uint8_t, uint8_t);
        Recoder(uint8_t, uint8_t, uint8_t, uint16_t);
        uint8_t readButton();
        uint8_t readMovement();
    private:
        void init(uint8_t, uint8_t, uint8_t, uint16_t);
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

#endif

