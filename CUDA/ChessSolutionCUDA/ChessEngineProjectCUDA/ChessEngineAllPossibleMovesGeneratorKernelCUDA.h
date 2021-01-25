#pragma once

#ifndef _CHESS_ENGINE_ALL_POSSIBLE_MOVES_GENERATOR_KERNEL_CUDA_H_
#define _CHESS_ENGINE_ALL_POSSIBLE_MOVES_GENERATOR_KERNEL_CUDA_H_

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <vector>

#include "ChessEngineConstants.h"
#include "ChessEngineTypesCUDA.h"

class ChessEngineAllPossibleMovesGeneratorKernelCUDA
{
private:
	template <ColorTypeCUDA Color>
	static __device__ bool IsPawnCUDA(const PieceNumTypeCUDA PieceNum);

	template <ColorTypeCUDA Color>
	static __device__ inline void CheckPawnPromotion(const PosTypeCUDA NewPosX, const PosTypeCUDA NewPosY, ChessPieceCUDA& PiecesOldActPieceNum);

	static __device__ inline void SumActuallyInvestigatedMovesPathScoreCUDA(const LengthTypeCUDA DeepLevelPathLength, PathScoreTypeCUDA& ActualMovesPathScoreOut, PathTypeTypeCUDA& ActualMovesPathTypeSumOut);

	template <ColorTypeCUDA Color>
	static __device__ inline void SaveTheBestMovesPathCUDA(const LengthTypeCUDA DeepLevelPathLength);

