
#include "DestinationPlatform.h"

#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif

#ifdef UNIX_PLATFORM
#include <memory>
#include <string.h>
#endif

#define USE_MPI

#ifdef USE_MPI
#include <mpi.h>
#endif

#include <chrono>
#include <sstream>

#include <algorithm>

#include "StringUtils.h"
#include "DateTimeUtils.h"
#include "ExceptionsMacro.h"

#include "ChessEngineChessboardFileReader.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

#include "ChessEngineMainThreadsManager.h"

#ifdef USE_MPI
#include "ChessEngineChessMoveBasicOperations.h"
#endif

using namespace std;

inline void ChessEngineMainThreadsManager::RegisterGenerateAllPossibleMovesForPieceFunctions()
{
	try
	{
        for (auto& GenerateAllPossibleMovesForPiecesObject : GenerateAllPossibleMovesForPiecesFunctors)
        {
            GenerateAllPossibleMovesForPiecesObject[KingNum[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKing<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
            GenerateAllPossibleMovesForPiecesObject[QueenNum[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForQueen<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

            GenerateAllPossibleMovesForPiecesObject[Rook1Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
            GenerateAllPossibleMovesForPiecesObject[Rook2Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForRook<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

            GenerateAllPossibleMovesForPiecesObject[Bishop1Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
            GenerateAllPossibleMovesForPiecesObject[Bishop2Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForBishop<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

            GenerateAllPossibleMovesForPiecesObject[Knight1Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
            GenerateAllPossibleMovesForPiecesObject[Knight2Num[White]] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForKnight<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;

            for (PieceNumType Pawn = Pawn1Num[White]; Pawn <= Pawn8Num[White]; Pawn++)
                GenerateAllPossibleMovesForPiecesObject[Pawn] = &ChessEngineAllPossibleMovesGenerator::GenerateAllPossibleMovesForPawn<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>;
        }
	}
	CATCH("registering generate all possible move for piece functions")
}

inline void ChessEngineMainThreadsManager::PrepareData(const string& ChessBoardFileName, const uint64_t TestId, const string& ActualDateTimeStr)
{
	try
	{
        const char* LogDirectory = "." OS_DIR_SEP;

        #ifdef USE_MPI
        if (MPIProcessIdentifier == 0)
        #endif
		LoggersManagerObject.InitializeLoggerManagerDataForTask(string("TEST_" + to_string(TestId)).c_str(), LogDirectory, ActualDateTimeStr, true, 0, function<void(const uint64_t& CurrentThreadId, const uint64_t FileNumber, const string& MessageStr)>());

        for (auto& ChessEngineAllPossibleMovesGeneratorObject : ChessEngineAllPossibleMovesGeneratorObjects)
        {
            ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.ClearAllStatistics();
            ChessEngineChessboardFileReader(ChessBoardFileName, &ChessEngineAllPossibleMovesGeneratorObject.ChessBoardStartData);
            ChessEngineAllPossibleMovesGeneratorObject.ChessEngineDataInitializerObject.ClearDataForAllPossibleMovesGeneratorAlgorithm();
            ChessEngineAllPossibleMovesGeneratorObject.ChessEngineDataInitializerObject.FindStartPositionsOfPieces();
        }

        RegisterGenerateAllPossibleMovesForPieceFunctions();

        #ifdef USE_MPI
        if (MPIProcessIdentifier == 0)
        {
        #endif
            LoggersManagerObject.Log(STREAM("CHESS GAME SITUATION:" << endl));
            ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineChessboardPrinterObject.LogPrintActualChessBoardStateToStreams(&LoggersManager::LogWithoutLineInfoOnlyToFiles, 1);
            ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineDataInitializerObject.PrintActualStateOfPieces();
            LoggersManagerObject.Log(STREAM(""));
        #ifdef USE_MPI
        }
        #endif
	}
	CATCH_AND_THROW("preparing data")
}

inline void ChessEngineMainThreadsManager::GenerateAllPossibleCombinationsOfMovesToFindChessBoardsForFullSpeedThreads()
{
    try
    {
        ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize = 0;
        ChessEngineThreadsStartData::FullSpeedExecutionInThreads = false;

        #ifdef USE_MPI
        ChessEngineConfigurationFileReaderWriterObject.MultiThreaded = false;
        #endif

        if (ChessEngineConfigurationFileReaderWriterObject.MultiThreaded == false)
            ChessEngineAllPossibleMovesGeneratorObjects[0].GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();
        else
        {
            const bool IsCheck = ChessEngineAllPossibleMovesGeneratorObjects[0].VerifyCheckOfKing<White, Black>();

            bool PossibleMove = false;

            ThreadNumType MaxThreadsForGet = 0;

            ChessPiece PiecesLocalCopy[NumberOfPieces];
            memcpy(PiecesLocalCopy, ChessEngineAllPossibleMovesGeneratorObjects[0].Pieces, (EndOfWhitePiecesNum + 1) * sizeof(ChessPiece));

            future<bool> ResultsFromGenerateAllPossibleMovesForThread[MaxNumberOfThreads];

            for (PieceNumType PieceNum = StartOfWhitePiecesNum; PieceNum <= EndOfWhitePiecesNum; PieceNum++)
                if (PiecesLocalCopy[PieceNum].Alive == true)
                {
                    ResultsFromGenerateAllPossibleMovesForThread[MaxThreadsForGet] = async(launch::async, GenerateAllPossibleMovesForPiecesFunctors[MaxThreadsForGet][PieceNum], &ChessEngineAllPossibleMovesGeneratorObjects[MaxThreadsForGet], PiecesLocalCopy[PieceNum].ActualPosX, PiecesLocalCopy[PieceNum].ActualPosY);
                    MaxThreadsForGet++;
                }

            for (ThreadNumType ThreadForGetPos = 0; ThreadForGetPos < MaxThreadsForGet; ThreadForGetPos++)
                if (ResultsFromGenerateAllPossibleMovesForThread[ThreadForGetPos].get() == true)
                    PossibleMove = true;

            ChessEngineAllPossibleMovesGeneratorObjects[0].UpdateDataForIsCheckAndPossibleMove<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>(IsCheck, PossibleMove);
        }
    }
    CATCH("generating all possible combinations of moves")
}

#ifndef USE_MPI
inline bool ChessEngineMainThreadsManager::CallListOfFunctionsFromVectorOfFunctors(const PosType StartChessBoardNum, const PosType EndChessBoardNum, const ThreadNumType ThreadPos) noexcept
{
    try
    {
        for (PosType ChessBoardNum = StartChessBoardNum; ChessBoardNum <= EndChessBoardNum; ChessBoardNum++)
        {
            ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].DeepLevel = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].DeepLevel;
            ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].IsAnyPawnPromoted = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].IsAnyPawnPromoted;

            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].ChessBoard, ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].ChessBoard, sizeof(PieceNumType) * MaxChessSizeX * MaxChessSizeY);
            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].Pieces, ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].Pieces, sizeof(ChessPiece) * NumberOfPieces);
            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].ActuallyInvestigatedMovesPath, ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].ActuallyInvestigatedMovesPath, sizeof(ChessMove) * ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].DeepLevel);

            ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();
        }
    }
    CATCH("calling list of functions from vector of functors")

    return true;
}

