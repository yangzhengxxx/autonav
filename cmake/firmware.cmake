set(STM32_CPU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

function(add_stm32_firmware TARGET)
  cmake_parse_arguments(FW "" "ROOT" "SOURCES;INCLUDES" ${ARGN})

  add_executable(${TARGET}
      ${FW_SOURCES}
      "${CMAKE_SOURCE_DIR}/cmake/startup_stm32f407xx_gcc.S"
      "${CMAKE_SOURCE_DIR}/cmake/syscalls.c"
  )

  target_include_directories(${TARGET} PRIVATE ${FW_INCLUDES})
  target_compile_definitions(${TARGET} PRIVATE
      STM32F40_41xxx
      USE_STDPERIPH_DRIVER
      HSE_VALUE=8000000
  )
  target_compile_options(${TARGET} PRIVATE
      ${STM32_CPU_FLAGS}
      $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
      $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
      $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
      $<$<COMPILE_LANGUAGE:C>:-Wall>
      $<$<COMPILE_LANGUAGE:C>:-Wextra>
      $<$<COMPILE_LANGUAGE:C>:-Wno-unused-parameter>
      $<$<CONFIG:Debug>:-Og>
      $<$<CONFIG:Debug>:-g3>
      $<$<CONFIG:Release>:-Os>
      $<$<CONFIG:Release>:-g0>
  )

  target_link_options(${TARGET} PRIVATE
      ${STM32_CPU_FLAGS}
      -T${CMAKE_SOURCE_DIR}/cmake/stm32f407vetx.ld
      --specs=nano.specs
      --specs=nosys.specs
      -Wl,--gc-sections
      -Wl,--print-memory-usage
      -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.map
  )
  target_link_libraries(${TARGET} PRIVATE m)

  set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET} SUFFIX ".elf")
  set_property(TARGET ${TARGET} APPEND PROPERTY
      LINK_DEPENDS "${CMAKE_SOURCE_DIR}/cmake/stm32f407vetx.ld"
  )

  add_custom_command(TARGET ${TARGET} POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O ihex
              $<TARGET_FILE:${TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.hex
      COMMAND ${CMAKE_OBJCOPY} -O binary
              $<TARGET_FILE:${TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.bin
      COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}>
      COMMENT "Generating ${TARGET}.hex and ${TARGET}.bin"
  )
endfunction()
