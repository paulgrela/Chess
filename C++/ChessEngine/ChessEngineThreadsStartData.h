#pragma once

#ifndef _CHESS_ENGINE_THREADS_START_DATA_H_
#define _CHESS_ENGINE_THREADS_START_DATA_H_

#include <mutex>

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

struct OneChessBoardForChessEngineThreadsStartData
{
    LengthType DeepLevel = 0;
    bool IsAnyPawnPromoted = false;

    ChessMove ActuallyInvestigatedMovesPath[MaximalLengthOfPathToFindChessBoardsForFullSpeed];
    PieceNumType ChessBoard[MaxChessSizeX][MaxChessSizeY];
    ChessPiece Pieces[NumberOfPieces];
};

class ChessEngineThreadsStartData
{
public:
    static inline int PassCounter;
    static inline int StartChessBoardNum;
    static inline int EndChessBoardNum;
    static inline LengthType ChessBoardsForFullSpeedThreadsArraySize;
    static inline std::vector<OneChessBoardForChessEngineThreadsStartData> ChessBoardsForFullSpeedThreadsArrayPointer;
    static inline bool FullSpeedExecutionInThreads;
};

#endif