	static __device__ inline void SaveActuallyInvestigatedMovesPathRecCUDA(const PosTypeCUDA ActualPosX, const PosTypeCUDA ActualPosY, const PosTypeCUDA NewPosX, const PosTypeCUDA NewPosY, const PieceNumTypeCUDA OldActPieceNum, ChessMoveCUDA& ChessMoveSave);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite>
	static __device__ bool MovePieceToCUDA(const PosTypeCUDA ActualPosX, const PosTypeCUDA ActualPosY, const PosTypeCUDA NewPosX, const PosTypeCUDA NewPosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ bool GenerateAllPossibleMovesForPawnCUDA(const PosTypeCUDA PawnStartPosX, const PosTypeCUDA PawnStartPosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ bool GenerateAllPossibleMovesForRookCUDA(const PosTypeCUDA RookStartPosX, const PosTypeCUDA RookStartPosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ bool GenerateAllPossibleMovesForBishopCUDA(const PosTypeCUDA BishopStartPosX, const PosTypeCUDA BishopStartPosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ bool GenerateAllPossibleMovesForKnightCUDA(const PosTypeCUDA KnightStartPosX, const PosTypeCUDA KnightStartPosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ bool GenerateAllPossibleMovesForQueenCUDA(const PosTypeCUDA QueenStartPosX, const PosTypeCUDA QueenStartPosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ bool GenerateAllPossibleMovesForKingCUDA(const PosTypeCUDA KingStartPosX, const PosTypeCUDA KingStartPosY);

	static __device__ inline void GenerateAllPossibleMovesForPieceCUDA(const PieceNumTypeCUDA PieceNum, bool& PossibleMove, bool (*FunctionName)(const PosTypeCUDA, const PosTypeCUDA));

public:
	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ void GenerateAllPossibleCombinationsOfMovesCUDA();

private:
	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite>
	static __device__ bool VerifyCheckOfKingCUDA();

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ void UpdateDataForIsCheckAndPossibleMoveCUDA(const bool IsCheck, const bool PossibleMove);

	template <ColorTypeCUDA ColorOpposite>
	static __device__ inline bool IsKingNearToCUDA(const PosTypeCUDA KingPosX, const PosTypeCUDA KingPosY);

	template <ColorTypeCUDA ColorOpposite>
	static __device__ inline bool IsCheckForKingCUDA(const PosTypeCUDA KingPosX, const PosTypeCUDA KingPosY);

	template<ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ inline bool IsEmptyFieldOrOppositePieceAndNoOppositeKingCUDA(const PosTypeCUDA PosX, const PosTypeCUDA PosY);

	template<PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ inline bool IsOppositePieceCUDA(const PosTypeCUDA PosX, const PosTypeCUDA PosY);

	template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
	static __device__ inline bool IsKingMoveImpossibleCUDA(const PosTypeCUDA PosX, const PosTypeCUDA PosY);
};

using namespace std;

__device__ __constant__ const PieceNumTypeCUDA QueenNumCUDA[NumberOfColors] = { 0, 16 };
__device__ __constant__ const PieceNumTypeCUDA Bishop1NumCUDA[NumberOfColors] = { 1, 17 };
__device__ __constant__ const PieceNumTypeCUDA Bishop2NumCUDA[NumberOfColors] = { 2, 18 };
__device__ __constant__ const PieceNumTypeCUDA Rook1NumCUDA[NumberOfColors] = { 3, 19 };
__device__ __constant__ const PieceNumTypeCUDA Rook2NumCUDA[NumberOfColors] = { 4, 20 };
__device__ __constant__ const PieceNumTypeCUDA Knight1NumCUDA[NumberOfColors] = { 5, 21 };
__device__ __constant__ const PieceNumTypeCUDA Knight2NumCUDA[NumberOfColors] = { 6, 22 };
__device__ __constant__ const PieceNumTypeCUDA Pawn1NumCUDA[NumberOfColors] = { 7, 23 };
__device__ __constant__ const PieceNumTypeCUDA Pawn8NumCUDA[NumberOfColors] = { 14, 30 };
__device__ __constant__ const PieceNumTypeCUDA KingNumCUDA[NumberOfColors] = { 15, 31 };

__device__ __constant__ PathScoreTypeCUDA PiecePowerCUDA[NumberOfPieces];
__device__ __constant__ LengthTypeCUDA MaxDeepLevelConstantCUDA[1];
__device__ __constant__ BoolTypeCUDA ReverseColorOfPiecesCUDA[1];

template <ColorTypeCUDA Color>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::IsPawnCUDA(const PieceNumTypeCUDA PieceNum)
{
	return (PieceNum >= Pawn1NumCUDA[Color] && PieceNum <= Pawn8NumCUDA[Color]);
}

template <ColorTypeCUDA Color>
__device__ inline void ChessEngineAllPossibleMovesGeneratorKernelCUDA::CheckPawnPromotion(const PosTypeCUDA NewPosX, const PosTypeCUDA NewPosY, ChessPieceCUDA& PiecesOldActPieceNum)
{
	extern __shared__ bool IsAnyPawnPromoted[16][16];

	if ((((ReverseColorOfPiecesCUDA[0] == false && ((Color == Black && NewPosY == 1) || (Color == White && NewPosY == MaxChessSizeY - 1)))) || ((ReverseColorOfPiecesCUDA[0] == true && ((Color == White && NewPosY == 1) || (Color == Black && NewPosY == MaxChessSizeY - 1))))) && PiecesOldActPieceNum.Promoted == false)
	{
		PiecesOldActPieceNum.Promoted = true;
		IsAnyPawnPromoted[threadIdx.x][threadIdx.y] = true;
	}
}

__device__ inline void ChessEngineAllPossibleMovesGeneratorKernelCUDA::SumActuallyInvestigatedMovesPathScoreCUDA(const LengthTypeCUDA DeepLevelPathLength, PathScoreTypeCUDA& ActualMovesPathScoreOut, PathTypeTypeCUDA& ActualMovesPathTypeSumOut)
{
	extern __shared__ ChessMovesPathTypeCUDA* ActuallyInvestigatedMovesPathPointer[16][16];

	PathScoreTypeCUDA ActualMovesPathScore = 0;
	PathTypeTypeCUDA ActualMovesPathTypeSum = 0;

	for (LengthTypeCUDA ChessMove = 0; ChessMove < DeepLevelPathLength; ChessMove++)
	{
		ActualMovesPathScore += (*ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y])[ChessMove].Score;
		ActualMovesPathTypeSum += (*ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y])[ChessMove].Type;
	}

	ActualMovesPathScoreOut = ActualMovesPathScore;
	ActualMovesPathTypeSumOut = ActualMovesPathTypeSum;
}

template <ColorTypeCUDA Color>
__device__ inline void ChessEngineAllPossibleMovesGeneratorKernelCUDA::SaveTheBestMovesPathCUDA(const LengthTypeCUDA DeepLevelPathLength)
{
	extern __shared__ ChessMovesPathTypeCUDA* ActuallyInvestigatedMovesPathPointer[16][16];
	extern __shared__ ChessScoredMovesPathForBothColorsCUDA* TheHighestScoredFoundCaptureMovesPathsPointer[16][16];

	PathScoreTypeCUDA ActualMovesPathScore = 0;
	PathTypeTypeCUDA ActualMovesPathTypeSum = 0;
	SumActuallyInvestigatedMovesPathScoreCUDA(DeepLevelPathLength, ActualMovesPathScore, ActualMovesPathTypeSum);

	if (Color == White)
	{
		ChessScoredMovesPathCUDA* WhitePath = TheHighestScoredFoundCaptureMovesPathsPointer[threadIdx.x][threadIdx.y]->WhitePath;
		if (ActualMovesPathScore > WhitePath->Score)
		{
			ChessScoredMovesPathCUDA* WhitePath = TheHighestScoredFoundCaptureMovesPathsPointer[threadIdx.x][threadIdx.y]->WhitePath;
			WhitePath->Score = ActualMovesPathScore;
			WhitePath->TypeSum = ActualMovesPathTypeSum;
			WhitePath->MovesPathLength = DeepLevelPathLength;
			memcpy(WhitePath->MovesPath, ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y], sizeof(ChessMoveCUDA) * DeepLevelPathLength);
		}
	}
	else
	{
		ChessScoredMovesPathCUDA* BlackPath = TheHighestScoredFoundCaptureMovesPathsPointer[threadIdx.x][threadIdx.y]->BlackPath;
		if (ActualMovesPathScore < BlackPath->Score)
		{
			ChessScoredMovesPathCUDA* BlackPath = TheHighestScoredFoundCaptureMovesPathsPointer[threadIdx.x][threadIdx.y]->BlackPath;
			BlackPath->Score = ActualMovesPathScore;
			BlackPath->TypeSum = ActualMovesPathTypeSum;
			BlackPath->MovesPathLength = DeepLevelPathLength;
			memcpy(BlackPath->MovesPath, ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y], sizeof(ChessMoveCUDA) * DeepLevelPathLength);
		}
	}
}

__device__ inline void ChessEngineAllPossibleMovesGeneratorKernelCUDA::SaveActuallyInvestigatedMovesPathRecCUDA(const PosTypeCUDA ActualPosX, const PosTypeCUDA ActualPosY, const PosTypeCUDA NewPosX, const PosTypeCUDA NewPosY, const PieceNumTypeCUDA OldActPieceNum, ChessMoveCUDA& ChessMoveSave)
{
	ChessMoveSave.PieceNum = OldActPieceNum;
	ChessMoveSave.PrevPosX = ActualPosX;
	ChessMoveSave.PrevPosY = ActualPosY;
	ChessMoveSave.NextPosX = NewPosX;
	ChessMoveSave.NextPosY = NewPosY;
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::MovePieceToCUDA(const PosTypeCUDA ActualPosX, const PosTypeCUDA ActualPosY, const PosTypeCUDA NewPosX, const PosTypeCUDA NewPosY)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];
	extern __shared__ ChessPiecesTypeCUDA Pieces[16][16];
	extern __shared__ LengthTypeCUDA DeepLevel[16][16];
	extern __shared__ ChessMovesPathTypeCUDA* ActuallyInvestigatedMovesPathPointer[16][16];

	const PieceNumTypeCUDA OldActPieceNum = ChessBoard[threadIdx.x][threadIdx.y][ActualPosX][ActualPosY];
	const PieceNumTypeCUDA OldNewPieceNum = ChessBoard[threadIdx.x][threadIdx.y][NewPosX][NewPosY];
	ChessBoard[threadIdx.x][threadIdx.y][NewPosX][NewPosY] = ChessBoard[threadIdx.x][threadIdx.y][ActualPosX][ActualPosY];
	ChessBoard[threadIdx.x][threadIdx.y][ActualPosX][ActualPosY] = SpaceNum;

	PosTypeCUDA KingPosXToTestCheck, KingPosYToTestCheck;
	if (OldActPieceNum == KingNumCUDA[Color])
	{
		KingPosXToTestCheck = NewPosX;
		KingPosYToTestCheck = NewPosY;
	}
	else
	{
		KingPosXToTestCheck = Pieces[threadIdx.x][threadIdx.y][KingNumCUDA[Color]].ActualPosX;
		KingPosYToTestCheck = Pieces[threadIdx.x][threadIdx.y][KingNumCUDA[Color]].ActualPosY;
	}

	if (IsCheckForKingCUDA<ColorOpposite>(KingPosXToTestCheck, KingPosYToTestCheck) == true)
	{
		ChessBoard[threadIdx.x][threadIdx.y][ActualPosX][ActualPosY] = OldActPieceNum;
		ChessBoard[threadIdx.x][threadIdx.y][NewPosX][NewPosY] = OldNewPieceNum;

		return false;
	}

	Pieces[threadIdx.x][threadIdx.y][OldActPieceNum].ActualPosX = NewPosX;
	Pieces[threadIdx.x][threadIdx.y][OldActPieceNum].ActualPosY = NewPosY;

	bool OldActPieceNumPromoted;
	if (IsPawnCUDA<Color>(OldActPieceNum) == true)
	{
		OldActPieceNumPromoted = Pieces[threadIdx.x][threadIdx.y][OldActPieceNum].Promoted;
		CheckPawnPromotion<Color>(NewPosX, NewPosY, Pieces[threadIdx.x][threadIdx.y][OldActPieceNum]);
	}

	LengthTypeCUDA DeepLevelLocal = DeepLevel[threadIdx.x][threadIdx.y];
	ChessMovesPathTypeCUDA* ActuallyInvestigatedMovesPathPointerLocal = ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y];

