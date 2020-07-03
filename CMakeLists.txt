

cmake_minimum_required(VERSION 3.1)

project(xtensor-rbf)

# set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9 )

message(STATUS "Forcing tests build type to Release")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/modules/")
include(CheckCXXCompilerFlag)
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

# Optional dependencies
# =====================



# Compilation flags
# =================

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
    if (NOT HAS_CPP14_FLAG)
        message(FATAL_ERROR "Unsupported compiler -- xtensor requires C++14 support!")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math -march=native -pthread")
endif()


# Sources and target
# ==================


# Mandatory dependencies
# ======================

find_package(xtensor REQUIRED CONFIG)
find_package(xsimd REQUIRED)
find_package(xtl REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Threads)


include_directories("/usr/local/include")
link_directories("/usr/local/lib:/usr/lib/aarch64-linux-gnu")

macro(add_xt NAME)
    add_executable(${NAME} ${ARGN} ${XTENSOR_HEADERS})
    target_include_directories(${NAME} PRIVATE ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS})
    target_compile_definitions(${NAME} PRIVATE  HAVE_CBLAS=1 NDEBUG=1)
    set_target_properties(${NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS NO)
    target_link_libraries(${NAME} ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(${NAME} PRIVATE EIGEN_FAST_MATH=1)
    # target_compile_definitions(${NAME} PRIVATE EIGEN_USE_LAPACKE EIGEN_USE_LAPACKE_STRICT)
    target_link_libraries(${NAME} Eigen3::Eigen glog gflags lapacke blas lapack ) 
endmacro(add_xt)


macro(add_simd_xt NAME)
    add_executable(${NAME} ${ARGN} ${XTENSOR_HEADERS})
    target_include_directories(${NAME} PRIVATE ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS})
    target_compile_definitions(${NAME} PRIVATE   NDEBUG=1)
    set_target_properties(${NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS NO)
    target_link_libraries(${NAME} ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(${NAME} PRIVATE EIGEN_FAST_MATH=1)
    target_link_libraries(${NAME} Eigen3::Eigen glog gflags blas lapack) 
    target_compile_definitions(${NAME} PRIVATE XTENSOR_USE_XSIMD )
endmacro(add_simd_xt)
if(APPLE)

endif()



message("\n\n          COMPILING WITH\n======================================\n\n")
message("COMPILER        : ${CMAKE_CXX_COMPILER}")
message("FLAGS           : ${CMAKE_CXX_FLAGS}\n")
message("Found xtensor   : ${xtensor_INCLUDE_DIRS}")
message("Found xsimd     : ${xsimd_INCLUDE_DIRS}\n\n")
message("Found eigen     : ${EIGEN3_INCLUDE_DIR}")




add_xt(xtrbf rbf_xt.cpp)

add_simd_xt(simd_xtrbf rbf_simd_xt.cpp)

add_xt(eigrbf rbf_eig.cpp rbf.cpp)

# add_custom_target(xbenchmark
#     COMMAND xtensor_benchmark --benchmark_out=bench.csv --benchmark_out_format=csv
#     DEPENDS ${XTENSOR_BENCHMARK_TARGET})

# add_custom_target(xpowerbench
#     COMMAND echo "sudo needed to set cpu power governor to performance"
#     COMMAND sudo cpupower frequency-set --governor performance
#     COMMAND xtensor_benchmark --benchmark_out=bench.csv --benchmark_out_format=csv
#     COMMAND sudo cpupower frequency-set --governor powersave
#     DEPENDS ${XTENSOR_BENCHMARK_TARGET})
