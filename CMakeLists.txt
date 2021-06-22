cmake_minimum_required(VERSION 3.19)
project(chess)

set(CMAKE_CXX_STANDARD 17)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019") # e.g. "path/to/Qt/lib/cmake\" or "/usr/include/{host}/qt{version}/"
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "mainwindow")

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

add_executable(chess
        "main.cpp"
        "mainwindow/mainwindow.cpp"
        "chessboard/board.cpp"
        "chessboard/field.cpp"
        "chessboard/chessObject.cpp"
        "gamecontrol/game.cpp"
        "gamecontrol/history.cpp"
        "gamemodes/local.cpp"
        "gamemodes/engine.cpp"
        "gamemodes/server.cpp"
        "gamemodes/client.cpp"
        "gamemodes/online.cpp"
        "gamemodes/gameMode.cpp"
        "icons/qtres.qrc")

target_link_libraries(chess ${REQUIRED_LIBS_QUALIFIED})
add_custom_command(TARGET chess
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chess> ${CMAKE_CURRENT_LIST_DIR})
