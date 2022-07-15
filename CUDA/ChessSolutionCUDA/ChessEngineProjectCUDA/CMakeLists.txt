cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CUDA_COMPILER "/usr/local/cuda-10.2/bin/nvcc")
#set(CMAKE_CUDA_COMPILER "/usr/local/cuda-10.2")

project(ChessEngineProjectCUDA LANGUAGES CUDA CXX)

string(APPEND CMAKE_CUDA_FLAGS " -gencode arch=compute_75,code=sm_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3")

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-Ofast")

include_directories(
        "../../Common/HandleError"
        "../../Common/Platform"
        "../../Common/Include"
        "../../Common/Logger"
        "../../Common/FileUtils"
        "../../Common/StringUtils"
        "../../Common/DateTimeUtils"
        "../../Common/TerminalColorsUtils"
)

add_executable(
        ChessEngineProjectCUDA
        ../../Common/Logger/Logger.cpp
        ../../Common/FileUtils/FileUtils.cpp
        ../../Common/StringUtils/StringUtils.cpp
        ../../Common/DateTimeUtils/DateTimeUtils.cpp
        ../../Common/HandleError/HandleErrorCUDA.cu
        ChessEngineMain.cu
        ChessEngineThreadsData.cu
        ChessEngineChessMoveBasicOperations.cu
        ChessEngineChessboardFileReader.cu
        ChessEngineConfigurationFileReaderWriter.cu
        ChessEngineChessboardPrinter.cu
        ChessEngineDataInitializer.cu
        ChessEngineBestMoveFinder.cu
        ChessEngineResultsStatistics.cu
        ChessEngineAllPossibleMovesGenerator.cu
        ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA.cu
        ChessEngineMainThreadsManager.cu
        )

target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++ -lpthread -lstdc++fs /usr/local/cuda-10.2/lib64/libcudart.so)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)