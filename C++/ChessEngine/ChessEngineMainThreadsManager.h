#pragma once

#ifndef _CHESS_ENGINE_MAIN_THREADS_MANAGER_H_
#define _CHESS_ENGINE_MAIN_THREADS_MANAGER_H_

#include <future>
#include <functional>

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

#include "ChessEngineAllPossibleMovesGenerator.h"

class ChessEngineMainThreadsManager
{
private:
    #ifdef USE_MPI
    int NumberOfMPIProcesses;
    int MPIProcessIdentifier;
    #endif
private:
    #ifndef USE_MPI
	ChessEngineAllPossibleMovesGenerator ChessEngineAllPossibleMovesGeneratorObjects[MaxNumberOfThreads];
    #endif
    #ifdef USE_MPI
    ChessEngineAllPossibleMovesGenerator ChessEngineAllPossibleMovesGeneratorObjects[1];
    #endif
	std::function<bool(ChessEngineAllPossibleMovesGenerator*, const PosType, const PosType)> GenerateAllPossibleMovesForPiecesFunctors[MaxNumberOfThreads][NumberOfPieces];
public:
	ChessEngineMainThreadsManager();
private:
	inline void GenerateAllPossibleCombinationsOfMovesToFindChessBoardsForFullSpeedThreads();
    #ifndef USE_MPI
    inline void GenerateAllPossibleCombinationsOfMovesInFullSpeedThreads();
    inline bool CallListOfFunctionsFromVectorOfFunctors(const PosType StartChessBoardNum, const PosType EndChessBoardNum, const ThreadNumType ThreadPos) noexcept;
    #endif
    #ifdef USE_MPI
    inline void GenerateAllPossibleCombinationsOfMovesInMPIProcesses();
    inline void GatherFoundChessScoredMovesPathsResultsFromAllMPIProcesses();
    inline void ReduceStatisticsResultsFromAllMPIProcesses();
    inline void CopyChessScoredMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(const int AllFoundMovesPathsToMateLengthsMaximum, const int* const AllFoundMovesPathsToMateLengths, ChessScoredMovesPath* const AllFoundMovesPathsToMatePointer, std::vector<ChessScoredMovesPath>& AllFoundMovesPaths, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreType x, const PathScoreType y)) const;
    inline void CopyChessScoredFoundCaptureMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(ChessScoredMovesPath* TheHighestScoredFoundCaptureMovesPathPointer, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreType x, const PathScoreType y)) const;
    inline void LogLengthsOfFoundMovesPaths(int AllFoundMovesPathsToMateBlackLengthsMaximum, int AllFoundMovesPathsToMateWhiteLengthsMaximum, int AllFoundMovesPathsToStalemateLengthsMaximum, int* AllFoundMovesPathsToMateBlackLengths, int* AllFoundMovesPathsToMateWhiteLengths, int NumberOfBytesToGatherFromEveryProcessToMateBlack, int NumberOfBytesToGatherFromEveryProcessToMateWhite) const;
    #endif
private:
	inline void RegisterGenerateAllPossibleMovesForPieceFunctions();
	inline void PrepareData(const std::string& ChessBoardFileName, const std::uint64_t TestId, const std::string& ActualDateTimeStr);
	inline void SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads();
private:
	static inline void InitializeLoggerManagerParameters();
	static inline void PrintDurationTimeAllFormats(const std::chrono::high_resolution_clock::time_point start_time, const std::chrono::high_resolution_clock::time_point stop_time, const char* TextToPrint, const char* ExceptionTextToPrint);
};

#endif