	SaveActuallyInvestigatedMovesPathRecCUDA(ActualPosX, ActualPosY, NewPosX, NewPosY, OldActPieceNum, (*ActuallyInvestigatedMovesPathPointerLocal)[DeepLevelLocal]);

	if (OldNewPieceNum != SpaceNum)
	{
		Pieces[threadIdx.x][threadIdx.y][OldNewPieceNum].Alive = false;

		(*ActuallyInvestigatedMovesPathPointerLocal)[DeepLevelLocal].Type = (Color == White ? 1 : -1);
		PieceNumType PieceNumForPower = (Pieces[threadIdx.x][threadIdx.y][OldNewPieceNum].Promoted == true ? 0 : OldNewPieceNum);
		(*ActuallyInvestigatedMovesPathPointerLocal)[DeepLevelLocal].Score = (*ActuallyInvestigatedMovesPathPointerLocal)[DeepLevelLocal].Type * PiecePowerCUDA[PieceNumForPower] * (MaxDeepLevelConstantCUDA[0] - DeepLevelLocal + 1);

		SaveTheBestMovesPathCUDA<Color>(DeepLevelLocal + 1);
	}

	if (DeepLevel[threadIdx.x][threadIdx.y] < MaxDeepLevelConstantCUDA[0])
	{
		DeepLevel[threadIdx.x][threadIdx.y]++;

		GenerateAllPossibleCombinationsOfMovesCUDA<ColorOpposite, Color, Color == White ? StartOfWhitePiecesNum : StartOfBlackPiecesNum, Color == White ? EndOfWhitePiecesNum : EndOfBlackPiecesNum>();

		DeepLevel[threadIdx.x][threadIdx.y]--;
	}

	(*ActuallyInvestigatedMovesPathPointerLocal)[DeepLevelLocal].Type = 0;
	(*ActuallyInvestigatedMovesPathPointerLocal)[DeepLevelLocal].Score = 0;

	if (IsPawnCUDA<Color>(OldActPieceNum) == true)
		Pieces[threadIdx.x][threadIdx.y][OldActPieceNum].Promoted = OldActPieceNumPromoted;

	ChessBoard[threadIdx.x][threadIdx.y][ActualPosX][ActualPosY] = OldActPieceNum;
	ChessBoard[threadIdx.x][threadIdx.y][NewPosX][NewPosY] = OldNewPieceNum;

	Pieces[threadIdx.x][threadIdx.y][OldActPieceNum].ActualPosX = ActualPosX;
	Pieces[threadIdx.x][threadIdx.y][OldActPieceNum].ActualPosY = ActualPosY;

	if (OldNewPieceNum != SpaceNum)
		Pieces[threadIdx.x][threadIdx.y][OldNewPieceNum].Alive = true;

	return true;
}

