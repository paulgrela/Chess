
#include "DestinationPlatform.h"

#ifdef UNIX_PLATFORM
#include <string.h>
#endif

#include <vector>
#include <sstream>

#include "ExceptionsMacro.h"

#include "ChessEngineChessboardFileReader.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

#include "ChessEngineChessMoveBasicOperations.h"

#include "ChessEngineAllPossibleMovesGenerator.h"

#define __DEBUG__
#undef __DEBUG__

using namespace std;

#ifdef __DEBUG__
template <ColorType ColorOpposite>
inline bool ChessEngineAllPossibleMovesGenerator::IsOppositePieceNum(const PieceNumType PieceNum) const noexcept
{
	if constexpr(ColorOpposite == White)
		return (StartOfWhitePiecesNum <= PieceNum && PieceNum <= EndOfWhitePiecesNum);
	else
	if constexpr(ColorOpposite == Black)
		return (StartOfBlackPiecesNum <= PieceNum && PieceNum <= EndOfBlackPiecesNum);
}
#endif

template <ColorType Color>
inline bool ChessEngineAllPossibleMovesGenerator::IsPawn(const PieceNumType PieceNum) const noexcept
{
	return (PieceNum >= Pawn1Num[Color] && PieceNum <= Pawn8Num[Color]);
}

template <ColorType Color>
inline void ChessEngineAllPossibleMovesGenerator::CheckPawnPromotion(const PosType NewPosX, const PosType NewPosY, ChessPiece& PiecesOldActPieceNum) noexcept
{
	try
	{
		if ((((ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == false && ((Color == Black && NewPosY == 1) || (Color == White && NewPosY == MaxChessSizeY - 1)))) || ((ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == true && ((Color == White && NewPosY == 1) || (Color == Black && NewPosY == MaxChessSizeY - 1))))) && PiecesOldActPieceNum.Promoted == false)
		{
			PiecesOldActPieceNum.Promoted = true;
			IsAnyPawnPromoted = true;
		}
    }
	CATCH("checking pawn promotion and setting first move")
}

inline tuple<PathScoreType, PathScoreType> ChessEngineAllPossibleMovesGenerator::SumActuallyInvestigatedMovesPathScore(const LengthType DeepLevelPathLength) noexcept
{
	PathScoreType ActualMovesPathScore = 0;
	PathScoreType ActualMovesPathTypeSum = 0;

	try
	{
		for (LengthType ChessMove = 0; ChessMove < DeepLevelPathLength; ChessMove++)
		{
			ActualMovesPathScore += ActuallyInvestigatedMovesPath[ChessMove].Score;
			ActualMovesPathTypeSum += ActuallyInvestigatedMovesPath[ChessMove].Type;
		}
	}
	CATCH("summing actually investigated moves path score")

	return make_tuple(ActualMovesPathScore, ActualMovesPathTypeSum);
}

template <ColorType Color, bool ForMate>
inline void ChessEngineAllPossibleMovesGenerator::SaveTheBestMovesPath(const LengthType DeepLevelPathLength, ChessScoredMovesPath& TheHighestScoredFoundMovesPathForAdvantageOfWhite, ChessScoredMovesPath& TheHighestScoredFoundMovesPathForAdvantageOfBlack) noexcept
{
	try
	{
		auto&& [ActualMovesPathScore, ActualMovesPathTypeSum] = SumActuallyInvestigatedMovesPathScore(DeepLevelPathLength);

		if constexpr(ForMate == false)
		{
			if constexpr(Color == White)
			{
				if (ActualMovesPathScore > TheHighestScoredFoundMovesPathForAdvantageOfWhite.Score)
                {
					TheHighestScoredFoundMovesPathForAdvantageOfWhite = ChessScoredMovesPath{ ActualMovesPathScore, ActualMovesPathTypeSum, {}, DeepLevelPathLength };
                    CopyChessMovesPathsVector(DeepLevelPathLength, TheHighestScoredFoundMovesPathForAdvantageOfWhite.MovesPath, ActuallyInvestigatedMovesPath);
                }
			}
			else
			{
				if (ActualMovesPathScore < TheHighestScoredFoundMovesPathForAdvantageOfBlack.Score)
                {
					TheHighestScoredFoundMovesPathForAdvantageOfBlack = ChessScoredMovesPath{ ActualMovesPathScore, ActualMovesPathTypeSum, {}, DeepLevelPathLength };
                    CopyChessMovesPathsVector(DeepLevelPathLength, TheHighestScoredFoundMovesPathForAdvantageOfBlack.MovesPath, ActuallyInvestigatedMovesPath);
                }
			}
		}
		else
		{
			if constexpr(Color == White)
			{
				if (ActualMovesPathScore < TheHighestScoredFoundMovesPathForAdvantageOfWhite.Score || TheHighestScoredFoundMovesPathForAdvantageOfWhite.MovesPathSize == 0)
                {
					TheHighestScoredFoundMovesPathForAdvantageOfWhite = ChessScoredMovesPath{ ActualMovesPathScore, ActualMovesPathTypeSum, {}, DeepLevelPathLength };
                    CopyChessMovesPathsVector(DeepLevelPathLength, TheHighestScoredFoundMovesPathForAdvantageOfWhite.MovesPath, ActuallyInvestigatedMovesPath);
                }
			}
			else
			{
                if (ActualMovesPathScore > TheHighestScoredFoundMovesPathForAdvantageOfBlack.Score || TheHighestScoredFoundMovesPathForAdvantageOfBlack.MovesPathSize == 0)
                {
					TheHighestScoredFoundMovesPathForAdvantageOfBlack = ChessScoredMovesPath{ ActualMovesPathScore, ActualMovesPathTypeSum, {}, DeepLevelPathLength };
                    CopyChessMovesPathsVector(DeepLevelPathLength, TheHighestScoredFoundMovesPathForAdvantageOfBlack.MovesPath, ActuallyInvestigatedMovesPath);
                }
			}
		}
	}
	CATCH("saving the best moves path")
}

