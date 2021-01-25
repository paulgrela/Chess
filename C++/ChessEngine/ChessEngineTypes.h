#pragma once

#ifndef _CHESS_ENGINE_TYPES_H_
#define _CHESS_ENGINE_TYPES_H_

using ThreadNumType = std::uint64_t;
using ThreadIdType = std::uint64_t;

using PosType = std::int64_t;
using LengthType = int;
using PathScoreType = int;
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
    short int PrevPosX;
    short int PrevPosY;
    short int NextPosX;
    short int NextPosY;
};

constexpr LengthType MaximalLengthOfFoundPath = 8;

struct ChessScoredMovesPath
{
    PathScoreType Score;
    PathScoreType TypeSum;
    ChessMove MovesPath[MaximalLengthOfFoundPath];
    LengthType MovesPathSize;
};

#endif