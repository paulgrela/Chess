
#include "ExceptionsMacro.h"

#include "ChessEngineMainThreadsManager.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

int main(const int argc, const char* argv[])
{
	try
	{
        ChessEngineConfigurationFileReaderWriterObject.ReadChessConfigurationFile(argv[1]);

	    ChessEngineMainThreadsManager ChessEngineMainThreadsManagerObject;
	}
	CATCH("executing main");

	return 0;
}