inline void ChessEngineAllPossibleMovesGenerator::SaveActuallyInvestigatedMovesPathRec(const PosType ActualPosX, const PosType ActualPosY, const PosType NewPosX, const PosType NewPosY, const PieceNumType OldActPieceNum, ChessMove& ChessMoveSave) noexcept
{
	try
	{
		ChessMoveSave.PieceNum = OldActPieceNum;
		ChessMoveSave.PrevPosX = ActualPosX;
		ChessMoveSave.PrevPosY = ActualPosY;
		ChessMoveSave.NextPosX = NewPosX;
		ChessMoveSave.NextPosY = NewPosY;
	}
	CATCH("saving actually investigated moves path")
}

inline void ChessEngineAllPossibleMovesGenerator::SaveChessBoardStateForFullSpeedThreadsExecution() noexcept
{
    try
    {
        OneChessBoardForChessEngineThreadsStartData OneChessBoardForChessEngineThreadsStartDataObject;

        OneChessBoardForChessEngineThreadsStartDataObject.DeepLevel = DeepLevel + 1;
        OneChessBoardForChessEngineThreadsStartDataObject.IsAnyPawnPromoted = IsAnyPawnPromoted;

        memcpy(OneChessBoardForChessEngineThreadsStartDataObject.ChessBoard, ChessBoard, sizeof(PieceNumType) * MaxChessSizeX * MaxChessSizeY);
        memcpy(OneChessBoardForChessEngineThreadsStartDataObject.Pieces, Pieces, sizeof(ChessPiece) * NumberOfPieces);
        memcpy(OneChessBoardForChessEngineThreadsStartDataObject.ActuallyInvestigatedMovesPath, ActuallyInvestigatedMovesPath, sizeof(ChessMove) * OneChessBoardForChessEngineThreadsStartDataObject.DeepLevel);

        lock_guard<mutex> LockGuardObject{ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayMutex};
        ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize] = OneChessBoardForChessEngineThreadsStartDataObject;
        if (ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize + 1 < ChessEngineConfigurationFileReaderWriterObject.MaxNumberOfChessBoardsForFastExecution)
            ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize++;
    }
    CATCH("saving chess boards for full speed threads")
}

