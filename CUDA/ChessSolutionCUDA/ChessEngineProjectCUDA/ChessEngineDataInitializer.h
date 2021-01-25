
#pragma once

#ifndef _CHESS_ENGINE_DATA_INITIALIZER_H_
#define _CHESS_ENGINE_DATA_INITIALIZER_H_

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

#include "ChessEngineChessboardFileReader.h"

class ChessEngineAllPossibleMovesGenerator;

class ChessEngineDataInitializer
{
private:
	ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorLocalReference;
public:
	explicit ChessEngineDataInitializer(ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorLocalParameter): ChessEngineAllPossibleMovesGeneratorLocalReference(ChessEngineAllPossibleMovesGeneratorLocalParameter)
	{
	}
public:
	void ClearDataForAllPossibleMovesGeneratorAlgorithm();
private:
	void ClearPiecesData();
	void SetPieceStartProperData(const PosType CX, const PosType CY, const PieceNumType PieceNum);
	void SetStartPromotedPawn(const PosType CX, const PosType CY, const PieceNumType PromotedPawnPieceNum, const PieceNumType PromotedPawnNum);
	bool FindChessMainPiece(const PosType CX, const PosType CY);
	bool FindChessPromotedPiece(const PosType CX, const PosType CY, const bool FoundChessMainPiece);
	void FindIfThereAreKingsOnChessBoard() const;
public:
	void FindStartPositionsOfPieces();
	void PrintActualStateOfPieces();
};

#endif