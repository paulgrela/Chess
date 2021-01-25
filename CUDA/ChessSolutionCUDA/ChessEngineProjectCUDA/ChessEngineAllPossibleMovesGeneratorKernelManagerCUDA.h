#pragma once

#ifndef _CHESS_ENGINE_ALL_POSSIBLE_MOVES_GENERATOR_KERNEL_MANAGER_CUDA_H_
#define _CHESS_ENGINE_ALL_POSSIBLE_MOVES_GENERATOR_KERNEL_MANAGER_CUDA_H_

#include "DestinationPlatform.h"

#include "ChessEngineTypes.h"

class ChessEngineMainThreadsManager;
class ChessEngineAllPossibleMovesGenerator;

class ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA
{
private:
	ChessEngineMainThreadsManager& ChessEngineMainThreadsManagerLocalReference;
public:
	ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA(ChessEngineMainThreadsManager& ChessEngineMainThreadsManagerReferenceLocalParameter) : ChessEngineMainThreadsManagerLocalReference(ChessEngineMainThreadsManagerReferenceLocalParameter)
	{
	}
private:
	ChessEngineThreadDataCUDA* ThreadStartDataCUDAArrayDevicePointer;
private:
	ChessScoredMovesPathsCUDA* AllFoundMovesPathsToMateWhitePointer;
	ChessScoredMovesPathsCUDA* AllFoundMovesPathsToMateBlackPointer;
	ChessScoredMovesPathsCUDA* AllFoundMovesPathsToStalematePointer;
	ChessScoredMovesPathCUDA* TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer;
	ChessScoredMovesPathCUDA* TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer;
public:
	void PrepareDataForKernelAndExecuteKernelCUDA();
public:
	void GetMemoryForThreadsDataArray();
	void FreeMemoryForThreadsDataArray();
private:
	void GetMemoryForChessEngineKernelDataCUDA();
	void FreeMemoryForChessEngineKernelDataCUDA();
private:
	void CopyConstantValuesToCUDA();
	void CopyThreadStartDataCUDAArrayToDevice();
private:
	void CopyChessScoredFoundCaptureMovesPathsCUDAToStatisticsFoundMovesPaths(ChessScoredMovesPathCUDA* TheHighestScoredFoundCaptureMovesPathPointer, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreTypeCUDA x, const PathScoreTypeCUDA y));
	void CopyChessScoredMovesPathsCUDAToStatisticsFoundMovesPaths(ChessScoredMovesPathsCUDA* const AllFoundMovesPathsToMatePointer, std::vector<ChessScoredMovesPath>& AllFoundMovesPaths, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreTypeCUDA x, const PathScoreTypeCUDA y));
	void CopyChessScoredMovesPathsCUDAToStatisticsMovesPaths(ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorObject);
private:
	void PrintSizesOfStartDataforCUDA();
    #ifdef WINDOWS_PLATFORM
	void PrintDurationTimesOfCUDAKernelAlgorithmExecution(const float msecTotal1, const float msecTotal3, const float msecTotal2, const std::chrono::time_point<std::chrono::steady_clock>& start_time, const std::chrono::time_point<std::chrono::steady_clock>& stop_time);
    #endif
    #ifdef UNIX_PLATFORM
	void PrintDurationTimesOfCUDAKernelAlgorithmExecution(const float msecTotal1, const float msecTotal3, const float msecTotal2, const std::chrono::system_clock::time_point& start_time, const std::chrono::system_clock::time_point& stop_time);
    #endif
};

#endif