template <ColorType Color, ColorType ColorOpposite>
bool ChessEngineAllPossibleMovesGenerator::MovePieceTo(const PosType ActualPosX, const PosType ActualPosY, const PosType NewPosX, const PosType NewPosY) noexcept
{
	try
	{	
		const PieceNumType OldActPieceNum = ChessBoard[ActualPosX][ActualPosY];
		const PieceNumType OldNewPieceNum = ChessBoard[NewPosX][NewPosY];
		ChessBoard[NewPosX][NewPosY] = ChessBoard[ActualPosX][ActualPosY];
		ChessBoard[ActualPosX][ActualPosY] = SpaceNum;

		#ifdef __DEBUG__
		if (ChessEngineConfigurationFileReaderWriterObject.PrintDetailedMoveDescription == true)
			LoggersManagerObject.Log(STREAM(ColorName[Color] << " P " << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldActPieceNum] << " " << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldNewPieceNum] << " " << int(ActualPosX) << " " << int(ActualPosY) << " " << int(NewPosX) << " " << int(NewPosY)));
		#endif

		PosType KingPosXToTestCheck, KingPosYToTestCheck;
		if (OldActPieceNum == KingNum[Color])
		{
			KingPosXToTestCheck = NewPosX;
			KingPosYToTestCheck = NewPosY;
		}
		else
		{
			KingPosXToTestCheck = Pieces[KingNum[Color]].ActualPosX;
			KingPosYToTestCheck = Pieces[KingNum[Color]].ActualPosY;
		}

		if (IsCheckForKing<ColorOpposite>(KingPosXToTestCheck, KingPosYToTestCheck) == true)
		{
            #ifdef __DEBUG__
			if (ChessEngineConfigurationFileReaderWriterObject.PrintChessBoardStateForCheck == true)
                ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);
			#endif

			ChessBoard[ActualPosX][ActualPosY] = OldActPieceNum;
			ChessBoard[NewPosX][NewPosY] = OldNewPieceNum;

			#ifdef __DEBUG__
			if (ChessEngineConfigurationFileReaderWriterObject.PrintDetailedMoveDescription == true)
			    LoggersManagerObject.Log(STREAM(ColorName[Color] << " C " << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldActPieceNum] << " " << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldNewPieceNum] << " " << int(ActualPosX) << " " << int(ActualPosY) << " " << int(NewPosX) << " " << int(NewPosY)));
			#endif

			ChessEngineResultsStatisticsObject.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Color]++;

			return false;
		}

		ChessEngineResultsStatisticsObject.NumberOfAllPiecesMovesByColor[Color]++;

		#ifdef __DEBUG__
		if (ChessEngineConfigurationFileReaderWriterObject.PrintActualChessBoardState == true)
			if (ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() % ChessEngineConfigurationFileReaderWriterObject.ModuloToWriteMove == 0)
                ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogOnlyToFiles, 2);
		#endif

		Pieces[OldActPieceNum].ActualPosX = NewPosX;
		Pieces[OldActPieceNum].ActualPosY = NewPosY;

		bool OldActPieceNumPromoted;
		if (IsPawn<Color>(OldActPieceNum) == true)
		{
			OldActPieceNumPromoted = Pieces[OldActPieceNum].Promoted;
			CheckPawnPromotion<Color>(NewPosX, NewPosY, Pieces[OldActPieceNum]);
		}

		#ifdef __DEBUG__
		if (OldNewPieceNum != SpaceNum && IsOppositePieceNum<ColorOpposite>(OldNewPieceNum) == false)
		{
            ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);
			LoggersManagerObject.Log(STREAM("#" << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldNewPieceNum] << "#" << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldActPieceNum] << "# ERROR = ( " << PosType(OldNewPieceNum) << " " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel) << " ) " << string(100, SpaceChar)));
			getchar();
		}
		#endif	

		SaveActuallyInvestigatedMovesPathRec(ActualPosX, ActualPosY, NewPosX, NewPosY, OldActPieceNum, ActuallyInvestigatedMovesPath[DeepLevel]);

        if (ChessEngineConfigurationFileReaderWriterObject.FirstMove == true)
        {
            ChessEngineConfigurationFileReaderWriterObject.FirstMove = false;
            ChessEngineConfigurationFileReaderWriterObject.FirstPossibleMove = ChessEngineConfigurationFileReaderWriterObject.PieceString[ChessBoard[NewPosX][NewPosY]] + "[" + GetChessMoveAsString(ActualPosX, ActualPosY, NewPosX, NewPosY) + "]";
        }

		if (OldNewPieceNum != SpaceNum)
		{
			Pieces[OldNewPieceNum].Alive = false;
			ChessEngineResultsStatisticsObject.NumberOfAllOppositePiecesRemovedByMoveOfColor[Color]++;

			ActuallyInvestigatedMovesPath[DeepLevel].Type = (Color == White ? 1 : -1);
			PieceNumType PieceNumForPower = (Pieces[OldNewPieceNum].Promoted == true ? 0 : OldNewPieceNum);
			ActuallyInvestigatedMovesPath[DeepLevel].Score = ActuallyInvestigatedMovesPath[DeepLevel].Type * ChessEngineConfigurationFileReaderWriterObject.PiecePower[PieceNumForPower] * (ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel - DeepLevel + 1);
						
			SaveTheBestMovesPath<Color, false>(DeepLevel + 1, ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White], ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black]);
		}

		#ifdef __DEBUG__
		if (ChessEngineConfigurationFileReaderWriterObject.PrintWhenRemovingPiece == true)
		{
			if (IsOppositePieceNum<ColorOpposite>(OldNewPieceNum) == true)
			{
                ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);
				LoggersManagerObject.Log(STREAM(""));
				LoggersManagerObject.Log(STREAM("VERIFY REMOVE = ( " << GetChessMovesPathAsString(GetChessMovesPathVector(DeepLevel, ActuallyInvestigatedMovesPath)) << "-> " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel) << " ) " << string(100, SpaceChar)));
				LoggersManagerObject.Log(STREAM("Piece " << ColorName[ColorOpposite] << ChessEngineConfigurationFileReaderWriterObject.PieceString[OldNewPieceNum] << " = " << Pieces[OldNewPieceNum].Alive));
				if (ChessEngineConfigurationFileReaderWriterObject.StopWhenRemovingPiece == true)
					getchar();
			}
		}

		if (ChessEngineConfigurationFileReaderWriterObject.PrintActualMove == true)
			if (ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() % ChessEngineConfigurationFileReaderWriterObject.ModuloToWriteMove == 0)
			{
				LoggersManagerObject.Log(STREAM(""));
				LoggersManagerObject.Log(STREAM("MOVE " << ColorName[Color] << " = ( " << GetChessMovesPathAsString(GetChessMovesPathVector(DeepLevel, ActuallyInvestigatedMovesPath)) << "-> " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel) << " ) " << string(100, SpaceChar)));
				if (ChessEngineConfigurationFileReaderWriterObject.StopWhenMove)
					getchar();
			}
		#endif

		if (ChessEngineThreadsStartData::FullSpeedExecutionInThreads == true)
        {
            if (DeepLevel < ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel)
            {
                DeepLevel++;
                GenerateAllPossibleCombinationsOfMoves<ColorOpposite, Color, Color == White ? StartOfWhitePiecesNum : StartOfBlackPiecesNum, Color == White ? EndOfWhitePiecesNum : EndOfBlackPiecesNum>();
                DeepLevel--;
            }
        }
		else
        {
            if (DeepLevel < ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel - 1)
            {
                ChessEngineResultsStatisticsObject.NumberOfExecutionsOnLevel[DeepLevel]++;

                if (DeepLevel == MaximalLengthOfPathToFindChessBoardsForFullSpeed - 1)
                    SaveChessBoardStateForFullSpeedThreadsExecution();
                else
                {
                    DeepLevel++;
                    GenerateAllPossibleCombinationsOfMoves<ColorOpposite, Color, Color == White ? StartOfWhitePiecesNum : StartOfBlackPiecesNum, Color == White ? EndOfWhitePiecesNum : EndOfBlackPiecesNum>();
                    DeepLevel--;
                }
            }
        }

		ActuallyInvestigatedMovesPath[DeepLevel].Type = 0;
		ActuallyInvestigatedMovesPath[DeepLevel].Score = 0;
		
		if (IsPawn<Color>(OldActPieceNum) == true)
			Pieces[OldActPieceNum].Promoted = OldActPieceNumPromoted;

		ChessBoard[ActualPosX][ActualPosY] = OldActPieceNum;
		ChessBoard[NewPosX][NewPosY] = OldNewPieceNum;

		Pieces[OldActPieceNum].ActualPosX = ActualPosX;
		Pieces[OldActPieceNum].ActualPosY = ActualPosY;

		if (OldNewPieceNum != SpaceNum)
			Pieces[OldNewPieceNum].Alive = true;
	}
	CATCH(string("moving piece ") + ColorName[Color] + string(" to "));

	return true;
}