inline void ChessEngineMainThreadsManager::GenerateAllPossibleCombinationsOfMovesInFullSpeedThreads()
{
    try
    {
        ChessEngineThreadsStartData::FullSpeedExecutionInThreads = true;

        LoggersManagerObject.Log(STREAM("Thread Start Data Array Size = " << ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize << endl));

        ChessEngineAllPossibleMovesGeneratorObjects[0].VerifyCheckOfKing<White, Black>();

        const ThreadNumType RealNumberOfCPUCores = std::thread::hardware_concurrency();

        LoggersManagerObject.Log(STREAM("HARDWARE CORES ACCESSIBLE = " << RealNumberOfCPUCores << endl));

        future<bool> ResultsFromGenerateAllPossibleMovesForThread[MaxNumberOfThreads];

        LengthType ChessBoardsAveragePerThread = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize / RealNumberOfCPUCores + 1;

        LoggersManagerObject.Log(STREAM("ChessBoards Average Per Thread = " << ChessBoardsAveragePerThread << endl));

        for (ThreadNumType ThreadPos = 1; ThreadPos < MaxNumberOfThreads; ThreadPos++)
            ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].ChessEngineResultsStatisticsObject.ClearAllStatistics();

        for (ThreadNumType ThreadPos = 1; ThreadPos <= RealNumberOfCPUCores; ThreadPos++)
        {
            PosType StartChessBoardNum = (ThreadPos - 1) * ChessBoardsAveragePerThread;
            PosType EndChessBoardNum = (ThreadPos - 1) * ChessBoardsAveragePerThread + ChessBoardsAveragePerThread - 1 < ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize ? (ThreadPos - 1) * ChessBoardsAveragePerThread + ChessBoardsAveragePerThread - 1 : ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize - 1;

            LoggersManagerObject.Log(STREAM("StartChessBoardNum = " << StartChessBoardNum << " EndChessBoardNum = " << EndChessBoardNum));

            ResultsFromGenerateAllPossibleMovesForThread[ThreadPos] = async(launch::async, &ChessEngineMainThreadsManager::CallListOfFunctionsFromVectorOfFunctors, this, StartChessBoardNum, EndChessBoardNum, ThreadPos);
        }

        for (ThreadNumType ThreadForGetPos = 1; ThreadForGetPos <= RealNumberOfCPUCores; ThreadForGetPos++)
            ResultsFromGenerateAllPossibleMovesForThread[ThreadForGetPos].get();
    }
    CATCH("generating all possible combinations of moves")
}
#endif

