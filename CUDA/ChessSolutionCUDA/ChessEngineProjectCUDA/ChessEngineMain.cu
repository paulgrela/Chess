
#include <iostream>

#include "ChessEngineConfigurationFileReaderWriter.h"
#include "ChessEngineMainThreadsManager.h"

using namespace std;

int main(int argc, char** argv)
{
	ChessEngineConfigurationFileReaderWriterObject.ReadChessConfigurationFile(argv[1]);

	ChessEngineMainThreadsManager ChessEngineMainThreadsManagerObject;

	return 0;
}