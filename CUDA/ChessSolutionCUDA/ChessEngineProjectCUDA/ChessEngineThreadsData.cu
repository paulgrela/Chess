
#include "ChessEngineThreadsData.h"

std::mutex ChessEngineThreadsData::ChessEngineThreadsDataArrayMutex;
LengthType ChessEngineThreadsData::ChessEngineThreadsDataArraySize;
ChessEngineThreadDataCUDA* ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer;
