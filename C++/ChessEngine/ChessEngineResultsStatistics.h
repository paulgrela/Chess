#pragma once

#ifndef _CHESS_ENGINE_RESULTS_STATISTICS_H_
#define _CHESS_ENGINE_RESULTS_STATISTICS_H_

#include <vector>
#include <memory>

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

#include "ChessEngineBestMoveFinder.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

class ChessEngineAllPossibleMovesGenerator;

class ChessEngineResultsStatistics
{
    friend class ChessEngineBestMoveFinder;
private:
	ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorLocalReference;
public:
	explicit ChessEngineResultsStatistics(ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorLocalReferenceParameter) : ChessEngineAllPossibleMovesGeneratorLocalReference(ChessEngineAllPossibleMovesGeneratorLocalReferenceParameter)
	{
	}
public:
    std::uint64_t NumberOfExecutionsOnLevel[MaximalLengthOfFoundPath];
public:
	std::uint64_t NumberOfAllRecursiveMoves = 0;
public:
	std::uint64_t NumberOfAllPiecesMovesByColor[NumberOfColors] = { 0, 0 };
	std::uint64_t NumberOfAllMovesWhenCheckedKingColor[NumberOfColors] = { 0, 0 };
	std::uint64_t NumberOfAllOppositePiecesRemovedByMoveOfColor[NumberOfColors] = { 0, 0 };
	std::uint64_t NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[NumberOfColors] = { 0, 0 };
public:
    ChessScoredMovesPath TheHighestScoredFoundCaptureMovesPathForAdvantageOf[NumberOfColors];
    ChessScoredMovesPath TheHighestScoredFoundMateMovesPathForAdvantageOf[NumberOfColors];
    ChessScoredMovesPath TheHighestScoredFoundStalemateMovesPath;
public:
	std::vector<ChessScoredMovesPath> AllFoundMovesPathsToMate[NumberOfColors];
	std::vector<ChessScoredMovesPath> AllFoundMovesPathsToStalemate;
public:
    ChessScoredMovesPath TheChosenMovesPathForAdvantageOf[NumberOfColors];
public:
	[[nodiscard]] const std::uint64_t GetNumberOfAllMovesTotal() const;
public:
	void PrintResultsAndStatistics();
private:
	void PrintGeneralStatistics() const;
    void PrintDetailedStatistics() const;
	void PrintMovesPath(const char* TaskStr, const PosType Color1, const PosType Color2, const ChessScoredMovesPath& ChessScoredMovesPathObject, const bool PrintTheHighestScoredMovesPathWhenPrintingStatistics, const bool PrintTheHighestScoredMovesPathOnChessBoardWhenPrintingStatistics);
	void PrintAllFoundMovesPaths();
	void PrintAllFoundMovesToMatePaths(const PosType Color);
	void PrintEveryMoveOnChessBoard(const ChessScoredMovesPath& OnePathToMate);
private:
	static void CompareTestStatisticsSingleData(const std::int64_t DataFromFile, const std::int64_t DataFromTest, const char* ErrorText, bool& TestOK);
public:
	void CompareTestStatistics(const ChessEngineTest& ChessTestObject) const;
	void SaveTestStatisticsToFile(const ChessEngineTest& ChessTestObjectParameter, const std::uint64_t TestId) const;
public:
	void ClearAllStatistics();
public:
	static void SumStatisticsFromTwoChessEngineResultsStatisticsObjects(ChessEngineResultsStatistics& ChessEngineResultsStatisticsObjectMain, const ChessEngineResultsStatistics& ChessEngineResultsStatisticsObjectToAdd);
};

#endif
