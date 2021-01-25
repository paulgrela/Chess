from ChessInterfaceConstants import ChessInterfaceConstants
from ChessInterfaceColors import ChessInterfaceColors
from ChessInterfaceNetwork import NetworkConfig
from ChessInterfaceTournament import ChessInterfaceTournament

from collections import defaultdict

import os
import sys
import glob
import parse
import shutil
import datetime
import xmltodict
import subprocess

class FileConstants:

    workdir = "workdir"
    engine = "engine"
    engines = "engines"
    game = "game"
    games = "games"
    tournament = "tournament"
    tournaments = "tourns"

    ChessEngineProjectToCheckMate = "ChessEngineProjectCUDA.exe"
    ChessEngineProjectConfigToCheckMate = "ChessEngineProjectConfigCUDA_CheckMate.txt"
    ChessInterfaceConfig_xml = "ChessInterfaceConfig"

    actual_tournament_start_datetime_txt = "actual_tournament_start_datetime.txt"
    this_tournament_results_and_statistics_history_txt = "this_tournament_results_and_statistics_history.txt"

    last_actual_move_txt = "last_actual_move.txt"
    last_engine_move_txt = "last_engine_move.txt"
    chosen_moves_paths_txt = "chosen_moves_paths.txt"
    chosen_moves_paths_type_txt = "chosen_moves_paths_type.txt"
    actual_game_start_datetime_txt = "actual_game_start_datetime.txt"
    all_games_all_moves_history_txt = "all_games_all_moves_history.txt"
    this_game_all_moves_history_txt = "this_game_all_moves_history.txt"
    last_actual_chessboard_state_txt = "last_actual_chessboard_state.txt"
    chessboard_state = "chessboard_state"

def MakeEndOfGame(ChessInterfaceFrameObject, MateStr):

    WriteTextInHistoryFiles(MateStr + "\n", ChessInterfaceFrameObject, False)
    ChessInterfaceFrameObject.ResultLabel.config(text = MateStr)
    ReadAllMovesInGameToVisualize(ChessInterfaceFrameObject)

    if ChessInterfaceFrameObject.TournamentStopped is False:
        ChessInterfaceTournament.UpdateTournamentStatus(ChessInterfaceFrameObject, MateStr)

def UseEngine(ChessInterfaceFrameObject):

    print(ChessInterfaceFrameObject.Mode, "uses engine to predict next move")
    print("selected engine index =", ChessInterfaceFrameObject.SelectedChessEngineIndex)
    ChessEngineProjectExeFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.engine, ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ChessInterfaceFrameObject.SelectedChessEngineIndex][2]))
    ChessEngineProjectExeFilePath = "{}".format(ChessEngineProjectExeFilePath)
    ChessEngineProjectConfigFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.engine, ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ChessInterfaceFrameObject.SelectedChessEngineIndex][3]))
    ChessEngineProjectConfigFilePath = "{}".format(ChessEngineProjectConfigFilePath)
    print(ChessEngineProjectExeFilePath)
    subprocess.run([ChessEngineProjectExeFilePath, ChessEngineProjectConfigFilePath], cwd = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.engine)))

    LastActualMoveFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.last_actual_move_txt))
    with open(LastActualMoveFilePath, "r") as LastActualMoveFile:
        LastActualMove = LastActualMoveFile.readlines()[-1]
        print(ChessInterfaceFrameObject.Mode, "engine predicted move:#{}#".format(LastActualMove))

    if LastActualMove == ChessInterfaceConstants.MATE_N:
        print(LastActualMove)
    elif LastActualMove == (ChessInterfaceConstants.STALEMATE + "\n"):
        print(LastActualMove)
    else:
        LastActualMoveParsed = parse.parse("{}[{} {}]", LastActualMove)
        print("PARSED MOVE = ", LastActualMoveParsed)
        LastActualMoveFrom, LastActualMoveTo = LastActualMoveParsed[1].lower(), LastActualMoveParsed[2].lower()

        ChessInterfaceFrameObject.ChessInterfaceEngineObject.SelectFigure(ChessInterfaceFrameObject.ChessInterfaceEngineObject.ChessBoard[LastActualMoveFrom[0], LastActualMoveFrom[1]].FieldDescriptionStr)
        ChessInterfaceFrameObject.ChessInterfaceEngineObject.MakeMove(LastActualMoveTo, ChessInterfaceFrameObject, True)

    ReadChosenMovesPathsToVisualize(ChessInterfaceFrameObject)

    print("Updating view")
    ChessInterfaceFrameObject.RefreshAllFigures()