#ifdef USE_MPI
inline void ChessEngineMainThreadsManager::LogLengthsOfFoundMovesPaths(int AllFoundMovesPathsToMateBlackLengthsMaximum, int AllFoundMovesPathsToMateWhiteLengthsMaximum, int AllFoundMovesPathsToStalemateLengthsMaximum, int* AllFoundMovesPathsToMateBlackLengths, int* AllFoundMovesPathsToMateWhiteLengths, int NumberOfBytesToGatherFromEveryProcessToMateBlack, int NumberOfBytesToGatherFromEveryProcessToMateWhite) const
{
    try
    {
        LoggersManagerObject.Log(STREAM("AllFoundMovesPathsToMateBlackLengthsMaximum = " << AllFoundMovesPathsToMateBlackLengthsMaximum));
        LoggersManagerObject.Log(STREAM("AllFoundMovesPathsToMateWhiteLengthsMaximum = " << AllFoundMovesPathsToMateWhiteLengthsMaximum));
        LoggersManagerObject.Log(STREAM("AllFoundMovesPathsToStalemateLengthsMaximum = " << AllFoundMovesPathsToStalemateLengthsMaximum));

        LoggersManagerObject.Log(STREAM(""));

        stringstream ListOfAllFoundMovesPathsToMateBlackLengthsFromEveryMPIProcess("AllFoundMovesPathsToMateBlackLengths = ");
        for (LengthType MPIProcessIndex = 0; MPIProcessIndex < NumberOfMPIProcesses; MPIProcessIndex++)
            ListOfAllFoundMovesPathsToMateBlackLengthsFromEveryMPIProcess << "P[" << MPIProcessIndex << "] = " << AllFoundMovesPathsToMateBlackLengths[MPIProcessIndex] << "; ";
        LoggersManagerObject.Log(ListOfAllFoundMovesPathsToMateBlackLengthsFromEveryMPIProcess);

        stringstream ListOfAllFoundMovesPathsToMateWhiteLengthsFromEveryMPIProcess("AllFoundMovesPathsToMateWhiteLengths = ");
        for (LengthType MPIProcessIndex = 0; MPIProcessIndex < NumberOfMPIProcesses; MPIProcessIndex++)
            ListOfAllFoundMovesPathsToMateWhiteLengthsFromEveryMPIProcess << "P[" << MPIProcessIndex << "] = " << AllFoundMovesPathsToMateWhiteLengths[MPIProcessIndex] << "; ";
        LoggersManagerObject.Log(ListOfAllFoundMovesPathsToMateWhiteLengthsFromEveryMPIProcess);

        LoggersManagerObject.Log(STREAM("NumberOfBytesToGatherFromEveryProcessToMateBlack = " << NumberOfBytesToGatherFromEveryProcessToMateBlack));
        LoggersManagerObject.Log(STREAM("NumberOfBytesToGatherFromEveryProcessToMateWhite = " << NumberOfBytesToGatherFromEveryProcessToMateWhite));
    }
    CATCH("logging lengths of found moves paths")
}

