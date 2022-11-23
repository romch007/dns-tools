cmake_minimum_required(VERSION 3.22)

project(dns-tools VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Network
)
qt_standard_project_setup()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(dns-tools
    ${SOURCES}
    ${HEADERS}
)

include_directories(include)

target_link_libraries(dns-tools PRIVATE
    Qt6::Widgets
    Qt6::Network
)

set_target_properties(dns-tools PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

include(GNUInstallDirs)

install(TARGETS dns-tools
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)