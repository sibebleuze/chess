cmake_minimum_required(VERSION 3.19)
project(chess)

set(CMAKE_CXX_STANDARD 17)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "mainwindow")

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

add_executable(chess
        "main.cpp"
        "mainwindow/mainwindow.cpp"
        "chessboard/board.cpp"
        "chessboard/line.cpp"
        "chessboard/field.cpp"
        "nvwa-1.1/nvwa/debug_new.cpp") # catch some memory leaks
# TODO: remove nvwa from final project

target_link_libraries(chess ${REQUIRED_LIBS_QUALIFIED})
add_custom_command(TARGET chess
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chess> ${CMAKE_CURRENT_LIST_DIR})
