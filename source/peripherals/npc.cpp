#include <string>
#include <malloc.h>
#include <gccore.h>
#include <math.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/machine/processor.h>
#include <ogc/lwp_threads.h>
#include "i2c.h"
#include "debug.h"
#include "npc.h"

namespace NPC {
    bool isConnected() {
        u32 level;
        u8 i2cRet;
        static bool firstTime = true;
        static bool ret = false;

        if (firstTime) {
            firstTime = false;
            _CPU_ISR_Disable(level);
            ret = true;
            i2c_start();
            i2cRet = i2c_sendByte(NPC_ADDR);
            if (!i2cRet) {
                ret = false;
            }
            i2c_stop();
            _CPU_ISR_Restore(level);
        }
        return ret;
    }

    u8 getVer() {
        u8 error;
        u8 verBuffer[1];
        i2c_readBuffer(NPC_ADDR, CMD_GETVER, &error, verBuffer, 1);
        Debug("NPC - Get Version");
        return verBuffer[0];
    }

    void setColor(u8 led_number, u8 r, u8 g, u8 b) {
        u8 error, buffer[3] = {r, g, b};
        Debug("NPC - SetColor");
        i2c_writeBuffer(NPC_ADDR, CMD_LED_COLOR, buffer, 3, &error);
    }

    void setLedMode(u8 mode) {
        u8 error;
        Debug("NPC - SetLEDMode");
        i2c_write8(NPC_ADDR, CMD_LED_MODE, mode, &error);
    }

    u8 getLedMode() {
        return 1;
    }

    void setLedCount(u8 count) {
        u8 error;
        Debug("NPC - SetLEDCount");
        i2c_write8(NPC_ADDR, CMD_LED_NUM, count, &error);
    }

    u8 getLedCount() {
        return 3;
    }

    void setDisplay(u8 cmd) {
        u8 error;
        Debug("NPC - SetDisplay");
        i2c_write8(NPC_ADDR, CMD_LED_NUM, cmd, &error);
    }
};
