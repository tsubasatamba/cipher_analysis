### Main ###

install(DIRECTORY ./
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
  FILES_MATCHING PATTERN "*.rb"
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)

### END
