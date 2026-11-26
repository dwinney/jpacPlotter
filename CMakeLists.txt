cmake_minimum_required (VERSION 3.30)
cmake_policy(VERSION 3.30)

project (jpacUtils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fPIC -O3") 
set(CMAKE_BUILD_TYPE "Release")

# Set output lib and bin locations
set(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_BINDIR})

##-----------------------------------------------------------------------
## ROOT finding and connecting

# Make sure we can find ROOT 
execute_process(COMMAND root-config --prefix 
                COMMAND tr -d \\n
                RESULT_VARIABLE RC_RESULT 
                OUTPUT_VARIABLE ROOT_PREFIX)
list(APPEND CMAKE_MODULE_PATH "${ROOT_PREFIX}/share/root/cmake")

# Find ROOT
find_package(ROOT REQUIRED COMPONENTS MathMore)
if (ROOT_FOUND)
    message("-- ROOT found! (${ROOT_USE_FILE})")
    #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
    include(${ROOT_USE_FILE})
    list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
else()
    message(SEND_ERROR "-- Root not found")
endif()

##-----------------------------------------------------------------------
# Build core library 

add_subdirectory(jpacUtils)