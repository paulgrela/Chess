#pragma once

#ifndef _CHESS_ENGINE_MAIN_THREADS_MANAGER_H_
#define _CHESS_ENGINE_MAIN_THREADS_MANAGER_H_

#include <future>
#include <functional>

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

#include "ChessEngineAllPossibleMovesGenerator.h"
#include "ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA.h"

class ChessEngineMainThreadsManager
{
	friend class ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA;
private:
	ChessEngineAllPossibleMovesGenerator ChessEngineAllPossibleMovesGeneratorObjects[MaxNumberOfThreads];
	std::function<bool(ChessEngineAllPossibleMovesGenerator*, const PosType, const PosType)> GenerateAllPossibleMovesForPiecesFunctors[MaxNumberOfThreads][NumberOfPieces];
private:
	ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA ChessEngineKernelManagerCUDAObject;
public:
	ChessEngineMainThreadsManager::ChessEngineMainThreadsManager();
private:
	inline void GenerateAllPossibleCombinationsOfMoves(const uint64_t TestId);
private:
	inline void RegisterGenerateAllPossibleMovesForPieceFunctions(const ThreadNumType MaxThreads);
	inline void PrepareData(const ThreadNumType MaxThreads, const std::string& ChessBoardFileName, const std::uint64_t TestId,const std::string& ActualDateTimeStr);
	inline void InitializeLoggerManagerParameters();
	inline void SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads(const ThreadNumType MaxThreads);
};

#endif