template<PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::IsOppositePieceCUDA(const PosTypeCUDA PosX, const PosTypeCUDA PosY)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];

	const PieceNumTypeCUDA ChessBoardPieceNum = ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY];
	if (OppositePieceNumRangeStart <= ChessBoardPieceNum && ChessBoardPieceNum <= OppositePieceNumRangeEnd)
		return true;
	else
		return false;
}

template<ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::IsEmptyFieldOrOppositePieceAndNoOppositeKingCUDA(const PosTypeCUDA PosX, const PosTypeCUDA PosY)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];

	const PieceNumTypeCUDA ChessBoardPieceNum = ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY];
	if (ChessBoardPieceNum == SpaceNum || (IsOppositePieceCUDA<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY) == true && ChessBoardPieceNum != KingNumCUDA[ColorOpposite]))
		return true;
	else
		return false;
}

#define GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnEndPosX, PawnEndPosY, ConditionMain) \
		if (ConditionMain) \
		{ \
			if (MovePieceToCUDA<Color, ColorOpposite>(PawnStartPosX, PawnStartPosY, PawnEndPosX, PawnEndPosY) == true) \
				PossibleMove = true; \
		}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForPawnCUDA(const PosTypeCUDA PawnStartPosX, const PosTypeCUDA PawnStartPosY)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];
	extern __shared__ ChessPiecesTypeCUDA Pieces[16][16];

	bool PossibleMove = false;

	const PieceNumTypeCUDA PieceNum = ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY];

	if (Pieces[threadIdx.x][threadIdx.y][PieceNum].Promoted == false)
	{
		if ((ReverseColorOfPiecesCUDA[0] == true && (Color == White && PawnStartPosY - 1 >= 1)) || (ReverseColorOfPiecesCUDA[0] == false && (Color == Black && PawnStartPosY - 1 >= 1)))
		{
			GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY - 1, (ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY - 1] == SpaceNum));
			GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX + 1, PawnStartPosY - 1, (PawnStartPosX + 1 < MaxChessSizeX && IsOppositePieceCUDA<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX + 1, PawnStartPosY - 1) == true && ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX + 1][PawnStartPosY - 1] != KingNumCUDA[ColorOpposite]));
			GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX - 1, PawnStartPosY - 1, (PawnStartPosX - 1 >= 1 && IsOppositePieceCUDA<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX - 1, PawnStartPosY - 1) == true && ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX - 1][PawnStartPosY - 1] != KingNumCUDA[ColorOpposite]));

			if (PawnStartPosY == 7)
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY - 2, (ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY - 2] == SpaceNum && ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY - 1] == SpaceNum));
		}
		else
		if ((ReverseColorOfPiecesCUDA[0] == false && (Color == White && PawnStartPosY + 1 < MaxChessSizeY)) || (ReverseColorOfPiecesCUDA[0] == true && (Color == Black && PawnStartPosY + 1 < MaxChessSizeY)))
		{
			GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY + 1, (ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY + 1] == SpaceNum));
			GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX + 1, PawnStartPosY + 1, (PawnStartPosX + 1 < MaxChessSizeX && IsOppositePieceCUDA<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX + 1, PawnStartPosY + 1) == true && ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX + 1][PawnStartPosY + 1] != KingNumCUDA[ColorOpposite]));
			GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX - 1, PawnStartPosY + 1, (PawnStartPosX - 1 >= 1 && IsOppositePieceCUDA<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX - 1, PawnStartPosY + 1) == true && ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX - 1][PawnStartPosY + 1] != KingNumCUDA[ColorOpposite]));

			if (PawnStartPosY == 2)
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY + 2, (ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY + 2] == SpaceNum && ChessBoard[threadIdx.x][threadIdx.y][PawnStartPosX][PawnStartPosY + 1] == SpaceNum));
		}
	}
	else
	{
		if (GenerateAllPossibleMovesForQueenCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX, PawnStartPosY) == true)
			PossibleMove = true;
	}

	return PossibleMove;
}

#define GenerateAllPossibleMovesInOneLineAndOneDirection(StartPosX1, StartPosY1, StartPosX2, StartPosY2, PosX, PosY, LoopCondition, LoopOperation) \
		for (PosX = StartPosX1, PosY = StartPosY1; LoopCondition && IsEmptyFieldOrOppositePieceAndNoOppositeKingCUDA<ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY); LoopOperation) \
		{ \
			const bool OppositePiece = IsOppositePieceCUDA<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY); \
			if (MovePieceToCUDA<Color, ColorOpposite>(StartPosX2, StartPosY2, PosX, PosY) == true) \
				PossibleMove = true; \
			if (OppositePiece) \
				break; \
		} 

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForRookCUDA(const PosTypeCUDA RookStartPosX, const PosTypeCUDA RookStartPosY)
{
	bool PossibleMove = false;

	PosTypeCUDA RookPosX, RookPosY;

	GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX, RookStartPosY + 1, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosY < MaxChessSizeY, RookPosY++);
	GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX, RookStartPosY - 1, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosY >= 1, RookPosY--);
	GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX + 1, RookStartPosY, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosX < MaxChessSizeX, RookPosX++);
	GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX - 1, RookStartPosY, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosX >= 1, RookPosX--);

	return PossibleMove;
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForBishopCUDA(const PosTypeCUDA BishopStartPosX, const PosTypeCUDA BishopStartPosY)
{
	bool PossibleMove = false;

	PosTypeCUDA BishopPosX, BishopPosY;

	GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX + 1, BishopStartPosY + 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX < MaxChessSizeX && BishopPosY < MaxChessSizeY, (BishopPosX++, BishopPosY++));
	GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX - 1, BishopStartPosY - 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX >= 1 && BishopPosY >= 1, (BishopPosX--, BishopPosY--));
	GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX - 1, BishopStartPosY + 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX >= 1 && BishopPosY < MaxChessSizeY, (BishopPosX--, BishopPosY++));
	GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX + 1, BishopStartPosY - 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX < MaxChessSizeX && BishopPosY >= 1, (BishopPosX++, BishopPosY--));

	return PossibleMove;
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForQueenCUDA(const PosTypeCUDA QueenStartPosX, const PosTypeCUDA QueenStartPosY)
{
	bool PossibleMove = false;

	if (GenerateAllPossibleMovesForRookCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(QueenStartPosX, QueenStartPosY) == true)
		PossibleMove = true;
	if (GenerateAllPossibleMovesForBishopCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(QueenStartPosX, QueenStartPosY) == true)
		PossibleMove = true;

	return PossibleMove;
}

