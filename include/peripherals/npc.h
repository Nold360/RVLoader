/*
 * Nolds Portable Coprocessor
 */

#define NPC_ADDR (0x42 << 1)

#define FEATURE_LED             0b00000001
#define FEATURE_DISPLAY         0b00000010
#define FEATURE_BTAUDIO         0b00000100

#define CMD_VERSION             0x00
#define CMD_FEATURES            0x01
#define CMD_RESET               0x02

#define CMD_LED_COLOR           0x10
#define CMD_LED_COLOR_END       0x1F
#define CMD_LED_SPEED           0x0A // for animation modes
#define CMD_LED_INDEX           0x0B // for mode = COLOR or animation
#define CMD_LED_NUM             0x0C // Number of LEDS
#define CMD_LED_MODE            0x0D // Current Mode
#define CMD_LED_PREDEF_COLORS   0x0E // Count of predefined colors
#define CMD_LED_ANIMATIONS      0x0F // Number of predefined animations

#define LED_MODE_STATIC         0x01
#define LED_MODE_ANIMATIO       0x02
#define LED_MODE_COLOR          0x02

// Display Commands
#define CMD_DISPLAY_UP          0x30
#define CMD_DISPLAY_DOWN        0x31
#define CMD_DISPLAY_MENU        0x32
#define CMD_DISPLAY_ENTER       0x33

//BTAUDIO Commands
#define CMD_BTAUDIO_ENABLE 			0x41
#define CMD_BTAUDIO_PAIR 				0x42

namespace NPC {
    bool isConnected();
    u8 getVer();
    u8 getFeatures();

    void setColor(u8 led_number, u8 r, u8 g, u8 b);
    void setLedMode(u8 mode);
    void setLedCount(u8 count);
    void setLedModeIndex(u8 index);
    void setLedModeSpeed(u8 speed);

    u8 getLedMode();
    u8 getLedCount();
    u8 getLedColorCount();
    u8 getLedAnimationCount();
    u8 getLedModeIndex();
    u8 getLedModeSpeed();

    void setDisplay(u8 cmd);

    void toggleBtAudioEnable();
    void toggleBtAudioPair();
};
