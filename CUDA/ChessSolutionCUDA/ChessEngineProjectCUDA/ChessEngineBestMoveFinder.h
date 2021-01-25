
#pragma once

#ifndef _CHESS_ENGINE_BEST_MOVE_FINDER_H_
#define _CHESS_ENGINE_BEST_MOVE_FINDER_H_

#include <vector>

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

#include "ChessEngineConfigurationFileReaderWriter.h"

class ChessEngineResultsStatistics;

class ChessEngineBestMoveFinder
{
private:
    ChessEngineResultsStatistics& ChessEngineResultsStatisticsLocalReference;
public:
    explicit ChessEngineBestMoveFinder(ChessEngineResultsStatistics& ChessEngineResultsStatisticsLocalReferenceParameter) : ChessEngineResultsStatisticsLocalReference(ChessEngineResultsStatisticsLocalReferenceParameter)
    {
    }
private:
    static void WriteEmptyTextToChosenMovesPathsTypeFile(std::ofstream& FileToWriteText);
    static void WriteGameResultToFiles(const char* TextToLog, const char* GameResult);
    static void ReadChosenMovesPathsFromFile(std::vector<std::vector<std::string>>& LastChosenMovesPathsToMate);
    static void SaveMovesPathToFile(const ChessScoredMovesPath& ChessScoredMovesPathToSave, std::ofstream& FileToWriteText);
private:
    void SaveChosenMovesPathsToFile(const char* TypeStr) const;
    void PrintLookingForEqualMoveFromChosenMovesPaths(const std::vector<std::vector<std::string>>& LastChosenMovesPathsToMate, const std::string& LastEngineMoveFrom, const std::string& LastEngineMoveTo, const std::string& LastOpponentMoveFrom, const std::string& LastOpponentMoveTo) const;
    bool ReadWriteChosenMovesPathsFromFileAndToFile(const char* TypeStr) const;
    void SaveChosenMovesPath(const char* TextForColorOppositeStr, const char* TextForColorStr, const char* MoveTypeStr, ColorType Color, ChessScoredMovesPath& ChosenChessScoredMovesPath);
    void FindRandomFirstPossibleMove(ColorType Color);
public:
    void ChooseBestMove();
};

#endif