#define GenerateOnePossibleMoveForKnight(StartPosX, StartPosY, EndPosX, EndPosY, KnightMoveCondition) \
		if (KnightMoveCondition && IsEmptyFieldOrOppositePieceAndNoOppositeKingCUDA<ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(EndPosX, EndPosY)) \
		{ \
			if (MovePieceToCUDA<Color, ColorOpposite>(StartPosX, StartPosY, EndPosX, EndPosY) == true) \
				PossibleMove = true; \
		}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForKnightCUDA(const PosTypeCUDA KnightStartPosX, const PosTypeCUDA KnightStartPosY)
{
	bool PossibleMove = false;

	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 1, KnightStartPosY + 2, (KnightStartPosX - 1 >= 1 && KnightStartPosY + 2 < MaxChessSizeY));
	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 1, KnightStartPosY + 2, (KnightStartPosX + 1 < MaxChessSizeX && KnightStartPosY + 2 < MaxChessSizeY));

	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 2, KnightStartPosY + 1, (KnightStartPosX + 2 < MaxChessSizeX && KnightStartPosY + 1 < MaxChessSizeY));
	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 2, KnightStartPosY - 1, (KnightStartPosX + 2 < MaxChessSizeX && KnightStartPosY - 1 >= 1));

	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 1, KnightStartPosY - 2, (KnightStartPosX - 1 >= 1 && KnightStartPosY - 2 >= 1));
	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 1, KnightStartPosY - 2, (KnightStartPosX + 1 < MaxChessSizeX && KnightStartPosY - 2 >= 1));

	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 2, KnightStartPosY + 1, (KnightStartPosX - 2 >= 1 && KnightStartPosY + 1 < MaxChessSizeY));
	GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 2, KnightStartPosY - 1, (KnightStartPosX - 2 >= 1 && KnightStartPosY - 1 >= 1));

	return PossibleMove;
}

#define FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY, PieceNumKingColorOpposite, KingPosCondition) \
		if (KingPosCondition && ChessBoard[threadIdx.x][threadIdx.y][KingPosX][KingPosY] == PieceNumKingColorOpposite) \
			return true; 

template <ColorTypeCUDA ColorOpposite>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::IsKingNearToCUDA(const PosTypeCUDA KingPosX, const PosTypeCUDA KingPosY)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];

	const PieceNumTypeCUDA PieceNumKingColorOpposite = KingNumCUDA[ColorOpposite];


	FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY, PieceNumKingColorOpposite, true);

	FindKingOppositeAndIfFoundReturnTrue(KingPosX - 1, KingPosY, PieceNumKingColorOpposite, KingPosX - 1 >= 1);
	FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY - 1, PieceNumKingColorOpposite, KingPosY - 1 >= 1);
	FindKingOppositeAndIfFoundReturnTrue(KingPosX - 1, KingPosY - 1, PieceNumKingColorOpposite, KingPosX - 1 >= 1 && KingPosY - 1 >= 1);
	FindKingOppositeAndIfFoundReturnTrue(KingPosX + 1, KingPosY, PieceNumKingColorOpposite, KingPosX + 1 < MaxChessSizeX);

	FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY + 1, PieceNumKingColorOpposite, KingPosY + 1 < MaxChessSizeY);
	FindKingOppositeAndIfFoundReturnTrue(KingPosX + 1, KingPosY + 1, PieceNumKingColorOpposite, KingPosX + 1 < MaxChessSizeX && KingPosY + 1 < MaxChessSizeY);
	FindKingOppositeAndIfFoundReturnTrue(KingPosX - 1, KingPosY + 1, PieceNumKingColorOpposite, KingPosX - 1 >= 1 && KingPosY + 1 < MaxChessSizeY);
	FindKingOppositeAndIfFoundReturnTrue(KingPosX + 1, KingPosY - 1, PieceNumKingColorOpposite, KingPosX + 1 < MaxChessSizeX && KingPosY - 1 >= 1);

	return false;
}

#define FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(PosX, PosY, KingStartPosX, KingStartPosY, PieceNum1ColorOpposite, PieceNum2ColorOpposite, PieceNumQueenColorOpposite, LoopCondition, LoopOperation) \
		for (PosX = KingStartPosX, PosY = KingStartPosY; LoopCondition && ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY] == SpaceNum; LoopOperation); \
		ChessBoardPieceNum = ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY]; \
		if (LoopCondition && (ChessBoardPieceNum == PieceNum1ColorOpposite || ChessBoardPieceNum == PieceNum2ColorOpposite || ChessBoardPieceNum == PieceNumQueenColorOpposite || (IsPawnCUDA<ColorOpposite>(ChessBoardPieceNum) == true && Pieces[threadIdx.x][threadIdx.y][ChessBoardPieceNum].Promoted == true))) \
			return true;

