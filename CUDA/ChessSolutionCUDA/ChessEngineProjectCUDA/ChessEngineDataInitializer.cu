
#include <sstream>

#include "ExceptionsMacro.h"

#include "Logger.h"
#include "ChessEngineConfigurationFileReaderWriter.h"
#include "ChessEngineAllPossibleMovesGenerator.h"

#include "ChessEngineDataInitializer.h"

using namespace std;

void ChessEngineDataInitializer::ClearDataForAllPossibleMovesGeneratorAlgorithm()
{
	try
	{
		ChessEngineAllPossibleMovesGeneratorLocalReference.DeepLevel = 0;
		ChessEngineAllPossibleMovesGeneratorLocalReference.IsAnyPawnPromoted = false;

		for (ChessMove& InvestigatedMove : ChessEngineAllPossibleMovesGeneratorLocalReference.ActuallyInvestigatedMovesPath)	
			InvestigatedMove = ChessMove{ 0, 0, 0, 0, 0, 0, 0 };
	}
	CATCH("clearing chess analzyer algorithm data");
}

void ChessEngineDataInitializer::ClearPiecesData()
{
	try
	{
		for (ChessPiece& Piece : ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces)
		{
			Piece.ActualPosX = 0;
			Piece.ActualPosY = 0;
			Piece.Alive = false;
			Piece.Promoted = false;
		}
	}
	CATCH("clearing pieces data");
}

void ChessEngineDataInitializer::SetPieceStartProperData(const PosType CX, const PosType CY, const PieceNumType PieceNum)
{
	try
	{
		ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].ActualPosX = CX;
		ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].ActualPosY = CY;
		ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].Alive = true;

		ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] = PieceNum;

		if ((ChessEngineAllPossibleMovesGeneratorLocalReference.IsPawn<White>(PieceNum) == true && CY == 1) || (ChessEngineAllPossibleMovesGeneratorLocalReference.IsPawn<Black>(PieceNum) == true && CY == MaxChessSizeY - 1))
		{
			ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].Promoted = true;
			ChessEngineAllPossibleMovesGeneratorLocalReference.IsAnyPawnPromoted = true;
		}
	}
	CATCH("set piece start proper data")
}

void ChessEngineDataInitializer::SetStartPromotedPawn(const PosType CX, const PosType CY, const PieceNumType PromotedPawnPieceNum, const PieceNumType PromotedPawnNum)
{
	try
	{
		if (ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PromotedPawnPieceNum].Alive == false)
		{
			SetPieceStartProperData(CX, CY, PromotedPawnPieceNum);
			ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PromotedPawnPieceNum].Promoted = true;
			ChessEngineAllPossibleMovesGeneratorLocalReference.IsAnyPawnPromoted = true;
		}
		else
			throw runtime_error(string(string("Pawn ") + PromotedPawnString[PromotedPawnNum] + " could not be promoted because already exists on chessboard as normal pawn").c_str());
	}
	CATCH_AND_THROW("setting start promoted pawn")
}

bool ChessEngineDataInitializer::FindChessMainPiece(const PosType CX, const PosType CY)
{
	try
	{
		if (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoardStartData[CX][CY] != EmptyFieldString)
			for (PieceNumType PieceNum = 0; PieceNum < NumberOfPieces; PieceNum++)
				if (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoardStartData[CX][CY] == ChessEngineConfigurationFileReaderWriterObject.PieceString[PieceNum])
				{
					PieceNumType PieceNumForChessBoard;
					if (ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == true)
						PieceNumForChessBoard = (PieceNum <= EndOfWhitePiecesNum ? PieceNum + EndOfWhitePiecesNum + 1 : PieceNum - EndOfWhitePiecesNum - 1);
					else
						PieceNumForChessBoard = PieceNum;

					if (ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNumForChessBoard].Alive == false)
					{
						if ((ChessEngineAllPossibleMovesGeneratorLocalReference.IsPawn<White>(PieceNumForChessBoard) == true && CY == MaxChessSizeY - 1) || (ChessEngineAllPossibleMovesGeneratorLocalReference.IsPawn<Black>(PieceNumForChessBoard) == true && CY == 1))
							throw runtime_error(string(string("Pawn ") + ChessEngineConfigurationFileReaderWriterObject.PieceString[PieceNumForChessBoard] + " in impossible start position").c_str());

						SetPieceStartProperData(CX, CY, PieceNumForChessBoard);

						return true;
					}
					else
						throw runtime_error(string(string("Piece ") + ChessEngineConfigurationFileReaderWriterObject.PieceString[PieceNumForChessBoard] + " already exists on chessboard").c_str());
				}
	}
	CATCH_AND_THROW("finding chess main piece")

	return false;
}