inline void ChessEngineMainThreadsManager::ReduceStatisticsResultsFromAllMPIProcesses()
{
    try
    {
        uint64_t NumberOfAllRecursiveMoves = 0;
        uint64_t NumberOfAllPiecesMovesByColorWhite = 0;
        uint64_t NumberOfAllPiecesMovesByColorBlack = 0;
        uint64_t NumberOfAllMovesWhenCheckedKingColorWhite = 0;
        uint64_t NumberOfAllMovesWhenCheckedKingColorBlack = 0;
        uint64_t NumberOfAllOppositePiecesRemovedByMoveOfColorWhite = 0;
        uint64_t NumberOfAllOppositePiecesRemovedByMoveOfColorBlack = 0;
        uint64_t NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColorWhite = 0;
        uint64_t NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColorBlack = 0;

        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllRecursiveMoves, &NumberOfAllRecursiveMoves, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllPiecesMovesByColor[White], &NumberOfAllPiecesMovesByColorWhite, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllPiecesMovesByColor[Black], &NumberOfAllPiecesMovesByColorBlack, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesWhenCheckedKingColor[White], &NumberOfAllMovesWhenCheckedKingColorWhite, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesWhenCheckedKingColor[Black], &NumberOfAllMovesWhenCheckedKingColorBlack, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllOppositePiecesRemovedByMoveOfColor[White], &NumberOfAllOppositePiecesRemovedByMoveOfColorWhite, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllOppositePiecesRemovedByMoveOfColor[Black], &NumberOfAllOppositePiecesRemovedByMoveOfColorBlack, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[White], &NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColorWhite, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Black], &NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColorBlack, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);

        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllRecursiveMoves = NumberOfAllRecursiveMoves;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllPiecesMovesByColor[White] = NumberOfAllPiecesMovesByColorWhite;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllPiecesMovesByColor[Black] = NumberOfAllPiecesMovesByColorBlack;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesWhenCheckedKingColor[White] = NumberOfAllMovesWhenCheckedKingColorWhite;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesWhenCheckedKingColor[Black] = NumberOfAllMovesWhenCheckedKingColorBlack;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllOppositePiecesRemovedByMoveOfColor[White] = NumberOfAllOppositePiecesRemovedByMoveOfColorWhite;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllOppositePiecesRemovedByMoveOfColor[Black] = NumberOfAllOppositePiecesRemovedByMoveOfColorBlack;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[White] = NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColorWhite;
        ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColor[Black] = NumberOfAllMovesCancelledBecauseOfDiscoveredPossibleCheckOfKingColorBlack;
    }
    CATCH("reducing statistics results from all mpi processes")
}

inline void ChessEngineMainThreadsManager::CopyChessScoredFoundCaptureMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(ChessScoredMovesPath* TheHighestScoredFoundCaptureMovesPathPointer, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreType x, const PathScoreType y)) const
{
    try
    {
        for (uint64_t MPIProcessIndex = 1; MPIProcessIndex < NumberOfMPIProcesses; MPIProcessIndex++)
            if (Comparison(TheHighestScoredFoundCaptureMovesPathPointer[MPIProcessIndex].Score, ChessScoredMovesPathObject.Score))
                memcpy(&ChessScoredMovesPathObject, &TheHighestScoredFoundCaptureMovesPathPointer[MPIProcessIndex], sizeof(ChessScoredMovesPath));
    }
    CATCH("copying chess scored found capture moves paths cuda to statistics found moves paths")
}

inline void ChessEngineMainThreadsManager::CopyChessScoredMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(const int AllFoundMovesPathsToMateLengthsMaximum, const int* const AllFoundMovesPathsToMateLengths, ChessScoredMovesPath* const AllFoundMovesPathsToMatePointer, std::vector<ChessScoredMovesPath>& AllFoundMovesPaths, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreType x, const PathScoreType y)) const
{
    try
    {
        AllFoundMovesPaths.reserve(AllFoundMovesPaths.size() + (NumberOfMPIProcesses * AllFoundMovesPathsToMateLengthsMaximum));
        for (uint64_t MPIProcessIndex = 1; MPIProcessIndex < NumberOfMPIProcesses; MPIProcessIndex++)
            for (uint64_t ChessPathIndex = AllFoundMovesPathsToMateLengthsMaximum * MPIProcessIdentifier; ChessPathIndex < AllFoundMovesPathsToMateLengthsMaximum * MPIProcessIdentifier + AllFoundMovesPathsToMateLengths[MPIProcessIndex]; ChessPathIndex++)
            {
                AllFoundMovesPaths.emplace_back(ChessScoredMovesPath{ AllFoundMovesPathsToMatePointer[ChessPathIndex].Score, AllFoundMovesPathsToMatePointer[ChessPathIndex].TypeSum, {}, AllFoundMovesPathsToMatePointer[ChessPathIndex].MovesPathSize });
                CopyChessMovesPathsVector(AllFoundMovesPathsToMatePointer[ChessPathIndex].MovesPathSize, AllFoundMovesPaths.back().MovesPath, AllFoundMovesPathsToMatePointer[ChessPathIndex].MovesPath);

                if (Comparison(AllFoundMovesPathsToMatePointer[ChessPathIndex].Score, ChessScoredMovesPathObject.Score) || (ChessScoredMovesPathObject.MovesPathSize == 0 && AllFoundMovesPathsToMatePointer[ChessPathIndex].MovesPathSize > 0))
                    memcpy(&ChessScoredMovesPathObject, &AllFoundMovesPathsToMatePointer[ChessPathIndex], sizeof(ChessScoredMovesPath));
            }
    }
    CATCH("copying chess scored moves paths to statistics found moves paths")
}

