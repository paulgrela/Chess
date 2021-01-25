
#include "DestinationPlatform.h"

#ifdef UNIX_PLATFORM
#include <string.h>
#endif

#include <fstream>
#include <sstream>

#include "ExceptionsMacro.h"

#include "Logger.h"
#include "FileUtils.h"
#include "StringUtils.h"
#include "DateTimeUtils.h"

#include "ChessEngineChessMoveBasicOperations.h"
#include "ChessEngineAllPossibleMovesGenerator.h"

#include "ChessEngineResultsStatistics.h"

#include "TerminalColorsUtils.h"

using namespace std;

void ChessEngineResultsStatistics::SumStatisticsFromTwoChessEngineResultsStatisticsObjects(ChessEngineResultsStatistics& ChessEngineResultsStatisticsObjectMain, const ChessEngineResultsStatistics& ChessEngineResultsStatisticsObjectToAdd)
{
	try
	{
		for (LengthType DeepLevel = 0; DeepLevel < ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel; DeepLevel++)
			ChessEngineResultsStatisticsObjectMain.NumberOfExecutionsOnLevel[DeepLevel] += ChessEngineResultsStatisticsObjectToAdd.NumberOfExecutionsOnLevel[DeepLevel];

		ChessEngineResultsStatisticsObjectMain.NumberOfAllRecursiveMoves += ChessEngineResultsStatisticsObjectToAdd.NumberOfAllRecursiveMoves;

		for (ColorType Color = White; Color <= Black; Color++)
		{
			ChessEngineResultsStatisticsObjectMain.NumberOfAllMovesWhenCheckedKingColor[Color] += ChessEngineResultsStatisticsObjectToAdd.NumberOfAllMovesWhenCheckedKingColor[Color];
			ChessEngineResultsStatisticsObjectMain.NumberOfAllPiecesMovesByColor[Color] += ChessEngineResultsStatisticsObjectToAdd.NumberOfAllPiecesMovesByColor[Color];
			ChessEngineResultsStatisticsObjectMain.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Color] += ChessEngineResultsStatisticsObjectToAdd.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Color];
			ChessEngineResultsStatisticsObjectMain.NumberOfAllOppositePiecesRemovedByMoveOfColor[Color] += ChessEngineResultsStatisticsObjectToAdd.NumberOfAllOppositePiecesRemovedByMoveOfColor[Color];

			if (ChessEngineResultsStatisticsObjectToAdd.AllFoundMovesPathsToMate[Color].size() > 0)
				ChessEngineResultsStatisticsObjectMain.AllFoundMovesPathsToMate[Color].insert(ChessEngineResultsStatisticsObjectMain.AllFoundMovesPathsToMate[Color].end(), ChessEngineResultsStatisticsObjectToAdd.AllFoundMovesPathsToMate[Color].begin(), ChessEngineResultsStatisticsObjectToAdd.AllFoundMovesPathsToMate[Color].end());
		}

		if (ChessEngineResultsStatisticsObjectToAdd.AllFoundMovesPathsToStalemate.size() > 0)
			ChessEngineResultsStatisticsObjectMain.AllFoundMovesPathsToStalemate.insert(ChessEngineResultsStatisticsObjectMain.AllFoundMovesPathsToStalemate.end(), ChessEngineResultsStatisticsObjectToAdd.AllFoundMovesPathsToStalemate.begin(), ChessEngineResultsStatisticsObjectToAdd.AllFoundMovesPathsToStalemate.end());

		if (ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White].Score >= ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White].Score)
			ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White] = ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White];
		if (ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black].Score <= ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black].Score)
			ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black] = ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black];

		if (ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundMateMovesPathForAdvantageOf[White].Score < ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundMateMovesPathForAdvantageOf[White].Score || ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundMateMovesPathForAdvantageOf[White].MovesPath.empty() == true && ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundMateMovesPathForAdvantageOf[White].MovesPath.empty() == false)
			ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundMateMovesPathForAdvantageOf[White] = ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundMateMovesPathForAdvantageOf[White];
		if (ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black].Score > ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black].Score || ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black].MovesPath.empty() == true && ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black].MovesPath.empty() == false)
			ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black] = ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black];

		if (ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundStalemateMovesPath.Score <= ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundStalemateMovesPath.Score)
			ChessEngineResultsStatisticsObjectMain.TheHighestScoredFoundStalemateMovesPath = ChessEngineResultsStatisticsObjectToAdd.TheHighestScoredFoundStalemateMovesPath;
	}
	CATCH("summing all statistics from all threads")
}

