### Main ###
set(TARGET draw_spectrum)

add_executable(${TARGET}
  main.cc
  ../src/CountSpectrum.cc
  )

target_link_libraries(${TARGET}
  ${ROOT_LIB}
  )

install(TARGETS ${TARGET} LIBRARY RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

### END
