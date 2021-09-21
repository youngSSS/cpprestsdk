cmake_minimum_required(VERSION 3.16)
project(kakao)

set(CMAKE_CXX_STANDARD 14)

set(CPPRESTSDK_INCLUDE_DIR /usr/local/opt/cpprestsdk/include)
set(CPPRESTSDK_SOURCE_DIR /usr/local/opt/cpprestsdk/lib)

set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/include)
set(OPENSSL_SOURCE_DIR /usr/local/opt/openssl/lib)

set(BOOST_INCLUDE_DIR /usr/local/opt/boost/include)
set(BOOST_SOURCE_DIR /usr/local/opt/boost/lib)

include_directories(${CPPRESTSDK_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})

link_directories(${CPPRESTSDK_SOURCE_DIR})
link_directories(${OPENSSL_SOURCE_DIR})
link_directories(${BOOST_SOURCE_DIR})

add_executable(kakao main.cpp)

target_link_libraries(kakao
        ${CPPRESTSDK_SOURCE_DIR}/libcpprest.2.10.dylib
        ${CPPRESTSDK_SOURCE_DIR}/libcpprest.dylib
        ${OPENSSL_SOURCE_DIR}/libssl.dylib
        ${OPENSSL_SOURCE_DIR}/libcrypto.dylib
        ${BOOST_SOURCE_DIR}/libboost_system.dylib
        )

#find_package(Boost)
#if (Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
#endif()
#
#find_package(OpenSSL REQUIRED)
#if (OPENSSL_FOUND)
#    include_directories(${OPENSSL_INCLUDE_DIR})
#
#    message(STATUS "OpenSSL Found: ${OPENSSL_VERSION}")
#    message(STATUS "OpenSSL Include: ${OPENSSL_INCLUDE_DIR}")
#    message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
#endif()
#
#add_executable(kakao main.cpp)
#
#target_include_directories(kakao PRIVATE ${CMAKE_SOURCE_DIR}/casablanca/Release/include)
#target_link_libraries(kakao ${OPENSSL_LIBRARIES})