template<PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
inline bool ChessEngineAllPossibleMovesGenerator::IsOppositePiece(const PosType PosX, const PosType PosY) const
{
	const PieceNumType ChessBoardPieceNum = ChessBoard[PosX][PosY];
	if (OppositePieceNumRangeStart <= ChessBoardPieceNum && ChessBoardPieceNum <= OppositePieceNumRangeEnd)
		return true;
	else
		return false;
}

template<ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
inline bool ChessEngineAllPossibleMovesGenerator::IsEmptyFieldOrOppositePieceAndNoOppositeKing(const PosType PosX, const PosType PosY) const
{
	const PieceNumType ChessBoardPieceNum = ChessBoard[PosX][PosY];
	if (ChessBoardPieceNum == SpaceNum || (IsOppositePiece<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY) == true && ChessBoardPieceNum != KingNum[ColorOpposite]))
		return true;
	else
		return false;
}

#define GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnEndPosX, PawnEndPosY, ConditionMain) \
		if (ConditionMain) \
		{ \
			if (MovePieceTo<Color, ColorOpposite>(PawnStartPosX, PawnStartPosY, PawnEndPosX, PawnEndPosY) == true) \
				PossibleMove = true; \
		}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
bool ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForPawn(const PosType PawnStartPosX, const PosType PawnStartPosY) noexcept
{
	bool PossibleMove = false;
	
	try
	{
		const PieceNumType PieceNum = ChessBoard[PawnStartPosX][PawnStartPosY];

		if (Pieces[PieceNum].Promoted == false)
		{

			if ((ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == true && (Color == White && PawnStartPosY - 1 >= 1)) || (ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == false && (Color == Black && PawnStartPosY - 1 >= 1)))
			{
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY - 1, (ChessBoard[PawnStartPosX][PawnStartPosY - 1] == SpaceNum));
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX + 1, PawnStartPosY - 1, (PawnStartPosX + 1 < MaxChessSizeX && IsOppositePiece<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX + 1, PawnStartPosY - 1) == true && ChessBoard[PawnStartPosX + 1][PawnStartPosY - 1] != KingNum[ColorOpposite]));
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX - 1, PawnStartPosY - 1, (PawnStartPosX - 1 >= 1 && IsOppositePiece<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX - 1, PawnStartPosY - 1) == true && ChessBoard[PawnStartPosX - 1][PawnStartPosY - 1] != KingNum[ColorOpposite]));

				if (PawnStartPosY == 7)
					GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY - 2, (ChessBoard[PawnStartPosX][PawnStartPosY - 2] == SpaceNum && ChessBoard[PawnStartPosX][PawnStartPosY - 1] == SpaceNum));
			}
			else
            if ((ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == false && (Color == White && PawnStartPosY + 1 < MaxChessSizeY)) || (ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == true && (Color == Black && PawnStartPosY + 1 < MaxChessSizeY)))
			{
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY + 1, (ChessBoard[PawnStartPosX][PawnStartPosY + 1] == SpaceNum));
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX + 1, PawnStartPosY + 1, (PawnStartPosX + 1 < MaxChessSizeX && IsOppositePiece<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX + 1, PawnStartPosY + 1) == true && ChessBoard[PawnStartPosX + 1][PawnStartPosY + 1] != KingNum[ColorOpposite]));
				GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX - 1, PawnStartPosY + 1, (PawnStartPosX - 1 >= 1 && IsOppositePiece<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX - 1, PawnStartPosY + 1) == true && ChessBoard[PawnStartPosX - 1][PawnStartPosY + 1] != KingNum[ColorOpposite]));

				if (PawnStartPosY == 2)
					GenerateOnePossibleMoveForPawn(PawnStartPosX, PawnStartPosY, PawnStartPosX, PawnStartPosY + 2, (ChessBoard[PawnStartPosX][PawnStartPosY + 2] == SpaceNum && ChessBoard[PawnStartPosX][PawnStartPosY + 1] == SpaceNum));
		    }
		}
		else
		{
			if (GenerateAllPossibleMovesForQueen<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PawnStartPosX, PawnStartPosY) == true)
				PossibleMove = true;
		}
	}
	CATCH("generating all possible moves for pawn");

	return PossibleMove;
}