void ChessEngineResultsStatistics::ClearAllStatistics()
{
	try
	{
		NumberOfAllRecursiveMoves = 0;

		for (LengthType DeepLevel = 0; DeepLevel < ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel; DeepLevel++)
			NumberOfExecutionsOnLevel[DeepLevel] = 0;

		for (ColorType Color = White; Color <= Black; Color++)
		{
			NumberOfAllMovesWhenCheckedKingColor[Color] = 0;
			NumberOfAllPiecesMovesByColor[Color] = 0;
			NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Color] = 0;
			NumberOfAllOppositePiecesRemovedByMoveOfColor[Color] = 0;
			AllFoundMovesPathsToMate[Color].clear();
			TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Color] = { 0, 0, vector<ChessMove>() };
			TheHighestScoredFoundMateMovesPathForAdvantageOf[Color] = { 0, 0, vector<ChessMove>() };
			TheChosenMovesPathForAdvantageOf[Color] = { 0, 0, vector<ChessMove>() };
		}

		TheHighestScoredFoundStalemateMovesPath = { 0, 0, vector<ChessMove>() };
		AllFoundMovesPathsToStalemate.clear();
	}
	CATCH("clearing all statistics")
}

uint64_t ChessEngineResultsStatistics::GetNumberOfAllMovesTotal() const
{
	return NumberOfAllPiecesMovesByColor[White] + NumberOfAllPiecesMovesByColor[Black];
}

void ChessEngineResultsStatistics::PrintGeneralStatistics() const
{
	try
	{		
		LoggersManagerObject.Log(STREAM("Number of all moves: " << GetNumberOfAllMovesTotal() << EndLineChar));

		LoggersManagerObject.Log(STREAM("Number of all recursive moves: " << NumberOfAllRecursiveMoves << EndLineChar));

		LoggersManagerObject.Log(STREAM("Number of all white pieces moves: " << NumberOfAllPiecesMovesByColor[White]));
		LoggersManagerObject.Log(STREAM("Number of all black pieces moves: " << NumberOfAllPiecesMovesByColor[Black] << EndLineChar));

		LoggersManagerObject.Log(STREAM("Number of all moves when king white is checked: " << NumberOfAllMovesWhenCheckedKingColor[White]));
		LoggersManagerObject.Log(STREAM("Number of all moves when king black is checked: " << NumberOfAllMovesWhenCheckedKingColor[Black] << EndLineChar));

		LoggersManagerObject.Log(STREAM("Number of all pieces lost by black and removed by white: " << NumberOfAllOppositePiecesRemovedByMoveOfColor[White]));
		LoggersManagerObject.Log(STREAM("Number of all pieces lost by white and removed by black: " << NumberOfAllOppositePiecesRemovedByMoveOfColor[Black] << EndLineChar));

		LoggersManagerObject.Log(STREAM("Number of all moves white cancelled because of discovered possible check of king white: " << NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[White]));
		LoggersManagerObject.Log(STREAM("Number of all moves black cancelled because of discovered possible check of king black: " << NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Black] << EndLineChar));
	}
	CATCH("printing general statistics")
}

void ChessEngineResultsStatistics::PrintDetailedStatistics() const
{
	try
	{
		for (LengthType DeepLevel = 0; DeepLevel < ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel; DeepLevel++)
			LoggersManagerObject.Log(STREAM("Number of exeuctions on level[" << DeepLevel <<"] = " << NumberOfExecutionsOnLevel[DeepLevel]));
		LoggersManagerObject.Log(STREAM(""));
	}
	CATCH("printing detailed statistics")
}