inline void ChessEngineMainThreadsManager::GatherFoundChessScoredMovesPathsResultsFromAllMPIProcesses()
{
    try
    {
        int AllFoundMovesPathsToMateWhiteSize = ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[White].size();
        int AllFoundMovesPathsToMateBlackSize = ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[Black].size();
        int AllFoundMovesPathsToStalemateSize = ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToStalemate.size();

        int AllFoundMovesPathsToMateBlackLengths[MaxNumberOfMPIProcesses];
        int AllFoundMovesPathsToMateWhiteLengths[MaxNumberOfMPIProcesses];
        int AllFoundMovesPathsToStalemateLengths[MaxNumberOfMPIProcesses];

        MPI_Gather(&AllFoundMovesPathsToMateBlackSize, 1, MPI_INT, AllFoundMovesPathsToMateBlackLengths, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(&AllFoundMovesPathsToMateWhiteSize, 1, MPI_INT, AllFoundMovesPathsToMateWhiteLengths, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(&AllFoundMovesPathsToStalemateSize, 1, MPI_INT, AllFoundMovesPathsToStalemateLengths, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int AllFoundMovesPathsToMateBlackLengthsMaximum;
        int AllFoundMovesPathsToMateWhiteLengthsMaximum;
        int AllFoundMovesPathsToStalemateLengthsMaximum;

        if (MPIProcessIdentifier == 0)
        {
            AllFoundMovesPathsToMateBlackLengthsMaximum = *max_element(AllFoundMovesPathsToMateBlackLengths, AllFoundMovesPathsToMateBlackLengths + NumberOfMPIProcesses);
            AllFoundMovesPathsToMateWhiteLengthsMaximum = *max_element(AllFoundMovesPathsToMateWhiteLengths, AllFoundMovesPathsToMateWhiteLengths + NumberOfMPIProcesses);
            AllFoundMovesPathsToStalemateLengthsMaximum = *max_element(AllFoundMovesPathsToStalemateLengths, AllFoundMovesPathsToStalemateLengths + NumberOfMPIProcesses);
        }

        MPI_Bcast(&AllFoundMovesPathsToMateBlackLengthsMaximum,1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&AllFoundMovesPathsToMateWhiteLengthsMaximum,1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&AllFoundMovesPathsToStalemateLengthsMaximum,1, MPI_INT, 0, MPI_COMM_WORLD);

        unique_ptr<ChessScoredMovesPath[]> TotalAllFoundMovesPathsToMateBlackPointer = make_unique<ChessScoredMovesPath[]>(AllFoundMovesPathsToMateBlackLengthsMaximum * NumberOfMPIProcesses + 1);
        unique_ptr<ChessScoredMovesPath[]> TotalAllFoundMovesPathsToMateWhitePointer = make_unique<ChessScoredMovesPath[]>(AllFoundMovesPathsToMateWhiteLengthsMaximum * NumberOfMPIProcesses + 1);
        unique_ptr<ChessScoredMovesPath[]> TotalAllFoundMovesPathsToStalematePointer = make_unique<ChessScoredMovesPath[]>(AllFoundMovesPathsToStalemateLengthsMaximum * NumberOfMPIProcesses + 1);
        unique_ptr<ChessScoredMovesPath[]> TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer = make_unique<ChessScoredMovesPath[]>(NumberOfMPIProcesses);
        unique_ptr<ChessScoredMovesPath[]> TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer = make_unique<ChessScoredMovesPath[]>(NumberOfMPIProcesses);

        int NumberOfBytesToGatherFromEveryProcessToMateBlack = static_cast<int>(AllFoundMovesPathsToMateBlackLengthsMaximum * sizeof(ChessScoredMovesPath));
        int NumberOfBytesToGatherFromEveryProcessToMateWhite = static_cast<int>(AllFoundMovesPathsToMateWhiteLengthsMaximum * sizeof(ChessScoredMovesPath));
        int NumberOfBytesToGatherFromEveryProcessToStalemate = static_cast<int>(AllFoundMovesPathsToStalemateLengthsMaximum * sizeof(ChessScoredMovesPath));

        MPI_Gather(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[Black].data(), NumberOfBytesToGatherFromEveryProcessToMateBlack, MPI_BYTE, TotalAllFoundMovesPathsToMateBlackPointer.get(), NumberOfBytesToGatherFromEveryProcessToMateBlack, MPI_BYTE, 0, MPI_COMM_WORLD);
        MPI_Gather(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[White].data(), NumberOfBytesToGatherFromEveryProcessToMateWhite, MPI_BYTE, TotalAllFoundMovesPathsToMateWhitePointer.get(), NumberOfBytesToGatherFromEveryProcessToMateWhite, MPI_BYTE, 0, MPI_COMM_WORLD);
        MPI_Gather(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToStalemate.data(), NumberOfBytesToGatherFromEveryProcessToStalemate, MPI_BYTE, TotalAllFoundMovesPathsToStalematePointer.get(), NumberOfBytesToGatherFromEveryProcessToStalemate, MPI_BYTE, 0, MPI_COMM_WORLD);
        MPI_Gather(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White], sizeof(ChessScoredMovesPath), MPI_BYTE, TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer.get(), sizeof(ChessScoredMovesPath), MPI_BYTE, 0, MPI_COMM_WORLD);
        MPI_Gather(&ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black], sizeof(ChessScoredMovesPath), MPI_BYTE, TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer.get(), sizeof(ChessScoredMovesPath), MPI_BYTE, 0, MPI_COMM_WORLD);

        if (MPIProcessIdentifier == 0)
        {
            CopyChessScoredMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(AllFoundMovesPathsToMateBlackLengthsMaximum, AllFoundMovesPathsToMateBlackLengths, TotalAllFoundMovesPathsToMateBlackPointer.get(), ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[Black],ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundMateMovesPathForAdvantageOf[White], [](const PathScoreType Score1, const PathScoreType Score2) { return Score1 < Score2; });
            CopyChessScoredMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(AllFoundMovesPathsToMateWhiteLengthsMaximum, AllFoundMovesPathsToMateWhiteLengths, TotalAllFoundMovesPathsToMateWhitePointer.get(), ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[White],ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black], [](const PathScoreType Score1, const PathScoreType Score2) { return Score1 > Score2; });
            CopyChessScoredMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(AllFoundMovesPathsToStalemateLengthsMaximum, AllFoundMovesPathsToStalemateLengths, TotalAllFoundMovesPathsToStalematePointer.get(), ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.AllFoundMovesPathsToStalemate,ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundStalemateMovesPath, [](const PathScoreType Score1, const PathScoreType Score2) { return Score1 < Score2; });
            CopyChessScoredFoundCaptureMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer.get(), ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White], [](const PathScoreType Score1, const PathScoreType Score2) { return Score1 > Score2; });
            CopyChessScoredFoundCaptureMovesPathsFromOtherMPIProcessesToStatisticsFoundMovesPathsOfMPIProcess0(TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer.get(), ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black], [](const PathScoreType Score1, const PathScoreType Score2) { return Score1 < Score2; });
        }

        if (ChessEngineConfigurationFileReaderWriterObject.PrintMPIProcessesInformationData == true and MPIProcessIdentifier == 0)
            LogLengthsOfFoundMovesPaths(AllFoundMovesPathsToMateBlackLengthsMaximum, AllFoundMovesPathsToMateWhiteLengthsMaximum, AllFoundMovesPathsToStalemateLengthsMaximum, AllFoundMovesPathsToMateBlackLengths, AllFoundMovesPathsToMateWhiteLengths, NumberOfBytesToGatherFromEveryProcessToMateBlack, NumberOfBytesToGatherFromEveryProcessToMateWhite);
    }
    CATCH("gathering chess scored moves paths results from all mpi processes")
}

