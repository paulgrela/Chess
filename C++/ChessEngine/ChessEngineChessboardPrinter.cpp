
#include "DestinationPlatform.h"

#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif

#include "ExceptionsMacro.h"

#include "ChessEngineAllPossibleMovesGenerator.h"
#include "ChessEngineConfigurationFileReaderWriter.h"

#include "ChessEngineChessboardPrinter.h"

#include "TerminalColorsUtils.h"

using namespace std;

void ChessEngineChessboardPrinter::CleanScreen() const noexcept
{
    try
    {
        #ifdef WINDOWS_PLATFORM
        if (ChessEngineConfigurationFileReaderWriterObject.CleanScreenBeforePrintingActualState == true)
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, 0 });
        #endif
    }
    CATCH("cleaning screen")
}

void ChessEngineChessboardPrinter::PrintChessBoardHorizontalSeparationLine(std::ostream& StreamObject) const noexcept
{
    try
    {
        StreamObject << SpaceChar << SpaceChar;
        for (PosType CX = 1; CX < MaxChessSizeX * 3 - 3; CX++)
            StreamObject << '-';
        StreamObject << endl;
    }
    CATCH("printing chess board horizontal separation line")
}

void ChessEngineChessboardPrinter::PrintChessBoardLetters(std::ostream& StreamObject) const noexcept
{
    try
    {
        StreamObject << SpaceChar;
        for (PosType CX = 1; CX < MaxChessSizeX; CX++)
            StreamObject << SpaceChar << char('A' + CX - 1) << " ";
        StreamObject << endl;
    }
    CATCH("printing chess board letters")
}

inline std::ostream& color1(std::ostream& s)
{
    #ifdef WINDOWS_PLATFORM
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ChessEngineConfigurationFileReaderWriterObject.Color1);
    return s;
    #endif
    #ifdef UNIX_PLATFORM
    return s << string(string("\033[1;") + to_string(ChessEngineConfigurationFileReaderWriterObject.Color1) + "m");
    #endif
}

inline std::ostream& color2(std::ostream& s)
{
    #ifdef WINDOWS_PLATFORM
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ChessEngineConfigurationFileReaderWriterObject.Color2);
    return s;
    #endif
    #ifdef UNIX_PLATFORM
    return s << string(string("\033[1;") + to_string(ChessEngineConfigurationFileReaderWriterObject.Color2) + "m");
    #endif
}

inline std::ostream& color3(std::ostream& s)
{
    #ifdef WINDOWS_PLATFORM
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ChessEngineConfigurationFileReaderWriterObject.Color3);
    return s;
    #endif
    #ifdef UNIX_PLATFORM
    return s << string(string("\033[1;") + to_string(ChessEngineConfigurationFileReaderWriterObject.Color3) + "m");
    #endif
}

inline std::ostream& color4(std::ostream& s)
{
    #ifdef WINDOWS_PLATFORM
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ChessEngineConfigurationFileReaderWriterObject.Color4);
    return s;
    #endif
    #ifdef UNIX_PLATFORM
    return s << string(string("\033[1;") + to_string(ChessEngineConfigurationFileReaderWriterObject.Color4) + "m");
    #endif
}

void ChessEngineChessboardPrinter::PrintChessBoardAndPieces(std::ostream& StreamObject, bool LogColors) const noexcept
{
    try
    {
        for (PosType CY = 1; CY < MaxChessSizeY; CY++)
        {
            StreamObject << int(CY) << '|';

            for (PosType CX = 1; CX < MaxChessSizeX; CX++)
            {
                if (LogColors == true)
                    ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] <= EndOfWhitePiecesNum ? ((CY * MaxChessSizeY + CX) % 2 == 0 ? StreamObject << color3 : StreamObject << color1) : ((CY * MaxChessSizeY + CX) % 2 == 0 ? StreamObject << color4 : StreamObject << color2);

                if (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] != SpaceNum)
                {
                    if (ChessEngineAllPossibleMovesGeneratorLocalReference.Pieces[ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY]].Promoted == false)
                        StreamObject << ChessEngineConfigurationFileReaderWriterObject.PieceString[ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY]];
                    else
                        StreamObject << (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] <= EndOfWhitePiecesNum ? "X" : "x") << int(ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] - (ChessEngineAllPossibleMovesGeneratorLocalReference.ChessBoard[CX][CY] <= EndOfWhitePiecesNum ? Pawn1Num[White] : Pawn1Num[Black]) + 1);
                }
                else
                    StreamObject << SpaceChar << SpaceChar;

                LogColors == true ? (StreamObject << terminal_colors_utils::background_black << terminal_colors_utils::white) : StreamObject << "";
                StreamObject << '|';
            }

            StreamObject << int(CY) << endl;

            PrintChessBoardHorizontalSeparationLine(StreamObject);
        }
    }
    CATCH("printing chess board and pieces")
}

ostream& ChessEngineChessboardPrinter::PrintActualChessBoardState(ostream&& StreamObject, bool LogColors) const
{
    try
    {
        CleanScreen();

        PrintChessBoardLetters(StreamObject);
        PrintChessBoardHorizontalSeparationLine(StreamObject);

        PrintChessBoardAndPieces(StreamObject, LogColors);

        PrintChessBoardLetters(StreamObject);
    }
    CATCH("printing chess board state")

    return StreamObject;
}

void ChessEngineChessboardPrinter::LogPrintActualChessBoardStateToStreams(function<void(LoggersManager*, const std::stringstream& Message)> LogFunction, uint64_t EndLineCounter1)
{
    try
    {
        stringstream ss;
        for (uint64_t EndLineCounter = 1; EndLineCounter <= EndLineCounter1; EndLineCounter++)
            ss << endl;
        ss << static_cast<stringstream&>(PrintActualChessBoardState(stringstream(), false)).str();
        LogFunction(&LoggersManagerObject, ss);

        {
            lock_guard<mutex> LockGuardObject{CoutForPrintingChessBoardInColorsMutexObject};
            PrintActualChessBoardState(move(cout), true) << flush;
        }
        cout << endl;
    }
    CATCH("logging printing chess board state to streams")
}
