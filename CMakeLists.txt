cmake_minimum_required(VERSION 3.16)
project(cpprestsdk)

set(CMAKE_CXX_STANDARD 14)

# Project Header Files
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# CPPRESTDSK (MS Casablanca)
set(CPPRESTSDK_INCLUDE_DIR /usr/local/opt/cpprestsdk/include)
set(CPPRESTSDK_SOURCE_DIR /usr/local/opt/cpprestsdk/lib)

# OpenSSL (Dependency for Casablanca)
set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/include)
set(OPENSSL_SOURCE_DIR /usr/local/opt/openssl/lib)

# Boost (Dependency for Casablanca)
set(BOOST_INCLUDE_DIR /usr/local/opt/boost/include)
set(BOOST_SOURCE_DIR /usr/local/opt/boost/lib)

include_directories(${CPPRESTSDK_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${HEADER_DIR})

link_directories(${CPPRESTSDK_SOURCE_DIR})
link_directories(${OPENSSL_SOURCE_DIR})
link_directories(${BOOST_SOURCE_DIR})

add_executable(cpprestsdk main.cpp Api.cpp Utils.cpp)

target_link_libraries(cpprestsdk
        ${CPPRESTSDK_SOURCE_DIR}/libcpprest.2.10.dylib
        ${CPPRESTSDK_SOURCE_DIR}/libcpprest.dylib
        ${OPENSSL_SOURCE_DIR}/libssl.dylib
        ${OPENSSL_SOURCE_DIR}/libcrypto.dylib
        ${BOOST_SOURCE_DIR}/libboost_system.dylib
        )