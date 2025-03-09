set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION "5")
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_PATH C:/Keil_v5/ARM/ARMCLANG)
find_program(ARM_CC NAMES armclang PATHS ${TOOLCHAIN_PATH}/bin REQUIRED NO_DEFAULT_PATH)
find_program(ARM_CXX NAMES armclang PATHS ${TOOLCHAIN_PATH}/bin REQUIRED NO_DEFAULT_PATH)
find_program(ARM_ASM NAMES armclang PATHS ${TOOLCHAIN_PATH}/bin REQUIRED NO_DEFAULT_PATH)
find_program(ARM_AR NAMES armar PATHS ${TOOLCHAIN_PATH}/bin REQUIRED NO_DEFAULT_PATH)
find_program(ARM_LINK NAMES armlink PATHS ${TOOLCHAIN_PATH}/bin REQUIRED NO_DEFAULT_PATH)
find_program(ARM_FROMELF NAMES fromelf PATHS ${TOOLCHAIN_PATH}/bin REQUIRED NO_DEFAULT_PATH)

set(CMAKE_C_COMPILER ${ARM_CC} CACHE STRING "")
set(CMAKE_CXX_COMPILER ${ARM_CXX} CACHE STRING "")
set(CMAKE_ASM_COMPILER ${ARM_ASM} CACHE STRING "")
set(CMAKE_LINKER ${ARM_LINK} CACHE STRING "")
set(CMAKE_OBJCOPY ${ARM_FROMELF} CACHE STRING "")
set(CMAKE_SIZE ${ARM_FROMELF} CACHE STRING "")

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH})

# MCU specific flags (from arch_options.cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS} -xc -std=c99 --target=arm-arm-none-eabi -mcpu=cortex-m3 -c")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -funsigned-char -fshort-enums -fshort-wchar")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4 -O3 -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -Wno-reserved-identifier")
# set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp --target=arm-arm-none-eabi -mcpu=cortex-m3 -masm=auto -Wa,armasm,--diag_suppress=A1950W -c")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -gdwarf-4")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,armasm,--pd,\"_RTE_ SETA 1\"")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

# Linker flags
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F103X6_FLASH.sct")
set(LINKER_FLAGS "--cpu=${MCU_ARCH} --strict --scatter \"${LINKER_SCRIPT}\" --keep=*(*initcall)")
#set(LINKER_FLAGS "--cpu=${MCU_ARCH} --strict")

set(LINKER_FLAGS "${LINKER_FLAGS} --summary_stderr --info summarysizes --map --load_addr_map_info")
set(LINKER_FLAGS "${LINKER_FLAGS} --xref --callgraph --symbols")
set(LINKER_FLAGS "${LINKER_FLAGS} --info sizes --info totals --info unused --info veneers")
set(LINKER_FLAGS "${LINKER_FLAGS} --list \"${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map\"")

set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS}")

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> ${CMAKE_EXE_LINKER_FLAGS} <OBJECTS> -o <TARGET>")
