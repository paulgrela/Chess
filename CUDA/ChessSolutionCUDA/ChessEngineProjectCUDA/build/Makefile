
# Location of the CUDA Toolkit
CUDA_PATH ?= /usr/local/cuda-11.2

HOST_COMPILER ?= g++
NVCC          := $(CUDA_PATH)/bin/nvcc -ccbin $(HOST_COMPILER)

INCLUDES  := -I../../../../common/inc
INCLUDES  += -I../../../../CUDA/Common/Platform
INCLUDES  += -I../../../../CUDA/Common/StringUtils
INCLUDES  += -I../../../../CUDA/Common/TerminalColorsUtils
INCLUDES  += -I../../../../CUDA/Common/FileUtils
INCLUDES  += -I../../../../CUDA/Common/DateTimeUtils
INCLUDES  += -I../../../../CUDA/Common/Include
INCLUDES  += -I../../../../CUDA/Common/Logger
INCLUDES  += -I../../../../CUDA/Common/HandleError

all: build

build: ./ChessEngineProjectCUDA

Logger.o: ../../../Common/Logger/Logger.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

DateTimeUtils.o: ../../../Common/DateTimeUtils/DateTimeUtils.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

FileUtils.o: ../../../Common/FileUtils/FileUtils.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

StringUtils.o: ../../../Common/StringUtils/StringUtils.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

HandleErrorCUDA.o: ../../../Common/HandleError/HandleErrorCUDA.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineMain.o: ../ChessEngineMain.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineChessMoveBasicOperations.o: ../ChessEngineChessMoveBasicOperations.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineChessboardFileReader.o: ../ChessEngineChessboardFileReader.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineConfigurationFileReaderWriter.o: ../ChessEngineConfigurationFileReaderWriter.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineChessboardPrinter.o: ../ChessEngineChessboardPrinter.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<
        
ChessEngineDataInitializer.o: ../ChessEngineDataInitializer.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineBestMoveFinder.o: ../ChessEngineBestMoveFinder.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineResultsStatistics.o: ../ChessEngineResultsStatistics.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineMainThreadsManager.o: ../ChessEngineMainThreadsManager.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineThreadsData.o: ../ChessEngineThreadsData.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineAllPossibleMovesGenerator.o: ../ChessEngineAllPossibleMovesGenerator.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA.o: ../ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA.cu
	$(EXEC) $(NVCC) $(INCLUDES) --gpu-architecture=compute_75 --gpu-code=sm_75,compute_75 -maxrregcount=0 -use_fast_math -m64 -Xcompiler -O3 -o $@ -c $<

./ChessEngineProjectCUDA: Logger.o DateTimeUtils.o FileUtils.o StringUtils.o HandleErrorCUDA.o ChessEngineMain.o ChessEngineChessMoveBasicOperations.o ChessEngineChessboardFileReader.o ChessEngineConfigurationFileReaderWriter.o ChessEngineChessboardPrinter.o ChessEngineDataInitializer.o ChessEngineBestMoveFinder.o ChessEngineResultsStatistics.o ChessEngineMainThreadsManager.o ChessEngineThreadsData.o ChessEngineAllPossibleMovesGenerator.o ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA.o
	$(EXEC) $(NVCC) -Xcompiler -static-libgcc -lpthread -lstdc++fs -o $@ $+

run: build
	$(EXEC) ./ChessEngineProjectCUDA

clobber: clean
