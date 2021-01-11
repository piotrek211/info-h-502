cmake_minimum_required(VERSION 3.16)


project(OPENGL_PROJECT)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenGL REQUIRED)


include_directories( ${OPENGL_INCLUDE_DIRS} )

include_directories( src )

include_directories( libs/spdlog 
					 libs/KHR
					 libs/glad
					 libs/stb
					 libs/assimp/include/)





set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )


# set variables that are needed
set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)


add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(libs/assimp)




# config bullet
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(libs/bullet3)
INCLUDE_DIRECTORIES(libs/bullet3/src)
LIST(APPEND LIBS BulletDynamics BulletCollision BulletCollision LinearMath)

message("start")
foreach(dir ${LIBS})
  message(STATUS "dir='${dir}'")
endforeach()
message("stop")


file(GLOB_RECURSE SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")


source_group("SOURCE" FILES ${SOURCE_LIST})

add_executable(${PROJECT_NAME} ${SOURCE_LIST} )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs ${CMAKE_CURRENT_SOURCE_DIR}/src  ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer)

#include(${CMAKE_SOURCE_DIR}/cmake/FindGLFW.cmake)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME}  ${LIBS})

target_link_libraries(${PROJECT_NAME} OpenGL::GL)

add_custom_target(copy_objects_directories ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/Objects" "${CMAKE_BINARY_DIR}/Objects"
    COMMENT "Copy objects directories to build tree"
    VERBATIM)

add_custom_target(copy_shaders_directories ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/Shaders" "${CMAKE_BINARY_DIR}/Shaders"
    COMMENT "Copy shaders directories to build tree"
    VERBATIM)