void ChessEngineResultsStatistics::PrintEveryMoveOnChessBoard(const vector<ChessMove>& OnePathToMate)
{
	try
	{
		PieceNumType ChessBoardForCopy[MaxChessSizeX][MaxChessSizeY];

		memcpy(ChessBoardForCopy, ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard, sizeof(PieceNumType) * MaxChessSizeX * MaxChessSizeY);

		ChessEngineAllPossibleMovesGeneratorLocalReference.ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);

		for (const auto& Move : OnePathToMate)
		{
			ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[Move.NextPosX][Move.NextPosY] = ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[Move.PrevPosX][Move.PrevPosY];
			ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[Move.PrevPosX][Move.PrevPosY] = SpaceNum;

			ChessEngineAllPossibleMovesGeneratorLocalReference.ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 0);
		}

		memcpy(ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard, ChessBoardForCopy, sizeof(PieceNumType) * MaxChessSizeX * MaxChessSizeY);
	}
	CATCH("printing every move to mate on chess board")
}

void ChessEngineResultsStatistics::PrintAllFoundMovesToMatePaths(const PosType Color)
{
	try
	{
		for (auto& OnePathMovesToMate : AllFoundMovesPathsToMate[Color])
		{
			if (ChessEngineConfigurationFileReaderWriterObject.PrintEveryMoveToMateOnChessBoardWhenPrintingStatistics == true)
			{
				LoggersManagerObject.Log(STREAM((EndLineChar)));
				PrintEveryMoveOnChessBoard(OnePathMovesToMate.MovesPath);
			}

			LoggersManagerObject.LogWithoutLineInfo((STREAM("PATH TO MATE " << ColorName[Color] << ": " << GetChessMovesPathAsString(OnePathMovesToMate.MovesPath))));

			if (ChessEngineConfigurationFileReaderWriterObject.PrintEveryMoveToMateOnChessBoardWhenPrintingStatistics == true)
				LoggersManagerObject.LogWithoutLineInfo((STREAM(EndLineChar)));

			if (ChessEngineConfigurationFileReaderWriterObject.StopWhenPrintEveryMoveToMateOnChessBoardWhenPrintingStatistics == true)
				getchar();
		}
	}
	CATCH("printing all found move to mate paths")
}

void ChessEngineResultsStatistics::PrintMovesPath(const char* TaskStr, const PosType Color1, const PosType Color2, const ChessScoredMovesPath& ChessScoredMovesPathObject, const bool PrintTheHighestScoredMovesPathWhenPrintingStatistics, const bool PrintTheHighestScoredMovesPathOnChessBoardWhenPrintingStatistics)
{
	try
	{
		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.Log(STREAM("Score of found best path to " << TaskStr << " " << ColorName[Color2] << "s so " << ColorName[Color1] << "s HAVE ADVANTAGE: Score = " << ChessScoredMovesPathObject.Score << " TypeSum = " << ChessScoredMovesPathObject.TypeSum));
		LoggersManagerObject.Log(STREAM(""));

		if (PrintTheHighestScoredMovesPathOnChessBoardWhenPrintingStatistics == true)
			PrintEveryMoveOnChessBoard(ChessScoredMovesPathObject.MovesPath);

		if (PrintTheHighestScoredMovesPathWhenPrintingStatistics == true)
		{
			LoggersManagerObject.LogWithoutLineInfo((STREAM("PATH TO " << TaskStr << " " << ColorName[Color2] << "s : " << GetChessMovesPathAsString(ChessScoredMovesPathObject.MovesPath))));
			LoggersManagerObject.Log(STREAM(""));
		}
	}
	CATCH("printing single paths")
}

