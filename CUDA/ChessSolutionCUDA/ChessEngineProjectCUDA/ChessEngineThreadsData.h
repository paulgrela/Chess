#pragma once

#ifndef _CHESS_ENGINE_THREADS_DATA_H_
#define _CHESS_ENGINE_THREADS_DATA_H_

#include <mutex>
#include <vector>

#include "ChessEngineTypes.h"

class ChessEngineThreadsData
{
	friend class ChessEngineMainThreadsManager;
	friend class ChessEngineAllPossibleMovesGenerator;
	friend class ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA;
private:
	static std::mutex ChessEngineThreadsDataArrayMutex;
	static LengthType ChessEngineThreadsDataArraySize;
	static ChessEngineThreadDataCUDA* ChessEngineThreadsDataArrayPointer;
};

#endif