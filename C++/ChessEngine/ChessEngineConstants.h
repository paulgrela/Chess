#pragma once

#ifndef _CHESS_ENGINE_CONSTANTS_H_
#define _CHESS_ENGINE_CONSTANTS_H_

#include <string>

#include "ChessEngineTypes.h"

constexpr ThreadNumType MaxNumberOfThreads = 128;
constexpr ThreadNumType MaxNumberOfMPIProcesses = 2048 * 24;
constexpr LengthType MaximalLengthOfPathToFindChessBoardsForFullSpeed = 4;

constexpr ColorType NumberOfColors = 2;
constexpr PieceNumType NumberOfPieces = 32;
constexpr PieceNumType MaxNumberOfPromotedPawns = 16;

constexpr PieceNumType StartOfWhitePiecesNum = 0;
constexpr PieceNumType EndOfWhitePiecesNum = 15;

constexpr PieceNumType StartOfBlackPiecesNum = 16;
constexpr PieceNumType EndOfBlackPiecesNum = 31;

const std::string ColorName[NumberOfColors] = { "White", "Black" };

constexpr ColorType White = 0;
constexpr ColorType Black = 1;

constexpr PieceNumType QueenNum[NumberOfColors] = { 0, 16 };
constexpr PieceNumType Bishop1Num[NumberOfColors] = { 1, 17 };
constexpr PieceNumType Bishop2Num[NumberOfColors] = { 2, 18 };
constexpr PieceNumType Rook1Num[NumberOfColors] = { 3, 19 };
constexpr PieceNumType Rook2Num[NumberOfColors] = { 4, 20 };
constexpr PieceNumType Knight1Num[NumberOfColors] = { 5, 21 };
constexpr PieceNumType Knight2Num[NumberOfColors] = { 6, 22 };
constexpr PieceNumType Pawn1Num[NumberOfColors] = { 7, 23 };
constexpr PieceNumType Pawn8Num[NumberOfColors] = { 14, 30 };
constexpr PieceNumType KingNum[NumberOfColors] = { 15, 31 };

constexpr PieceNumType SpaceNum = 100;

const std::string PromotedPawnString[MaxNumberOfPromotedPawns] =
{
	"X1", "X2", "X3", "X4", "X5", "X6", "X7", "X8", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8"
};

constexpr char EndLineChar = '\n';
constexpr char SpaceChar = ' ';
constexpr char ChessBoardFieldSeparatorChar = '|';

const std::string EmptyFieldString = "  ";

constexpr PosType MaxChessSizeX = 9;
constexpr PosType MaxChessSizeY = 9;

#endif