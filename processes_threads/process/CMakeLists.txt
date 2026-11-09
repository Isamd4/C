

project(ProcessPlay)

cmake_minimum_required(VERSION 3.20)

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main 
    ${APP_DIR}/main.c

)

target_include_directories(main PRIVATE
    ${APP_DIR}
    
)


