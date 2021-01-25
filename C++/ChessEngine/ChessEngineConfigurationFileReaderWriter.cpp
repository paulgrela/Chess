
#include <tuple>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ChessEngineConstants.h"
#include "ExceptionsMacro.h"

#include "ChessEngineConfigurationFileReaderWriter.h"

using namespace std;

void ChessEngineConfigurationFileReaderWriter::ReadChessConfigurationFile(const char* ConfigFileNameParameter)
{
	try
	{
		using boost::property_tree::ptree;

		ptree MainConfigPropertyTree;

        this->ConfigFileName = ConfigFileNameParameter;

		read_xml(ConfigFileName, MainConfigPropertyTree, boost::property_tree::xml_parser::trim_whitespace);

		for (const ptree::value_type& MainConfigPropertyTreeElement : MainConfigPropertyTree.get_child("Settings"))
		{
			if (MainConfigPropertyTreeElement.first == "Algorithm")
			{
				MultiThreaded = MainConfigPropertyTreeElement.second.get<bool>("MultiThreaded");
				SetProcessPriorityHighest = MainConfigPropertyTreeElement.second.get<bool>("SetProcessPriorityHighest");

				ReadPreviousMovePathsFromFile = MainConfigPropertyTreeElement.second.get<bool>("ReadPreviousMovePathsFromFile");

				CompareTestStatistics = MainConfigPropertyTreeElement.second.get<bool>("CompareTestStatistics");
				WriteTestsStatisticsToFile = MainConfigPropertyTreeElement.second.get<bool>("WriteTestsStatisticsToFile");

                MaxNumberOfChessBoardsForFastExecution = MainConfigPropertyTreeElement.second.get<uint64_t>("MaxNumberOfChessBoardsForFastExecution");
			}
			else
            if (MainConfigPropertyTreeElement.first == "ChessBoardColors")
            {
                Color1 = MainConfigPropertyTreeElement.second.get<uint8_t>("Color1");
                Color2 = MainConfigPropertyTreeElement.second.get<uint8_t>("Color2");
                Color3 = MainConfigPropertyTreeElement.second.get<uint8_t>("Color3");
                Color4 = MainConfigPropertyTreeElement.second.get<uint8_t>("Color4");
            }
            else
            if (MainConfigPropertyTreeElement.first == "ChessBoardPiecesSymbols")
            {
                PieceString[QueenNum[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteQueenSymbol");
                PieceString[Bishop1Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteBishop1Symbol");
                PieceString[Bishop2Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteBishop2Symbol");
                PieceString[Rook1Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteRook1Symbol");
                PieceString[Rook2Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteRook2Symbol");
                PieceString[Knight1Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteKnight1Symbol");
                PieceString[Knight2Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteKnight2Symbol");
                PieceString[Pawn1Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn1Symbol");
                PieceString[Pawn1Num[White] + 1] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn2Symbol");
                PieceString[Pawn1Num[White] + 2] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn3Symbol");
                PieceString[Pawn1Num[White] + 3] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn4Symbol");
                PieceString[Pawn1Num[White] + 4] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn5Symbol");
                PieceString[Pawn1Num[White] + 5] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn6Symbol");
                PieceString[Pawn1Num[White] + 6] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn7Symbol");
                PieceString[Pawn8Num[White]] = MainConfigPropertyTreeElement.second.get<string>("WhitePawn8Symbol");
                PieceString[KingNum[White]] = MainConfigPropertyTreeElement.second.get<string>("WhiteKingSymbol");
                PieceString[QueenNum[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackQueenSymbol");
                PieceString[Bishop1Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackBishop1Symbol");
                PieceString[Bishop2Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackBishop2Symbol");
                PieceString[Rook1Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackRook1Symbol");
                PieceString[Rook2Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackRook2Symbol");
                PieceString[Knight1Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackKnight1Symbol");
                PieceString[Knight2Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackKnight2Symbol");
                PieceString[Pawn1Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn1Symbol");
                PieceString[Pawn1Num[Black] + 1] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn2Symbol");
                PieceString[Pawn1Num[Black] + 2] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn3Symbol");
                PieceString[Pawn1Num[Black] + 3] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn4Symbol");
                PieceString[Pawn1Num[Black] + 4] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn5Symbol");
                PieceString[Pawn1Num[Black] + 5] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn6Symbol");
                PieceString[Pawn1Num[Black] + 6] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn7Symbol");
                PieceString[Pawn8Num[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackPawn8Symbol");
                PieceString[KingNum[Black]] = MainConfigPropertyTreeElement.second.get<string>("BlackKingSymbol");
            }
            else
            if (MainConfigPropertyTreeElement.first == "ChessBoardPiecesPowers")
            {
                PiecePower[QueenNum[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteQueenPower");
                PiecePower[Bishop1Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteBishop1Power");
                PiecePower[Bishop2Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteBishop2Power");
                PiecePower[Rook1Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteRook1Power");
                PiecePower[Rook2Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteRook2Power");
                PiecePower[Knight1Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteKnight1Power");
                PiecePower[Knight2Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteKnight2Power");
                PiecePower[Pawn1Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn1Power");
                PiecePower[Pawn1Num[White] + 1] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn2Power");
                PiecePower[Pawn1Num[White] + 2] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn3Power");
                PiecePower[Pawn1Num[White] + 3] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn4Power");
                PiecePower[Pawn1Num[White] + 4] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn5Power");
                PiecePower[Pawn1Num[White] + 5] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn6Power");
                PiecePower[Pawn1Num[White] + 6] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn7Power");
                PiecePower[Pawn8Num[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhitePawn8Power");
                PiecePower[KingNum[White]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("WhiteKingPower");
                PiecePower[QueenNum[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackQueenPower");
                PiecePower[Bishop1Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackBishop1Power");
                PiecePower[Bishop2Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackBishop2Power");
                PiecePower[Rook1Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackRook1Power");
                PiecePower[Rook2Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackRook2Power");
                PiecePower[Knight1Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackKnight1Power");
                PiecePower[Knight2Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackKnight2Power");
                PiecePower[Pawn1Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn1Power");
                PiecePower[Pawn1Num[Black] + 1] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn2Power");
                PiecePower[Pawn1Num[Black] + 2] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn3Power");
                PiecePower[Pawn1Num[Black] + 3] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn4Power");
                PiecePower[Pawn1Num[Black] + 4] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn5Power");
                PiecePower[Pawn1Num[Black] + 5] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn6Power");
                PiecePower[Pawn1Num[Black] + 6] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn7Power");
                PiecePower[Pawn8Num[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackPawn8Power");
                PiecePower[KingNum[Black]] = MainConfigPropertyTreeElement.second.get<PathScoreType>("BlackKingPower");
            }
            else
			if (MainConfigPropertyTreeElement.first == "Logger")
			{
				PrintLogToConsole = MainConfigPropertyTreeElement.second.get<bool>("PrintToConsole");
				PrintLogToFiles = MainConfigPropertyTreeElement.second.get<bool>("PrintToFiles");

				PrintLogLineNumberToConsole = MainConfigPropertyTreeElement.second.get<bool>("PrintLineNumberToConsole");				
				PrintLogDateTimeToConsole = MainConfigPropertyTreeElement.second.get<bool>("PrintDateTimeToConsole");
				PrintLogProcessIdToConsole = MainConfigPropertyTreeElement.second.get<bool>("PrintProcessIdToConsole");
				PrintLogProcessPriorityLevelToConsole = MainConfigPropertyTreeElement.second.get<bool>("PrintProcessPriorityLevelToConsole");
				PrintLogThreadIdToConsole = MainConfigPropertyTreeElement.second.get<bool>("PrintThreadIdToConsole");

				PrintLogLineNumberToFile = MainConfigPropertyTreeElement.second.get<bool>("PrintLineNumberToFile");
				PrintLogDateTimeToFile = MainConfigPropertyTreeElement.second.get<bool>("PrintDateTimeToFile");
				PrintLogProcessIdToFile = MainConfigPropertyTreeElement.second.get<bool>("PrintProcessIdToFile");
				PrintLogProcessPriorityLevelToFile = MainConfigPropertyTreeElement.second.get<bool>("PrintProcessPriorityLevelToFile");
				PrintLogThreadIdToFile = MainConfigPropertyTreeElement.second.get<bool>("PrintThreadIdToFile");

				MaximalNumberOfLinesInOneFile = MainConfigPropertyTreeElement.second.get<uint64_t>("MaximalNumberOfLinesInOneFile");
			}
			else
			if (MainConfigPropertyTreeElement.first == "PrintConditions")
			{
                PrintMPIProcessesInformationData = MainConfigPropertyTreeElement.second.get<bool>("PrintMPIProcessesInformationData");
				PrintActualStartPositionOfPieces = MainConfigPropertyTreeElement.second.get<bool>("PrintActualStartPositionOfPieces");
				StopWhenMove = MainConfigPropertyTreeElement.second.get<bool>("StopWhenMove");
				StopWhenMateFound = MainConfigPropertyTreeElement.second.get<bool>("StopWhenMateFound");

				PrintActualMove = MainConfigPropertyTreeElement.second.get<bool>("PrintActualMove");

				PrintActualChessBoardState = MainConfigPropertyTreeElement.second.get<bool>("PrintActualChessBoardState");
				PrintActualStateWhenMateFound = MainConfigPropertyTreeElement.second.get<bool>("PrintActualStateWhenMateFound");
				CleanScreenBeforePrintingActualState = MainConfigPropertyTreeElement.second.get<bool>("CleanScreenBeforePrintingActualState");

				PrintPossibleMoveFalseState = MainConfigPropertyTreeElement.second.get<bool>("PrintPossibleMoveFalseState");
				StopWhenPossibleMoveFalseState = MainConfigPropertyTreeElement.second.get<bool>("StopWhenPossibleMoveFalseState");
				PrintDetailedMoveDescription = MainConfigPropertyTreeElement.second.get<bool>("PrintDetailedMoveDescription");
				PrintGeneralInterMoveInfo = MainConfigPropertyTreeElement.second.get<bool>("PrintGeneralInterMoveInfo");
				PrintChessBoardStateForCheck = MainConfigPropertyTreeElement.second.get<bool>("PrintChessBoardStateForCheck");
				PrintWhenRemovingPiece = MainConfigPropertyTreeElement.second.get<bool>("PrintWhenRemovingPiece");
				StopWhenRemovingPiece = MainConfigPropertyTreeElement.second.get<bool>("StopWhenRemovingPiece");

				PrintTheHighestScoredMovesPathToCaptureWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintTheHighestScoredMovesPathToCaptureWhenPrintingStatistics");
				PrintTheHighestScoredMovesPathToCaptureOnChessBoardWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintTheHighestScoredMovesPathToCaptureOnChessBoardWhenPrintingStatistics");
				PrintTheHighestScoredMovesPathToMateWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintTheHighestScoredMovesPathToMateWhenPrintingStatistics");
				PrintTheHighestScoredMovesPathToMateOnChessBoardWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintTheHighestScoredMovesPathToMateOnChessBoardWhenPrintingStatistics");
				PrintEveryPathToMateWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintEveryPathToMateWhenPrintingStatistics");
				PrintEveryMoveToMateOnChessBoardWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintEveryMoveToMateOnChessBoardWhenPrintingStatistics");
				StopWhenPrintEveryMoveToMateOnChessBoardWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("StopWhenPrintEveryMoveToMateOnChessBoardWhenPrintingStatistics");
				PrintChosenMovesPathWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintChosenMovesPathWhenPrintingStatistics");
				PrintChosenMovesPathOnChessBoardWhenPrintingStatistics = MainConfigPropertyTreeElement.second.get<bool>("PrintChosenMovesPathOnChessBoardWhenPrintingStatistics");

                PrintOnlyIfChosenMovesPathsToMateIsSmallerThanThisNumber = MainConfigPropertyTreeElement.second.get<uint64_t>("PrintOnlyIfChosenMovesPathsToMateIsSmallerThanThisNumber");
                PrintLookingForEqualMoveFromChosenMovesPaths = MainConfigPropertyTreeElement.second.get<bool>("PrintLookingForEqualMoveFromChosenMovesPaths");
                PrintToMovesFile = MainConfigPropertyTreeElement.second.get<bool>("PrintToMovesFile");

				ModuloToWriteMove = MainConfigPropertyTreeElement.second.get<uint64_t>("ModuloToWriteMove");

				WaitForKeyOnTheEnd = MainConfigPropertyTreeElement.second.get<bool>("WaitForKeyOnTheEnd");
			}
			else
			if (MainConfigPropertyTreeElement.first == "Tests")
				for (const ptree::value_type& TestPropertyTreeElement : MainConfigPropertyTree.get_child("Settings.Tests"))
				{
					ChessEngineTest ChessTestObject;

					ChessTestObject.TestId = TestPropertyTreeElement.second.get<uint64_t>("<xmlattr>.id");
					ChessTestObject.ExecuteTestBool = TestPropertyTreeElement.second.get<bool>("ExecuteTestBool");
					ChessTestObject.ChessBoardFileName = TestPropertyTreeElement.second.get<string>("ChessBoardFileName");
					ChessTestObject.MaxDeepLevel = TestPropertyTreeElement.second.get<uint64_t>("MaxDeepLevel");
					ChessTestObject.ReverseColorOfPieces = TestPropertyTreeElement.second.get<bool>("ReverseColorOfPieces");
					ChessTestObject.WriteTestStatisticsToFile = TestPropertyTreeElement.second.get<bool>("WriteTestStatisticsToFile");
					ChessTestObject.PrintTestComparisonResult = TestPropertyTreeElement.second.get<bool>("PrintTestComparisonResult");
					ChessTestObject.NumberOfAllMoves = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllMoves");
					ChessTestObject.NumberOfAllWhitePiecesMoves = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllWhitePiecesMoves");
					ChessTestObject.NumberOfAllBlackPiecesMoves = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllBlackPiecesMoves");
					ChessTestObject.NumberOfAllWhiteMovesWhenCheckedKing = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllWhiteMovesWhenCheckedKing");
					ChessTestObject.NumberOfAllBlackMovesWhenCheckedKing = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllBlackMovesWhenCheckedKing");
					ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfWhite = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllOppositePiecesRemovedByMoveOfWhite");
					ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfBlack = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllOppositePiecesRemovedByMoveOfBlack");
					ChessTestObject.NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing");
					ChessTestObject.NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing");
					ChessTestObject.NumberOfAllFoundMovesPathsToMateWhite = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllFoundMovesPathsToMateWhite");
					ChessTestObject.NumberOfAllFoundMovesPathsToMateBlack = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllFoundMovesPathsToMateBlack");
					ChessTestObject.NumberOfAllFoundMovesPathsToStalemate = TestPropertyTreeElement.second.get<uint64_t>("NumberOfAllFoundMovesPathsToStalemate");
					ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite = TestPropertyTreeElement.second.get<int64_t>("TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite");
					ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack = TestPropertyTreeElement.second.get<int64_t>("TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack");
					ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfWhite = TestPropertyTreeElement.second.get<int64_t>("TheHighestFoundMateMovesPathScoreForAdvantageOfWhite");
					ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfBlack = TestPropertyTreeElement.second.get<int64_t>("TheHighestFoundMateMovesPathScoreForAdvantageOfBlack");

					ChessTests.push_back(ChessTestObject);					
				}
		}
	}
	CATCH("chess print configuration constructor")
}

void ChessEngineConfigurationFileReaderWriter::SaveTestStatisticsToFile(const ChessEngineTest& ChessTestObject) const
{
	try
	{
		using boost::property_tree::ptree;

		ptree MainConfigPropertyTree;

		read_xml(this->ConfigFileName, MainConfigPropertyTree, boost::property_tree::xml_parser::trim_whitespace);

		for (ptree::value_type& TestPropertyTreeElement : MainConfigPropertyTree.get_child("Settings.Tests"))
		{
			if (ChessTestObject.TestId == TestPropertyTreeElement.second.get<uint64_t>("<xmlattr>.id"))
			{
				ptree& TestPropertyTreeElementToWriteInFile = TestPropertyTreeElement.second;
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllMoves", ChessTestObject.NumberOfAllMoves);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllWhitePiecesMoves", ChessTestObject.NumberOfAllWhitePiecesMoves);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllBlackPiecesMoves", ChessTestObject.NumberOfAllBlackPiecesMoves);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllWhiteMovesWhenCheckedKing", ChessTestObject.NumberOfAllWhiteMovesWhenCheckedKing);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllBlackMovesWhenCheckedKing", ChessTestObject.NumberOfAllBlackMovesWhenCheckedKing);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllOppositePiecesRemovedByMoveOfWhite", ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfWhite);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllOppositePiecesRemovedByMoveOfBlack", ChessTestObject.NumberOfAllOppositePiecesRemovedByMoveOfBlack);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing", ChessTestObject.NumberOfAllWhiteMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing", ChessTestObject.NumberOfAllBlackMovesCancelledBecauseOfDiscoveredPossibleCheckOfKing);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllFoundMovesPathsToMateWhite", ChessTestObject.NumberOfAllFoundMovesPathsToMateWhite);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllFoundMovesPathsToMateBlack", ChessTestObject.NumberOfAllFoundMovesPathsToMateBlack);
				TestPropertyTreeElementToWriteInFile.put("NumberOfAllFoundMovesPathsToStalemate", ChessTestObject.NumberOfAllFoundMovesPathsToStalemate);
				TestPropertyTreeElementToWriteInFile.put("TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite", ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfWhite);
				TestPropertyTreeElementToWriteInFile.put("TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack", ChessTestObject.TheHighestFoundCaptureMovesPathScoreForAdvantageOfBlack);
				TestPropertyTreeElementToWriteInFile.put("TheHighestFoundMateMovesPathScoreForAdvantageOfWhite", ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfWhite);
				TestPropertyTreeElementToWriteInFile.put("TheHighestFoundMateMovesPathScoreForAdvantageOfBlack", ChessTestObject.TheHighestFoundMateMovesPathScoreForAdvantageOfBlack);
			}
		}

		std::ofstream OutputConfigFile(this->ConfigFileName);
		write_xml(OutputConfigFile, MainConfigPropertyTree, boost::property_tree::xml_writer_make_settings<string>('\t', 1));
	}
	CATCH("chess print configuration constructor")
}