#define GenerateAllPossibleMovesInOneLineAndOneDirection(StartPosX1, StartPosY1, StartPosX2, StartPosY2, PosX, PosY, LoopCondition, LoopOperation) \
		for (PosX = StartPosX1, PosY = StartPosY1; LoopCondition && IsEmptyFieldOrOppositePieceAndNoOppositeKing<ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY); LoopOperation) \
		{ \
			const bool OppositePiece = IsOppositePiece<OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY); \
			if (MovePieceTo<Color, ColorOpposite>(StartPosX2, StartPosY2, PosX, PosY) == true) \
				PossibleMove = true; \
			if (OppositePiece) \
				break; \
		} 

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
bool ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook(const PosType RookStartPosX, const PosType RookStartPosY) noexcept
{
	bool PossibleMove = false;

	try
	{			
		PosType RookPosX, RookPosY;

		GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX, RookStartPosY + 1, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosY < MaxChessSizeY, RookPosY++);
		GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX, RookStartPosY - 1, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosY >= 1, RookPosY--);
		GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX + 1, RookStartPosY, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosX < MaxChessSizeX, RookPosX++);
		GenerateAllPossibleMovesInOneLineAndOneDirection(RookStartPosX - 1, RookStartPosY, RookStartPosX, RookStartPosY, RookPosX, RookPosY, RookPosX >= 1, RookPosX--);
	}
	CATCH("generating all possible moves for rook");

	return PossibleMove;
}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
bool ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop(const PosType BishopStartPosX, const PosType BishopStartPosY) noexcept
{
	bool PossibleMove = false;

	try
	{
 		PosType BishopPosX, BishopPosY;

		GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX + 1, BishopStartPosY + 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX < MaxChessSizeX && BishopPosY < MaxChessSizeY, (BishopPosX++, BishopPosY++));
		GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX - 1, BishopStartPosY - 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX >= 1 && BishopPosY >= 1, (BishopPosX--, BishopPosY--));
		GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX - 1, BishopStartPosY + 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX >= 1 && BishopPosY < MaxChessSizeY, (BishopPosX--, BishopPosY++));
		GenerateAllPossibleMovesInOneLineAndOneDirection(BishopStartPosX + 1, BishopStartPosY - 1, BishopStartPosX, BishopStartPosY, BishopPosX, BishopPosY, BishopPosX < MaxChessSizeX && BishopPosY >= 1, (BishopPosX++, BishopPosY--));
	}
	CATCH("generating all possible moves for bishop");

	return PossibleMove;
}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
bool ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForQueen(const PosType QueenStartPosX, const PosType QueenStartPosY) noexcept
{
	bool PossibleMove = false;

	try
	{
		if (GenerateAllPossibleMovesForRook<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(QueenStartPosX, QueenStartPosY) == true)
			PossibleMove = true;
		if (GenerateAllPossibleMovesForBishop<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(QueenStartPosX, QueenStartPosY) == true)
			PossibleMove = true;
	}
	CATCH("generating all possible moves for queen");

	return PossibleMove;
}

#define GenerateOnePossibleMoveForKnight(StartPosX, StartPosY, EndPosX, EndPosY, KnightMoveCondition) \
		if (KnightMoveCondition && IsEmptyFieldOrOppositePieceAndNoOppositeKing<ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(EndPosX, EndPosY)) \
		{ \
			if (MovePieceTo<Color, ColorOpposite>(StartPosX, StartPosY, EndPosX, EndPosY) == true) \
				PossibleMove = true; \
		}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
bool ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight(const PosType KnightStartPosX, const PosType KnightStartPosY) noexcept
{
	bool PossibleMove = false;

	try
	{
		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 1, KnightStartPosY + 2, (KnightStartPosX - 1 >= 1 && KnightStartPosY + 2 < MaxChessSizeY));
		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 1, KnightStartPosY + 2, (KnightStartPosX + 1 < MaxChessSizeX && KnightStartPosY + 2 < MaxChessSizeY));

		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 2, KnightStartPosY + 1, (KnightStartPosX + 2 < MaxChessSizeX && KnightStartPosY + 1 < MaxChessSizeY));
		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 2, KnightStartPosY - 1, (KnightStartPosX + 2 < MaxChessSizeX && KnightStartPosY - 1 >= 1));

		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 1, KnightStartPosY - 2, (KnightStartPosX - 1 >= 1 && KnightStartPosY - 2 >= 1));
		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX + 1, KnightStartPosY - 2, (KnightStartPosX + 1 < MaxChessSizeX && KnightStartPosY - 2 >= 1));

		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 2, KnightStartPosY + 1, (KnightStartPosX - 2 >= 1 && KnightStartPosY + 1 < MaxChessSizeY));
		GenerateOnePossibleMoveForKnight(KnightStartPosX, KnightStartPosY, KnightStartPosX - 2, KnightStartPosY - 1, (KnightStartPosX - 2 >= 1 && KnightStartPosY - 1 >= 1));
	}
	CATCH("generating all possible moves for knight");

	return PossibleMove;
}

#define FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY, PieceNumKingColorOpposite, KingPosCondition) \
		if (KingPosCondition && ChessBoard[KingPosX][KingPosY] == PieceNumKingColorOpposite) \
			return true; 

