#include <stdint.h>

/* i.MX8MP Clock Control Module (CCM) Base */
#define CCM_BASE            0x30380000
#define CCM_CCGR92_I2C3     (*(volatile uint32_t *)(CCM_BASE + 0x4250))
#define CCM_CCGR93_I2C4     (*(volatile uint32_t *)(CCM_BASE + 0x4260))

/* Clock Gate States: 0x3 = Clock always on during RUN/WAIT */
#define CLOCK_ON_VALUE      0x00000003

/* The startup script expects this to exist */
void SystemInit(void) {
    /* Basic CPU/FPU init can go here later */
}

/* The startup script expects a jump to _start, so we provide it as an alias to main */
void _start(void) {
    main();
}

void I2C_Clock_Init(void) {
    CCM_CCGR92_I2C3 = CLOCK_ON_VALUE;
    CCM_CCGR93_I2C4 = CLOCK_ON_VALUE;
    __asm volatile ("dsb sy");
    __asm volatile ("isb");
}

int main(void) {
    I2C_Clock_Init();
    while(1) {
        __asm volatile ("nop");
    }
    return 0;
}
