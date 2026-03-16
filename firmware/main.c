#include <stdint.h>

/* i.MX8MP Clock Control Module (CCM) Base */
#define CCM_BASE            0x30380000
#define CCM_CCGR92_I2C3     (*(volatile uint32_t *)(CCM_BASE + 0x4250))
#define CCM_CCGR93_I2C4     (*(volatile uint32_t *)(CCM_BASE + 0x4260))

/* i.MX8MP IOMUX Controller (IOMUXC) Base */
#define IOMUXC_BASE         0x30330000
/* IOMUX Registers for I2C3/I2C4 SCL/SDA */
#define SW_MUX_I2C3_SCL     (*(volatile uint32_t *)(IOMUXC_BASE + 0x0188))
#define SW_MUX_I2C3_SDA     (*(volatile uint32_t *)(IOMUXC_BASE + 0x018C))
#define SW_MUX_I2C4_SCL     (*(volatile uint32_t *)(IOMUXC_BASE + 0x0190))
#define SW_MUX_I2C4_SDA     (*(volatile uint32_t *)(IOMUXC_BASE + 0x0194))

/* Mux Mode: 0 is ALT0 (I2C Function) */
#define MUX_MODE_ALT0       0x00000000

void SystemInit(void) {
    /* Startup hook - keep empty for now */
}

void Board_Hardware_Takeover(void) {
    /* 1. Enable Clock Gates (Always On) */
    CCM_CCGR92_I2C3 = 0x00000003;
    CCM_CCGR93_I2C4 = 0x00000003;

    /* 2. Configure Physical Pins to I2C Function */
    SW_MUX_I2C3_SCL = MUX_MODE_ALT0;
    SW_MUX_I2C3_SDA = MUX_MODE_ALT0;
    SW_MUX_I2C4_SCL = MUX_MODE_ALT0;
    SW_MUX_I2C4_SDA = MUX_MODE_ALT0;

    /* Data/Instruction Barriers */
    __asm volatile ("dsb sy");
    __asm volatile ("isb");
}

int main(void) {
    /* Claim the hardware */
    Board_Hardware_Takeover();

    /* Placeholder for Milestone 2.5: Bus Scan */

    while(1) {
        __asm volatile ("nop");
    }
    return 0;
}

void _start(void) {
    main();
}