inline void ChessEngineMainThreadsManager::GenerateAllPossibleCombinationsOfMovesInMPIProcesses()
{
    try
    {
        ChessEngineThreadsStartData::FullSpeedExecutionInThreads = true;

        if (MPIProcessIdentifier == 0)
            LoggersManagerObject.Log(STREAM("Thread Start Data Array Size = " << ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize << endl));

        ChessEngineAllPossibleMovesGeneratorObjects[0].VerifyCheckOfKing<White, Black>();

        if (MPIProcessIdentifier > 0)
            ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.ClearAllStatistics();

//        LengthType ChessBoardsAveragePerThread = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize / NumberOfMPIProcesses + 1;
//
//        PosType StartChessBoardNum = MPIProcessIdentifier * ChessBoardsAveragePerThread;
//        PosType EndChessBoardNum = MPIProcessIdentifier * ChessBoardsAveragePerThread + ChessBoardsAveragePerThread - 1 < ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize ? MPIProcessIdentifier * ChessBoardsAveragePerThread + ChessBoardsAveragePerThread - 1 : ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize - 1;
//
//        if (ChessEngineConfigurationFileReaderWriterObject.PrintMPIProcessesInformationData == true)
//            cout <<"MPI process identifier = " << MPIProcessIdentifier << " Number of MPI processes = " << NumberOfMPIProcesses << " StartChessBoardNum = " << StartChessBoardNum << " EndChessBoardNum = " << EndChessBoardNum << endl;

//        for (PosType ChessBoardNum = StartChessBoardNum; ChessBoardNum <= EndChessBoardNum; ChessBoardNum++)
//        {
//            ChessEngineAllPossibleMovesGeneratorObjects[0].DeepLevel = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].DeepLevel;
//            ChessEngineAllPossibleMovesGeneratorObjects[0].IsAnyPawnPromoted = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].IsAnyPawnPromoted;
//
//            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessBoard, ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].ChessBoard, sizeof(PieceNumType) * MaxChessSizeX * MaxChessSizeY);
//            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[0].Pieces, ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].Pieces, sizeof(ChessPiece) * NumberOfPieces);
//            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[0].ActuallyInvestigatedMovesPath, ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].ActuallyInvestigatedMovesPath, sizeof(ChessMove) * ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer[ChessBoardNum].DeepLevel);
//
//            ChessEngineAllPossibleMovesGeneratorObjects[0].GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();
//        }

        for (const auto& ChessBoardObject : ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer)
        {
            ChessEngineAllPossibleMovesGeneratorObjects[0].DeepLevel = ChessBoardObject.DeepLevel;
            ChessEngineAllPossibleMovesGeneratorObjects[0].IsAnyPawnPromoted = ChessBoardObject.IsAnyPawnPromoted;

            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessBoard, ChessBoardObject.ChessBoard, sizeof(PieceNumType) * MaxChessSizeX * MaxChessSizeY);
            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[0].Pieces, ChessBoardObject.Pieces, sizeof(ChessPiece) * NumberOfPieces);
            memcpy(ChessEngineAllPossibleMovesGeneratorObjects[0].ActuallyInvestigatedMovesPath, ChessBoardObject.ActuallyInvestigatedMovesPath, sizeof(ChessMove) * ChessBoardObject.DeepLevel);

            ChessEngineAllPossibleMovesGeneratorObjects[0].GenerateAllPossibleCombinationsOfMoves<White, Black, StartOfBlackPiecesNum, EndOfBlackPiecesNum>();
        }
    }
    CATCH("generating all possible combinations of moves in mpi processes")
}

