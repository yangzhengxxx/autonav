set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(ARM_GCC_ROOT "" CACHE PATH "Arm GNU Toolchain installation directory")
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES ARM_GCC_ROOT)
set(_ARM_GCC_HINTS
    "${ARM_GCC_ROOT}/bin"
    "$ENV{ARM_GCC_ROOT}/bin"
)

if(NOT CMAKE_C_COMPILER)
  find_program(CMAKE_C_COMPILER
      NAMES arm-none-eabi-gcc
      HINTS ${_ARM_GCC_HINTS}
      REQUIRED
  )
endif()
if(NOT CMAKE_ASM_COMPILER)
  find_program(CMAKE_ASM_COMPILER
      NAMES arm-none-eabi-gcc
      HINTS ${_ARM_GCC_HINTS}
      REQUIRED
  )
endif()
find_program(CMAKE_OBJCOPY
    NAMES arm-none-eabi-objcopy
    HINTS ${_ARM_GCC_HINTS}
    REQUIRED
)
find_program(CMAKE_SIZE
    NAMES arm-none-eabi-size
    HINTS ${_ARM_GCC_HINTS}
    REQUIRED
)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
