cmake_minimum_required (VERSION 3.8)

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME}
    Resources.qrc
    main.cpp
    mapwidget.h
    mapwidget.cpp
    mapwidgetWingloong2.h
    mapwidgetWingloong2.cpp
    displaymsg.h
    displaymsg.cpp
)
#
find_package(Qt5 COMPONENTS Core Widgets Positioning REQUIRED)

#
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Positioning)
target_link_libraries(${PROJECT_NAME} PRIVATE Lib::GraphicsMap)
target_link_libraries(${PROJECT_NAME} PRIVATE Lib::Simulation)

#
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION install)
