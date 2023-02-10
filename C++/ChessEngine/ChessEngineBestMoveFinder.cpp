#include "DestinationPlatform.h"

#ifdef UNIX_PLATFORM
#include <string.h>
#endif

#include <sstream>

#include "ExceptionsMacro.h"

#include "Logger.h"
#include "FileUtils.h"
#include "StringUtils.h"

#include "ChessEngineChessMoveBasicOperations.h"
#include "ChessEngineAllPossibleMovesGenerator.h"

#include "ChessEngineBestMoveFinder.h"

using namespace std;

bool operator==(const ChessMove& CM1, const ChessMove& CM2)
{
    if ((CM1.PrevPosX == CM2.PrevPosX) && (CM1.PrevPosY == CM2.PrevPosY) && (CM1.NextPosX == CM2.NextPosX) && (CM1.NextPosY == CM2.NextPosY))
        return true;
    else
        return false;
}

void ChessEngineBestMoveFinder::WriteEmptyTextToChosenMovesPathsTypeFile(ofstream& FileToWriteText)
{
    try
    {
        FileToWriteText << "EMPTY" << endl;
        FileUtils::RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths_type.txt", "EMPTY");
    }
    CATCH_AND_THROW("writing empty text to chosen moves paths type file");
}

void ChessEngineBestMoveFinder::WriteGameResultToFiles(const char* TextToLog, const char* GameResult)
{
    using namespace FileUtils;

    try
    {
        LoggersManagerObject.Log(STREAM(TextToLog));
        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_actual_move.txt", GameResult);

        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths.txt", "EMPTY");
        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths_type.txt", "EMPTY");
    }
    CATCH("writing game result to files");
}

void ChessEngineBestMoveFinder::SaveMovesPathToFile(const ChessScoredMovesPath& ChessScoredMovesPathToSave, ofstream& FileToWriteText)
{
    try
    {
        for (uint64_t ChessMoveIndex = 0; ChessMoveIndex < ChessScoredMovesPathToSave.MovesPathSize; ChessMoveIndex++)
        {
            ChessMove ChosenChessMove = ChessScoredMovesPathToSave.MovesPath[ChessMoveIndex];
            FileToWriteText << " " << GetChessMoveAsString(ChosenChessMove.PrevPosX, ChosenChessMove.PrevPosY, ChosenChessMove.NextPosX, ChosenChessMove.NextPosY);
        }
        FileToWriteText << " " << ChessScoredMovesPathToSave.Score << " " << endl;
    }
    CATCH("saving chosen moves path to file");
}

