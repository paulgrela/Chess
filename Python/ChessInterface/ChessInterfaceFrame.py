from ChessInterfaceEngine import ChessInterfaceEngine
from ChessInterfaceNetwork import ChessNetSocketClient, ChessNetSocketServer, NetworkConfig
from ChessInterfaceConstants import ChessInterfaceConstants
from ChessInterfaceTournament import ChessInterfaceTournament

import ChessInterfaceFiles

from tkinter import *
from tkinter import messagebox
from tkinter import filedialog

import os
import time
import parse
import subprocess
import platform

class ChessInterfaceFrame:

    @classmethod
    def CloseThisProcess(cls):
        if platform.system() == "Windows":
            subprocess.Popen("taskkill /F /PID " + str(os.getpid()))
        elif platform.system() == "Linux":
            subprocess.run(["kill", "-9", str(os.getpid())])

    def DrawChessBoard(self):
        # drawing chessboard
        for PosX in ChessInterfaceConstants.Cols:
            for PosY in ChessInterfaceConstants.Rows:
                DrawPosX, DrawPosY = (self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].DrawPosX, self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].DrawPosY)
                self.Canvas.create_image(DrawPosX, DrawPosY, image = self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Image)

        # write down numbers
        for PosY in ChessInterfaceConstants.RowsInverted:
            self.Canvas.create_text(self.ChessInterfaceEngineObject.ChessBoard["a", PosY].DrawPosX - 45, self.ChessInterfaceEngineObject.ChessBoard["a", PosY].DrawPosY, text = PosY)
        for PosYReversed in ChessInterfaceConstants.RowsInverted:
            self.Canvas.create_text(self.ChessInterfaceEngineObject.ChessBoard["h", PosYReversed].DrawPosX + 45, self.ChessInterfaceEngineObject.ChessBoard["h", PosYReversed].DrawPosY, text = PosYReversed)
        # write down letters
        for PosX in ChessInterfaceConstants.Cols:
            self.Canvas.create_text(self.ChessInterfaceEngineObject.ChessBoard[PosX, "1"].DrawPosX, self.ChessInterfaceEngineObject.ChessBoard[PosX, "1"].DrawPosY + 45, text = PosX)
        for PosX1 in ChessInterfaceConstants.Cols:
            self.Canvas.create_text(self.ChessInterfaceEngineObject.ChessBoard[PosX1, "8"].DrawPosX, self.ChessInterfaceEngineObject.ChessBoard[PosX1, "8"].DrawPosY - 45, text = PosX1)

    def PutWindowInTheCenterOfScreen(self):

        WindowWidth = self.Canvas.winfo_reqwidth()
        WindowHeight = self.Canvas.winfo_reqheight()

        PositionRight = int(self.TkRoot.winfo_screenwidth() / 2 - WindowWidth / 2)
        PositionDown = int(self.TkRoot.winfo_screenheight() / 2 - WindowHeight / 2)

        self.TkRoot.geometry("+{}+{}".format(PositionRight, PositionDown))

        print("Width =", WindowWidth, "Height =", WindowHeight, "Screen Width =", self.TkRoot.winfo_screenwidth(), "Screen Height =", self.TkRoot.winfo_screenheight(), "PosX =", PositionRight, "PosY =", PositionDown)

    def NewGame(self):

        self.CurrentPlayer = "None"
        self.FirstMove = True

        self.ChosenMovesListbox.delete(0, "end")
        self.ListOfAllMovesInGameListbox.delete(0, "end")

        if self.Mode == ChessInterfaceConstants.client:
            MainColor = ChessInterfaceConstants.WHITE
        else:
            MainColor = ChessInterfaceConstants.BLACK
        self.ChessInterfaceEngineObject.NewGame(MainColor)

        self.DrawChessBoard()

        self.CurrentMovePlayerColorLabel.config(text = "[Move " + str(self.ChessInterfaceEngineObject.NumberOfMoves) + "]")
        self.ResultLabel.config(text = ChessInterfaceConstants.GAME_ONGOING)

        ChessInterfaceFiles.InitializeGameDirectoriesAndFiles(self)

    def StopGame(self):
        self.GameStopped = True
        self.StartStopGameButton.config(text = ChessInterfaceConstants.START_GAME)
        self.NewGameButton["state"] = "normal"
        self.UseEngineCheckbutton["state"] = "normal"
        self.FirstMoveInNewGameFromEngineCheckbutton["state"] = "normal"
        ChessInterfaceFiles.WriteTextInHistoryFiles(ChessInterfaceConstants.GAME_STOPPED + "\n", self, False)

    def StartGame(self):
        self.GameStopped = False
        self.StartStopGameButton.config(text = ChessInterfaceConstants.STOP_GAME)
        self.NewGameButton["state"] = "disabled"
        self.UseEngineCheckbutton["state"] = "disabled"
        self.FirstMoveInNewGameFromEngineCheckbutton["state"] = "disabled"
        ChessInterfaceFiles.WriteTextInHistoryFiles(ChessInterfaceConstants.GAME_STARTED + "\n", self, False)

        if self.TournamentStopped is False:
            time.sleep(10)

        if self.UseEngineVar.get() == 1:
            if self.Mode == ChessInterfaceConstants.server:
                print("GAME RESTARTED FROM SERVER-BLACK")
                if (self.FirstMove is False and self.CurrentPlayer == ChessInterfaceConstants.BLACK) or (self.FirstMove is True and self.FirstMoveInNewGameFromEngineVar.get() == 1):
                    ChessInterfaceFiles.UseEngine(self)
            elif self.Mode == ChessInterfaceConstants.client:
                print("GAME RESTARTED FROM CLIENT-WHITE")
                if (self.FirstMove is False and self.CurrentPlayer == ChessInterfaceConstants.WHITE) or (self.FirstMove is True and self.FirstMoveInNewGameFromEngineVar.get() == 1):
                    ChessInterfaceFiles.UseEngine(self)

    def UpdateFirstPlayerColorData(self, WhoseMove):
        if self.CurrentPlayer == "None":
            if self.Mode == ChessInterfaceConstants.server:
                if WhoseMove == ChessInterfaceConstants.FIRST_MY_MOVE:
                    self.CurrentPlayer = ChessInterfaceConstants.BLACK
                else:
                    self.CurrentPlayer = ChessInterfaceConstants.WHITE
            else:
                if WhoseMove == ChessInterfaceConstants.FIRST_MY_MOVE:
                    self.CurrentPlayer = ChessInterfaceConstants.WHITE
                else:
                    self.CurrentPlayer = ChessInterfaceConstants.BLACK

            self.FirstMoveColor = self.CurrentPlayer

    def UpdateCurrentPlayerColorData(self):
        if self.CurrentPlayer == ChessInterfaceConstants.BLACK:
            self.CurrentPlayer = ChessInterfaceConstants.WHITE
        else:
            self.CurrentPlayer = ChessInterfaceConstants.BLACK
        self.CurrentMovePlayerColorLabel.config(text = self.CurrentPlayer + " [Move " + str(self.ChessInterfaceEngineObject.NumberOfMoves) + "]")
        ChessInterfaceFiles.ReadAllMovesInGameToVisualize(self)

    def ChosenMovesShowHide(self):
        if self.PrintAllFoundPathsToMateVar.get() == 0:
            self.FoundPathTypeTextLabel.lower()
            self.FoundPathTypeLabel.lower()
            self.ChosenMovesListbox.lower()
        else:
            self.FoundPathTypeTextLabel.lift()
            self.FoundPathTypeLabel.lift()
            self.ChosenMovesListbox.lift()

    def __PrintAllFoundPathsToMateClick(self):
        self.ChosenMovesShowHide()

    def __StartStopGameClick(self):
        if not self.GameStopped:
            if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Would you like to stop game?"):
                self.StopGame()
                self.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.STOP_GAME + "#").encode())
            else:
                messagebox.showwarning(ChessInterfaceConstants.Information, ChessInterfaceConstants.GAME_NOT_STOPPED + "!")
        else:
            if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Would you like to start game?"):
                self.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.START_GAME + "|" + self.EngineNumberForOpponentEntryboxVar.get() + "|" + "#").encode())
                self.StartGame()
            else:
                messagebox.showwarning(ChessInterfaceConstants.Information, ChessInterfaceConstants.GAME_NOT_STARTED + "!")

    def __NewGameClick(self):
        if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Would you like a new game?"):
            self.NewGame()
            self.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.NEW_GAME + "|" + self.EngineNumberForOpponentEntryboxVar.get() + "|" + "#").encode())
        else:
            messagebox.showwarning(ChessInterfaceConstants.Information, "New game not started!")

    def AreEnginesDifferentInGameInTournament(self):
        return self.TournamentGamesOnlyBetweenDifferentEnginesVar.get() == 0 or (self.TournamentGamesOnlyBetweenDifferentEnginesVar.get() == 1 and self.SelectedChessEngineIndex != self.TournamentPartnerEngineIndex)

    def __NewTournamentClick(self):
        if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Would you like a new tournament?"):
            ChessInterfaceFiles.InitializeTournamentDirectoriesAndFiles(self)
            self.TournamentResultsListbox.delete(0, "end")
            self.FirstMoveInNewGameFromEngineVar.set(1)
            self.TournamentGameIndex = 1
            self.TournamentPartnerEngineIndex = 0
            self.SelectedChessEngineIndex = 0
            self.ListOfEnginesListbox.selection_set(self.SelectedChessEngineIndex)
            self.EngineNumberForOpponentEntrybox["state"] = "disabled"
            self.NewTournamentButton["state"] = "disabled"
            self.TournamentStopped = False
            self.TournamentResultsList = []
            if self.AreEnginesDifferentInGameInTournament() is True:
                self.StopGame()
                self.NewGame()
                self.StartGame()
            else:
                ChessInterfaceTournament.UpdateTournamentStatus(self, "NO GAME")
            self.TournamentStatusLabel.config(text = "Local = " + str(self.SelectedChessEngineIndex) + " Partner = " + str(self.TournamentPartnerEngineIndex) + " Repeat = " + str(self.TournamentGameIndex))
        else:
            messagebox.showwarning(ChessInterfaceConstants.Information, "New tournament not started!")

    def __LoadGameFromFile(self):
        if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Would you like to read game from file?"):
            GameFilePath = filedialog.askopenfilename(initialdir = os.path.abspath(os.path.join(ChessInterfaceFiles.FileConstants.engines, self.Mode, ChessInterfaceFiles.FileConstants.games)), title = "Select game file", filetypes = (("txt files", "*.txt"), ("all files", "*.*")))
            if GameFilePath != "":
                print("OPENED GAME FILE =", GameFilePath)

                with open(GameFilePath, "r") as GameFile:
                    self.StopGame()
                    self.NewGame()
                    self.StartGame()
                    GameFileLines = GameFile.readlines()
                    GameFileLinesIndex = 1
                    GameFileMovesIndex = 1

                    for GameFileLine in GameFileLines:
                        print(str(GameFileLinesIndex), GameFileLine)
                        GameMoveParsed = parse.parse("{}[{} {}]{}", GameFileLine)
                        print("PARSED MOVE =", GameMoveParsed)
                        if GameMoveParsed is not None:
                            GameMoveFrom, GameMoveTo = GameMoveParsed[1].lower(), GameMoveParsed[2].lower()
                            print("MOVE =", GameMoveFrom, GameMoveTo, GameMoveParsed[0][-9:-4:1])
                            self.CurrentPlayer = GameMoveParsed[0][-9:-4:1]
                            self.ChessInterfaceEngineObject.SelectFigure(self.ChessInterfaceEngineObject.ChessBoard[GameMoveParsed[1][0], GameMoveParsed[1][1]].FieldDescriptionStr)
                            if (self.Mode == ChessInterfaceConstants.server and GameMoveParsed[0][-9:-4:1] == ChessInterfaceConstants.BLACK) or (self.Mode == ChessInterfaceConstants.client and GameMoveParsed[0][-9:-4:1] == ChessInterfaceConstants.WHITE):
                                self.ChessInterfaceEngineObject.MakeMove(GameMoveTo, self, False)
                            else:
                                self.MakeOpponentMove(GameMoveFrom, GameMoveTo, None)
                                self.UpdateCurrentPlayerColorData()
                            self.RefreshFigure((GameMoveParsed[1][0], GameMoveParsed[1][1]))
                            self.RefreshFigure(GameMoveTo)

                            ChessInterfaceFiles.UpdateActualGameFilesWhenGameReadFromFile(self, GameMoveFrom, GameMoveTo, GameFileMovesIndex, GameMoveParsed, GameFilePath[0:-len(ChessInterfaceFiles.FileConstants.this_game_all_moves_history_txt):1])

                            if int(self.AskForContinueAfterEveryMoveWhenGameReadFromFileVar.get()) == 1:
                                if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Continue") is False:
                                    break

                            if GameFileMovesIndex + 1 > int(self.OnWhichMoveStopWhenGameReadFromFileEntryboxVar.get()):
                                break

                            GameFileMovesIndex += 1
                        GameFileLinesIndex += 1
        else:
            messagebox.showwarning(ChessInterfaceConstants.Information, "Game not read from file!")

    def __CloseAllApplicationsClick(self):
        if messagebox.askokcancel(ChessInterfaceConstants.Confirmation, "Would you like to close all applications?"):
            print("SEND CLOSE APPLICATIONS SIGNAL")
            if self.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection:
                self.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.CLOSE_ALL_APPLICATIONS + "#").encode())
            self.CloseThisProcess()
        else:
            messagebox.showwarning(ChessInterfaceConstants.Information, "All applications not closed!")

    @classmethod
    def DrawTextLabel(cls, TextLabelObjectText, TextLabelObjectX, TextLabelObjectY, TextLabelObjectWidth, TextLabelObjectHeight, LabelObjectText, LabelObjectX, LabelObjectY, LabelObjectWidth, LabelObjectHeight, Color, Anchor):

        TextLabelObject = Label(text = TextLabelObjectText, relief = RIDGE, width = 6, bg = Color, anchor = Anchor)
        TextLabelObject.pack()
        TextLabelObject.place(x = TextLabelObjectX, y = TextLabelObjectY, width = TextLabelObjectWidth, height = TextLabelObjectHeight)

        LabelObject = Label(text = LabelObjectText, relief = RIDGE, width = 6, bg = Color, anchor = Anchor)
        LabelObject.pack()
        LabelObject.place(x = LabelObjectX, y = LabelObjectY, width = LabelObjectWidth, height = LabelObjectHeight)

        return [TextLabelObject, LabelObject]

    @classmethod
    def DrawButton(cls, Text, Color, ButtonX, ButtonY, ButtonWidth, ButtonHeight, Command):

        ButtonObject = Button(text = Text, width = 6, bg = Color, command = Command)
        ButtonObject.pack()
        ButtonObject.place(x = ButtonX, y = ButtonY, width = ButtonWidth, height = ButtonHeight)

        return ButtonObject

    def DrawCheckbutton(self, Text, CheckbuttonObjectValue, CheckbuttonX, CheckbuttonY, CheckbuttonWidth, CheckbuttonHeight, State, Command):

        CheckbuttonObjectVar = IntVar()
        CheckbuttonObjectVar.set(int(CheckbuttonObjectValue))

        CheckbuttonObject = Checkbutton(self.TkRoot, text = Text, variable = CheckbuttonObjectVar, onvalue = 1, offvalue = 0, anchor = "w", command = Command)
        CheckbuttonObject.pack()
        CheckbuttonObject.place(x = CheckbuttonX, y = CheckbuttonY, width = CheckbuttonWidth, height = CheckbuttonHeight)
        CheckbuttonObject["state"] = State

        return [CheckbuttonObject, CheckbuttonObjectVar]

    def __ListOfEnginesListboxClick(self, ListOfEnginesListboxClickEmptyEvent):
        try:
            self.SelectedChessEngineIndex = self.ListOfEnginesListbox.curselection()[0]
            self.SelectedChessEngineLabel.config(text = self.ListOfEnginesAndConfigsToRun[self.SelectedChessEngineIndex][2])
        except:
            True

    def DrawListbox(self, ListboxX, ListboxY, ListboxWidth, ListboxHeight, BackgroundColor, ForegroundColor):

        ListboxObject = Listbox(self.TkRoot, height = 10, width = 20, bg = BackgroundColor, activestyle = "dotbox", font = "Helvetica", fg = ForegroundColor, selectmode = "SINGLE")

        ScrollbarObject = Scrollbar(ListboxObject)
        ScrollbarObject.pack(side = RIGHT, fill = BOTH)
        ListboxObject.config(yscrollcommand = ScrollbarObject.set)
        ScrollbarObject.config(command = ListboxObject.yview)

        ListboxObject.pack()
        ListboxObject.place(x = ListboxX, y = ListboxY, width = ListboxWidth, height = ListboxHeight)

        return ListboxObject

    def DrawEntrybox(self, EntryboxX, EntryboxY, EntryboxWidth, EntryboxHeight, StartText, TextLabelObjectText, TextLabelObjectX, TextLabelObjectY, TextLabelObjectWidth, TextLabelObjectHeight, Color):

        TextLabelObject = Label(text = TextLabelObjectText, relief = RIDGE, width = 6, bg = Color, anchor = "w")
        TextLabelObject.pack()
        TextLabelObject.place(x = TextLabelObjectX, y = TextLabelObjectY, width = TextLabelObjectWidth, height = TextLabelObjectHeight)

        EntryboxStringVar = StringVar(self.TkRoot, value = StartText)

        EntryboxObject = Entry(self.TkRoot, textvariable = EntryboxStringVar)
        EntryboxObject.pack()
        EntryboxObject.place(x = EntryboxX, y = EntryboxY + 1, width = EntryboxWidth - 2, height = EntryboxHeight - 4)

        return [EntryboxObject, EntryboxStringVar]

    @classmethod
    def LimitTheNumberOfCharactersTo3inEntryBox(cls, EntryBoxStringVar):
        if len(EntryBoxStringVar.get()) > 3:
            EntryBoxStringVar.set(EntryBoxStringVar.get()[0:1])
        try:
            int(EntryBoxStringVar.get())
        except:
            EntryBoxStringVar.set("0")

    def LimitTheNumberOfCharactersTo1inEntryBox(self, EntryBoxStringVar):
        if len(EntryBoxStringVar.get()) > 0:
            EntryBoxStringVar.set(EntryBoxStringVar.get()[-1:])

        try:
            int(EntryBoxStringVar.get())
        except:
            EntryBoxStringVar.set("0")

        if int(EntryBoxStringVar.get()) < 0:
            EntryBoxStringVar.set("0")
        if int(EntryBoxStringVar.get()) >= self.ListOfEnginesListbox.size():
            EntryBoxStringVar.set("0")

    def DrawChessWindowObjects(self, UseEngine):

        [self.ResultTextLabel, self.ResultLabel] = self.DrawTextLabel(TextLabelObjectText = "RESULT", TextLabelObjectX = 590, TextLabelObjectY = 32, TextLabelObjectWidth = 370 / 2, TextLabelObjectHeight = 30, LabelObjectText = ChessInterfaceConstants.GAME_ONGOING, LabelObjectX =590 + 370 / 2, LabelObjectY = 32, LabelObjectWidth =370 / 2, LabelObjectHeight = 30, Color ="azure", Anchor ="center")

        [self.CurrentMovePlayerColorTextLabel, self.CurrentMovePlayerColorLabel] = self.DrawTextLabel(TextLabelObjectText = " CURRENT MOVE PLAYER COLOR", TextLabelObjectX = 590, TextLabelObjectY = 72, TextLabelObjectWidth = 370 / 2, TextLabelObjectHeight = 30, LabelObjectText = ChessInterfaceConstants.WHITE, LabelObjectX =590 + 370 / 2, LabelObjectY = 72, LabelObjectWidth =370 / 2, LabelObjectHeight = 30, Color ="lavender", Anchor ="center")

        FoundPathsY = 102
        [self.FoundPathTypeTextLabel, self.FoundPathTypeLabel] = self.DrawTextLabel(TextLabelObjectText = "FOUND PATHS TYPE", TextLabelObjectX = 590, TextLabelObjectY = FoundPathsY, TextLabelObjectWidth = 370 / 2, TextLabelObjectHeight = 30, LabelObjectText = "NONE", LabelObjectX = 590 + 370 / 2, LabelObjectY = FoundPathsY, LabelObjectWidth = 370 / 2, LabelObjectHeight = 30, Color = "lavender", Anchor = "center")
        self.ChosenMovesListbox = self.DrawListbox(ListboxX = 590, ListboxY = FoundPathsY + 30, ListboxWidth = 370, ListboxHeight = 250, BackgroundColor = "grey", ForegroundColor = "yellow")

        SegmentY = 412 - 40

        [self.PrintAllFoundPathsToMateCheckbutton, self.PrintAllFoundPathsToMateVar] = self.DrawCheckbutton(Text = "Print All Found Paths To Mate", CheckbuttonObjectValue = self.PrintAllFoundPathsToMate, CheckbuttonX = 590, CheckbuttonY = SegmentY + 15, CheckbuttonWidth = 370 / 2, CheckbuttonHeight = 30, State = "normal", Command = self.__PrintAllFoundPathsToMateClick)
        self.ChosenMovesShowHide()
        [self.UseEngineCheckbutton, self.UseEngineVar] = self.DrawCheckbutton(Text = "Use engine", CheckbuttonObjectValue = UseEngine, CheckbuttonX = 590, CheckbuttonY = SegmentY + 40, CheckbuttonWidth = 370 / 2, CheckbuttonHeight = 30, State = "disabled", Command = None)
        [self.FirstMoveInNewGameFromEngineCheckbutton, self.FirstMoveInNewGameFromEngineVar] = self.DrawCheckbutton(Text = "First move in new game from engine", CheckbuttonObjectValue = 0, CheckbuttonX = 590, CheckbuttonY = SegmentY + 65, CheckbuttonWidth = 370 / 2 + 100, CheckbuttonHeight = 30, State = "disabled", Command = None)

        self.NewGameButton = self.DrawButton(Text = "NEW GAME", Color = "OliveDrab1", ButtonX = 590, ButtonY = SegmentY + 105, ButtonWidth = 370 / 2, ButtonHeight = 30,  Command = self.__NewGameClick)
        self.NewGameButton["state"] = "disabled"
        self.StartStopGameButton = self.DrawButton(Text = "STOP GAME", Color = "khaki1", ButtonX = 590 + 370 / 2, ButtonY = SegmentY + 105, ButtonWidth = 370 / 2, ButtonHeight = 30, Command = self.__StartStopGameClick)

        self.LoadGameFromFileButton = self.DrawButton(Text = "LOAD GAME FROM FILE", Color = "khaki1", ButtonX = 590 + 370 / 2, ButtonY = SegmentY + 150, ButtonWidth = 370 / 2, ButtonHeight = 30, Command = self.__LoadGameFromFile)

        self.NewTournamentButton = self.DrawButton(Text = "NEW TOURNAMENT", Color = "green yellow", ButtonX = 590, ButtonY = SegmentY + 150, ButtonWidth = 370 / 2, ButtonHeight = 30, Command = self.__NewTournamentClick)
        [self.TournamentGamesOnlyBetweenDifferentEnginesCheckbutton, self.TournamentGamesOnlyBetweenDifferentEnginesVar] = self.DrawCheckbutton(Text = "Tournament games only between different engines", CheckbuttonObjectValue = 1, CheckbuttonX = 590, CheckbuttonY = SegmentY + 185, CheckbuttonWidth = 370, CheckbuttonHeight = 30, State = "normal", Command = None)
        self.TournamentResultsListbox = self.DrawListbox(ListboxX = 990, ListboxY = 32, ListboxWidth = 570, ListboxHeight = 870, BackgroundColor = "grey", ForegroundColor = "yellow")
        [self.TournamentStatusTextLabel, self.TournamentStatusLabel] = self.DrawTextLabel(TextLabelObjectText = "TOURNAMENT STATUS", TextLabelObjectX = 590, TextLabelObjectY = SegmentY + 320, TextLabelObjectWidth = 370 / 2, TextLabelObjectHeight = 30, LabelObjectText = "NONE", LabelObjectX = 590 + 370 / 2, LabelObjectY = SegmentY + 320, LabelObjectWidth = 370 / 2, LabelObjectHeight = 30, Color = "lavender", Anchor = "center")

        self.ListOfAllMovesInGameListbox = self.DrawListbox(ListboxX = 30, ListboxY = SegmentY + 220, ListboxWidth = 930 - 370 - 30, ListboxHeight = 250 + 100 - 3, BackgroundColor = "grey", ForegroundColor = "yellow")

        self.ListOfEnginesListbox = self.DrawListbox(ListboxX = 590, ListboxY = SegmentY + 220, ListboxWidth = 370, ListboxHeight = 60, BackgroundColor = "grey", ForegroundColor = "yellow")
        self.ListOfEnginesListbox.bind("<<ListboxSelect>>", self.__ListOfEnginesListboxClick)
        [self.SelectedChessEngineTextLabel, self.SelectedChessEngineLabel] = self.DrawTextLabel(TextLabelObjectText = "SELECTED CHESS ENGINE", TextLabelObjectX = 590, TextLabelObjectY = SegmentY + 280, TextLabelObjectWidth = 370 / 2, TextLabelObjectHeight = 30, LabelObjectText = "0", LabelObjectX = 590 + 370 / 2, LabelObjectY = SegmentY + 280, LabelObjectWidth = 370 / 2, LabelObjectHeight = 30, Color = "azure", Anchor = "center")

        [self.UseEngineRepeatNumberInOneTournamentEntrybox, self.UseEngineRepeatNumberInOneTournamentEntryboxVar] = self.DrawEntrybox(EntryboxX = 590 + 370 - 370 / 8, EntryboxY = SegmentY + 360, EntryboxWidth = 370 / 8, EntryboxHeight = 30, StartText = "1", TextLabelObjectText = " Use engine repeat number in one tournament", TextLabelObjectX = 590, TextLabelObjectY = SegmentY + 360, TextLabelObjectWidth = 370, TextLabelObjectHeight = 30, Color = "gray99")
        self.UseEngineRepeatNumberInOneTournamentEntryboxVar.trace("w", lambda *args: self.LimitTheNumberOfCharactersTo3inEntryBox(self.UseEngineRepeatNumberInOneTournamentEntryboxVar))

        [self.EngineNumberForOpponentEntrybox, self.EngineNumberForOpponentEntryboxVar] = self.DrawEntrybox(EntryboxX = 590 + 370 - 370 / 8 , EntryboxY = SegmentY + 395 , EntryboxWidth = 370 / 8, EntryboxHeight = 30, StartText = "0", TextLabelObjectText = " Engine number for opponent partner", TextLabelObjectX = 590, TextLabelObjectY = SegmentY + 395, TextLabelObjectWidth = 370, TextLabelObjectHeight = 30, Color = "gray99")
        self.EngineNumberForOpponentEntryboxVar.trace("w", lambda *args: self.LimitTheNumberOfCharactersTo1inEntryBox(self.EngineNumberForOpponentEntryboxVar))

        [self.ForceStalemateAfterNumberOfMovesEntrybox, self.ForceStalemateAfterNumberOfMovesEntryboxVar] = self.DrawEntrybox(EntryboxX = 590 + 370 - 370 / 8 , EntryboxY = SegmentY + 435, EntryboxWidth = 370 / 8, EntryboxHeight = 30, StartText = "999", TextLabelObjectText = " Force stalemate after number of moves", TextLabelObjectX = 590, TextLabelObjectY = SegmentY + 435, TextLabelObjectWidth = 370, TextLabelObjectHeight = 30, Color = "linen")
        self.ForceStalemateAfterNumberOfMovesEntryboxVar.trace("w", lambda *args: self.LimitTheNumberOfCharactersTo3inEntryBox(self.ForceStalemateAfterNumberOfMovesEntryboxVar))

        [self.OnWhichMoveStopWhenGameReadFromFileEntrybox, self.OnWhichMoveStopWhenGameReadFromFileEntryboxVar] = self.DrawEntrybox(EntryboxX = 590 + 370 - 370 / 8 , EntryboxY = SegmentY + 475, EntryboxWidth = 370 / 8, EntryboxHeight = 30, StartText = "999", TextLabelObjectText = " On which move stop when game read from file", TextLabelObjectX = 590, TextLabelObjectY = SegmentY + 475, TextLabelObjectWidth = 370, TextLabelObjectHeight = 30, Color = "light yellow")
        self.OnWhichMoveStopWhenGameReadFromFileEntryboxVar.trace("w", lambda *args: self.LimitTheNumberOfCharactersTo3inEntryBox(self.OnWhichMoveStopWhenGameReadFromFileEntryboxVar))

        [self.AskForContinueAfterEveryMoveWhenGameReadFromFileCheckbutton, self.AskForContinueAfterEveryMoveWhenGameReadFromFileVar] = self.DrawCheckbutton(Text = "Ask for continue after every move when game read from file", CheckbuttonObjectValue = 1, CheckbuttonX = 590, CheckbuttonY = SegmentY + 515, CheckbuttonWidth = 370, CheckbuttonHeight = 30, State = "normal", Command = None)

        [self.ReadChosenMovesPathsWhenGameReadFromFileCheckbutton, self.ReadChosenMovesPathsWhenGameReadFromFileVar] = self.DrawCheckbutton(Text = "Read chosen moves paths when game read from file", CheckbuttonObjectValue = 1, CheckbuttonX = 590, CheckbuttonY = SegmentY + 545, CheckbuttonWidth = 370, CheckbuttonHeight = 30, State = "normal", Command = None)

        self.CloseAllApplicationsButton = self.DrawButton(Text = "CLOSE ALL APPLICATIONS", Color = "cyan2", ButtonX = 990, ButtonY = SegmentY + 150 + 250 + 40 + 100, ButtonWidth = 570, ButtonHeight = 30, Command = self.__CloseAllApplicationsClick)

    def __init__(self, Mode, UseEngine, InvisibleWindow):

        self.PrintAllFoundPathsToMate = 1

        ChessInterfaceFiles.ReadColorsFromChessInterfaceConfigFile()
        ChessInterfaceFiles.ReadNetworkParametersFromChessInterfaceConfigFile()
        ChessInterfaceFiles.ReadAllFoundPathsToMateParametersFromChessInterfaceConfigFile(self)

        self.ChessInterfaceEngineObject = None
        self.TournamentStopped = True
        self.TournamentGameIndex = 0
        self.TournamentPartnerEngineIndex = 0
        self.TournamentResultsList = []

        self.GameStopped = False
        self.Mode = Mode

        self.TkRoot = Tk()
        self.TkRoot.title("CHESS INTERFACE PROJECT | Mode = " + Mode + " | Network Parameters [Host = " + NetworkConfig.ClientHost + " Port = " + str(NetworkConfig.Port) + "] |")

        if Mode == ChessInterfaceConstants.server and InvisibleWindow is True:
            self.TkRoot.withdraw()

        self.TkRoot.resizable(False, False)

        self.Canvas = Canvas(self.TkRoot, width = 985 + 600, height = 585 + 300 - 30 + 100)
        self.Canvas.pack()

        self.PutWindowInTheCenterOfScreen()

        self.DrawChessWindowObjects(UseEngine)

        self.ListOfEnginesAndConfigsToRun = []
        ChessInterfaceFiles.ReadListOfEnginesFromChessInterfaceConfigFile(self)
        self.__ListOfEnginesListboxClick(NONE)
        ChessInterfaceFiles.ReadNumberOfMovesToDeclareStalemateFromChessInterfaceConfigFile(self)

        self.ChessInterfaceEngineObject = ChessInterfaceEngine()
        self.RepeatMainLoop = True

        if Mode == ChessInterfaceConstants.server:
            self.ChessNetSocketObject = ChessNetSocketServer(self)
        else:
            self.ChessNetSocketObject = ChessNetSocketClient(self)
        self.ChessNetSocketObject.start()

        self.ChessInterfaceEngineObject.SetNetworkHandle(self.ChessNetSocketObject)

        self.NewGame()

        self.Canvas.bind("<Button-1>", self.__MouseClick)

        self.TkRoot.mainloop()

        self.CloseThisProcess()


    def MakeOpponentMove(self, PosFrom, PosTo, BitPassing):

        self.ChessInterfaceEngineObject.MakeOpponentMove(PosFrom, PosTo, BitPassing, self)
        self.RefreshFigure(PosFrom)
        self.RefreshFigure(PosTo)

        if BitPassing is not None:
            # castling
            if BitPassing == "o0":
                self.__MakeCastling(PosTo)
            # bit in passing
            else:
                self.RefreshFigure(BitPassing)

        self.UpdateFirstPlayerColorData(ChessInterfaceConstants.FIRST_OPPONENT_MOVE)
        ChessInterfaceFiles.SaveMoveInHistoryFiles(PosFrom, PosTo, self)

        return self.ChessInterfaceEngineObject.CheckIfThereIsEndOfGame(self)

    def RefreshAllFigures(self):
        for PosX in ChessInterfaceConstants.Cols:
            for PosY in ChessInterfaceConstants.Rows:
                self.RefreshFigure("{}{}".format(PosX, PosY))

    def RefreshFigure(self, PosFrom):
        # calculate the item_id of the figure based on coordinates
        ItemIndex = (ChessInterfaceConstants.Cols.index(PosFrom[0])) * 8 + ChessInterfaceConstants.Rows.index(PosFrom[1]) + 1
        self.Canvas.itemconfig(ItemIndex, image = self.ChessInterfaceEngineObject.ChessBoard[PosFrom[0], PosFrom[1]].Image)

    def __MakeCastling(self, PosTo):
        if PosTo[0] == "c":
            self.RefreshFigure("a" + PosTo[1])
            self.RefreshFigure("d" + PosTo[1])
        else:
            self.RefreshFigure("h" + PosTo[1])
            self.RefreshFigure("f" + PosTo[1])

    @classmethod
    def DrawInformation(cls, InformationStr):
        print(InformationStr, "\n")
        messagebox.showinfo(ChessInterfaceConstants.Information, InformationStr)

    def __MouseClick(self, Event):

        if self.UseEngineVar.get() == 1 and self.FirstMove is False or (self.FirstMove is True and self.FirstMoveInNewGameFromEngineVar.get() == 1):
            self.DrawInformation("User cannot change position! Engine makes moves!")
            return

        if self.GameStopped is True:
            self.DrawInformation("Can not make move because game is stopped!")
            return

        # decoding board area based on mouse coordinates
        DecodedPosX = (Event.x - self.ChessInterfaceEngineObject.ChessBoardDrawStartX + 33) // self.ChessInterfaceEngineObject.ChessBoardFieldSquareDimension
        DecodedPosY = (Event.y - self.ChessInterfaceEngineObject.ChessBoardDrawStartY + 33) // self.ChessInterfaceEngineObject.ChessBoardFieldSquareDimension

        # selecting/deselecting figure
        if (DecodedPosX >= 0) and (DecodedPosX <= 7) and (DecodedPosY >= 0) and (DecodedPosY <= 7) and (self.FirstMove is True or (self.CurrentPlayer == ChessInterfaceConstants.WHITE and self.Mode == ChessInterfaceConstants.client) or (self.CurrentPlayer == ChessInterfaceConstants.BLACK and self.Mode == ChessInterfaceConstants.server)):
            if self.Canvas.find_withtag(CURRENT):

                PosX = ChessInterfaceConstants.Cols[DecodedPosX]
                PosY = ChessInterfaceConstants.RowsInverted[DecodedPosY]

                # selecting
                if (self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Selected is False) and (self.ChessInterfaceEngineObject.SelectedFigure is False) and (self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD) is False):
                    if self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetFigure()[len(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetFigure()) - 1] == self.ChessInterfaceEngineObject.Player[0]:
                        self.ChessInterfaceEngineObject.SelectFigure(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].FieldDescriptionStr)
                        self.Canvas.itemconfig(CURRENT, image = self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Image)
                        # self.canvas.update_idletasks()

                # deselecting
                elif self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Selected is True:
                    if self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetFigure()[len(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].GetFigure()) - 1] == self.ChessInterfaceEngineObject.Player[0]:
                        self.ChessInterfaceEngineObject.UnselectFigure(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].FieldDescriptionStr)
                        self.Canvas.itemconfig(CURRENT, image = self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Image)

                # do Move if it"s correct
                elif self.ChessInterfaceEngineObject.SelectedFigure is True:

                    ReturnValue = self.ChessInterfaceEngineObject.MakeMove(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].FieldDescriptionStr, self, True)
                    if ReturnValue["Result"]:
                        #bitpass = self.ChessInterfaceEngineObject.MakeMove(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].FieldDescriptionStr, self)
                        self.ChessInterfaceEngineObject.MakeMove(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].FieldDescriptionStr, self, True)

                        self.Canvas.itemconfig(CURRENT, image = self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Image)
                        self.RefreshFigure(self.ChessInterfaceEngineObject.SelectedPos)
                        # bit in passing
                        if ReturnValue["BitPassing"] is not None:
                            self.RefreshFigure(ReturnValue["BitPassing"])
                        # castling
                        if ReturnValue["Castling"] is True:
                            self.__MakeCastling(self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].FieldDescriptionStr)
                else:
                    self.Canvas.itemconfig(CURRENT, image = self.ChessInterfaceEngineObject.ChessBoard[PosX, PosY].Image)