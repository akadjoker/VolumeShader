
project(core)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

if (WIN32)
    set(LIBS_DIR "E:/windows/libs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -D_CRT_SECURE_NO_WARNINGS")
    if (MSVC)
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            add_compile_options(/RTC1 /Od /Zi)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
        endif()     
    endif()

endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)





set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)



file(GLOB SOURCES "src/*.cpp")
add_library(core STATIC  ${SOURCES})



target_include_directories(core PUBLIC "${LIBS_DIR}/include" include include/std src)
target_precompile_headers(core PRIVATE src/pch.h)


if(CMAKE_BUILD_TYPE MATCHES Debug)

if (UNIX)
    target_compile_options(core PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g -Winvalid-pch -D_DEBUG)
    target_link_options(core PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g -Winvalid-pch -D_DEBUG) 
endif()

elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(core PRIVATE -O3 -march=native -flto -funroll-loops -DNDEBUG)
    target_link_options(core PRIVATE -O3 -march=native -flto -funroll-loops -DNDEBUG)
endif()