#endif

inline void ChessEngineMainThreadsManager::SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads()
{
	try
	{
		for (ThreadNumType ThreadPos = 1; ThreadPos < MaxNumberOfThreads; ThreadPos++)
            ChessEngineResultsStatistics::SumStatisticsFromTwoChessEngineResultsStatisticsObjects(ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject, ChessEngineAllPossibleMovesGeneratorObjects[ThreadPos].ChessEngineResultsStatisticsObject);
	}
	CATCH("showing results")
}

inline void ChessEngineMainThreadsManager::PrintDurationTimeAllFormats(const chrono::high_resolution_clock::time_point start_time, const chrono::high_resolution_clock::time_point stop_time, const char* TextToPrint, const char* ExceptionTextToPrint)
{
	try
	{
		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.Log(STREAM(GetDurationTimeInOneLineStr(start_time, stop_time, TextToPrint, ExceptionTextToPrint)));

		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::nanoseconds>(stop_time - start_time).count() << " nanoseconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::microseconds>(stop_time - start_time).count() << " microseconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::milliseconds>(stop_time - start_time).count() << " milliseconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::seconds>(stop_time - start_time).count() << " seconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::minutes>(stop_time - start_time).count() << " minutes in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::hours>(stop_time - start_time).count() << " hours in total"));

		LoggersManagerObject.Log(STREAM(""));
	}
	CATCH(ExceptionTextToPrint)
}

inline void ChessEngineMainThreadsManager::InitializeLoggerManagerParameters()
{
	try
	{
		using namespace string_utils;

		LoggersManagerObject.InitializeFilesNames({ "AllMessages", "Information", "Important", "Debugs", "Warnings", "Errors", "Exceptions",  "ErrorsAndExceptions", "Critical", "Statistics" });
		LoggersManagerObject.InitializeSelectiveWordsFunctions(
		{ 
          [](const string& s) { return true; },
          [](const string& s) { return upper_case_str_transform(s).find("INFO") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("IMPORTANT") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("DEBUG") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("WARN") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("ERROR") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("EXCEPTION") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("ERRORS") != string::npos || upper_case_str_transform(s).find("EXCEPTION") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("CRITICAL") != string::npos; },
          [](const string& s) { return upper_case_str_transform(s).find("STATISTICS") != string::npos; }
		});

        LoggersManagerObject.InitializePrintingParameters(ChessEngineConfigurationFileReaderWriterObject.PrintLogToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogToFiles,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogLineNumberToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogDateTimeToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessIdToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessPriorityLevelToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogThreadIdToConsole,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogLineNumberToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogDateTimeToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessIdToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogProcessPriorityLevelToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.PrintLogThreadIdToFile,
                                                          ChessEngineConfigurationFileReaderWriterObject.MaximalNumberOfLinesInOneFile);
	}
	CATCH("initializing logger manager parameter")
}

