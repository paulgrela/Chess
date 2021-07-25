
INCLUDES  := -I../../Common/Platform
INCLUDES  += -I../../Common/StringUtils
INCLUDES  += -I../../Common/TerminalColorsUtils
INCLUDES  += -I../../Common/FileUtils
INCLUDES  += -I../../Common/DateTimeUtils
INCLUDES  += -I../../Common/Include
INCLUDES  += -I../../Common/Logger

all: build

build: ./ChessEngineProjectMPI

Logger.o: ../../Common/Logger/Logger.cpp ../../Common/Logger/Logger.h
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

DateTimeUtils.o: ../../Common/DateTimeUtils/DateTimeUtils.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

FileUtils.o: ../../Common/FileUtils/FileUtils.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

StringUtils.o: ../../Common/StringUtils/StringUtils.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineMain.o: ../ChessEngineMain.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineChessMoveBasicOperations.o: ../ChessEngineChessMoveBasicOperations.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineChessboardFileReader.o: ../ChessEngineChessboardFileReader.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineConfigurationFileReaderWriter.o: ../ChessEngineConfigurationFileReaderWriter.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineChessboardPrinter.o: ../ChessEngineChessboardPrinter.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<
        
ChessEngineDataInitializer.o: ../ChessEngineDataInitializer.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineBestMoveFinder.o: ../ChessEngineBestMoveFinder.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineResultsStatistics.o: ../ChessEngineResultsStatistics.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineMainThreadsManager.o: ../ChessEngineMainThreadsManager.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

ChessEngineAllPossibleMovesGenerator.o: ../ChessEngineAllPossibleMovesGenerator.cpp
	mpic++ $(INCLUDES) -std=c++20 -Ofast -o $@ -c $<

./ChessEngineProjectMPI: DateTimeUtils.o FileUtils.o StringUtils.o Logger.o ChessEngineMain.o ChessEngineChessMoveBasicOperations.o ChessEngineChessboardFileReader.o ChessEngineConfigurationFileReaderWriter.o ChessEngineChessboardPrinter.o ChessEngineDataInitializer.o ChessEngineBestMoveFinder.o ChessEngineResultsStatistics.o ChessEngineMainThreadsManager.o ChessEngineAllPossibleMovesGenerator.o
	mpic++ -std=c++17 -Ofast -lm -static-libgcc -static-libstdc++ -lpthread -lstdc++fs -o $@ $+

run: build
	$(EXEC) ./ChessEngineProjectMPI

clobber: clean