def ReadChosenMovesPathsToVisualize(ChessInterfaceFrameObject):

    ChessInterfaceFrameObject.ChosenMovesListbox.delete(0, "end")
    ChosenMovesPathsFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.chosen_moves_paths_txt))
    with open(ChosenMovesPathsFilePath, "r") as ChosenMovesPathsFile:
        ChosenMovesPathsFileLines = ChosenMovesPathsFile.readlines()
        ChosenMovesPathsLinesIndex = 1
        for ChosenMovesPathLine in ChosenMovesPathsFileLines:
            ChessInterfaceFrameObject.ChosenMovesListbox.insert(ChosenMovesPathsLinesIndex, "[" + str(ChosenMovesPathsLinesIndex) + "] " + ChosenMovesPathLine.strip())
            ChosenMovesPathsLinesIndex += 1

    ChosenMovesPathsTypeFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.chosen_moves_paths_type_txt))
    with open(ChosenMovesPathsTypeFilePath, "r") as ChosenMovesPathsTypeFile:
        ChessInterfaceFrameObject.FoundPathTypeLabel.config(text = ChosenMovesPathsTypeFile.readlines()[-1].strip() + " [" + str(ChosenMovesPathsLinesIndex - 1) + "]")

def ReadAllMovesInGameToVisualize(ChessInterfaceFrameObject):

    ChessInterfaceFrameObject.ListOfAllMovesInGameListbox.delete(0, "end")
    with open(os.path.join(GetActualGameStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), FileConstants.this_game_all_moves_history_txt), "r") as ThisGameAllMovesHistoryFile:
        ThisGameAllMovesHistoryFileLines =  ThisGameAllMovesHistoryFile.readlines()
        ThisGameAllMovesLinesIndex = 1
        for MoveLine in ThisGameAllMovesHistoryFileLines:
            ChessInterfaceFrameObject.ListOfAllMovesInGameListbox.insert(ThisGameAllMovesLinesIndex, MoveLine.strip()[21:])
            ThisGameAllMovesLinesIndex += 1
    ChessInterfaceFrameObject.ListOfAllMovesInGameListbox.yview(ThisGameAllMovesLinesIndex - 14)

# def CountNumberOfPawns1(ChessInterfaceEngineObject):
#     NumberOfPawnsWhite = 0
#     NumberOfPawnsBlack = 0
#     for PosY in ChessInterfaceConstants.Rows:
#         for PosX in ChessInterfaceConstants.Cols:
#             PieceName = ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetEngineFigureNameFromInterfaceName()
#             if PieceName[0] == "P":
#                 NumberOfPawnsWhite += 1
#             if PieceName[0] == "p":
#                 NumberOfPawnsBlack += 1
#     return [NumberOfPawnsWhite, NumberOfPawnsBlack]

def CountNumberOfPawns(ChessInterfaceEngineObject):

    NumberOfPawnsWhite = len([Figure for Figure in [ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetEngineFigureNameFromInterfaceName()[0] for PosY in ChessInterfaceConstants.Rows for PosX in ChessInterfaceConstants.Cols] if Figure == "P"])
    NumberOfPawnsBlack = len([Figure for Figure in [ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetEngineFigureNameFromInterfaceName()[0] for PosY in ChessInterfaceConstants.Rows for PosX in ChessInterfaceConstants.Cols] if Figure == "p"])
    print("NumberOfPawnsWhite = ", NumberOfPawnsWhite)
    print("NumberOfPawnsBlack = ", NumberOfPawnsBlack)

    return [NumberOfPawnsWhite, NumberOfPawnsBlack]

