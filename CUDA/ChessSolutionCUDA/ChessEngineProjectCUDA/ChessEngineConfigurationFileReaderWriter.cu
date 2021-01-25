
#include "DestinationPlatform.h"

#include <fstream>

#include "ChessEngineConstants.h"
#include "ExceptionsMacro.h"

#include "ChessEngineConfigurationFileReaderWriter.h"

using namespace std;

ChessEngineConfigurationFileReaderWriter ChessEngineConfigurationFileReaderWriterObject;

void ChessEngineConfigurationFileReaderWriter::ReadChessConfigurationFile(const char* ConfigFileName)
{
	try
	{
		std::ifstream ChessEngineProjectConfigFile(ConfigFileName);

		MultiThreaded = true;
		SetProcessPriorityHighest = true;

		ReadPreviousMovePathsFromFile = true;

		CompareTestStatistics = false;
		WriteTestsStatisticsToFile = false;

        #ifdef WINDOWS_PLATFORM
		Color1 = 10;
		Color2 = 13;
		Color3 = 10;
		Color4 = 13;
        #endif
        #ifdef UNIX_PLATFORM
		Color1 = 92;
		Color2 = 95;
		Color3 = 92;
		Color4 = 95;
		#endif

		PieceString[QueenNum[White]] = "Q ";
		PieceString[Bishop1Num[White]] = "G1";
		PieceString[Bishop2Num[White]] = "G2";
		PieceString[Rook1Num[White]] = "W1";
		PieceString[Rook2Num[White]] = "W2";
		PieceString[Knight1Num[White]] = "S1";
		PieceString[Knight2Num[White]] = "S2";
		PieceString[Pawn1Num[White]] = "P1";
		PieceString[Pawn1Num[White] + 1] = "P2";
		PieceString[Pawn1Num[White] + 2] = "P3";
		PieceString[Pawn1Num[White] + 3] = "P4";
		PieceString[Pawn1Num[White] + 4] = "P5";
		PieceString[Pawn1Num[White] + 5] = "P6";
		PieceString[Pawn1Num[White] + 6] = "P7";
		PieceString[Pawn8Num[White]] = "P8";
		PieceString[KingNum[White]] = "K ";
		PieceString[QueenNum[Black]] = "q ";
		PieceString[Bishop1Num[Black]] = "g1";
		PieceString[Bishop2Num[Black]] = "g2";
		PieceString[Rook1Num[Black]] = "w1";
		PieceString[Rook2Num[Black]] = "w2";
		PieceString[Knight1Num[Black]] = "s1";
		PieceString[Knight2Num[Black]] = "s2";
		PieceString[Pawn1Num[Black]] = "p1";
		PieceString[Pawn1Num[Black] + 1] = "p2";
		PieceString[Pawn1Num[Black] + 2] = "p3";
		PieceString[Pawn1Num[Black] + 3] = "p4";
		PieceString[Pawn1Num[Black] + 4] = "p5";
		PieceString[Pawn1Num[Black] + 5] = "p6";
		PieceString[Pawn1Num[Black] + 6] = "p7";
		PieceString[Pawn8Num[Black]] = "p8";
		PieceString[KingNum[Black]] = "k ";

		PiecePower[QueenNum[White]] = 16;
		PiecePower[Bishop1Num[White]] = 7;
		PiecePower[Bishop2Num[White]] = 7;
		PiecePower[Rook1Num[White]] = 8;
		PiecePower[Rook2Num[White]] = 8;
		PiecePower[Knight1Num[White]] = 5;
		PiecePower[Knight2Num[White]] = 5;
		PiecePower[Pawn1Num[White]] = 1;
		PiecePower[Pawn1Num[White] + 1] = 1;
		PiecePower[Pawn1Num[White] + 2] = 1;
		PiecePower[Pawn1Num[White] + 3] = 1;
		PiecePower[Pawn1Num[White] + 4] = 1;
		PiecePower[Pawn1Num[White] + 5] = 1;
		PiecePower[Pawn1Num[White] + 6] = 1;
		PiecePower[Pawn8Num[White]] = 1;
		PiecePower[KingNum[White]] = 1000;
		PiecePower[QueenNum[Black]] = 16;
		PiecePower[Bishop1Num[Black]] = 7;
		PiecePower[Bishop2Num[Black]] = 7;
		PiecePower[Rook1Num[Black]] = 8;
		PiecePower[Rook2Num[Black]] = 8;
		PiecePower[Knight1Num[Black]] = 5;
		PiecePower[Knight2Num[Black]] = 5;
		PiecePower[Pawn1Num[Black]] = 1;
		PiecePower[Pawn1Num[Black] + 1] = 1;
		PiecePower[Pawn1Num[Black] + 2] = 1;
		PiecePower[Pawn1Num[Black] + 3] = 1;
		PiecePower[Pawn1Num[Black] + 4] = 1;
		PiecePower[Pawn1Num[Black] + 5] = 1;
		PiecePower[Pawn1Num[Black] + 6] = 1;
		PiecePower[Pawn8Num[Black]] = 1;
		PiecePower[KingNum[Black]] = 1000;

		PrintLogToConsole = true;
		PrintLogToFiles = true;

		PrintLogLineNumberToConsole = false;
		PrintLogDateTimeToConsole = false;
		PrintLogProcessIdToConsole = false;
		PrintLogProcessPriorityLevelToConsole = false;
		PrintLogThreadIdToConsole = false;

		PrintLogLineNumberToFile = true;	
		PrintLogDateTimeToFile = true;
		PrintLogProcessIdToFile = false;
		PrintLogProcessPriorityLevelToFile = false;
		PrintLogThreadIdToFile = true;

		MaximalNumberOfLinesInOneFile = 200;

		PrintActualStartPositionOfPieces = true;
		StopWhenMove = false;
		StopWhenMateFound = false;

		PrintActualMove = false;

		PrintActualChessBoardState = true;
		PrintActualStateWhenMateFound = false;
		CleanScreenBeforePrintingActualState = false;

		PrintPossibleMoveFalseState = false;
		StopWhenPossibleMoveFalseState = false;
		PrintDetailedMoveDescription = false;
		PrintGeneralInterMoveInfo = false;
		PrintChessBoardStateForCheck = false;
		PrintWhenRemovingPiece = false;
		StopWhenRemovingPiece = false;

		ChessEngineProjectConfigFile >> PrintTheHighestScoredMovesPathToCaptureWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintTheHighestScoredMovesPathToCaptureOnChessBoardWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintTheHighestScoredMovesPathToMateWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintTheHighestScoredMovesPathToMateOnChessBoardWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintChosenMovesPathWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintChosenMovesPathOnChessBoardWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintEveryPathToMateWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> PrintEveryMoveToMateOnChessBoardWhenPrintingStatistics;
		ChessEngineProjectConfigFile >> StopWhenPrintEveryMoveToMateOnChessBoardWhenPrintingStatistics;

		PrintOnlyIfChosenMovesPathsToMateIsSmallerThanThisNumber = 11;
		PrintLookingForEqualMoveFromChosenMovesPaths = true;
		ChessEngineProjectConfigFile >> PrintToMovesFile;

		ModuloToWriteMove = 10000000;

		ChessEngineProjectConfigFile >> WaitForKeyOnTheEnd;

		ChessEngineTest ChessTestObject;

		ChessTestObject.TestId = 1;
		ChessTestObject.ExecuteTestBool = true;
		ChessEngineProjectConfigFile >> ChessTestObject.ChessBoardFileName;
		ChessTestObject.MaxDeepLevel = 5;
		ChessEngineProjectConfigFile >> ChessTestObject.ReverseColorOfPieces;
		ChessTestObject.WriteTestStatisticsToFile = true;
		ChessTestObject.PrintTestComparisonResult = true;
		ChessTestObject.NumberOfAllMoves = 355386817;
		ChessTestObject.NumberOfAllWhitePiecesMoves = 18057358;
		ChessTestObject.NumberOfAllBlackPiecesMoves = 337329459;
		ChessTestObject.NumberOfAllWhiteMovesWhenCheckedKing = 35530;
		ChessTestObject.NumberOfAllBlackMovesWhenCheckedKing = 3581321;
		ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfWhite = 475193;
		ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfBlack = 14163507;
		ChessTestObject.NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing = 1996240;
		ChessTestObject.NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing = 131364094;
		ChessTestObject.NumberOfAllFoundMovesPathsToMateWhite = 2;
		ChessTestObject.NumberOfAllFoundMovesPathsToMateBlack = 184586;
		ChessTestObject.NumberOfAllFoundMovesPathsToStalemate = 162;
		ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite = 46;
		ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack = -111;
		ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfWhite = -40;
		ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfBlack = 0;

		ChessTests.push_back(ChessTestObject);
	}
	CATCH("chess print configuration constructor")
}

void ChessEngineConfigurationFileReaderWriter::SaveTestStatisticsToFile(const ChessEngineTest& ChessTestObject) const
{
	try
	{
	}
	CATCH("chess print configuration constructor")
}
