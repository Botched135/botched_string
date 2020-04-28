cmake_minimum_required(VERSION 3.0.0)
project(bons_str VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(bons_str "")

target_sources(bons_str
	PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bstring.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(bons_str
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)