def SaveChessBoardState(ChessInterfaceEngineObject, ChessBoardFileName = sys.stdout, ChessBoardHistoryFileName = None):

    ChessBoardStringDelimiter = "|"

    NumberOfFoundPieceInstancesOnChessBoardDictionary = defaultdict(int)

    ChessBoardTextPicture = ""
    ChessBoardTextPicture = ChessBoardTextPicture + "{} {}".format(ChessBoardStringDelimiter, ChessBoardStringDelimiter)
    for PosX in ChessInterfaceConstants.Cols.upper():
        ChessBoardTextPicture = ChessBoardTextPicture + "{} {}".format(PosX, ChessBoardStringDelimiter)
    ChessBoardTextPicture = ChessBoardTextPicture + "\n|" + "-" * (8 * 3 + 2) + "\n"

    [NumberOfPawnsWhite, NumberOfPawnsBlack] = CountNumberOfPawns(ChessInterfaceEngineObject)

    for PosY in ChessInterfaceConstants.Rows:
        ChessBoardTextPicture = ChessBoardTextPicture + "{}{}{}".format(ChessBoardStringDelimiter, PosY, ChessBoardStringDelimiter)
        for PosX in ChessInterfaceConstants.Cols:
            PieceName = ChessInterfaceEngineObject.ChessBoard[(PosX, PosY)].GetEngineFigureNameFromInterfaceName()
            if PieceName[1] == "0":
                NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] += 1
                PieceName = "{}{}".format(PieceName[0], NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName])

            if PieceName[0] == "Q":
                NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] += 1
                print(ChessInterfaceConstants.QUEEN_WHITE, PieceName[0], " ", NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName])
                if NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] > 1:
                    PieceName = "X" + str(NumberOfPawnsWhite + NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] - 1)

            if PieceName[0] == "q":
                NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] += 1
                print(ChessInterfaceConstants.QUEEN_BLACK, PieceName[0], " ", NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName])
                if NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] > 1:
                    PieceName = "x" + str(NumberOfPawnsBlack + NumberOfFoundPieceInstancesOnChessBoardDictionary[PieceName] - 1)

            ChessBoardTextPicture = ChessBoardTextPicture + "{}{}".format(PieceName, ChessBoardStringDelimiter)
        ChessBoardTextPicture = ChessBoardTextPicture + "\n|" + "-" * (8 * 3 + 2) + "\n"

    print(ChessBoardTextPicture)
    with open(ChessBoardFileName, "w") as ChessBoardFile:
        ChessBoardFile.write(ChessBoardTextPicture)

    if ChessBoardHistoryFileName:
        with open(ChessBoardHistoryFileName, "w") as ChessBoardHistoryFile:
            ChessBoardHistoryFile.write(ChessBoardTextPicture)

        print("CHESSBOARD SAVED TO HISTORY FILE:", ChessBoardHistoryFileName)

def LocalNumberOfMoves(ChessInterfaceFrameObject):
    if (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.client and ChessInterfaceFrameObject.CurrentPlayer == ChessInterfaceConstants.BLACK) or (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.server and ChessInterfaceFrameObject.CurrentPlayer == ChessInterfaceConstants.WHITE):
        return (ChessInterfaceFrameObject.ChessInterfaceEngineObject.NumberOfMoves - 1)
    else:
        return ChessInterfaceFrameObject.ChessInterfaceEngineObject.NumberOfMoves

def WhoPreparedMove(ChessInterfaceFrameObject):
    if ChessInterfaceFrameObject.UseEngineVar.get() == 1:
        if (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.client and ChessInterfaceFrameObject.CurrentPlayer == ChessInterfaceConstants.WHITE) or (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.server and ChessInterfaceFrameObject.CurrentPlayer == ChessInterfaceConstants.BLACK):
            return ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ChessInterfaceFrameObject.SelectedChessEngineIndex][2]
        else:
            return ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[int(ChessInterfaceFrameObject.EngineNumberForOpponentEntryboxVar.get())][2]
    else:
        return "PERSON"