#define FindKnightForCheckAndIfFoundReturnTrue(PosX, PosY, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, Condition) \
		if (Condition && (ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY] == PieceNumKnight1ColorOpposite || ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY] == PieceNumKnight2ColorOpposite)) \
			return true;

#define FindPawnForCheckAndIfFoundReturnTrue(PosX, PosY, Condition) \
		if (Condition && (IsPawnCUDA<ColorOpposite>(ChessBoard[threadIdx.x][threadIdx.y][PosX][PosY]) == true)) \
			return true;

template <ColorTypeCUDA ColorOpposite>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::IsCheckForKingCUDA(const PosTypeCUDA KingPosX, const PosTypeCUDA KingPosY)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];
	extern __shared__ ChessPiecesTypeCUDA Pieces[16][16];

	extern __shared__ bool IsAnyPawnPromoted[16][16];

	PieceNumTypeCUDA ChessBoardPieceNum;

	const PieceNumTypeCUDA PieceNumQueenColorOpposite = QueenNumCUDA[ColorOpposite];

	const PieceNumTypeCUDA PieceNumBishop1ColorOpposite = Bishop1NumCUDA[ColorOpposite];
	const PieceNumTypeCUDA PieceNumBishop2ColorOpposite = Bishop2NumCUDA[ColorOpposite];

	if (Pieces[threadIdx.x][threadIdx.y][PieceNumBishop1ColorOpposite].Alive == true || Pieces[threadIdx.x][threadIdx.y][PieceNumBishop2ColorOpposite].Alive == true || Pieces[threadIdx.x][threadIdx.y][PieceNumQueenColorOpposite].Alive == true || IsAnyPawnPromoted[threadIdx.x][threadIdx.y] == true)
	{
		PosTypeCUDA BishopPosX, BishopPosY;
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX + 1, KingPosY + 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX < MaxChessSizeX && BishopPosY < MaxChessSizeY, (BishopPosX++, BishopPosY++));
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX - 1, KingPosY - 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX >= 1 && BishopPosY >= 1, (BishopPosX--, BishopPosY--));
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX - 1, KingPosY + 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX >= 1 && BishopPosY < MaxChessSizeY, (BishopPosX--, BishopPosY++));
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX + 1, KingPosY - 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX < MaxChessSizeX && BishopPosY >= 1, (BishopPosX++, BishopPosY--));
	}

	const PieceNumTypeCUDA PieceNumRook1ColorOpposite = Rook1NumCUDA[ColorOpposite];
	const PieceNumTypeCUDA PieceNumRook2ColorOpposite = Rook2NumCUDA[ColorOpposite];

	if (Pieces[threadIdx.x][threadIdx.y][PieceNumRook1ColorOpposite].Alive == true || Pieces[threadIdx.x][threadIdx.y][PieceNumRook2ColorOpposite].Alive == true || Pieces[threadIdx.x][threadIdx.y][PieceNumQueenColorOpposite].Alive == true || IsAnyPawnPromoted[threadIdx.x][threadIdx.y] == true)
	{
		PosTypeCUDA RookPosX, RookPosY;
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX, KingPosY + 1, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosY < MaxChessSizeY, RookPosY++);
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX, KingPosY - 1, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosY >= 1, RookPosY--);
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX + 1, KingPosY, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosX < MaxChessSizeX, RookPosX++);
		FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX - 1, KingPosY, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosX >= 1, RookPosX--);
	}

	const PieceNumTypeCUDA PieceNumKnight1ColorOpposite = Knight1NumCUDA[ColorOpposite];
	const PieceNumTypeCUDA PieceNumKnight2ColorOpposite = Knight2NumCUDA[ColorOpposite];

	if (Pieces[threadIdx.x][threadIdx.y][PieceNumKnight1ColorOpposite].Alive == true || Pieces[threadIdx.x][threadIdx.y][PieceNumKnight1ColorOpposite].Alive == true)
	{
		FindKnightForCheckAndIfFoundReturnTrue(KingPosX - 1, KingPosY + 2, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX - 1 >= 1 && KingPosY + 2 < MaxChessSizeY));
		FindKnightForCheckAndIfFoundReturnTrue(KingPosX + 1, KingPosY + 2, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX + 1 < MaxChessSizeX && KingPosY + 2 < MaxChessSizeY));

		FindKnightForCheckAndIfFoundReturnTrue(KingPosX + 2, KingPosY + 1, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX + 2 < MaxChessSizeX && KingPosY + 1 < MaxChessSizeY));
		FindKnightForCheckAndIfFoundReturnTrue(KingPosX + 2, KingPosY - 1, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX + 2 < MaxChessSizeX && KingPosY - 1 >= 1));

		FindKnightForCheckAndIfFoundReturnTrue(KingPosX - 1, KingPosY - 2, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX - 1 >= 1 && KingPosY - 2 >= 1));
		FindKnightForCheckAndIfFoundReturnTrue(KingPosX + 1, KingPosY - 2, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX + 1 < MaxChessSizeX && KingPosY - 2 >= 1));

		FindKnightForCheckAndIfFoundReturnTrue(KingPosX - 2, KingPosY + 1, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX - 2 >= 1 && KingPosY + 1 < MaxChessSizeY));
		FindKnightForCheckAndIfFoundReturnTrue(KingPosX - 2, KingPosY - 1, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, (KingPosX - 2 >= 1 && KingPosY - 1 >= 1));
	}

	if ((ReverseColorOfPiecesCUDA[0] == false && (ColorOpposite == Black && KingPosY + 1 < MaxChessSizeY)) || (ReverseColorOfPiecesCUDA[0] == true && (ColorOpposite == White && KingPosY + 1 < MaxChessSizeY)))
	{
		FindPawnForCheckAndIfFoundReturnTrue(KingPosX + 1, KingPosY + 1, KingPosX + 1 < MaxChessSizeX);
		FindPawnForCheckAndIfFoundReturnTrue(KingPosX - 1, KingPosY + 1, KingPosX - 1 >= 1);
	}
	else
	if ((ReverseColorOfPiecesCUDA[0] == false && (ColorOpposite == White && KingPosY - 1 >= 1)) || (ReverseColorOfPiecesCUDA[0] == true && (ColorOpposite == Black && KingPosY - 1 >= 1)))
	{
		FindPawnForCheckAndIfFoundReturnTrue(KingPosX + 1, KingPosY - 1, KingPosX + 1 < MaxChessSizeX);
		FindPawnForCheckAndIfFoundReturnTrue(KingPosX - 1, KingPosY - 1, KingPosX - 1 >= 1);
	}

	return false;
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::IsKingMoveImpossibleCUDA(const PosTypeCUDA PosX, const PosTypeCUDA PosY)
{
	if (IsEmptyFieldOrOppositePieceAndNoOppositeKingCUDA<ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY) == true && IsKingNearToCUDA<ColorOpposite>(PosX, PosY) == false)
		return false;
	else
		return true;
}