bool ChessEngineDataInitializer::FindChessPromotedPiece(const PosType CX, const PosType CY, const bool FoundChessMainPiece)
{
	try
	{
		if (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoardStartData[CX][CY] != EmptyFieldString && FoundChessMainPiece == false)
			for (PieceNumType PromotedPawnNum = 0; PromotedPawnNum < MaxNumberOfPromotedPawns; PromotedPawnNum++)
				if (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoardStartData[CX][CY] == PromotedPawnString[PromotedPawnNum])
				{
					if (ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == false)
					{
						if (PromotedPawnNum < 8)
							SetStartPromotedPawn(CX, CY, Pawn1Num[White] + PromotedPawnNum, PromotedPawnNum);
						else
						if (PromotedPawnNum >= 8)
							SetStartPromotedPawn(CX, CY, Pawn1Num[Black] + PromotedPawnNum - 8, PromotedPawnNum);
					}
					else
					{
						if (PromotedPawnNum < 8)
							SetStartPromotedPawn(CX, CY, Pawn1Num[Black] + PromotedPawnNum, PromotedPawnNum);
						else
						if (PromotedPawnNum >= 8)
							SetStartPromotedPawn(CX, CY, Pawn1Num[White] + PromotedPawnNum - 8, PromotedPawnNum);
					}
					return true;
				}
	}
	CATCH_AND_THROW("finding chess promoted piece")

	return false;
}

void ChessEngineDataInitializer::FindIfThereAreKingsOnChessBoard() const
{
	try
	{
		if (ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[KingNum[White]].Alive == false)
			throw runtime_error("Lacks King White on chessboard");

		if (ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[KingNum[Black]].Alive == false)
			throw runtime_error("Lacks Black White on chessboard");
	}
	CATCH_AND_THROW("finding if there are any kings on chessboard")
}

void ChessEngineDataInitializer::FindStartPositionsOfPieces() 
{
	try
	{
		ClearPiecesData();

		for (PosType CY = 1; CY < MaxChessSizeY; CY++)
			for (PosType CX = 1; CX < MaxChessSizeX; CX++)
			{
				ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] = SpaceNum;

				const bool FoundChessMainPiece = FindChessMainPiece(CX, CY);
				const bool FoundPromotedPawn = FindChessPromotedPiece(CX, CY, FoundChessMainPiece);

				if (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoardStartData[CX][CY] != EmptyFieldString && FoundChessMainPiece == false && FoundPromotedPawn == false)
					throw runtime_error(string(string("Bad string ") + ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoardStartData[CX][CY] + " on chessboard in field(x,y) = (" + to_string(CX) + "," + to_string(CY) + ")").c_str());
			}

		FindIfThereAreKingsOnChessBoard();
	}
	CATCH_AND_THROW("finding start positions of pieces")
}

void ChessEngineDataInitializer::PrintActualStateOfPieces() 
{
	try
	{
		if (ChessEngineConfigurationFileReaderWriterObject.PrintActualStartPositionOfPieces == true)
		{
			for (PieceNumType PieceNum = 0; PieceNum < NumberOfPieces; PieceNum++)
				if (ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].Alive == true)
					LoggersManagerObject.Log(STREAM(ChessEngineConfigurationFileReaderWriterObject.PieceString[PieceNum] << "(x,y) = (" << int(ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].ActualPosX) << "," << int(ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].ActualPosY) << ") Alive = " << ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].Alive << " Promoted Pawn = " << ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[PieceNum].Promoted));
		}
	}
	CATCH("printing start positions of pieces")
}