def UpdateActualGameFilesWhenGameReadFromFile(ChessInterfaceFrameObject, GameMoveFrom, GameMoveTo, GameFileMovesIndex, GameMoveParsed, ChosenMovesHistoryDirectory):

    with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.last_actual_move_txt), "w") as LastActualMoveFile:
        LastActualMoveFile.write("N0[" + GameMoveFrom + " " + GameMoveTo + "]")

    if (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.server and GameMoveParsed[0][-9:-4:1] == ChessInterfaceConstants.BLACK) or (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.client and GameMoveParsed[0][-9:-4:1] == ChessInterfaceConstants.WHITE):
        with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.last_engine_move_txt), "w") as LastEngineMoveFile:
            LastEngineMoveFile.write("N0[" + GameMoveFrom + " " + GameMoveTo + "]")

        if ChessInterfaceFrameObject.ReadChosenMovesPathsWhenGameReadFromFileVar.get() == 1:

            ChosenMovesPathsFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.chosen_moves_paths_txt))
            for ChosenMovesHistoryFile in glob.glob(os.path.join(ChosenMovesHistoryDirectory, "ChosenMoves_*_MOVE_" + str(GameFileMovesIndex) + "_*.txt")):
                print("COPIED", ChosenMovesHistoryFile, "TO", ChosenMovesPathsFilePath)
                shutil.copyfile(ChosenMovesHistoryFile, ChosenMovesPathsFilePath)

            ReadChosenMovesPathsToVisualize(ChessInterfaceFrameObject)

def SaveChosenMovesToHistoryFile(ChessInterfaceFrameObject):
    if ChessInterfaceFrameObject.CurrentPlayer == ChessInterfaceFrameObject.ChessInterfaceEngineObject.Player:
        ChosenMovesPathsFilePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.chosen_moves_paths_txt))
        ChosenMovesHistoryPath = os.path.join(GetActualGameStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), "ChosenMoves" + "_{}_MOVE_{}_{}_{}.txt".format(WhoPreparedMove(ChessInterfaceFrameObject), LocalNumberOfMoves(ChessInterfaceFrameObject), ChessInterfaceFrameObject.CurrentPlayer, datetime.datetime.now().strftime("%Y.%m.%d.%H.%M.%S.%f")))
        print("ChosenMovesPathsFilePath =", ChosenMovesPathsFilePath)
        print("ChosenMovesHistoryPath =", ChosenMovesHistoryPath)
        shutil.copyfile(ChosenMovesPathsFilePath, ChosenMovesHistoryPath)

def SaveMoveInHistoryFiles(PosFrom, PosTo, ChessInterfaceFrameObject):
    PosX = PosTo[0]
    PosY = PosTo[1]

    SaveChessBoardState(ChessInterfaceFrameObject.ChessInterfaceEngineObject, os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.last_actual_chessboard_state_txt), ChessBoardHistoryFileName = os.path.join(GetActualGameStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), FileConstants.chessboard_state + "_{}_MOVE_{}_{}_{}.txt".format(WhoPreparedMove(ChessInterfaceFrameObject), LocalNumberOfMoves(ChessInterfaceFrameObject), ChessInterfaceFrameObject.CurrentPlayer, datetime.datetime.now().strftime("%Y.%m.%d.%H.%M.%S.%f"))))

    SaveChosenMovesToHistoryFile(ChessInterfaceFrameObject)

    ChessMoveStr = "{}[{} {}]".format(ChessInterfaceFrameObject.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetEngineFigureNameFromInterfaceName(), PosFrom, PosTo)
    with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.last_actual_move_txt), "w") as LastActualMoveFile:
        LastActualMoveFile.write(ChessMoveStr)
    WriteTextInHistoryFiles(ChessMoveStr, ChessInterfaceFrameObject, True)

