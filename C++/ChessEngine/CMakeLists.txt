cmake_minimum_required(VERSION 3.0.0)
project(ChessEngineProject)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-Ofast")

include_directories(
        "../Common/Platform"
        "../Common/Include"
        "../Common/Logger"
        "../Common/FileUtils"
        "../Common/StringUtils"
        "../Common/DateTimeUtils"
        "../Common/TerminalColorsUtils"
)

add_executable(
        ChessEngineProject
        ../Common/Logger/Logger.h
        ../Common/Logger/Logger.cpp
        ../Common/FileUtils/FileUtils.h
        ../Common/FileUtils/FileUtils.cpp
        ../Common/StringUtils/StringUtils.h
        ../Common/StringUtils/StringUtils.cpp
        ../Common/DateTimeUtils/DateTimeUtils.h
        ../Common/DateTimeUtils/DateTimeUtils.cpp
        ChessEngineMain.cpp
        ChessEngineChessMoveBasicOperations.cpp
        ChessEngineChessboardFileReader.cpp
        ChessEngineConfigurationFileReaderWriter.cpp
        ChessEngineChessboardPrinter.cpp
        ChessEngineDataInitializer.cpp
        ChessEngineBestMoveFinder.cpp
        ChessEngineResultsStatistics.cpp
        ChessEngineAllPossibleMovesGenerator.cpp
        ChessEngineMainThreadsManager.cpp)

target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++ -lpthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)