ChessEngineMainThreadsManager::ChessEngineMainThreadsManager()
{	
	try
	{
        #ifdef USE_MPI
        MPI_Init(0, {});
        MPI_Comm_rank(MPI_COMM_WORLD, &MPIProcessIdentifier);
        MPI_Comm_size(MPI_COMM_WORLD, &NumberOfMPIProcesses);
        #endif

        #ifdef WINDOWS_PLATFORM
        ChessEngineConfigurationFileReaderWriterObject.SetProcessPriorityHighest == true ? SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS) : 0;
        #endif

        InitializeLoggerManagerParameters();

        #ifdef WINDOWS_PLATFORM
        const string ActualDateTimeStr = string("Logs." + GetActualDateTimeWindows(".", ".", ".", ".", ".", "."));
        #endif

        #ifdef UNIX_PLATFORM
        const string ActualDateTimeStr = string("Logs." + GetActualDateTimeStandardCPP(".", ".", ".", ".", "."));
        #endif

        #ifdef USE_MPI
        if (ChessEngineConfigurationFileReaderWriterObject.PrintMPIProcessesInformationData == true)
        {
            cout << "MPI process identifier = " << MPIProcessIdentifier << " Number of MPI processes = " << NumberOfMPIProcesses << endl;
            cout << "OneChessBoardForChessEngineThreadsStartDataSIZE = " << sizeof(OneChessBoardForChessEngineThreadsStartData) << endl;
        }
        #endif

        for (const auto &ChessEngineTestObject : ChessEngineConfigurationFileReaderWriterObject.ChessTests)
            if (ChessEngineTestObject.ExecuteTestBool == true)
            {
                ChessEngineConfigurationFileReaderWriterObject.FirstMove = false;

                ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel = ChessEngineTestObject.MaxDeepLevel;
                ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces = ChessEngineTestObject.ReverseColorOfPieces;

                PrepareData(ChessEngineTestObject.ChessBoardFileName, ChessEngineTestObject.TestId, ActualDateTimeStr);

                ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer.clear();
                ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArrayPointer.reserve(ChessEngineConfigurationFileReaderWriterObject.MaxNumberOfChessBoardsForFastExecution);

                const auto start_time = chrono::high_resolution_clock::now();


                ChessEngineThreadsStartData::NumberOfPass = 1;
                GenerateAllPossibleCombinationsOfMovesToFindChessBoardsForFullSpeedThreads();

                ChessEngineThreadsStartData::NumberOfPass = 2;
                LengthType ChessBoardsAveragePerThread = ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize / NumberOfMPIProcesses + 1;
                ChessEngineThreadsStartData::StartChessBoardNum = MPIProcessIdentifier * ChessBoardsAveragePerThread;
                ChessEngineThreadsStartData::EndChessBoardNum = MPIProcessIdentifier * ChessBoardsAveragePerThread + ChessBoardsAveragePerThread - 1 < ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize ? MPIProcessIdentifier * ChessBoardsAveragePerThread + ChessBoardsAveragePerThread - 1 : ChessEngineThreadsStartData::ChessBoardsForFullSpeedThreadsArraySize - 1;

                GenerateAllPossibleCombinationsOfMovesToFindChessBoardsForFullSpeedThreads();



                #ifndef USE_MPI
                SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads();

                GenerateAllPossibleCombinationsOfMovesInFullSpeedThreads();

                #endif

                #ifdef USE_MPI
                GenerateAllPossibleCombinationsOfMovesInMPIProcesses();

                GatherFoundChessScoredMovesPathsResultsFromAllMPIProcesses();

                ReduceStatisticsResultsFromAllMPIProcesses();
                #endif

                #ifndef USE_MPI
                SumStatisticsFromAllChessEngineAllPossibleMovesGeneratorObjectsThreads();
                #endif

                const auto stop_time = chrono::high_resolution_clock::now();

                #ifdef USE_MPI
                if (MPIProcessIdentifier == 0)
                {
                #endif
                    PrintDurationTimeAllFormats(start_time, stop_time, "Execution of looking for mates has taken time: ","executing printing duration_time");

                    ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.PrintResultsAndStatistics();

                    ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.CompareTestStatistics(ChessEngineTestObject);

                    ChessEngineAllPossibleMovesGeneratorObjects[0].ChessEngineResultsStatisticsObject.SaveTestStatisticsToFile(ChessEngineTestObject, ChessEngineTestObject.TestId);

                    LoggersManagerObject.Log(STREAM("END" << endl << endl));
                #ifdef USE_MPI
                }
                #endif
            }

        #ifdef USE_MPI
        MPI_Finalize();
        exit(0);
        #endif

        #ifndef USE_MPI
        ChessEngineConfigurationFileReaderWriterObject.WaitForKeyOnTheEnd == true ? getchar() : 0;
        #endif
	}
	CATCH_AND_WORK("executing chess main threaded engine", "", getchar());
}