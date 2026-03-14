cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(MCUX_PROJ rpmsg_lite_bridge_cm7)
project(${MCUX_PROJ} LANGUAGES C CXX ASM)

set(SDK_ROOT /home/sharedaccount/m7_sdk_full/mcuxsdk)
set(BOARD_ROOT ${SDK_ROOT}/examples/_boards/evkmimx8mp)
set(EXAMPLE_ROOT ${BOARD_ROOT}/multicore_examples/rpmsg_lite_str_echo_rtos)

# 1. ARCHITECTURE
set(SPECIFIC_FLAGS -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16)
add_compile_options(${SPECIFIC_FLAGS})
add_link_options(${SPECIFIC_FLAGS})

# 2. DEFINE EXECUTABLE
add_executable(${MCUX_PROJ} src/main.c)

# 3. IDENTITY & OS FEATURES
target_compile_definitions(${MCUX_PROJ} PRIVATE
    CPU_MIMX8ML8DVNLZ_cm7
    __USE_SHMEM
    SDK_OS_FREE_RTOS
    MCUXPRESSO_SDK
    FSL_RTOS_FREE_RTOS
    INCLUDE_vTaskDelete=1
)

# 4. SOURCES (Prefixing local files with src/)
target_sources(${MCUX_PROJ} PRIVATE
    src/pin_mux.c 
    src/hardware_init.c 
    src/board.c 
    src/clock_config.c 
    src/system_MIMX8ML8_cm7.c
    ${SDK_ROOT}/devices/i.MX/i.MX8MP/MIMX8ML8/gcc/startup_MIMX8ML8_cm7.S
    ${SDK_ROOT}/drivers/common/fsl_common.c
    ${SDK_ROOT}/drivers/common/fsl_common_arm.c
    ${SDK_ROOT}/drivers/iuart/fsl_uart.c
    ${SDK_ROOT}/drivers/mu/fsl_mu.c
    ${SDK_ROOT}/drivers/rdc/fsl_rdc.c
    ${SDK_ROOT}/devices/i.MX/i.MX8MP/MIMX8ML8/drivers/fsl_clock.c
    ${SDK_ROOT}/devices/i.MX/i.MX8MP/MIMX8ML8/drivers/fsl_audiomix.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/rpmsg_lite/rpmsg_lite.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/common/llist.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/rpmsg_lite/porting/environment/rpmsg_env_freertos.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/virtio/virtqueue.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/rpmsg_lite/rpmsg_ns.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/rpmsg_lite/rpmsg_queue.c
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/rpmsg_lite/porting/platform/imx8mp_m7/rpmsg_platform.c
    ${SDK_ROOT}/components/debug_console_lite/fsl_debug_console.c
    ${SDK_ROOT}/components/uart/fsl_adapter_iuart.c
    ${SDK_ROOT}/components/str/fsl_str.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/tasks.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/list.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/queue.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/timers.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/event_groups.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/portable/MemMang/heap_4.c
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/portable/GCC/ARM_CM7/r0p1/port.c
    ${BOARD_ROOT}/rsc_table.c
)

# 5. INCLUDES (Adding . and src to find app.h and pin_mux.h)
target_include_directories(${MCUX_PROJ} PRIVATE
    src
    .
    ${BOARD_ROOT}
    ${EXAMPLE_ROOT}
    ${EXAMPLE_ROOT}/remote
    ${SDK_ROOT}/arch/arm/CMSIS/Core/Include
    ${SDK_ROOT}/devices/i.MX/i.MX8MP/MIMX8ML8
    ${SDK_ROOT}/devices/i.MX/i.MX8MP/MIMX8ML8/drivers
    ${SDK_ROOT}/devices/i.MX/i.MX8MP/periph
    ${SDK_ROOT}/drivers/common
    ${SDK_ROOT}/drivers/iuart
    ${SDK_ROOT}/drivers/mu
    ${SDK_ROOT}/drivers/rdc
    ${SDK_ROOT}/components/uart
    ${SDK_ROOT}/components/debug_console_lite
    ${SDK_ROOT}/components/str
    ${SDK_ROOT}/middleware/multicore/remoteproc
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/include
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/include/platform/imx8mp_m7
    ${SDK_ROOT}/middleware/multicore/rpmsg-lite/lib/include/environment/freertos
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/include
    ${SDK_ROOT}/rtos/freertos/freertos-kernel/portable/GCC/ARM_CM7/r0p1
)

# 6. LINKER
set(LINKER_SCRIPT ${SDK_ROOT}/examples/_boards/evkmimx8mp/gcc/MIMX8ML8xxxxx_cm7_lpa_ram.ld)
target_link_options(${MCUX_PROJ} PRIVATE
    -T${LINKER_SCRIPT}
    ${SPECIFIC_FLAGS}
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)
