### Main ###
set(TARGET set_modulation_curve)

add_executable(${TARGET}
  main.cc
  ../src/FitModulationCurve.cc
  ../src/GetAngle.cc
  )

target_link_libraries(${TARGET}
  ${ROOT_LIB}
  )

install(TARGETS ${TARGET} LIBRARY RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

### END
