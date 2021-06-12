
#define NPC_ADDR (0x42 << 1)

#define FEATURE_LED             0b00000001
#define FEATURE_DISPLAY         0b00000010

#define CMD_GETVER              0x00
#define CMD_FEATURES            0x01
#define CMD_LED_COLOR           0x10
#define CMD_LED_COLOR_END       0x1F
#define CMD_LED_NUM             0x0E
#define CMD_LED_MODE            0x0F

#define LED_MODE_STATIC         0x01
#define LED_MODE_RAINBOW        0x02

#define CMD_DISPLAY_UP          0x30
#define CMD_DISPLAY_DOWN        0x31
#define CMD_DISPLAY_MENU        0x32
#define CMD_DISPLAY_ENTER       0x33

namespace NPC {
    bool isConnected();
    u8 getVer();
    void setColor(u8 led_number, u8 r, u8 g, u8 b);
    void setLedMode(u8 mode);
    u8 getLedMode();
    void setLedCount(u8 count);
    u8 getLedCount();
    void setDisplay(u8 cmd);
};
