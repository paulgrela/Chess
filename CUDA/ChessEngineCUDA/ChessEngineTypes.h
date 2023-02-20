#pragma once

#ifndef _CHESS_ENGINE_TYPES_H_
#define _CHESS_ENGINE_TYPES_H_

using ThreadNumType = std::uint64_t;
using ThreadIdType = std::uint64_t;

using PosType = std::int64_t;
using LengthType = std::uint64_t;
using PathScoreType = std::int64_t;
using PieceNumType = unsigned char;
using ColorType = unsigned char;

struct ChessPiece
{
	PosType ActualPosX;
	PosType ActualPosY;
	bool Alive;
	bool Promoted;
};

struct ChessMove
{
	PathScoreType Score;
	PathScoreType Type;

	PieceNumType PieceNum;
	PosType PrevPosX;
	PosType PrevPosY;
	PosType NextPosX;
	PosType NextPosY;
};

#include "ChessEngineTypesCUDA.h"

#include <vector>

struct ChessScoredMovesPath
{
	PathScoreType Score;
	PathScoreType TypeSum;
	std::vector<ChessMove> MovesPath;
public:
	ChessScoredMovesPath() = default;
	ChessScoredMovesPath(const PathScoreType ScoreParameter, const PathScoreType TypeSumParameter, std::vector<ChessMove> MovesPathParameter): Score(ScoreParameter), TypeSum(TypeSumParameter), MovesPath(MovesPathParameter)
	{
	}
	ChessScoredMovesPath(const PathScoreType ScoreParameter, const PathScoreType TypeSumParameter, const LengthType MovesPathLengthParameter, ChessMovesPathTypeCUDA& MovesPathParameter)
	{
		Score = ScoreParameter;
		TypeSum = TypeSumParameter;
		MovesPath.clear();
		for (LengthType ChessMoveIndex = 0; ChessMoveIndex < MovesPathLengthParameter; ChessMoveIndex++)
			MovesPath.emplace_back(ChessMove{ MovesPathParameter[ChessMoveIndex].Score, MovesPathParameter[ChessMoveIndex].Type, MovesPathParameter[ChessMoveIndex].PieceNum, MovesPathParameter[ChessMoveIndex].PrevPosX , MovesPathParameter[ChessMoveIndex].PrevPosY, MovesPathParameter[ChessMoveIndex].NextPosX, MovesPathParameter[ChessMoveIndex].NextPosY });
	}
};

#include "ExceptionsMacro.h"

inline void CopyChessScoredMovesPathCUDAObjectToChessScoredMovesPathObject(ChessScoredMovesPath& ChessScoredMovesPathObject, const ChessScoredMovesPathCUDA& ChessScoredMovesPathCUDAObject)
{
	try
	{
		ChessScoredMovesPathObject.Score = ChessScoredMovesPathCUDAObject.Score;
		ChessScoredMovesPathObject.TypeSum = ChessScoredMovesPathCUDAObject.TypeSum;
		ChessScoredMovesPathObject.MovesPath.resize(ChessScoredMovesPathCUDAObject.MovesPathLength);

		for (LengthType ChessMove = 0; ChessMove < ChessScoredMovesPathCUDAObject.MovesPathLength; ChessMove++)
		{
			ChessScoredMovesPathObject.MovesPath[ChessMove].Score = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].Score;
			ChessScoredMovesPathObject.MovesPath[ChessMove].Type = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].Type;
			ChessScoredMovesPathObject.MovesPath[ChessMove].PieceNum = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].PieceNum;
			ChessScoredMovesPathObject.MovesPath[ChessMove].PrevPosX = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].PrevPosX;
			ChessScoredMovesPathObject.MovesPath[ChessMove].PrevPosY = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].PrevPosY;
			ChessScoredMovesPathObject.MovesPath[ChessMove].NextPosX = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].NextPosX;
			ChessScoredMovesPathObject.MovesPath[ChessMove].NextPosY = ChessScoredMovesPathCUDAObject.MovesPath[ChessMove].NextPosY;
		}
	}
	CATCH("copying ChessScoredMovesPathCUDAObject to ChessScoredMovesPathObject")
}

#endif