void ChessEngineBestMoveFinder::ReadChosenMovesPathsFromFile(vector<vector<string>>& LastChosenMovesPathsToMate)
{
    try
    {
        ifstream FileToReadText;

        FileToReadText.open(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths.txt");

        LastChosenMovesPathsToMate.emplace_back(vector<string>());
        for (string s; getline(FileToReadText, s, SpaceChar);)
            if (s.empty() == false && s.front() == EndLineChar)
                LastChosenMovesPathsToMate.emplace_back(vector<string>());
            else
            if (s != "")
                LastChosenMovesPathsToMate.back().push_back(s);

        FileToReadText.close();
    }
    CATCH("reading chosen moves paths from file");
}

void ChessEngineBestMoveFinder::SaveChosenMovesPathsToFile(const char* TypeStr) const
{
    try
    {
        FileUtils::RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths_type.txt", TypeStr);

        ofstream FileToWriteText;

        FileToWriteText.open(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths.txt", ios_base::out | ios_base::trunc);

        if (string(TypeStr) == string("MATE"))
        {
            if (ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToMate[Black].size() > 0)
                for (const ChessScoredMovesPath& OnePathMovesToMate : ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToMate[Black])
                {
                    if (OnePathMovesToMate.MovesPath[0] == ChessEngineResultsStatisticsLocalReference.TheChosenMovesPathForAdvantageOf[White].MovesPath[0])
                        SaveMovesPathToFile(OnePathMovesToMate, FileToWriteText);
                }
            else
                WriteEmptyTextToChosenMovesPathsTypeFile(FileToWriteText);
        }
        else
        if (string(TypeStr) == string("CAPTURE"))
        {
            if (ChessEngineResultsStatisticsLocalReference.TheChosenMovesPathForAdvantageOf[White].MovesPathSize > 0)
                SaveMovesPathToFile(ChessEngineResultsStatisticsLocalReference.TheChosenMovesPathForAdvantageOf[White], FileToWriteText);
            else
                WriteEmptyTextToChosenMovesPathsTypeFile(FileToWriteText);
        }

        FileToWriteText.close();
    }
    CATCH("saving chosen moves paths to file");
}

void ChessEngineBestMoveFinder::PrintLookingForEqualMoveFromChosenMovesPaths(const vector<vector<string>>& LastChosenMovesPathsToMate, const string& LastEngineMoveFrom, const string& LastEngineMoveTo, const string& LastOpponentMoveFrom, const string& LastOpponentMoveTo)
{
    try
    {
        if (LastChosenMovesPathsToMate.size() < ChessEngineConfigurationFileReaderWriterObject.PrintOnlyIfChosenMovesPathsToMateIsSmallerThanThisNumber)
        {
            if (ChessEngineConfigurationFileReaderWriterObject.PrintLookingForEqualMoveFromChosenMovesPaths == true)
                for (const auto& LastChosenMovesPath : LastChosenMovesPathsToMate)
                    if (LastChosenMovesPath.size() > 0)
                    {
                        stringstream TextToLogLastChosenMovesPathAllStringStream("");
                        TextToLogLastChosenMovesPathAllStringStream << "LastChosenMovesPathAll = ";
                        for (const auto &LastChosenMovesPathPos : LastChosenMovesPath)
                            TextToLogLastChosenMovesPathAllStringStream << LastChosenMovesPathPos << " ";
                        LoggersManagerObject.Log(TextToLogLastChosenMovesPathAllStringStream);

                        if (LastEngineMoveFrom == LastChosenMovesPath[0] && LastEngineMoveTo == LastChosenMovesPath[1] && LastOpponentMoveFrom == LastChosenMovesPath[2] && LastOpponentMoveTo == LastChosenMovesPath[3])
                        {
                            LoggersManagerObject.Log(STREAM("EQUAL TO MOVE = " << LastChosenMovesPath[2] << " -> " << LastChosenMovesPath[3]));

                            stringstream TextToLogLastChosenMovesPathStringStream("");
                            TextToLogLastChosenMovesPathStringStream << "LastChosenMovesPath = ";
                            if (LastChosenMovesPath.size() - 4 > 0)
                            {
                                for (uint64_t Pos = 4; Pos < LastChosenMovesPath.size(); Pos++)
                                    TextToLogLastChosenMovesPathStringStream << to_string(Pos) << " " << LastChosenMovesPath[Pos];

                                LoggersManagerObject.Log(TextToLogLastChosenMovesPathStringStream);
                            }
                        }
                    }
        }
        else
            LoggersManagerObject.Log(STREAM("Chosen moves paths to mate to big to be printed!" << endl));
    }
    CATCH("printing for equal move from chosen moves paths")
}

bool ChessEngineBestMoveFinder::ReadWriteChosenMovesPathsFromFileAndToFile(const char* TypeStr) const
{
    using namespace FileUtils;

    try
    {
        if (ChessEngineConfigurationFileReaderWriterObject.PrintToMovesFile == true)
            if (ReadFirstLineFromFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths_type.txt") == TypeStr)
        {
            vector<vector<string>> LastChosenMovesPathsToMate;

            ReadChosenMovesPathsFromFile(LastChosenMovesPathsToMate);

            string LastEngineMove = ReadFirstLineFromFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_engine_move.txt");
            string LastOpponentMove = ReadFirstLineFromFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_actual_move.txt");
            if (LastOpponentMove != "MATE")
            {
                string LastEngineMoveFrom = string_utils::upper_case_str_transform(LastEngineMove.substr(3, 2));
                string LastEngineMoveTo = string_utils::upper_case_str_transform(LastEngineMove.substr(6, 2));

                string LastOpponentMoveFrom = string_utils::upper_case_str_transform(LastOpponentMove.substr(3, 2));
                string LastOpponentMoveTo = string_utils::upper_case_str_transform(LastOpponentMove.substr(6, 2));

                ofstream FileToWriteText;

                FileToWriteText.open(".." OS_DIR_SEP "workdir" OS_DIR_SEP "chosen_moves_paths.txt", ios_base::out | ios_base::trunc);

                PathScoreType MaxPathScore = 0;
                vector<vector<string>> NewLastChosenMovesPathsToMate;
                vector<string> TheHighestScoredLastChosenMovesPathToMate;

                PrintLookingForEqualMoveFromChosenMovesPaths(LastChosenMovesPathsToMate, LastEngineMoveFrom, LastEngineMoveTo, LastOpponentMoveFrom, LastOpponentMoveTo);

                for (const auto& LastChosenMovesPath : LastChosenMovesPathsToMate)
                    if (LastChosenMovesPath.size() > 0)
                        if (LastEngineMoveFrom == LastChosenMovesPath[0] && LastEngineMoveTo == LastChosenMovesPath[1] && LastOpponentMoveFrom == LastChosenMovesPath[2] && LastOpponentMoveTo == LastChosenMovesPath[3])
                        {
                            NewLastChosenMovesPathsToMate.push_back(LastChosenMovesPath);
                            if (LastChosenMovesPath.size() - 4 > 0)
                            {
                                for (uint64_t Pos = 4; Pos < LastChosenMovesPath.size(); Pos++)
                                    FileToWriteText << SpaceChar << LastChosenMovesPath[Pos];
                                FileToWriteText << SpaceChar << endl;
                            }
                            else
                                WriteEmptyTextToChosenMovesPathsTypeFile(FileToWriteText);

                            if (stoi(LastChosenMovesPath.back()) > MaxPathScore)
                            {
                                MaxPathScore = stoi(LastChosenMovesPath.back());
                                TheHighestScoredLastChosenMovesPathToMate = LastChosenMovesPath;
                            }
                        }
                FileToWriteText.close();

                if (TheHighestScoredLastChosenMovesPathToMate.empty() == false)
                    if (ChessEngineConfigurationFileReaderWriterObject.PrintToMovesFile == true)
                    {
                        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_actual_move.txt", "T0[" + TheHighestScoredLastChosenMovesPathToMate[4] + " " + TheHighestScoredLastChosenMovesPathToMate[5] + "]");
                        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_engine_move.txt", "T0[" + TheHighestScoredLastChosenMovesPathToMate[4] + " " + TheHighestScoredLastChosenMovesPathToMate[5] + "]");
                        LoggersManagerObject.Log(STREAM("CHOSEN MOVE TYPE 1 = " << TheHighestScoredLastChosenMovesPathToMate[4] << " " << TheHighestScoredLastChosenMovesPathToMate[5]));
                        return true;
                    }

                if (TheHighestScoredLastChosenMovesPathToMate.empty() == true && NewLastChosenMovesPathsToMate.size() > 0)
                    if (ChessEngineConfigurationFileReaderWriterObject.PrintToMovesFile == true)
                    {
                        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_actual_move.txt", "M0[" + NewLastChosenMovesPathsToMate[0][4] + " " + NewLastChosenMovesPathsToMate[0][5] + "]");
                        RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_engine_move.txt", "M0[" + NewLastChosenMovesPathsToMate[0][4] + " " + NewLastChosenMovesPathsToMate[0][5] + "]");
                        LoggersManagerObject.Log(STREAM("CHOSEN MOVE TYPE 2 = " << NewLastChosenMovesPathsToMate[0][4] << " " << NewLastChosenMovesPathsToMate[0][5]));
                        return true;
                    }
            }
        }
    }
    CATCH("reading writing chosen moves paths from file and to file");

    return false;
}

void ChessEngineBestMoveFinder::SaveChosenMovesPath(const char* TextForColorOppositeStr, const char* TextForColorStr, const char* MoveTypeStr, ColorType Color, ChessScoredMovesPath& ChosenChessScoredMovesPath)
{
    try
    {
        LoggersManagerObject.Log(STREAM(TextForColorOppositeStr << ColorName[1 - Color] << TextForColorStr << ColorName[Color]));
        ChessEngineResultsStatisticsLocalReference.TheChosenMovesPathForAdvantageOf[Color] = ChosenChessScoredMovesPath;
        SaveChosenMovesPathsToFile(MoveTypeStr);
    }
    CATCH("saving chosen moves path");
}

void ChessEngineBestMoveFinder::FindRandomFirstPossibleMove(ColorType Color)
{
    try
    {
        if (ChessEngineConfigurationFileReaderWriterObject.PrintToMovesFile == true)
        {
            ChessEngineConfigurationFileReaderWriterObject.MultiThreaded = false;
            ChessEngineConfigurationFileReaderWriterObject.FirstMove = true;
            ChessEngineConfigurationFileReaderWriterObject.MaxDepthLevel = 1;
            ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize = 0;
            ChessEngineThreadsStartData::FullSpeedExecutionInThreads = false;

            ChessEngineResultsStatisticsLocalReference.ChessEngineAllPossibleMovesGeneratorLocalReference.ChessEngineResultsStatisticsObject.ClearAllStatistics();
            ChessEngineResultsStatisticsLocalReference.ChessEngineAllPossibleMovesGeneratorLocalReference.ChessEngineDataInitializerObject.ClearDataForAllPossibleMovesGeneratorAlgorithm();
            ChessEngineResultsStatisticsLocalReference.ChessEngineAllPossibleMovesGeneratorLocalReference.ChessEngineDataInitializerObject.FindStartPositionsOfPieces();

            ChessEngineResultsStatisticsLocalReference.ChessEngineAllPossibleMovesGeneratorLocalReference.GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();

            FileUtils::RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_actual_move.txt", string_utils::trim_whitespace_surrounding(ChessEngineConfigurationFileReaderWriterObject.FirstPossibleMove));
            FileUtils::RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_engine_move.txt", string_utils::trim_whitespace_surrounding(ChessEngineConfigurationFileReaderWriterObject.FirstPossibleMove));
        }

        LoggersManagerObject.Log(STREAM("No moves to mate " << ColorName[1 - Color] << " => Random Best Move for color " << ColorName[Color] << " Move = " << ChessEngineConfigurationFileReaderWriterObject.FirstPossibleMove));
    }
    CATCH("finding random first possible move");
}

void ChessEngineBestMoveFinder::ChooseBestMove()
{
    try
    {
        LoggersManagerObject.Log(STREAM("Chosen Best Move:"));
        LoggersManagerObject.Log(STREAM(""));

        if (ReadWriteChosenMovesPathsFromFileAndToFile("MATE") == false)
        {
            if (ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToStalemate.size() == 1 && ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToStalemate[0].MovesPathSize == 0)
            {
                WriteGameResultToFiles("Chessboard stalemate on start", "STALEMATE");
                return;
            }

            if (ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToMate[White].size() == 1 && ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToMate[White][0].MovesPathSize == 0)
            {
                WriteGameResultToFiles("Chessboard mate on start so winning color is Black", "MATE");
                return;
            }

            ColorType ColorEnd = (ChessEngineConfigurationFileReaderWriterObject.ReadPreviousMovePathsFromFile == true ? White : Black);

            for (ColorType Color = White; Color <= ColorEnd; Color++)
                if (ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToMate[1 - Color].empty() == true)
                {
                    if (ReadWriteChosenMovesPathsFromFileAndToFile("CAPTURE") == false)
                    {
                        if (ChessEngineResultsStatisticsLocalReference.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Color].MovesPathSize > 0)
                            SaveChosenMovesPath("Chosen Moves Path only to capture ", " with capture so advantage is for color ", "CAPTURE", Color, ChessEngineResultsStatisticsLocalReference.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Color]);
                        else
                        {
                            FindRandomFirstPossibleMove(Color);
                            return;
                        }
                    }
                    else
                        return;
                }
                else
                {
                    if (ChessEngineResultsStatisticsLocalReference.TheHighestScoredFoundMateMovesPathForAdvantageOf[Color].MovesPathSize > 0)
                        SaveChosenMovesPath("Chosen Moves Path to mate ", " with capture so winning color is ", "MATE", Color, ChessEngineResultsStatisticsLocalReference.TheHighestScoredFoundMateMovesPathForAdvantageOf[Color]);
                    else
                        SaveChosenMovesPath("Chosen Moves Path to mate ", " without capture so winning color is ", "MATE", Color, ChessEngineResultsStatisticsLocalReference.AllFoundMovesPathsToMate[1 - Color][0]);
                }

            if (ChessEngineConfigurationFileReaderWriterObject.PrintToMovesFile == true)
            {
                FileUtils::RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_actual_move.txt", string_utils::trim_whitespace_surrounding(GetChessMovesPathAsString(vector(1, ChessEngineResultsStatisticsLocalReference.TheChosenMovesPathForAdvantageOf[White].MovesPath[0]))));
                FileUtils::RewriteTextToFile(".." OS_DIR_SEP "workdir" OS_DIR_SEP "last_engine_move.txt", string_utils::trim_whitespace_surrounding(GetChessMovesPathAsString(vector(1, ChessEngineResultsStatisticsLocalReference.TheChosenMovesPathForAdvantageOf[White].MovesPath[0]))));
            }
        }
    }
    CATCH("choosing best move");
}