def WriteTextInHistoryFiles(TextToWriteInHistoryFiles, ChessInterfaceFrameObject, NormalMove):

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())
        LogDateTimeToMovesHistoryFiles = ChessInterfaceConfigXMLOrderedDict["Settings"]["General"]["LogDateTimeToMovesHistoryFiles"]

        with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.all_games_all_moves_history_txt), "a+") as AllGamesAllMovesHistoryFile:
            if LogDateTimeToMovesHistoryFiles == "true":
                if NormalMove == True:
                    AllGamesAllMovesHistoryFile.write("DATE_TIME_{}_MOVE_{}_{}: ".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f"), LocalNumberOfMoves(ChessInterfaceFrameObject), ChessInterfaceFrameObject.CurrentPlayer))
                    AllGamesAllMovesHistoryFile.write(TextToWriteInHistoryFiles + " (" + WhoPreparedMove(ChessInterfaceFrameObject) + ")\n")
                else:
                    AllGamesAllMovesHistoryFile.write("DATE_TIME_{}_EXECUTED_OPRERATION : ".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")))
                    AllGamesAllMovesHistoryFile.write(TextToWriteInHistoryFiles)

        with open(os.path.join(GetActualGameStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), FileConstants.this_game_all_moves_history_txt), "a+") as ThisGameAllMovesHistoryFile:
            if LogDateTimeToMovesHistoryFiles == "true":
                if NormalMove == True:
                    ThisGameAllMovesHistoryFile.write("DATE_TIME_{}_MOVE_{}_{}: ".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f"), LocalNumberOfMoves(ChessInterfaceFrameObject), ChessInterfaceFrameObject.CurrentPlayer))
                    ThisGameAllMovesHistoryFile.write(TextToWriteInHistoryFiles + " (" + WhoPreparedMove(ChessInterfaceFrameObject) + ")\n")
                else:
                    ThisGameAllMovesHistoryFile.write("DATE_TIME_{}_EXECUTED_OPERATION : ".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")))
                    ThisGameAllMovesHistoryFile.write(TextToWriteInHistoryFiles)

def InitializeTournamentDirectoriesAndFiles(ChessInterfaceFrameObject):

    TournamentStartDateTimePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.tournaments, (FileConstants.tournament +"_" + datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S"))))
    print(TournamentStartDateTimePath)

    os.makedirs(TournamentStartDateTimePath)

    with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.actual_tournament_start_datetime_txt), "w") as ActualTournamentStartDateTimeFile:
        ActualTournamentStartDateTimeFile.write(TournamentStartDateTimePath)

def GetActualTournamentStartDateTimeFromFile(Mode):

    ActualTournamentStartDateTimeFilePath = os.path.abspath(os.path.join(FileConstants.engines, Mode, FileConstants.workdir, FileConstants.actual_tournament_start_datetime_txt))
    print("FILE =", ActualTournamentStartDateTimeFilePath)

    with open(ActualTournamentStartDateTimeFilePath, "r") as ActualTournamentStartDatetimeFile:
        ActualTournamentStartDateTime = ActualTournamentStartDatetimeFile.readlines()[-1]
        print("tournament_start_datetime = ", ActualTournamentStartDateTime)
        return ActualTournamentStartDateTime

def InitializeGameDirectoriesAndFiles(ChessInterfaceFrameObject):

    GameStartDateTimePath = os.path.abspath(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.games, (FileConstants.game +"_" + datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S"))))
    print(GameStartDateTimePath)

    with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.actual_game_start_datetime_txt), "w") as ActualGameStartDateTimeFile:
        ActualGameStartDateTimeFile.write(GameStartDateTimePath)

    os.makedirs(GameStartDateTimePath)

    with open(os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.chosen_moves_paths_txt), "w") as ChosenMovesPathsFile:
        ChosenMovesPathsFile.write(ChessInterfaceConstants.EMPTY)

    SaveChessBoardState(ChessInterfaceFrameObject.ChessInterfaceEngineObject, os.path.join(FileConstants.engines, ChessInterfaceFrameObject.Mode, FileConstants.workdir, FileConstants.last_actual_chessboard_state_txt))

    WriteTextInHistoryFiles("\nDATE_TIME_{}_EXECUTED_OPERATION : NEW GAME STARTED\n".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")), ChessInterfaceFrameObject, False)

def GetActualGameStartDateTimeFromFile(Mode):

    ActualGameStartDateTimeFilePath = os.path.abspath(os.path.join(FileConstants.engines, Mode, FileConstants.workdir, FileConstants.actual_game_start_datetime_txt))
    print("FILE =", ActualGameStartDateTimeFilePath)

    with open(ActualGameStartDateTimeFilePath, "r") as ActualGameStartDatetimeFile:
        ActualGameStartDateTime = ActualGameStartDatetimeFile.readlines()[-1]
        print("game_start_datetime = ", ActualGameStartDateTime)
        return ActualGameStartDateTime

def ReadAndPrintChessInterfaceConfigFile():

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())

        for FieldAndValue in ChessInterfaceConfigXMLOrderedDict["Settings"]["General"].items():
            print(FieldAndValue[0], FieldAndValue[1])

        for FieldAndValue in ChessInterfaceConfigXMLOrderedDict["Settings"]["Network"].items():
            print(FieldAndValue[0], FieldAndValue[1])

def ReadColorsFromChessInterfaceConfigFile():

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())

    ChessInterfaceColors.BackgroundFirstColor = (int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BackgroundFirstColorR"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BackgroundFirstColorG"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BackgroundFirstColorB"]))
    ChessInterfaceColors.BackgroundSecondColor = (int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BackgroundSecondColorR"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BackgroundSecondColorG"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BackgroundSecondColorB"]))
    ChessInterfaceColors.WhitePiecesColor =  (int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["WhitePiecesColorR"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["WhitePiecesColorG"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["WhitePiecesColorB"]))
    ChessInterfaceColors.BlackPiecesColor =  (int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BlackPiecesColorR"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BlackPiecesColorG"]), int(ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessBoard"]["ChessBoardColors"]["BlackPiecesColorB"]))

def ReadNetworkParametersFromChessInterfaceConfigFile():

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())

    NetworkConfig.ClientHost = ChessInterfaceConfigXMLOrderedDict["Settings"]["Network"]["Host"]
    NetworkConfig.Port = int(ChessInterfaceConfigXMLOrderedDict["Settings"]["Network"]["Port"])

def ReadAllFoundPathsToMateParametersFromChessInterfaceConfigFile(ChessInterfaceFrameObject):

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())

    ChessInterfaceFrameObject.PrintAllFoundPathsToMate = int(ChessInterfaceConfigXMLOrderedDict["Settings"]["General"]["PrintAllFoundPathsToMate"])

def ReadNumberOfMovesToDeclareStalemateFromChessInterfaceConfigFile(ChessInterfaceFrameObject):

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())

    ChessInterfaceFrameObject.ForceStalemateAfterNumberOfMovesEntryboxVar.set(int(ChessInterfaceConfigXMLOrderedDict["Settings"]["General"]["ForceStalemateAfterNumberOfMoves"]))

