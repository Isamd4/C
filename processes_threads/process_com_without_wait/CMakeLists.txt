project(PROCESS_COM_NO_WAIT)

cmake_minimum_required(VERSION 3.20)

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${APP_DIR}/main.c)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${APP_DIR}


)
