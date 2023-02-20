#pragma once

#ifndef _CHESS_ENGINE_TYPES_CUDA_H_
#define _CHESS_ENGINE_TYPES_CUDA_H_

using PosTypeCUDA = unsigned char;
using LengthTypeCUDA = std::uint16_t;
using PathScoreTypeCUDA = std::int16_t;
using PathTypeTypeCUDA = signed char;
using PieceNumTypeCUDA = unsigned char;
using ColorTypeCUDA = unsigned char;
using BoolTypeCUDA = unsigned char;

struct ChessPieceCUDA
{
	PosTypeCUDA ActualPosX : 4;
	PosTypeCUDA ActualPosY : 4;
	bool Alive : 1;
	bool Promoted : 1;
};

struct ChessMoveCUDA
{
	PathScoreTypeCUDA Score = 0;
	PathTypeTypeCUDA Type;

	PieceNumTypeCUDA PieceNum;
	PosTypeCUDA PrevPosX : 4;
	PosTypeCUDA PrevPosY : 4;
	PosTypeCUDA NextPosX : 4;
	PosTypeCUDA NextPosY : 4;
};

constexpr LengthTypeCUDA MaximalLengthOfFoundPathCUDA = 10;
constexpr LengthTypeCUDA MaximalNumberOfFoundPathsInOneThreadCUDA = 10;
constexpr ColorTypeCUDA NumberOfColorsCUDA = 2;
constexpr PieceNumTypeCUDA NumberOfPiecesCUDA = 32;
constexpr PosTypeCUDA MaxChessSizeXCUDA = 9;
constexpr PosTypeCUDA MaxChessSizeYCUDA = 9;

typedef PieceNumTypeCUDA ChessBoardTypeCUDA[MaxChessSizeXCUDA][MaxChessSizeYCUDA];
typedef ChessPieceCUDA ChessPiecesTypeCUDA[NumberOfPiecesCUDA];
typedef ChessMoveCUDA ChessMovesPathTypeCUDA[MaximalLengthOfFoundPathCUDA];

struct ChessScoredMovesPathCUDA
{
public:
	PathScoreTypeCUDA Score;
	PathTypeTypeCUDA TypeSum;
	LengthTypeCUDA MovesPathLength;
	ChessMovesPathTypeCUDA MovesPath;
public:
	__device__ ChessScoredMovesPathCUDA() = default;
	__device__ ChessScoredMovesPathCUDA(const PathScoreTypeCUDA ScoreParameter, const PathScoreTypeCUDA TypeSumParameter, LengthTypeCUDA MovesPathLengthParameter, ChessMovesPathTypeCUDA* MovesPathParameter)
	{
		Score = ScoreParameter;
		TypeSum = TypeSumParameter;
		MovesPathLength = MovesPathLengthParameter;
		memcpy(MovesPath, MovesPathParameter, sizeof(ChessMoveCUDA) * MovesPathLengthParameter);
	}
};

struct ChessScoredMovesPathsCUDA
{
	LengthTypeCUDA NumberOfFoundMovesPaths = 0;
	ChessScoredMovesPathCUDA MovesPaths[MaximalNumberOfFoundPathsInOneThreadCUDA];
};

struct ChessScoredMovesPathForBothColorsCUDA
{
	ChessScoredMovesPathCUDA* WhitePath;
	ChessScoredMovesPathCUDA* BlackPath;
};

struct ChessEngineThreadDataCUDA
{
	LengthTypeCUDA DeepLevel = 0;
	bool IsAnyPawnPromoted = false;
	
	ChessMovesPathTypeCUDA ActuallyInvestigatedMovesPath;
	ChessBoardTypeCUDA ChessBoard;
	ChessPiecesTypeCUDA Pieces;

	ChessScoredMovesPathsCUDA* AllFoundMovesPathsToMatePointer[NumberOfColorsCUDA];
	ChessScoredMovesPathsCUDA* AllFoundMovesPathsToStalematePointer;
	
	ChessScoredMovesPathForBothColorsCUDA TheHighestScoredFoundCaptureMovesPathsPointer;
};

#endif