void ChessEngineResultsStatistics::PrintAllFoundMovesPaths()
{
	try
	{
		PrintMovesPath("capture", Black, White, TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black], ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToCaptureWhenPrintingStatistics, ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToCaptureOnChessBoardWhenPrintingStatistics);
		PrintMovesPath("capture", White, Black, TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White], ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToCaptureWhenPrintingStatistics, ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToCaptureOnChessBoardWhenPrintingStatistics);

		PrintMovesPath("mate", Black, White, TheHighestScoredFoundMateMovesPathForAdvantageOf[Black], ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToMateWhenPrintingStatistics, ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToMateOnChessBoardWhenPrintingStatistics);
		PrintMovesPath("mate", White, Black, TheHighestScoredFoundMateMovesPathForAdvantageOf[White], ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToMateWhenPrintingStatistics, ChessEngineConfigurationFileReaderWriterObject.PrintTheHighestScoredMovesPathToMateOnChessBoardWhenPrintingStatistics);

		ChessEngineBestMoveFinder(*this).ChooseBestMove();

		PrintMovesPath("choose", Black, White, TheChosenMovesPathForAdvantageOf[Black], ChessEngineConfigurationFileReaderWriterObject.PrintChosenMovesPathWhenPrintingStatistics, ChessEngineConfigurationFileReaderWriterObject.PrintChosenMovesPathOnChessBoardWhenPrintingStatistics);
		PrintMovesPath("choose", White, Black, TheChosenMovesPathForAdvantageOf[White], ChessEngineConfigurationFileReaderWriterObject.PrintChosenMovesPathWhenPrintingStatistics, ChessEngineConfigurationFileReaderWriterObject.PrintChosenMovesPathOnChessBoardWhenPrintingStatistics);

		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::blue, STREAM("Number of found mates to mate white so BLACKS ARE WINNERS: " << AllFoundMovesPathsToMate[White].size()));
		if (ChessEngineConfigurationFileReaderWriterObject.PrintEveryPathToMateWhenPrintingStatistics == true)
			PrintAllFoundMovesToMatePaths(White);

		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::blue, STREAM("Number of found mates to mate black so WHITES ARE WINNERS: " << AllFoundMovesPathsToMate[Black].size()));
		if (ChessEngineConfigurationFileReaderWriterObject.PrintEveryPathToMateWhenPrintingStatistics == true)
			PrintAllFoundMovesToMatePaths(Black);

		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::blue, STREAM("Number of found stalemates: " << AllFoundMovesPathsToStalemate.size()));
		if (ChessEngineConfigurationFileReaderWriterObject.PrintEveryPathToMateWhenPrintingStatistics == true)
			for (const auto& OnePathMovesToStalemate : AllFoundMovesPathsToStalemate)
				LoggersManagerObject.Log(STREAM("PATH TO MATE STALEMATE: " << GetChessMovesPathAsString(OnePathMovesToStalemate.MovesPath) << EndLineChar));
	}
	CATCH("printing all found paths")
}

void ChessEngineResultsStatistics::PrintResultsAndStatistics()
{
	try
	{
		LoggersManagerObject.Log(STREAM("Statistics"));
		LoggersManagerObject.Log(STREAM(""));

		PrintGeneralStatistics();
		PrintDetailedStatistics();
		PrintAllFoundMovesPaths();
	}
	CATCH("printing results and statistics");
}

void ChessEngineResultsStatistics::SaveTestStatisticsToFile(const ChessEngineTest& ChessTestObject, const uint64_t TestId) const
{
	try
	{
		if (ChessEngineConfigurationFileReaderWriterObject.WriteTestsStatisticsToFile == true && ChessTestObject.WriteTestStatisticsToFile == true)
		{
			ChessEngineTest ChessTestObject;

			ChessTestObject.TestId = TestId;

			ChessTestObject.NumberOfAllMoves = GetNumberOfAllMovesTotal();

			ChessTestObject.NumberOfAllWhitePiecesMoves = NumberOfAllPiecesMovesByColor[White];
			ChessTestObject.NumberOfAllBlackPiecesMoves = NumberOfAllPiecesMovesByColor[Black];

			ChessTestObject.NumberOfAllWhiteMovesWhenCheckedKing = NumberOfAllMovesWhenCheckedKingColor[White];
			ChessTestObject.NumberOfAllBlackMovesWhenCheckedKing = NumberOfAllMovesWhenCheckedKingColor[Black];

			ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfWhite = NumberOfAllOppositePiecesRemovedByMoveOfColor[White];
			ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfBlack = NumberOfAllOppositePiecesRemovedByMoveOfColor[Black];

			ChessTestObject.NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing = NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[White];
			ChessTestObject.NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing = NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Black];

			ChessTestObject.NumberOfAllFoundMovesPathsToMateWhite = AllFoundMovesPathsToMate[White].size();
			ChessTestObject.NumberOfAllFoundMovesPathsToMateBlack = AllFoundMovesPathsToMate[Black].size();

			ChessTestObject.NumberOfAllFoundMovesPathsToStalemate = AllFoundMovesPathsToStalemate.size();

			ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite = TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White].Score;
			ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack = TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black].Score;

			ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfWhite = TheHighestScoredFoundMateMovesPathForAdvantageOf[White].Score;
			ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfBlack = TheHighestScoredFoundMateMovesPathForAdvantageOf[Black].Score;

			ChessEngineConfigurationFileReaderWriterObject.SaveTestStatisticsToFile(ChessTestObject);
		}
	}
	CATCH("writing statistics to file")
}

