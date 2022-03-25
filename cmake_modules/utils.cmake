


macro(subdirlist result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    message(STATUS "children:${children}")
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            STRING(REGEX REPLACE "(.*)\.cpp" "\\1" FILE_NAME ${child})
            list(APPEND dirlist ${FILE_NAME})
        endif()
    endforeach(child ${children})
    set(${result} ${dirlist})
endmacro(subdirlist result curdir)