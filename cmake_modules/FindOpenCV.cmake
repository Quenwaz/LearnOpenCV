


set(LIBNAME OpenCV)
set(SDKDIR F:/DevEnvs/CppLibrary/opencvsdk)
# set(SDKDIR E:/DevEnvs/Cpp/opencvsdk)


set(${LIBNAME}_INCLUDES ${SDKDIR}/include)
if (${IsDebug})
    set(${LIBNAME}_PATH ${SDKDIR}/${ARCHITECTURE}/libd)
else(${IsDebug})
    set(${LIBNAME}_PATH ${SDKDIR}/${ARCHITECTURE}/lib)
endif(${IsDebug})


file(GLOB ${LIBNAME}_LIBRARIES ${${LIBNAME}_PATH}/*.lib)
# find_file(${LIBNAME}_LIBRARIES NAMES *.lib PATHS ${${LIBNAME}_PATH})

message(STATUS ${LIBNAME}_PATH=${${LIBNAME}_PATH})
message(STATUS ${LIBNAME}_LIBRARIES=${${LIBNAME}_LIBRARIES})

if (${LIBNAME}_INCLUDES AND ${LIBNAME}_LIBRARIES)
    set(${LIBNAME}_FOUND TRUE)
endif (${LIBNAME}_INCLUDES AND ${LIBNAME}_LIBRARIES)

set($ENV{PATH} $ENV{PATH} ${${LIBNAME}_PATH})