template <ColorType ColorOpposite>
inline bool ChessEngineAllPossibleMovesGenerator::IsKingNearTo(const PosType KingPosX, const PosType KingPosY) const noexcept
{
	try
	{
		const PieceNumType PieceNumKingColorOpposite = KingNum[ColorOpposite];

		FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY, PieceNumKingColorOpposite, true);

		FindKingOppositeAndIfFoundReturnTrue(KingPosX - 1, KingPosY, PieceNumKingColorOpposite, KingPosX - 1 >= 1);
		FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY - 1, PieceNumKingColorOpposite, KingPosY - 1 >= 1);
		FindKingOppositeAndIfFoundReturnTrue(KingPosX - 1, KingPosY - 1, PieceNumKingColorOpposite, KingPosX - 1 >= 1 && KingPosY - 1 >= 1);
		FindKingOppositeAndIfFoundReturnTrue(KingPosX + 1, KingPosY, PieceNumKingColorOpposite, KingPosX + 1 < MaxChessSizeX);

		FindKingOppositeAndIfFoundReturnTrue(KingPosX, KingPosY + 1, PieceNumKingColorOpposite, KingPosY + 1 < MaxChessSizeY);
		FindKingOppositeAndIfFoundReturnTrue(KingPosX + 1, KingPosY + 1, PieceNumKingColorOpposite, KingPosX + 1 < MaxChessSizeX && KingPosY + 1 < MaxChessSizeY);
		FindKingOppositeAndIfFoundReturnTrue(KingPosX - 1, KingPosY + 1, PieceNumKingColorOpposite, KingPosX - 1 >= 1 && KingPosY + 1 < MaxChessSizeY);
		FindKingOppositeAndIfFoundReturnTrue(KingPosX + 1, KingPosY - 1, PieceNumKingColorOpposite, KingPosX + 1 < MaxChessSizeX && KingPosY - 1 >= 1);
	}
	CATCH("is king near to");

	return false;
}

#define FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(PosX, PosY, KingStartPosX, KingStartPosY, PieceNum1ColorOpposite, PieceNum2ColorOpposite, PieceNumQueenColorOpposite, LoopCondition, LoopOperation) \
		for (PosX = KingStartPosX, PosY = KingStartPosY; LoopCondition && ChessBoard[PosX][PosY] == SpaceNum; LoopOperation); \
		ChessBoardPieceNum = ChessBoard[PosX][PosY]; \
		if (LoopCondition && (ChessBoardPieceNum == PieceNum1ColorOpposite || ChessBoardPieceNum == PieceNum2ColorOpposite || ChessBoardPieceNum == PieceNumQueenColorOpposite || (IsPawn<ColorOpposite>(ChessBoardPieceNum) == true && Pieces[ChessBoardPieceNum].Promoted == true))) \
			return true;

#define FindKnightForCheckAndIfFoundReturnTrue(PosX, PosY, PieceNumKnight1ColorOpposite, PieceNumKnight2ColorOpposite, Condition) \
		if (Condition && (ChessBoard[PosX][PosY] == PieceNumKnight1ColorOpposite || ChessBoard[PosX][PosY] == PieceNumKnight2ColorOpposite)) \
			return true;

#define FindPawnForCheckAndIfFoundReturnTrue(PosX, PosY, Condition) \
		if (Condition && (IsPawn<ColorOpposite>(ChessBoard[PosX][PosY]) == true)) \
			return true;

template <ColorType ColorOpposite>
inline bool ChessEngineAllPossibleMovesGenerator::IsCheckForKing(const PosType KingPosX, const PosType KingPosY) const noexcept
{
	try
	{
		PieceNumType ChessBoardPieceNum;

		const PieceNumType PieceNumQueenColorOpposite = QueenNum[ColorOpposite];

		const PieceNumType PieceNumBishop1ColorOpposite = Bishop1Num[ColorOpposite];
		const PieceNumType PieceNumBishop2ColorOpposite = Bishop2Num[ColorOpposite];

		if (Pieces[PieceNumBishop1ColorOpposite].Alive == true || Pieces[PieceNumBishop2ColorOpposite].Alive == true || Pieces[PieceNumQueenColorOpposite].Alive == true || IsAnyPawnPromoted == true) 
		{
			PosType BishopPosX, BishopPosY;
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX + 1, KingPosY + 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX < MaxChessSizeX && BishopPosY < MaxChessSizeY, (BishopPosX++, BishopPosY++));
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX - 1, KingPosY - 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX >= 1 && BishopPosY >= 1, (BishopPosX--, BishopPosY--));
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX - 1, KingPosY + 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX >= 1 && BishopPosY < MaxChessSizeY, (BishopPosX--, BishopPosY++));
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(BishopPosX, BishopPosY, KingPosX + 1, KingPosY - 1, PieceNumBishop1ColorOpposite, PieceNumBishop2ColorOpposite, PieceNumQueenColorOpposite, BishopPosX < MaxChessSizeX && BishopPosY >= 1, (BishopPosX++, BishopPosY--));
		}

		const PieceNumType PieceNumRook1ColorOpposite = Rook1Num[ColorOpposite];
		const PieceNumType PieceNumRook2ColorOpposite = Rook2Num[ColorOpposite];

		if (Pieces[PieceNumRook1ColorOpposite].Alive == true || Pieces[PieceNumRook2ColorOpposite].Alive == true || Pieces[PieceNumQueenColorOpposite].Alive == true || IsAnyPawnPromoted == true)
		{
			PosType RookPosX, RookPosY;
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX, KingPosY + 1, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosY < MaxChessSizeY, RookPosY++);
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX, KingPosY - 1, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosY >= 1, RookPosY--);
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX + 1, KingPosY, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosX < MaxChessSizeX, RookPosX++);
			FindQueenOrBishopOrRookForCheckAndIfFoundReturnTrue(RookPosX, RookPosY, KingPosX - 1, KingPosY, PieceNumRook1ColorOpposite, PieceNumRook2ColorOpposite, PieceNumQueenColorOpposite, RookPosX >= 1, RookPosX--);
		}

		const PieceNumType PieceNumKnight1ColorOpposite = Knight1Num[ColorOpposite];
		const PieceNumType PieceNumKnight2ColorOpposite = Knight2Num[ColorOpposite];

		if (Pieces[PieceNumKnight1ColorOpposite].Alive == true || Pieces[PieceNumKnight1ColorOpposite].Alive == true)
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

		if ((ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == false && (ColorOpposite == Black && KingPosY + 1 < MaxChessSizeY)) || (ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == true && (ColorOpposite == White && KingPosY + 1 < MaxChessSizeY)))
		{
			FindPawnForCheckAndIfFoundReturnTrue(KingPosX + 1, KingPosY + 1, KingPosX + 1 < MaxChessSizeX);
			FindPawnForCheckAndIfFoundReturnTrue(KingPosX - 1, KingPosY + 1, KingPosX - 1 >= 1);
		}
		else
		if ((ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == false && (ColorOpposite == White && KingPosY - 1 >= 1)) || (ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces == true && (ColorOpposite == Black && KingPosY - 1 >= 1)))
		{
			FindPawnForCheckAndIfFoundReturnTrue(KingPosX + 1, KingPosY - 1, KingPosX + 1 < MaxChessSizeX);
			FindPawnForCheckAndIfFoundReturnTrue(KingPosX - 1, KingPosY - 1, KingPosX - 1 >= 1);
		}
	}
	CATCH("veryfing if is check for king");

	return false;
}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
inline bool ChessEngineAllPossibleMovesGenerator::IsKingMoveImpossible(const PosType PosX, const PosType PosY) const
{
	if (IsEmptyFieldOrOppositePieceAndNoOppositeKing<ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(PosX, PosY) == true && IsKingNearTo<ColorOpposite>(PosX, PosY) == false)
		return false;
	else
		return true;
}

