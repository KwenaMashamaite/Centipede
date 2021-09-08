set(SRC_FILES
        main.cpp
        Actors/Mushroom.cpp
        Actors/MushroomField.cpp
        Actors/Player.cpp
        Actors/Bullet.cpp
        Actors/Scorpion.cpp
        Actors/Flea.cpp
        Actors/CentipedeSegment.cpp
        Actors/Centipede.cpp
        GameLoop/Game.cpp
        Scoreboard/Score.cpp
        Scoreboard/Scoreboard.cpp
        Grid/Grid.cpp
        Scenes/GameplayScene.cpp)

# Set executables output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Create executable project source files
add_executable(Centipede ${SRC_FILES})

# Find third party dependency
set(IME_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/lib/cmake/IME")
set(IME_BIN_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/bin")
find_package(IME 2.3.0 REQUIRED)

# Link third party dependency to executable
target_link_libraries (Centipede PRIVATE ime)

# Add <project>/ as include directory
include_directories(${PROJECT_SOURCE_DIR}/)

# Copy runtime dependencies to the executable output folder
add_custom_command(TARGET Centipede PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/${CMAKE_BUILD_TYPE}/" $<TARGET_FILE_DIR:Centipede>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/Runtime/" $<TARGET_FILE_DIR:Centipede>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Res" $<TARGET_FILE_DIR:Centipede>/Res
)