def ReadListOfEnginesFromChessInterfaceConfigFile(ChessInterfaceFrameObject):

    with open(FileConstants.ChessInterfaceConfig_xml, "r") as ChessInterfaceConfigFile:
        ChessInterfaceConfigXMLOrderedDict = xmltodict.parse(ChessInterfaceConfigFile.read())

    ChessInterfaceFrameObject.ListOfEnginesListbox.delete(0, "end")
    ListOfEnginesLinesIndex = 0
    ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun = []
    for FieldAndValue1 in ChessInterfaceConfigXMLOrderedDict["Settings"]["ChessEngines"]["ChessEngine"]:
        ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun.append([])
        ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ListOfEnginesLinesIndex].append(str(ListOfEnginesLinesIndex))
        for FieldAndValue in FieldAndValue1.items():
            ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ListOfEnginesLinesIndex].append(FieldAndValue[1])
        ChessInterfaceFrameObject.ListOfEnginesListbox.insert(ListOfEnginesLinesIndex, ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ListOfEnginesLinesIndex][1] + " " + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ListOfEnginesLinesIndex][2] + " " + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ListOfEnginesLinesIndex][3])
        ListOfEnginesLinesIndex += 1

    print(ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun)

    ChessInterfaceFrameObject.SelectedChessEngineIndex = 0
    ChessInterfaceFrameObject.ListOfEnginesListbox.selection_set(0)