# Architecture Options

# Define architecture-specific flags
if(MCU_ARCH STREQUAL "Cortex-M3")
    set(TARGET_FLAGS "-mcpu=Cortex-M3 -mthumb")
    set(ARMCC_TARGET_FLAGS "--cpu=Cortex-M3 --thumb")
elseif(MCU_ARCH STREQUAL "Cortex-M4")
    set(TARGET_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp")
    set(ARMCC_TARGET_FLAGS "--cpu=Cortex-M4 --thumb --fpu=FPv4-SP-D16 --float-abi=softfp")
elseif(MCU_ARCH STREQUAL "Cortex-M7")
    set(TARGET_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp")
    set(ARMCC_TARGET_FLAGS "--cpu=Cortex-M7 --thumb --fpu=FPv5-D16 --float-abi=softfp")
else()
    message(FATAL_ERROR "Unsupported MCU architecture: ${MCU_ARCH}")
endif()
