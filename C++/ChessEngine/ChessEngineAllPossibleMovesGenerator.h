#pragma once

#ifndef _CHESS_ENGINE_ALL_POSSIBLE_MOVES_GENERATOR_H_
#define _CHESS_ENGINE_ALL_POSSIBLE_MOVES_GENERATOR_H_

#include <tuple>
#include <string>

#include "Logger.h"

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

#include "ChessEngineThreadsStartData.h"

#include "ChessEngineChessboardPrinter.h"
#include "ChessEngineDataInitializer.h"
#include "ChessEngineResultsStatistics.h"

class ChessEngineAllPossibleMovesGenerator
{
	friend class ChessEngineMainThreadsManager;

    friend class ChessEngineChessboardPrinter;
    friend class ChessEngineDataInitializer;
	friend class ChessEngineBestMoveFinder;
    friend class ChessEngineResultsStatistics;
private:
	LengthType DeepLevel = 0;
	bool IsAnyPawnPromoted = false;
private:
	ChessMove ActuallyInvestigatedMovesPath[MaximalLengthOfFoundPath];
	std::string ChessBoardStartData[MaxChessSizeX][MaxChessSizeY];
private:
	PieceNumType ChessBoard[MaxChessSizeX][MaxChessSizeY];
	ChessPiece Pieces[NumberOfPieces];
private:
	ChessEngineChessboardPrinter ChessEngineChessboardPrinterObject;
	ChessEngineResultsStatistics ChessEngineResultsStatisticsObject;
	ChessEngineDataInitializer ChessEngineDataInitializerObject;
public:
	ChessEngineAllPossibleMovesGenerator() : ChessEngineDataInitializerObject(*this), ChessEngineResultsStatisticsObject(*this), ChessEngineChessboardPrinterObject(*this)
	{
	}
private:
	template <ColorType Color>
    [[nodiscard]] bool IsPawn(const PieceNumType PieceNum) const noexcept;
private:
	template <ColorType Color>
	inline void CheckPawnPromotion(const PosType NewPosX, const PosType NewPosY, ChessPiece& PiecesOldActPieceNum) noexcept;
private:
	inline std::tuple<PathScoreType, PathScoreType> SumActuallyInvestigatedMovesPathScore(const LengthType DeepLevelPathLength) noexcept;
private:
	template <ColorType Color, bool ForMate>
	inline void SaveTheBestMovesPath(const LengthType DeepLevelPathLength, ChessScoredMovesPath& TheHighestScoredFoundMovesPathForAdvantageOfWhite, ChessScoredMovesPath& TheHighestScoredFoundMovesPathForAdvantageOfBlack) noexcept;
private:
	static inline void SaveActuallyInvestigatedMovesPathRec(const PosType ActualPosX, const PosType ActualPosY, const PosType NewPosX, const PosType NewPosY, const PieceNumType OldActPieceNum, ChessMove& ChessMoveSave) noexcept;
private:
    inline void SaveChessBoardStateForFullSpeedThreadsExecution() noexcept;
private:
	template <ColorType Color, ColorType ColorOpposite>
	bool MovePieceTo(const PosType ActualPosX, const PosType ActualPosY, const PosType NewPosX, const PosType NewPosY) noexcept;
private:
	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	bool GenerateAllPossibleMovesForPawn(const PosType PawnStartPosX, const PosType PawnStartPosY) noexcept;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	bool GenerateAllPossibleMovesForRook(const PosType RookStartPosX, const PosType RookStartPosY) noexcept;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	bool GenerateAllPossibleMovesForBishop(const PosType BishopStartPosX, const PosType BishopStartPosY) noexcept;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	bool GenerateAllPossibleMovesForKnight(const PosType KnightStartPosX, const PosType KnightStartPosY) noexcept;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	bool GenerateAllPossibleMovesForQueen(const PosType QueenStartPosX, const PosType QueenStartPosY) noexcept;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	bool GenerateAllPossibleMovesForKing(const PosType KingStartPosX, const PosType KingStartPosY) noexcept;

	inline void GenerateAllPossibleMovesForPiece(const PieceNumType PieceNum, bool& PossibleMove, bool (ChessEngineAllPossibleMovesGenerator::*FunctionName)(const PosType, const PosType));
private:
	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	void GenerateAllPossibleCombinationsOfMoves() noexcept;

	template <ColorType Color, ColorType ColorOpposite>
	bool VerifyCheckOfKing() noexcept;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
	void UpdateDataForIsCheckAndPossibleMove(const bool IsCheck, const bool PossibleMove) noexcept;
private:
	template <ColorType ColorOpposite>
    [[nodiscard]] inline bool IsKingNearTo(const PosType KingPosX, const PosType KingPosY) const noexcept;

	template <ColorType ColorOpposite>
    [[nodiscard]] inline bool IsCheckForKing(const PosType KingPosX, const PosType KingPosY) const noexcept;

	template<ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
    [[nodiscard]] inline bool IsEmptyFieldOrOppositePieceAndNoOppositeKing(const PosType PosX, const PosType PosY) const;

	template<PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
    [[nodiscard]] inline bool IsOppositePiece(const PosType PosX, const PosType PosY) const;

	template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
    [[nodiscard]] inline bool IsKingMoveImpossible(const PosType PosX, const PosType PosY) const;

	template <ColorType ColorOpposite>
    [[nodiscard]] inline bool IsOppositePieceNum(const PieceNumType PieceNum) const noexcept;
};

#endif