cmake_minimum_required(VERSION 3.15)
project(SplatDemo)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    set(IrrKlang_DIR ${PROJECT_SOURCE_DIR}/lib/windows/irrKlang) 
    find_package (IrrKlang REQUIRED)
    include_directories(include
                        src/include)
    
    link_directories(${PROJECT_SOURCE_DIR}/lib/windows)
    
    
    add_executable(SplatDemo    src/main.cpp 
                                src/glad.c
                                src/Shaders.cpp 
                                src/Camera.cpp
                                src/GameLogic.cpp
                                src/GameWindow.cpp
                                src/Renderer.cpp
                                src/SkyBox.cpp
                                src/Floor.cpp
                                src/PlayerView.cpp
                                src/PhysicalEngine.cpp
                                src/Shadow.cpp 
                                src/Music.cpp )
    target_link_libraries(SplatDemo glfw3)# ${IRRKLANG_LIBRARY})
endif()

if(APPLE)

    include_directories(include
                        src/include)

    # 添加目标链接
    set(GLEW_LINK /usr/local/Cellar/glew/2.2.0/lib/libGLEW.2.2.dylib)
    set(GLFW_LINK /usr/local/Cellar/glfw/3.3.2/lib/libglfw.3.dylib)
    set(IRR_KLANG ${PROJECT_SOURCE_DIR}/lib/macOS)
    link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK}
                    ${IRR_KLANG}/libirrklang.dylib
                    ${IRR_KLANG}/ikpFLAC.dylib
                    ${IRR_KLANG}/ikpMP3.dylib)

    # 执行编译命令
    # 这个glad.c在第2步中说过要拉到项目中，注意路径
    set(SOURCE_FILES    src/glad.c 
                        src/main.cpp 
                        src/Shaders.cpp 
                        src/Camera.cpp
                        src/GameLogic.cpp
                        src/GameWindow.cpp
                        src/Renderer.cpp
                        src/SkyBox.cpp
                        src/Floor.cpp
                        src/PlayerView.cpp
                        src/PhysicalEngine.cpp 
                        src/Shadow.cpp 
                        src/Music.cpp)

    add_executable(SplatDemo ${SOURCE_FILES})

    target_link_libraries(SplatDemo "-framework OpenGL")
    target_link_libraries(SplatDemo "-framework GLUT")
endif()