cmake_minimum_required(VERSION 3.22)

project(autonav_stm32f4 LANGUAGES C ASM)

option(BUILD_BAREMETAL "Build the bare-metal firmware" ON)
option(BUILD_FREERTOS "Build the FreeRTOS firmware" ON)

include(cmake/firmware.cmake)

function(collect_stdperiph_sources OUT ROOT)
  file(GLOB _sources CONFIGURE_DEPENDS "${ROOT}/FWLIB/src/*.c")
  # Match the Keil target exactly: STM32F407 uses FSMC, not the FMC driver.
  list(FILTER _sources EXCLUDE REGEX "/stm32f4xx_fmc\\.c$")
  set(${OUT} ${_sources} PARENT_SCOPE)
endfunction()

function(collect_app_sources OUT ROOT)
  file(GLOB _user CONFIGURE_DEPENDS "${ROOT}/USER/*.c")
  file(GLOB _bsp CONFIGURE_DEPENDS "${ROOT}/BSP/*.c")
  file(GLOB _driver CONFIGURE_DEPENDS "${ROOT}/DRIVER/*.c")
  set(${OUT}
      ${_user}
      ${_bsp}
      ${_driver}
      "${ROOT}/SYSTEM/system_stm32f4xx.c"
      PARENT_SCOPE
  )
endfunction()

if(BUILD_BAREMETAL)
  set(_root "${CMAKE_SOURCE_DIR}/BareMetal")
  collect_stdperiph_sources(_fwlib "${_root}")
  collect_app_sources(_app "${_root}")
  add_stm32_firmware(autonav_baremetal
      ROOT "${_root}"
      SOURCES ${_app} ${_fwlib}
      INCLUDES
          "${_root}/CORE"
          "${_root}/USER"
          "${_root}/SYSTEM"
          "${_root}/FWLIB/inc"
          "${_root}/DRIVER"
          "${_root}/BSP"
  )
endif()

if(BUILD_FREERTOS)
  set(_root "${CMAKE_SOURCE_DIR}/FreeRTOS")
  set(_kernel "${_root}/Middlewares/FreeRTOS")
  collect_stdperiph_sources(_fwlib "${_root}")
  collect_app_sources(_app "${_root}")
  set(_freertos_sources
      "${_kernel}/tasks.c"
      "${_kernel}/queue.c"
      "${_kernel}/list.c"
      "${_kernel}/portable/MemMang/heap_4.c"
      "${_kernel}/portable/GCC/ARM_CM4F/port.c"
  )
  add_stm32_firmware(autonav_freertos
      ROOT "${_root}"
      SOURCES ${_app} ${_fwlib} ${_freertos_sources}
      INCLUDES
          "${_root}/CORE"
          "${_root}/USER"
          "${_root}/SYSTEM"
          "${_root}/FWLIB/inc"
          "${_root}/DRIVER"
          "${_root}/BSP"
          "${_kernel}/include"
          "${_kernel}/portable/GCC/ARM_CM4F"
  )
endif()
