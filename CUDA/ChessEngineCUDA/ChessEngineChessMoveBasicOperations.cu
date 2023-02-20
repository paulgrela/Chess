
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