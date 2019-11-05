find_library(LIB_FUZZING_ENGINE FuzzingEngine)

if (NOT LIB_FUZZING_ENGINE)
    find_library(LIB_FUZZER Fuzzer)
    if (NOT LIB_FUZZER)
        message(FATAL_ERROR "Fuzzer not found")
    endif()

    set(LIB_FUZZING_ENGINE ${LIB_FUZZER})
endif()

macro(fuzzer name)
add_executable(${name} ${name}.cc)
target_link_libraries(${name}
    ${LIB_FUZZING_ENGINE}
    event_openssl_static
    event_extra_static    
    event_core_static
    event_static
    event_pthreads_static
    pthread)
endmacro()

fuzzer(bufferevent_fuzz)
fuzzer(regress_fuzz)