#define GenerateOnePossibleMoveForKing(KingStartPosX1, KingStartPosY1, KingStartPosX2, KingStartPosY2, Condition) \
		if (Condition && IsKingMoveImpossible<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(KingStartPosX2, KingStartPosY2) == false) \
		{ \
			if(MovePieceTo<Color, ColorOpposite>(KingStartPosX1, KingStartPosY1, KingStartPosX2, KingStartPosY2) == true) \
				PossibleMove = true; \
		}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
bool ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKing(const PosType KingStartPosX, const PosType KingStartPosY)	noexcept
{
	bool PossibleMove = false;

	try
	{
		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX - 1, KingStartPosY, KingStartPosX - 1 >= 1);
		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX, KingStartPosY - 1, KingStartPosY - 1 >= 1);
		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX - 1, KingStartPosY - 1, KingStartPosX - 1 >= 1 && KingStartPosY - 1 >= 1);

        GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX + 1, KingStartPosY, KingStartPosX + 1 < MaxChessSizeX);
		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX, KingStartPosY + 1, KingStartPosY + 1 < MaxChessSizeY);
		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX + 1, KingStartPosY + 1, KingStartPosX + 1 < MaxChessSizeX && KingStartPosY + 1 < MaxChessSizeY);

		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX - 1, KingStartPosY + 1, KingStartPosX - 1 >= 1 && KingStartPosY + 1 < MaxChessSizeY);
		GenerateOnePossibleMoveForKing(KingStartPosX, KingStartPosY, KingStartPosX + 1, KingStartPosY - 1, KingStartPosX + 1 < MaxChessSizeX && KingStartPosY - 1 >= 1);
	}
	CATCH("generating all possible moves for king");

	return PossibleMove;
}

template <ColorType Color, ColorType ColorOpposite>
inline bool ChessEngineAllPossibleMovesGenerator::VerifyCheckOfKing()	noexcept
{
	bool IsCheck;

	try
	{
		IsCheck = IsCheckForKing<ColorOpposite>(Pieces[KingNum[Color]].ActualPosX, Pieces[KingNum[Color]].ActualPosY);
		if (IsCheck == true) 
			ChessEngineResultsStatisticsObject.NumberOfAllMovesWhenCheckedKingColor[Color]++;

		#ifdef __DEBUG__
		if (ChessEngineConfigurationFileReaderWriterObject.PrintGeneralInterMoveInfo == true)
		{
			LoggersManagerObject.Log(STREAM(""));
			LoggersManagerObject.Log(STREAM("Inter Move " << ColorName[Color] << " S1 = ( " << GetChessMovesPathAsString(GetChessMovesPathVector(DeepLevel, ActuallyInvestigatedMovesPath)) << "-> " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel) << " ) " << string(100, SpaceChar)));
		}
		#endif
	}
	CATCH("veryfing checking of king");

	return IsCheck;
}

