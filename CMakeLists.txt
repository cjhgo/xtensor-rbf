

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



# Mandatory dependencies
# ======================

find_package(xtensor REQUIRED CONFIG)
find_package(xsimd REQUIRED)
find_package(xtl REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Threads)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)



message("\n\n          COMPILING WITH\n======================================\n\n")
message("COMPILER        : ${CMAKE_CXX_COMPILER}")
message("FLAGS           : ${CMAKE_CXX_FLAGS}\n")
message("Found xtensor   : ${xtensor_INCLUDE_DIRS}")
message("Found xsimd     : ${xsimd_INCLUDE_DIRS}\n\n")
message("Found eigen     : ${EIGEN3_INCLUDE_DIR}")
message(STATUS "BLA VENDOR:    " ${BLA_VENDOR})
message(STATUS "BLAS LIBRARIES: " ${BLAS_LIBRARIES})
message(STATUS "LAPACK LIBRARIES: " ${LAPACK_LIBRARIES})

add_definitions(-DHAVE_CBLAS=1)


macro(add_exe NAME)
    add_executable(${NAME} ${ARGN} ${XTENSOR_HEADERS})
    target_include_directories(${NAME} PRIVATE ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS})
    target_include_directories(${NAME} PRIVATE ${EIGEN3_INCLUDE_DIR} )
    target_compile_definitions(${NAME} PRIVATE   NDEBUG=1)
    set_target_properties(${NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS NO)
    target_link_libraries(${NAME} ${CMAKE_THREAD_LIBS_INIT})
endmacro(add_exe)


macro(add_xt_lib NAME)
    add_library(${NAME} OBJECT ${ARGN} ${XTENSOR_HEADERS})
    target_include_directories(${NAME} PRIVATE ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS})
    target_compile_definitions(${NAME} PRIVATE  NDEBUG=1)
    set_target_properties(${NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS NO)
    target_link_libraries(${NAME} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(${NAME} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ) 
endmacro(add_xt_lib)


macro(add_simd_xt NAME)
    add_xt_lib(lib_rbf_simd_xt rbf_xt.cpp)
    target_compile_definitions(lib_rbf_simd_xt PRIVATE  XTENSOR_USE_XSIMD=1)

    add_exe(${NAME} rbf_xt_main.cpp)
    target_link_libraries(${NAME} lib_rbf_simd_xt)
    target_compile_definitions(${NAME} PRIVATE  XTENSOR_USE_XSIMD=1)
endmacro(add_simd_xt)



macro(add_eig_lib NAME)
    add_library(${NAME} OBJECT ${ARGN} )
    target_include_directories(${NAME} PRIVATE ${EIGEN3_INCLUDE_DIR} )
    target_compile_definitions(${NAME} PRIVATE  NDEBUG=1)
    set_target_properties(${NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS NO)
    target_link_libraries(${NAME} ${CMAKE_THREAD_LIBS_INIT})
endmacro(add_eig_lib)

add_xt_lib(lib_rbf_xt rbf_xt.cpp)
add_exe(rbf_xt rbf_xt_main.cpp)
target_link_libraries(rbf_xt lib_rbf_xt)

add_simd_xt(rbf_simd_xt)


add_eig_lib(lib_rbf_eig rbf_eig.cpp)
add_exe(rbf_eig rbf_eig_main.cpp)
target_link_libraries(rbf_eig lib_rbf_eig)



add_eig_lib(lib_rbf_eig_la rbf_eig.cpp)
target_compile_definitions(lib_rbf_eig_la PRIVATE EIGEN_USE_LAPACKE)
if(APPLE)
target_link_libraries(lib_rbf_eig_la  "/usr/local/Cellar/lapack/3.8.0_2/lib/liblapacke.dylib")
else()
target_link_libraries(lib_rbf_eig_la lapacke)
endif()

add_exe(rbf_eig_la rbf_eig_main.cpp)
target_link_libraries(rbf_eig_la lib_rbf_eig_la)