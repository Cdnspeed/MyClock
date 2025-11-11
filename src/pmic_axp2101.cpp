#include "pmic_axp2101.h"

static XPowersAXP2101 s_pmic;
static bool s_ready = false;

#ifndef AXP2101_SLAVE_ADDRESS
#define AXP2101_SLAVE_ADDRESS 0x34
#endif

bool pmic_init() {
    // Bring up I2C at 400 kHz on the configured pins if available.
#if defined(IIC_SDA) && defined(IIC_SCL)
    Wire.begin(IIC_SDA, IIC_SCL, 400000);
#else
    Wire.begin();
#endif

    if (!s_pmic.begin(Wire, AXP2101_SLAVE_ADDRESS, IIC_SDA, IIC_SCL)) {
        return false;
    }

    s_ready = true;
    pmic_enable_display_power();
    return true;
}

void pmic_enable_display_power() {
    if (!s_ready) return;

    // Match the behaviour of your known-good project: enable all ALDOs.
    s_pmic.enableALDO1();
    s_pmic.enableALDO2();
    s_pmic.enableALDO3();
    s_pmic.enableALDO4();
}

void pmic_set_brightness(uint8_t level_0_to_10) {
    (void)level_0_to_10;
    // On this board we prefer panel-side brightness via the CO5300 DCS
    // registers (0x51/0x53), so this is intentionally left as a no-op.
}