void ChessEngineResultsStatistics::CompareTestStatisticsSingleData(const int64_t DataFromFile, const int64_t DataFromTest, const char* ErrorText, bool& TestOK) const
{
	try
	{
		if (DataFromFile != DataFromTest)
		{
            LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::red, STREAM(ErrorText << DataFromFile << " != " << DataFromTest));
			TestOK = false;
		}
	}
	CATCH("comparing test data")
}

void ChessEngineResultsStatistics::CompareTestStatistics(const ChessEngineTest& ChessTestObject) const
{
	try
	{
		if (ChessEngineConfigurationFileReaderWriterObject.CompareTestStatistics == true && ChessTestObject.PrintTestComparisonResult == true)
		{
			LoggersManagerObject.Log(STREAM(""));
			LoggersManagerObject.Log(STREAM(""));

			bool TestOK = true;

			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllMoves, GetNumberOfAllMovesTotal(), "TEST ERROR Number of all moves different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllWhitePiecesMoves, NumberOfAllPiecesMovesByColor[White], "TEST ERROR Number of all white pieces moves different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllBlackPiecesMoves, NumberOfAllPiecesMovesByColor[Black], "TEST ERROR Number of all black pieces moves different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllWhiteMovesWhenCheckedKing, NumberOfAllMovesWhenCheckedKingColor[White], "TEST ERROR Number of all white pieces moves when checked king different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllBlackMovesWhenCheckedKing, NumberOfAllMovesWhenCheckedKingColor[Black], "TEST ERROR Number of all black pieces moves when checked king different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfWhite, NumberOfAllOppositePiecesRemovedByMoveOfColor[White], "TEST ERROR Number of all opposite pieces moves removed bt move of white different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfBlack, NumberOfAllOppositePiecesRemovedByMoveOfColor[Black], "TEST ERROR Number of all opposite pieces moves removed bt move of black different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing, NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[White], "TEST ERROR Number of all white moves cancelled because of discovered possible check of king white different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing, NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Black], "TEST ERROR Number of all white moves cancelled because of discovered possible check of king black different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllFoundMovesPathsToMateWhite, AllFoundMovesPathsToMate[White].size(), "TEST ERROR Number of all found moves paths to mate white different then awaited: ", TestOK);			
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllFoundMovesPathsToMateBlack, AllFoundMovesPathsToMate[Black].size(), "TEST ERROR Number of all found moves paths to mate black different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.NumberOfAllFoundMovesPathsToStalemate, AllFoundMovesPathsToStalemate.size(), "TEST ERROR Number of all found moves paths to stalemate different then awaited: ", TestOK);

			CompareTestStatisticsSingleData(ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite, TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White].Score, "TEST ERROR Score of found best advantage move path to capture black different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack, TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black].Score, "TEST ERROR Score of found best advantage move path to capture white different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfWhite, TheHighestScoredFoundMateMovesPathForAdvantageOf[White].Score, "TEST ERROR Score of found best advantage move path to mate black different then awaited: ", TestOK);
			CompareTestStatisticsSingleData(ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfBlack, TheHighestScoredFoundMateMovesPathForAdvantageOf[Black].Score, "TEST ERROR Score of found best advantage move path to mate white different then awaited: ", TestOK);

			if (TestOK == true)
               LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::green, STREAM(("Test OK")));

			LoggersManagerObject.Log(STREAM(""));
		}
	}
	CATCH("comparing test statistics")
}