#define GenerateOnePossibleMoveForKing(KingStartPosX1, KingStartPosY1, KingStartPosX2, KingStartPosY2, Condition) \
		if (Condition && IsKingMoveImpossibleCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(KingStartPosX2, KingStartPosY2) == false) \
		{ \
			if(MovePieceToCUDA<Color, ColorOpposite>(KingStartPosX1, KingStartPosY1, KingStartPosX2, KingStartPosY2) == true) \
				PossibleMove = true; \
		} 

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForKingCUDA(const PosTypeCUDA KingStartPosX, const PosTypeCUDA KingStartPosY)
{
	bool PossibleMove = false;

	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX - 1, KingStartPosY, KingStartPosX - 1 >= 1);
	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX, KingStartPosY - 1, KingStartPosY - 1 >= 1);
	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX - 1, KingStartPosY - 1, KingStartPosX - 1 >= 1 && KingStartPosY - 1 >= 1);

	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX + 1, KingStartPosY, KingStartPosX + 1 < MaxChessSizeX);
	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX, KingStartPosY + 1, KingStartPosY + 1 < MaxChessSizeY);
	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX + 1, KingStartPosY + 1, KingStartPosX + 1 < MaxChessSizeX && KingStartPosY + 1 < MaxChessSizeY);

	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX - 1, KingStartPosY + 1, KingStartPosX - 1 >= 1 && KingStartPosY + 1 < MaxChessSizeY);
	GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX + 1, KingStartPosY - 1, KingStartPosX + 1 < MaxChessSizeX && KingStartPosY - 1 >= 1);

	return PossibleMove;
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite>
__device__ inline bool ChessEngineAllPossibleMovesGeneratorKernelCUDA::VerifyCheckOfKingCUDA()
{
	extern __shared__ ChessPiecesTypeCUDA Pieces[16][16];

	bool IsCheck;

	IsCheck = IsCheckForKingCUDA<ColorOpposite>(Pieces[threadIdx.x][threadIdx.y][KingNumCUDA[Color]].ActualPosX, Pieces[threadIdx.x][threadIdx.y][KingNumCUDA[Color]].ActualPosY);

	return IsCheck;
}

