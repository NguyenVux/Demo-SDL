cmake_minimum_required(VERSION 3.30)
project(DemoSDL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(GAME_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/FSM.cpp
    ${CMAKE_SOURCE_DIR}/src/FSM.h
    ${CMAKE_SOURCE_DIR}/src/IState.h
    ${CMAKE_SOURCE_DIR}/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/AssetsManager.cpp
    ${CMAKE_SOURCE_DIR}/src/AssetsManager.h
    ${CMAKE_SOURCE_DIR}/src/DebugLayer.cpp
    ${CMAKE_SOURCE_DIR}/src/DebugLayer.h
    ${CMAKE_SOURCE_DIR}/src/ILayer.h
    ${CMAKE_SOURCE_DIR}/src/MainGameLayer.cpp
    ${CMAKE_SOURCE_DIR}/src/MainGameLayer.h
    ${CMAKE_SOURCE_DIR}/src/Time.h
    ${CMAKE_SOURCE_DIR}/src/Time.cpp
    ${CMAKE_SOURCE_DIR}/src/Animation.h
    ${CMAKE_SOURCE_DIR}/src/Animation.cpp
    ${CMAKE_SOURCE_DIR}/src/LayerStack.cpp
    ${CMAKE_SOURCE_DIR}/src/LayerStack.h
    ${CMAKE_SOURCE_DIR}/src/RenderQueue.h
    ${CMAKE_SOURCE_DIR}/src/RenderQueue.cpp
    ${CMAKE_SOURCE_DIR}/src/Components.h
    ${CMAKE_SOURCE_DIR}/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene.h
)

add_subdirectory(entt)

find_package(glm CONFIG REQUIRED)
LIST(APPEND LINK_OPTIONS)
LIST(APPEND COMPILE_OPTIONS)
LIST(APPEND LINK_LIBRARIES glm::glm EnTT::EnTT)

if(EMSCRIPTEN)
    LIST(APPEND COMPILE_OPTIONS 
        --use-port=sdl2
        --use-port=sdl2_image:formats=png
    )
    LIST(APPEND LINK_OPTIONS 
        --use-port=sdl2
        --use-port=sdl2_image:formats=png
        -sWASM=1 
        -sALLOW_MEMORY_GROWTH=1
        -sFULL_ES2=1
        -sUSE_WEBGL2=1
        --shell-file ${CMAKE_SOURCE_DIR}/dist/index.html
        --preload-file ${CMAKE_SOURCE_DIR}/assets/@assets/
        --use-preload-plugins
    )
else()
    find_package(SDL2 REQUIRED CONFIG)
    find_package(SDL2_image REQUIRED CONFIG)
    set(CMAKE_C_LINKER_WRAPPER_FLAG "-Wl,")
    set(CMAKE_CXX_LINKER_WRAPPER_FLAG "-Wl,")
    LIST(APPEND LINK_LIBRARIES SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)
endif()

if(WIN32)
add_executable(DemoSDL WIN32 ${GAME_SOURCES})
else()
add_executable(DemoSDL ${GAME_SOURCES})
endif()
target_link_libraries(DemoSDL PRIVATE ${LINK_LIBRARIES})
target_precompile_headers(DemoSDL PRIVATE "${CMAKE_SOURCE_DIR}/src/pch.h")
target_compile_options(DemoSDL PRIVATE ${COMPILE_OPTIONS})
target_link_options(DemoSDL PRIVATE ${LINK_OPTIONS})
