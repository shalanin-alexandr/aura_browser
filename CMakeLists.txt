cmake_minimum_required(VERSION 3.16)
project(AuraBrowser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets)

qt6_add_resources(RESOURCES resources.qrc)

add_executable(AuraBrowser
    main.cpp
    BrowserWindow.cpp
    BrowserWindow.h
    TabWidget.cpp
    TabWidget.h
    HistoryManager.cpp
    HistoryManager.h
    BookmarkManager.cpp
    BookmarkManager.h
    DataManager.cpp
    DataManager.h
    SplashScreen.cpp
    SplashScreen.h
    ${RESOURCES}
)

target_link_libraries(AuraBrowser Qt6::Widgets Qt6::WebEngineWidgets)
