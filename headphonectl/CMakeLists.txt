project(headphonectl)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(headphonectl_SRCS media.cpp radiomode.cpp nullmode.cpp vlcmode.cpp mode.cpp headphonectl.cpp main.cpp)
qt4_automoc(${headphonectl_SRCS})
add_executable(headphonectl ${headphonectl_SRCS})
target_link_libraries(headphonectl ${QT_QTCORE_LIBRARY})
target_link_libraries(headphonectl ${QT_QTGUI_LIBRARY})
target_link_libraries(headphonectl Xtst)
