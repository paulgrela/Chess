
#include <vector>

#include "ChessEngineConstants.h"
#include "ExceptionsMacro.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

using namespace std;

string GetChessMoveAsString(const PosType cx1, const PosType cy1, const PosType cx2, const PosType cy2)
{
	return char('A' + cx1 - 1) + to_string(cy1) + SpaceChar + char('A' + cx2 - 1) + to_string(cy2);
}

string GetChessMovesPathAsString(const vector<ChessMove>& MovesPath)
{
	string MovesPathStr;

	try
	{
		for (const ChessMove& ChessMove : MovesPath)
			MovesPathStr += ChessEngineConfigurationFileReaderWriterObject.PieceString[ChessMove.PieceNum] + "[" + GetChessMoveAsString(ChessMove.PrevPosX, ChessMove.PrevPosY, ChessMove.NextPosX, ChessMove.NextPosY) + "] ";
	}
	CATCH("getting moves path to string")

	return MovesPathStr;
}

string GetChessMovesPathAsString(const ChessScoredMovesPath& MovesPathObject)
{
    string MovesPathStr;

    try
    {
        for (uint64_t ChessMoveIndex = 0; ChessMoveIndex < MovesPathObject.MovesPathSize; ChessMoveIndex++)
        {
            ChessMove ChessMoveObject = MovesPathObject.MovesPath[ChessMoveIndex];
            MovesPathStr += ChessEngineConfigurationFileReaderWriterObject.PieceString[ChessMoveObject.PieceNum] + "[" + GetChessMoveAsString(ChessMoveObject.PrevPosX, ChessMoveObject.PrevPosY, ChessMoveObject.NextPosX, ChessMoveObject.NextPosY) + "] ";
        }
    }
    CATCH("getting moves path to string")

    return MovesPathStr;
}

void CopyChessMovesPathsVector(const LengthType DeepLevelPathLength, ChessMove* const OneMovesPath, const ChessMove* const ActuallyInvestigatedMovesPath)
{
    try
    {
        for (LengthType ChessMoveIndex = 0; ChessMoveIndex < DeepLevelPathLength; ChessMoveIndex++)
            OneMovesPath[ChessMoveIndex] = ActuallyInvestigatedMovesPath[ChessMoveIndex];
    }
    CATCH("copying moves path to vector")
}

vector<ChessMove> GetChessMovesPathVector(const LengthType DeepLevelPathLength, const ChessMove* const ActuallyInvestigatedMovesPath)
{
    vector<ChessMove> OnePathMoves;

    try
    {
        for (LengthType ChessMove = 0; ChessMove < DeepLevelPathLength; ChessMove++)
            OnePathMoves.emplace_back(ActuallyInvestigatedMovesPath[ChessMove]);
    }
    CATCH("getting moves path to vector")

    return OnePathMoves;
}