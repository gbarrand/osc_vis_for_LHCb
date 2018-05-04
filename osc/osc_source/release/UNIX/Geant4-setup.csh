
setenv G4ANALYSIS_USE 1
setenv G4ANALYSIS_AIDA_CONFIG_CFLAGS `aida-config --include`
setenv G4ANALYSIS_AIDA_CONFIG_LIBS `aida-config --lib`

setenv G4VIS_NONE 1
setenv G4UI_NONE 1

unsetenv G4LIB_USE_GRANULAR
setenv G4LIB_BUILD_SHARED 1