inline void ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForPiece(const PieceNumType PieceNum, bool& PossibleMove, bool (ChessEngineAllPossibleMovesGenerator::*FunctionName)(const PosType, const PosType))
{
	if (Pieces[PieceNum].Alive == true)
	{
		if ((this->*FunctionName)(Pieces[PieceNum].ActualPosX, Pieces[PieceNum].ActualPosY) == true)
			PossibleMove = true;
	}
}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
void ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleCombinationsOfMoves() noexcept
{
	try
	{
		ChessEngineResultsStatisticsObject.NumberOfAllRecursiveMoves++;

		const bool IsCheck = VerifyCheckOfKing<Color, ColorOpposite>();

		bool PossibleMove = false;

        for (PieceNumType Pawn = Pawn1Num[Color]; Pawn <= Pawn8Num[Color]; Pawn++)
            GenerateAllPossibleMovesForPiece(Pawn, PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForPawn<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
        GenerateAllPossibleMovesForPiece(QueenNum[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForQueen<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
		GenerateAllPossibleMovesForPiece(Rook1Num[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
		GenerateAllPossibleMovesForPiece(Rook2Num[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
		GenerateAllPossibleMovesForPiece(Bishop1Num[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
		GenerateAllPossibleMovesForPiece(Bishop2Num[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
		GenerateAllPossibleMovesForPiece(Knight1Num[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
		GenerateAllPossibleMovesForPiece(Knight2Num[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);
        GenerateAllPossibleMovesForPiece(KingNum[Color], PossibleMove, &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKing<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>);

		UpdateDataForIsCheckAndPossibleMove<Color, ColorOpposite, OppositePieceNumRangeStart, OppositePieceNumRangeEnd>(IsCheck, PossibleMove);
	}
	CATCH("generating all possible combinations of moves");
}

template <ColorType Color, ColorType ColorOpposite, PieceNumType OppositePieceNumRangeStart, PieceNumType OppositePieceNumRangeEnd>
inline void ChessEngineAllPossibleMovesGenerator::UpdateDataForIsCheckAndPossibleMove(const bool IsCheck, const bool PossibleMove) noexcept
{
	try
	{
		#ifdef __DEBUG__
		if (ChessEngineConfigurationFileReaderWriterObject.PrintGeneralInterMoveInfo == true)
		{
			LoggersManagerObject.Log(STREAM(""));
			LoggersManagerObject.Log(STREAM("Inter Move " << ColorName[Color] << " S2 = ( " << GetChessMovesPathAsString(GetChessMovesPathVector(DeepLevel, ActuallyInvestigatedMovesPath)) << "-> " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel) << " ) " << string(100, SpaceChar)));
		}

		if (PossibleMove == false && ChessEngineConfigurationFileReaderWriterObject.PrintPossibleMoveFalseState == true)
		{
			ChessEngineDataInitializerObject.PrintActualStateOfPieces();
            ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);
			LoggersManagerObject.Log(STREAM(""));
			LoggersManagerObject.Log(STREAM("Poss Move " << ColorName[Color] << " False = ( " << GetChessMovesPathAsString(GetChessMovesPathVector(DeepLevel, ActuallyInvestigatedMovesPath)) << "-> " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel) << " ) " << string(100, SpaceChar)));
			if (ChessEngineConfigurationFileReaderWriterObject.StopWhenPossibleMoveFalseState == true)
				getchar();
		}
        #endif

		if (IsCheck == true && PossibleMove == false)
		{
            SaveTheBestMovesPath<ColorOpposite, true>(DeepLevel, ChessEngineResultsStatisticsObject.TheHighestScoredFoundMateMovesPathForAdvantageOf[White], ChessEngineResultsStatisticsObject.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black]);
			
			auto&& [ActualMovesPathScore, ActualMovesPathTypeSum] = SumActuallyInvestigatedMovesPathScore(DeepLevel);

			ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[Color].emplace_back(ChessScoredMovesPath{ActualMovesPathScore, ActualMovesPathTypeSum, {}, DeepLevel });
            CopyChessMovesPathsVector(DeepLevel, ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[Color].back().MovesPath, ActuallyInvestigatedMovesPath);

			#ifdef __DEBUG__
			if (ChessEngineConfigurationFileReaderWriterObject.PrintActualStateWhenMateFound == true)
			{
				LoggersManagerObject.Log(STREAM(""));
				LoggersManagerObject.Log(STREAM("FOUND MATE"));
                ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);
				LoggersManagerObject.Log(STREAM("PATH TO MATE: " << GetChessMovesPathAsString(GetChessMovesPathVector(DeepLevel, ActuallyInvestigatedMovesPath)) << "-> " << ChessEngineResultsStatisticsObject.GetNumberOfAllMovesTotal() << " " << int(DeepLevel)));
				ChessEngineDataInitializerObject.PrintActualStateOfPieces();
			}
			if (ChessEngineConfigurationFileReaderWriterObject.StopWhenMateFound == true)
                getchar();
			#endif
		}
		else
		if (IsCheck == false && PossibleMove == false)
        {
            auto&& [ActualMovesPathScore, ActualMovesPathTypeSum] = SumActuallyInvestigatedMovesPathScore(DeepLevel);

			ChessEngineResultsStatisticsObject.AllFoundMovesPathsToStalemate.emplace_back(ChessScoredMovesPath{ActualMovesPathScore, ActualMovesPathTypeSum, {}, DeepLevel });
            CopyChessMovesPathsVector(DeepLevel, ChessEngineResultsStatisticsObject.AllFoundMovesPathsToStalemate.back().MovesPath, ActuallyInvestigatedMovesPath);
        }
	}
	CATCH("updating data for check and possible move");
}

template void ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>() noexcept;

template bool ChessEngineAllPossibleMovesGenerator::IsPawn<White>(const PieceNumType PieceNum) const noexcept;
template bool ChessEngineAllPossibleMovesGenerator::IsPawn<Black>(const PieceNumType PieceNum) const noexcept;

template bool ChessEngineAllPossibleMovesGenerator::VerifyCheckOfKing<White, Black>() noexcept;
template void ChessEngineAllPossibleMovesGenerator::UpdateDataForIsCheckAndPossibleMove<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>(const bool IsCheck, const bool PossibleMove) noexcept;