__device__ inline void ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForPieceCUDA(const PieceNumTypeCUDA PieceNum, bool& PossibleMove, bool (*FunctionName)(const PosTypeCUDA, const PosTypeCUDA))
{
	extern __shared__ ChessPiecesTypeCUDA Pieces[16][16];

	if (Pieces[threadIdx.x][threadIdx.y][PieceNum].Alive == true)
	{
		if ((*FunctionName)(Pieces[threadIdx.x][threadIdx.y][PieceNum].ActualPosX, Pieces[threadIdx.x][threadIdx.y][PieceNum].ActualPosY) == true)
			PossibleMove = true;
	}
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ void ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleCombinationsOfMovesCUDA()
{
	const bool IsCheck = VerifyCheckOfKingCUDA<Color, ColorOpposite>();

	bool PossibleMove = false;

	GenerateAllPossibleMovesForPieceCUDA(KingNumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForKingCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(QueenNumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForQueenCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(Rook1NumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForRookCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(Rook2NumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForRookCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(Bishop1NumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForBishopCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(Bishop2NumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForBishopCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(Knight1NumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForKnightCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	GenerateAllPossibleMovesForPieceCUDA(Knight2NumCUDA[Color], PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForKnightCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
	for (PieceNumTypeCUDA Pawn = Pawn1NumCUDA[Color]; Pawn <= Pawn8NumCUDA[Color]; Pawn++)
		GenerateAllPossibleMovesForPieceCUDA(Pawn, PossibleMove, &ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleMovesForPawnCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);

	UpdateDataForIsCheckAndPossibleMoveCUDA<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(IsCheck, PossibleMove);
}

template <ColorTypeCUDA Color, ColorTypeCUDA ColorOpposite, PieceNumTypeCUDA OppositePieceNumRangeStart, PieceNumTypeCUDA OppositePieceNumRangeEnd>
__device__ inline void ChessEngineAllPossibleMovesGeneratorKernelCUDA::UpdateDataForIsCheckAndPossibleMoveCUDA(const bool IsCheck, const bool PossibleMove)
{
	extern __shared__ LengthTypeCUDA DeepLevel[16][16];
	extern __shared__ ChessMovesPathTypeCUDA* ActuallyInvestigatedMovesPathPointer[16][16];

	extern __shared__ ChessScoredMovesPathsCUDA* AllFoundMovesPathsToMatePointer[16][16][2];
	extern __shared__ ChessScoredMovesPathsCUDA* AllFoundMovesPathsToStalematePointer[16][16];

	if (IsCheck == true && PossibleMove == false)
	{
		PathScoreTypeCUDA ActualMovesPathScore = 0;
		PathTypeTypeCUDA ActualMovesPathTypeSum = 0;
		SumActuallyInvestigatedMovesPathScoreCUDA(DeepLevel[threadIdx.x][threadIdx.y], ActualMovesPathScore, ActualMovesPathTypeSum);

		if (AllFoundMovesPathsToMatePointer[threadIdx.x][threadIdx.y][Color]->NumberOfFoundMovesPaths + 1 < MaximalNumberOfFoundPathsInOneThreadCUDA)
		{
			LengthTypeCUDA ActualAllFoundMovesPathsToMateLength = (AllFoundMovesPathsToMatePointer[threadIdx.x][threadIdx.y][Color]->NumberOfFoundMovesPaths++);
			AllFoundMovesPathsToMatePointer[threadIdx.x][threadIdx.y][Color]->MovesPaths[ActualAllFoundMovesPathsToMateLength] = ChessScoredMovesPathCUDA(ActualMovesPathScore, ActualMovesPathTypeSum, DeepLevel[threadIdx.x][threadIdx.y], ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y]);
		}
	}
	else
	if (IsCheck == false && PossibleMove == false)
	{
		PathScoreTypeCUDA ActualMovesPathScore = 0;
		PathTypeTypeCUDA ActualMovesPathTypeSum = 0;
		SumActuallyInvestigatedMovesPathScoreCUDA(DeepLevel[threadIdx.x][threadIdx.y], ActualMovesPathScore, ActualMovesPathTypeSum);
		
		if (AllFoundMovesPathsToStalematePointer[threadIdx.x][threadIdx.y]->NumberOfFoundMovesPaths + 1 < MaximalNumberOfFoundPathsInOneThreadCUDA)
		{
			LengthTypeCUDA ActualAllFoundMovesPathsToStalemateLength = (AllFoundMovesPathsToStalematePointer[threadIdx.x][threadIdx.y]->NumberOfFoundMovesPaths++);
			AllFoundMovesPathsToStalematePointer[threadIdx.x][threadIdx.y]->MovesPaths[ActualAllFoundMovesPathsToStalemateLength] = ChessScoredMovesPathCUDA(ActualMovesPathScore, ActualMovesPathTypeSum, DeepLevel[threadIdx.x][threadIdx.y], ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y]);
		}
	}
}

__global__ void ChessEngineAllPossibleMovesGeneratorKernelCUDA(ChessEngineThreadDataCUDA* const __restrict__ ThreadStartDataArrayCUDA, unsigned int CUDAThreadStartDataArraySize)
{
	extern __shared__ ChessBoardTypeCUDA ChessBoard[16][16];
	extern __shared__ ChessPiecesTypeCUDA Pieces[16][16];

	extern __shared__ LengthTypeCUDA DeepLevel[16][16];
	extern __shared__ bool IsAnyPawnPromoted[16][16];

	extern __shared__ ChessMovesPathTypeCUDA* ActuallyInvestigatedMovesPathPointer[16][16];

	extern __shared__ ChessScoredMovesPathsCUDA* AllFoundMovesPathsToMatePointer[16][16][2];
	extern __shared__ ChessScoredMovesPathsCUDA* AllFoundMovesPathsToStalematePointer[16][16];

	extern __shared__ ChessScoredMovesPathForBothColorsCUDA* TheHighestScoredFoundCaptureMovesPathsPointer[16][16];

	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	int offset = x + y * blockDim.x * gridDim.x;

	if (offset < CUDAThreadStartDataArraySize)
	{
		memcpy(ChessBoard[threadIdx.x][threadIdx.y], ThreadStartDataArrayCUDA[offset].ChessBoard, sizeof(ChessBoardTypeCUDA));
		memcpy(Pieces[threadIdx.x][threadIdx.y], ThreadStartDataArrayCUDA[offset].Pieces, sizeof(ChessPiecesTypeCUDA));

		DeepLevel[threadIdx.x][threadIdx.y] = ThreadStartDataArrayCUDA[offset].DeepLevel;
		IsAnyPawnPromoted[threadIdx.x][threadIdx.y] = ThreadStartDataArrayCUDA[offset].IsAnyPawnPromoted;
		ActuallyInvestigatedMovesPathPointer[threadIdx.x][threadIdx.y] = &ThreadStartDataArrayCUDA[offset].ActuallyInvestigatedMovesPath;

		AllFoundMovesPathsToMatePointer[threadIdx.x][threadIdx.y][White] = ThreadStartDataArrayCUDA[offset].AllFoundMovesPathsToMatePointer[White];
		AllFoundMovesPathsToMatePointer[threadIdx.x][threadIdx.y][Black] = ThreadStartDataArrayCUDA[offset].AllFoundMovesPathsToMatePointer[Black];
		AllFoundMovesPathsToStalematePointer[threadIdx.x][threadIdx.y] = ThreadStartDataArrayCUDA[offset].AllFoundMovesPathsToStalematePointer;

		TheHighestScoredFoundCaptureMovesPathsPointer[threadIdx.x][threadIdx.y] = &ThreadStartDataArrayCUDA[offset].TheHighestScoredFoundCaptureMovesPathsPointer;

		ChessEngineAllPossibleMovesGeneratorKernelCUDA::GenerateAllPossibleCombinationsOfMovesCUDA<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();
	}
}

#endif