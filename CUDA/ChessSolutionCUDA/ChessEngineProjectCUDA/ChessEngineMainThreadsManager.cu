
#include "DestinationPlatform.h"

#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif

#ifdef UNIX_PLATFORM
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#endif

#include <chrono>
#include <sstream>

#include "StringUtils.h"
#include "DateTimeUtils.h"
#include "ExceptionsMacro.h"

#include "ChessEngineChessboardFileReader.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

#include "ChessEngineMainThreadsManager.h"

using namespace std;

inline void ChessEngineMainThreadsManager::RegisterGenerateAllPossibleMovesForPieceFunctions(const ThreadNumType MaxThreads)
{
	try
	{
		for (auto& GenerateAllPossibleMovesForPiecesObject : GenerateAllPossibleMovesForPiecesFunctors)
		{
			GenerateAllPossibleMovesForPiecesObject[KingNum[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKing<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
			GenerateAllPossibleMovesForPiecesObject[QueenNum[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForQueen<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

			GenerateAllPossibleMovesForPiecesObject[Rook1Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
			GenerateAllPossibleMovesForPiecesObject[Rook2Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

			GenerateAllPossibleMovesForPiecesObject[Bishop1Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
			GenerateAllPossibleMovesForPiecesObject[Bishop2Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

			GenerateAllPossibleMovesForPiecesObject[Knight1Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
			GenerateAllPossibleMovesForPiecesObject[Knight2Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

			for (PieceNumType Pawn = Pawn1Num[White]; Pawn <= Pawn8Num[White]; Pawn++)
				GenerateAllPossibleMovesForPiecesObject[Pawn] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForPawn<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
		}
	}
	CATCH("registering generate all possible move for piece functions")
}

inline void ChessEngineMainThreadsManager::PrepareData(const ThreadNumType MaxThreads, const string& ChessBoardFileName, const uint64_t TestId, const string& ActualDateTimeStr)
{
	try
	{
		LoggersManagerObject.InitializeLoggerManagerDataForTask(string("TEST_" + to_string(TestId)).c_str(), "." OS_DIR_SEP, ActualDateTimeStr, true, 0, function<void(const uint64_t& CurrentThreadId, const uint64_t FileNumber, const string& MessageStr)>());

		for (auto& ChessEngineAllPossibleMovesGeneratorObject : ChessEngineAllPossibleMovesGeneratorObjects)
		{
			ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.ClearAllStatistics();
			ChessEngineChessboardFileReader(ChessBoardFileName, &ChessEngineAllPossibleMovesGeneratorObject.ChessBoardStartData);
			ChessEngineAllPossibleMovesGeneratorObject.ChessEngineDataInitializerObject.ClearDataForAllPossibleMovesGeneratorAlgorithm();
			ChessEngineAllPossibleMovesGeneratorObject.ChessEngineDataInitializerObject.FindStartPositionsOfPieces();
		}

		RegisterGenerateAllPossibleMovesForPieceFunctions(MaxThreads);

		LoggersManagerObject.Log(STREAM("CHESS GAME SITUATION:"));
		ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 1);
		ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineDataInitializerObject.PrintActualStateOfPieces();
		
		LoggersManagerObject.Log(STREAM(""));
	}
	CATCH_AND_THROW("preparing data")
}

inline void ChessEngineMainThreadsManager::GenerateAllPossibleCombinationsOfMoves(const uint64_t TestId)
{
	try
	{
		if (ChessEngineConfigurationFileReaderWriterObject.MultiThreaded == false)
			ChessEngineAllPossibleMovesGeneratorObjects[0].GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();
		else
		{
			const bool IsCheck = ChessEngineAllPossibleMovesGeneratorObjects[0].VerifyCheckOfKing<White, Black>();

			bool PossibleMove = false;

			ThreadNumType MaxThreadsForGet = 0;

			ChessPiece PiecesLocalCopy[NumberOfPieces];
			memcpy(PiecesLocalCopy, ChessEngineAllPossibleMovesGeneratorObjects[0].Pieces, (EndOfWhitePiecesNum + 1) * sizeof(ChessPiece));

			future<bool> ResultsFromGenerateAllPossibleMovesForThread[MaxNumberOfThreads];

			for (PieceNumType PieceNum = StartOfWhitePiecesNum; PieceNum <= EndOfWhitePiecesNum; PieceNum++)
				if (PiecesLocalCopy[PieceNum].Alive == true)
				{
					ResultsFromGenerateAllPossibleMovesForThread[MaxThreadsForGet] = async(launch::async, GenerateAllPossibleMovesForPiecesFunctors[MaxThreadsForGet][PieceNum], &ChessEngineAllPossibleMovesGeneratorObjects[MaxThreadsForGet], PiecesLocalCopy[PieceNum].ActualPosX, PiecesLocalCopy[PieceNum].ActualPosY);
					MaxThreadsForGet++;
				}

			for (ThreadNumType ThreadForGetPos = 0; ThreadForGetPos < MaxThreadsForGet; ThreadForGetPos++)
				if (ResultsFromGenerateAllPossibleMovesForThread[ThreadForGetPos].get() == true)
					PossibleMove = true;

			ChessEngineAllPossibleMovesGeneratorObjects[0].UpdateDataForIsCheckAndPossibleMove<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>(IsCheck, PossibleMove);
		}
	}
	CATCH("generating all possible combinations of moves")
}

inline void ChessEngineMainThreadsManager::SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads(const ThreadNumType MaxThreads)
{
	try
	{
		for (ThreadNumType ThreadPos = 1; ThreadPos <= MaxThreads; ThreadPos++)
			ChessEngineResultsStatistics::SumStatisticsFromTwoChessEngineResultsStatisticsObjects(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject, ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].ChessEngineResultsStatisticsObject);
	}
	CATCH("showing results")
}

inline void ChessEngineMainThreadsManager::InitializeLoggerManagerParameters()
{
	try
	{
		using namespace string_utils;

		LoggersManagerObject.InitializeFilesNames({ "AllMessages", "Information", "Important", "Debugs", "Warnings", "Errors", "Exceptions",  "ErrorsAndExceptions", "Critical", "Statistics" });
		LoggersManagerObject.InitializeSelectiveWordsFunctions(
		{ 
          [](const string& s) { return true; },
          [](const string& s) { return upper_case_str_transform(s).find("INFO") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("IMPORTANT") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("DEBUG") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("WARN") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("ERROR") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("EXCEPTION") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("ERRORS") != string::npos || upper_case_str_transform(s).find("EXCEPTION") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("CRITICAL") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("STATISTICS") != string::npos; }
		});

        LoggersManagerObject.InitializePrintingParameters(ChessEngineConfigurationFileReaderWriterObject.PrintLogToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogToFiles,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogLineNumberToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogDateTimeToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessIdToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessPriorityLevelToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogThreadIdToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogLineNumberToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogDateTimeToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessIdToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessPriorityLevelToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogThreadIdToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.MaximalNumberOfLinesInOneFile);
	}
	CATCH("initializing logger manager parameter")
}

ChessEngineMainThreadsManager::ChessEngineMainThreadsManager() : ChessEngineKernelManagerCUDAObject(*this)
{	
	try
	{
        #ifdef WINDOWS_PLATFORM
		ChessEngineConfigurationFileReaderWriterObject.SetProcessPriorityHighest == true ? SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS) : 0;
        #endif

		InitializeLoggerManagerParameters();

		const ThreadNumType MaxThreads = MaxNumberOfThreads - 1;
		
        #ifdef WINDOWS_PLATFORM
		const string ActualDateTimeStr = string("Logs." + GetActualDateTimeWindows(".", ".", ".", ".", ".", "."));
        #endif

        #ifdef UNIX_PLATFORM
        const string ActualDateTimeStr = string("Logs." + GetActualDateTimeStandardCPP(".", ".", ".", ".", "."));
        #endif
																														
		for (const auto& ChessTestObject : ChessEngineConfigurationFileReaderWriterObject.ChessTests)
			if(ChessTestObject.ExecuteTestBool == true)
			{
				ChessEngineKernelManagerCUDAObject.GetMemoryForThreadsDataArray();

				ChessEngineConfigurationFileReaderWriterObject.FirstMove = false;

				ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel = ChessTestObject.MaxDeepLevel;
				ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces = ChessTestObject.ReverseColorOfPieces;

				PrepareData(MaxThreads, ChessTestObject.ChessBoardFileName, ChessTestObject.TestId, ActualDateTimeStr);
				
				const auto start_time = chrono::high_resolution_clock::now();

				GenerateAllPossibleCombinationsOfMoves(ChessTestObject.TestId);

				const auto stop_time = chrono::high_resolution_clock::now();

				PrintDurationTimeAllFormats(start_time, stop_time, "Execution of looking for mates has taken time: ", "executing printing duration_time");

				SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads(MaxThreads);
				
				ChessEngineKernelManagerCUDAObject.PrepareDataForKernelAndExecuteKernelCUDA();

				ChessEngineKernelManagerCUDAObject.FreeMemoryForThreadsDataArray();

				ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.PrintResultsAndStatistics();

				ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.CompareTestStatistics(ChessTestObject);

				ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.SaveTestStatisticsToFile(ChessTestObject, ChessTestObject.TestId);
				
				LoggersManagerObject.Log(STREAM("END" << EndLineChar << EndLineChar));
			}

		ChessEngineConfigurationFileReaderWriterObject.WaitForKeyOnTheEnd == true ? getchar() : 0;
	}
	CATCH_AND_WORK("executing chess main threaded engine", "", getchar());
}