from ChessInterfaceFigure import ChessInterfaceFigure
from ChessInterfaceColors import ChessInterfaceColors
from ChessInterfaceConstants import ChessInterfaceConstants

import ChessInterfaceFiles

import math

class ChessArea:

    def __init__(self, FieldDescriptionStr, Figure, Background, DrawPosX, DrawPosY):
        self.FieldDescriptionStr = FieldDescriptionStr
        self.__Figure = Figure
        self.Selected = False
        self.Background = Background
        self.CanCastling = False
        self.DrawPosX = DrawPosX
        self.DrawPosY = DrawPosY
        self.__CreateImage()

    def SetFigure(self, Figure):
        self.__Figure = Figure
        self.Selected = False
        self.__CreateImage()

    def GetFigure(self):
        return self.__Figure

    def CheckFigure(self, Figure):
        if self.__Figure == Figure:
            return True
        else:
            return False

    def GetFigureColor(self):
        if self.__Figure[len(self.__Figure) - 1] == "W":
            return ChessInterfaceColors.WhitePiecesColor
        elif self.__Figure[len(self.__Figure) - 1] == "B":
            return ChessInterfaceColors.BlackPiecesColor
        else:
            return None

    def GetFigureName(self):
        return self.__Figure[0:len(self.__Figure) - 2]

    def GetEngineFigureNameFromInterfaceName(self):

        ChessInterfaceFigureName = self.GetFigureName()
        ChessEngineFigureName = ""
        if ChessInterfaceFigureName == ChessInterfaceConstants.EMPTY:
            ChessEngineFigureName = "  "
        elif ChessInterfaceFigureName == ChessInterfaceConstants.PAWN:
            ChessEngineFigureName = "p0"
        elif ChessInterfaceFigureName == ChessInterfaceConstants.KNIGHT:
            ChessEngineFigureName = "s0"
        elif ChessInterfaceFigureName == ChessInterfaceConstants.BISHOP:
            ChessEngineFigureName = "g0"
        elif ChessInterfaceFigureName == ChessInterfaceConstants.ROOK:
            ChessEngineFigureName = "w0"
        elif ChessInterfaceFigureName == ChessInterfaceConstants.QUEEN:
            ChessEngineFigureName = "q "
        elif ChessInterfaceFigureName == ChessInterfaceConstants.KING:
            ChessEngineFigureName = "k "

        if self.GetFigureColor() == ChessInterfaceColors.WhitePiecesColor:
            ChessEngineFigureName = ChessEngineFigureName.upper()

        return ChessEngineFigureName

    def __CreateImage(self):
        color = self.GetFigureColor()
        self.Image = ChessInterfaceFigure.CreateFigure(self.GetFigureName(), self.Background, color, self.Selected)

    def Select(self):
        self.Selected = True
        self.__CreateImage()

    def Unselect(self):
        self.Selected = False
        self.__CreateImage()


class ChessRules:

    @classmethod
    def IsCheck(cls, ChessBoard, PlayerColor, KingPos = None):

        if PlayerColor == ChessInterfaceColors.WhitePiecesColor:
            King = ChessInterfaceConstants.KING_WHITE
        else:
            King = ChessInterfaceConstants.KING_BLACK

        # find the king
        if KingPos is None:
            for KingPosX in ChessInterfaceConstants.Cols:
                for KingPosY in ChessInterfaceConstants.Rows:
                    if ChessBoard[KingPosX, KingPosY].CheckFigure(King):
                        break
                if ChessBoard[KingPosX, KingPosY].CheckFigure(King):
                    break
        else:
            KingPosX = KingPos[0]
            KingPosY = KingPos[1]

        # check king
        for PosX in range(-1, 2):
            for PosY in range(-1, 2):
                if (PosX == 0) and (PosY == 0):
                    continue

                ResultValue = cls.__SearchDirection(ChessBoard, (ChessInterfaceConstants.Cols.index(KingPosX), ChessInterfaceConstants.Rows.index(KingPosY)), (PosX, PosY), PlayerColor)
                if ResultValue is not None:
                    SearchedPosX = ChessInterfaceConstants.Cols[ResultValue[0]]
                    SearchedPosY = ChessInterfaceConstants.Rows[ResultValue[1]]

                    if (PosX == 0) or (PosY == 0):
                        if (ChessBoard[SearchedPosX, SearchedPosY].GetFigureName() == ChessInterfaceConstants.QUEEN) or (ChessBoard[SearchedPosX, SearchedPosY].GetFigureName() == ChessInterfaceConstants.ROOK):
                            return True
                    else:
                        if (ChessBoard[SearchedPosX, SearchedPosY].GetFigureName() == ChessInterfaceConstants.QUEEN) or (ChessBoard[SearchedPosX, SearchedPosY].GetFigureName() == ChessInterfaceConstants.BISHOP):
                            return True
                        elif ChessBoard[SearchedPosX, SearchedPosY].GetFigureName() == ChessInterfaceConstants.PAWN:
                            if (PlayerColor == ChessInterfaceColors.WhitePiecesColor) and (ResultValue[1] == ChessInterfaceConstants.Rows.index(KingPosY) + 1):
                                return True
                            elif (PlayerColor == ChessInterfaceColors.BlackPiecesColor) and (ResultValue[1] == ChessInterfaceConstants.Rows.index(KingPosY) - 1):
                                return True
        # check knight
        for PosX in range(-2, 3):
            for PosY in range(-2, 3):
                SearchedPosX = ChessInterfaceConstants.Cols.index(KingPosX) + PosX
                SearchedPosY = ChessInterfaceConstants.Rows.index(KingPosY) + PosY
                if ((SearchedPosX >= 0) and (SearchedPosX <= 7) and (SearchedPosY >= 0) and (SearchedPosY <= 7)) and (PosX != 0) and (PosY != 0):
                    if (ChessBoard[ChessInterfaceConstants.Cols[SearchedPosX], ChessInterfaceConstants.Rows[SearchedPosY]].Background != ChessBoard[KingPosX, KingPosY].Background) and ((math.fabs(PosX) + math.fabs(PosY)) == 3):
                        if PlayerColor == ChessInterfaceColors.WhitePiecesColor:
                            OpponentFigure = ChessInterfaceConstants.KNIGHT_BLACK
                        else:
                            OpponentFigure = ChessInterfaceConstants.KNIGHT_WHITE
                        if ChessBoard[ChessInterfaceConstants.Cols[SearchedPosX], ChessInterfaceConstants.Rows[SearchedPosY]].CheckFigure(OpponentFigure):
                            return True
        return False

    @classmethod
    # search until find the opponent s figure
    def __SearchDirection(cls, ChessBoard, Origin, Direction, PalyerColor):

        ResultValue = None

        PosX = Origin[0] + Direction[0]
        PosY = Origin[1] + Direction[1]

        if PalyerColor == ChessInterfaceColors.WhitePiecesColor:
            OpponentColor = ChessInterfaceColors.BlackPiecesColor
        else:
            OpponentColor =ChessInterfaceColors. WhitePiecesColor

        if (PosX >= 0) and (PosX <= 7) and (PosY >= 0) and (PosY <= 7):
            if ChessBoard[ChessInterfaceConstants.Cols[PosX], ChessInterfaceConstants.Rows[PosY]].GetFigureColor() == OpponentColor:
                ResultValue = (PosX, PosY)
            elif ChessBoard[ChessInterfaceConstants.Cols[PosX], ChessInterfaceConstants.Rows[PosY]].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD):
                ResultValue = cls.__SearchDirection(ChessBoard, (PosX, PosY), Direction, PalyerColor)

        return ResultValue

    @classmethod
    def IsBitInPassing(cls, ChessBoard, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        if ChessBoard[PosXFrom, PosYFrom].GetFigureColor() == ChessInterfaceColors.WhitePiecesColor:
            if (PosYFrom == "2") and (PosYTo == "4") and (PosXFrom == PosXTo):
                return PosTo
            else:
                return None
        elif ChessBoard[PosXFrom, PosYFrom].GetFigureColor() == ChessInterfaceColors.BlackPiecesColor:
            if (PosYFrom == "7") and (PosYTo == "5") and (PosXFrom == PosXTo):
                return PosTo
            else:
                return None
        else:
            return None

    @classmethod
    def IsTheOwnFigure(cls, ChessBoard, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        # the figure of the same color
        if ChessBoard[PosXTo, PosYTo].GetFigureColor() == ChessBoard[PosXFrom, PosYFrom].GetFigureColor():
            return True
        else:
            return False

    @classmethod
    def IsTheKing(cls, ChessBoard, PosTo):
        PosX = PosTo[0]
        PosY = PosTo[1]

        if ChessBoard[PosX, PosY].CheckFigure(ChessInterfaceConstants.KING_BLACK) or ChessBoard[PosX, PosY].CheckFigure(ChessInterfaceConstants.KING_WHITE):
            return True
        else:
            return False

    @classmethod
    def KingNeighborhood(cls, ChessBoard, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        for PosX in range(ChessInterfaceConstants.Cols.index(PosXTo) - 1, ChessInterfaceConstants.Cols.index(PosXTo) + 2):
            for PosY in range(ChessInterfaceConstants.Rows.index(PosYTo) - 1, ChessInterfaceConstants.Rows.index(PosYTo) + 2):
                if (PosX >= 0) and (PosX <= 7) and (PosY >= 0) and (PosY <= 7):
                    if not ((PosX == ChessInterfaceConstants.Cols.index(PosXFrom)) and (PosY == ChessInterfaceConstants.Rows.index(PosYFrom))):
                        if ChessBoard[ChessInterfaceConstants.Cols[PosX], ChessInterfaceConstants.Rows[PosY]].CheckFigure(ChessInterfaceConstants.KING_BLACK) or ChessBoard[ChessInterfaceConstants.Cols[PosX], ChessInterfaceConstants.Rows[PosY]].CheckFigure(ChessInterfaceConstants.KING_WHITE):
                            return True
        return False

    @classmethod
    def Knight(cls, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        if (((math.fabs(ChessInterfaceConstants.Cols.index(PosXFrom) - ChessInterfaceConstants.Cols.index(PosXTo)) == 2) and (math.fabs(ChessInterfaceConstants.Rows.index(PosYFrom) - ChessInterfaceConstants.Rows.index(PosYTo)) == 1))
            or
            ((math.fabs(ChessInterfaceConstants.Cols.index(PosXFrom) - ChessInterfaceConstants.Cols.index(PosXTo)) == 1) and (math.fabs(ChessInterfaceConstants.Rows.index(PosYFrom) - ChessInterfaceConstants.Rows.index(PosYTo)) == 2))):
            return True
        else:
            return False

    @classmethod
    def Bishop(cls, ChessBoard, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        # the same color of area and movement across
        if not ((((ChessInterfaceConstants.Cols.index(PosXFrom) + ChessInterfaceConstants.Rows.index(PosYFrom)) == (ChessInterfaceConstants.Cols.index(PosXTo) + ChessInterfaceConstants.Rows.index(PosYTo))) or ((ChessInterfaceConstants.Cols.index(PosXFrom) + ChessInterfaceConstants.Rows.index(PosYTo)) == (ChessInterfaceConstants.Rows.index(PosYFrom) + ChessInterfaceConstants.Cols.index(PosXTo)))) and (ChessBoard[PosXTo, PosYTo].Background == ChessBoard[PosXFrom, PosYFrom].Background)):
            return False

        if PosXFrom > PosXTo:
            PosXTo = PosFrom[0]
            PosYTo = PosFrom[1]
            PosXFrom = PosTo[0]
            PosYFrom = PosTo[1]
        PosY = ChessInterfaceConstants.Rows.index(PosYFrom)

        # areas on the bishop s road
        for PosX in range(ChessInterfaceConstants.Cols.index(PosXFrom) + 1, ChessInterfaceConstants.Cols.index(PosXTo)):
            if PosYFrom > PosYTo:
                PosY = PosY - 1
            else:
                PosY = PosY + 1
            if not ChessBoard[ChessInterfaceConstants.Cols[PosX], ChessInterfaceConstants.Rows[PosY]].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD):
                return False

        return True

    @classmethod
    def Rook(cls, ChessBoard, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        # horizontal line
        if PosYFrom == PosYTo:
            if PosXFrom > PosXTo:
                PosXFrom = PosTo[0]
                PosXTo = PosFrom[0]
            for PosX in range(ChessInterfaceConstants.Cols.index(PosXFrom) + 1, ChessInterfaceConstants.Cols.index(PosXTo)):
                if not ChessBoard[ChessInterfaceConstants.Cols[PosX], PosYFrom].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD):
                    return False
        # vertical line
        elif PosXFrom == PosXTo:
            if PosYFrom > PosYTo:
                PosYFrom = PosTo[1]
                PosYTo = PosFrom[1]
            for PosY in range(ChessInterfaceConstants.Rows.index(PosYFrom) + 1, ChessInterfaceConstants.Rows.index(PosYTo)):
                if not ChessBoard[PosXFrom, ChessInterfaceConstants.Rows[PosY]].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD):
                    return False
        else:
            return False
        ChessBoard[PosXFrom, PosYFrom].CanCastling = False
        return True

    @classmethod
    def Queen(cls, ChessBoard, PosFrom, PosTo):
        if cls.Rook(ChessBoard, PosFrom, PosTo) or cls.Bishop(ChessBoard, PosFrom, PosTo):
            return True
        else:
            return False

    @classmethod
    # isMoveCorrect , isCastling
    def King(cls, ChessBoard, PosFrom, PosTo):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        if cls.KingNeighborhood(ChessBoard, PosFrom, PosTo):
            return False, False

        if (math.fabs(ChessInterfaceConstants.Cols.index(PosXFrom) - ChessInterfaceConstants.Cols.index(PosXTo)) <= 1) and (math.fabs(ChessInterfaceConstants.Rows.index(PosYFrom) - ChessInterfaceConstants.Rows.index(PosYTo)) <= 1):
            ChessBoard[PosXFrom, PosYFrom].CanCastling = False
            return True, False
        # Castling
        elif (PosXFrom == "e") and (PosYFrom == PosYTo) and (ChessBoard[PosXFrom, PosYFrom].CanCastling is True) and ((PosXTo == "g") or (PosXTo == "c")):
            if (ChessBoard[PosXFrom, PosYFrom].GetFigureColor() == ChessInterfaceColors.WhitePiecesColor) and (PosYFrom == "1"):
                if PosXTo > PosXFrom:
                    if (ChessBoard["f", "1"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["g", "1"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["h", "1"].CheckFigure(ChessInterfaceConstants.ROOK_WHITE)) and (ChessBoard["h", "1"].CanCastling is True):
                        if (cls.IsCheck(ChessBoard, ChessInterfaceColors.WhitePiecesColor, "e1") is False) and (cls.IsCheck(ChessBoard, ChessInterfaceColors.WhitePiecesColor, "f1") is False):
                            return True, True
                else:
                    if (ChessBoard["d", "1"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["c", "1"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["b", "1"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["a", "1"].CheckFigure(ChessInterfaceConstants.ROOK_WHITE)) and (ChessBoard["a", "1"].CanCastling is True):
                        if (cls.IsCheck(ChessBoard, ChessInterfaceColors.WhitePiecesColor, "e1") is False) and (cls.IsCheck(ChessBoard, ChessInterfaceColors.WhitePiecesColor, "d1") is False):
                            return True, True
            elif (ChessBoard[PosXFrom, PosYFrom].GetFigureColor() == ChessInterfaceColors.BlackPiecesColor) and (PosYFrom == "8"):
                if PosXTo > PosXFrom:
                    if (ChessBoard["f", "8"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["g", "8"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["h", "8"].CheckFigure(ChessInterfaceConstants.ROOK_BLACK)) and (ChessBoard["h", "8"].CanCastling is True):
                        if (cls.IsCheck(ChessBoard, ChessInterfaceColors.BlackPiecesColor, "e8") is False) and (cls.IsCheck(ChessBoard, ChessInterfaceColors.BlackPiecesColor, "f8") is False):
                            return True, True
                else:
                    if (ChessBoard["d", "8"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["c", "8"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["b", "8"].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)) and (ChessBoard["a", "8"].CheckFigure(ChessInterfaceConstants.ROOK_BLACK)) and (ChessBoard["a", "8"].CanCastling is True):
                        if (cls.IsCheck(ChessBoard, ChessInterfaceColors.BlackPiecesColor, "e8") is False) and (cls.IsCheck(ChessBoard, ChessInterfaceColors.BlackPiecesColor, "d8") is False):
                            return True, True
        return False, False

    @classmethod
    # return IsMoveCorrect, IsBitInPassing
    def Pawn(cls, ChessBoard, PosFrom, PosTo, BitPassing):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        if ChessBoard[PosXFrom, PosYFrom].GetFigureColor() == ChessInterfaceColors.WhitePiecesColor:
            # move forward
            if (PosXFrom == PosXTo) and ((ChessInterfaceConstants.Rows.index(PosYTo) == ChessInterfaceConstants.Rows.index(PosYFrom) + 1) or ((PosYFrom == "2") and (PosYTo == "4"))) and (ChessBoard[PosXTo, PosYTo].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)):
                return True, False
            # bit
            elif (math.fabs(ChessInterfaceConstants.Cols.index(PosXTo) - ChessInterfaceConstants.Cols.index(PosXFrom)) == 1) and (ChessInterfaceConstants.Rows.index(PosYTo) == ChessInterfaceConstants.Rows.index(PosYFrom) + 1) and (ChessBoard[PosXTo, PosYTo].GetFigureColor() == ChessInterfaceColors.BlackPiecesColor):
                return True, False
            # bit in passing
            elif BitPassing is not None:
                if (BitPassing[1] == PosYFrom) and (math.fabs(ChessInterfaceConstants.Cols.index(BitPassing[0]) - ChessInterfaceConstants.Cols.index(PosXFrom)) == 1) and (ChessInterfaceConstants.Rows.index(PosYTo) == (ChessInterfaceConstants.Rows.index(BitPassing[1]) + 1)) and (ChessInterfaceConstants.Cols.index(PosXTo) == ChessInterfaceConstants.Cols.index(BitPassing[0])) and (ChessBoard[PosXTo, PosYTo].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)):
                    return True, True
                else:
                    return False, False
            else:
                return False, False
        else:
            # move forward
            if (PosXFrom == PosXTo) and ((ChessInterfaceConstants.Rows.index(PosYTo) == ChessInterfaceConstants.Rows.index(PosYFrom) - 1) or ((PosYFrom == "7") and (PosYTo == "5"))) and (ChessBoard[PosXTo, PosYTo].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)):
                return True, False
            # bit
            elif (math.fabs(ChessInterfaceConstants.Cols.index(PosXTo) - ChessInterfaceConstants.Cols.index(PosXFrom)) == 1) and (ChessInterfaceConstants.Rows.index(PosYTo) == ChessInterfaceConstants.Rows.index(PosYFrom) - 1) and (ChessBoard[PosXTo, PosYTo].GetFigureColor() == ChessInterfaceColors.WhitePiecesColor):
                return True, False
            # bit in passing
            elif BitPassing is not None:
                if (BitPassing[1] == PosYFrom) and (math.fabs(ChessInterfaceConstants.Cols.index(BitPassing[0]) - ChessInterfaceConstants.Cols.index(PosXFrom)) == 1) and (ChessInterfaceConstants.Rows.index(PosYTo) == (ChessInterfaceConstants.Rows.index(BitPassing[1]) - 1)) and (ChessInterfaceConstants.Cols.index(PosXTo) == ChessInterfaceConstants.Cols.index(BitPassing[0])) and (ChessBoard[PosXTo, PosYTo].CheckFigure(ChessInterfaceConstants.EMPTY_FIELD)):
                    return True, True
                else:
                    return False, False
            else:
                return False, False


class ChessInterfaceEngine:

    ChessBoard = {}

    def __init__(self):
        self.SelectedFigure = False
        self.SelectedPos = ""
        self.ChessNetSocketObject = None
        self.Player = None
        self.BitPassing = None
        self.BitFigure = None
        self.NumberOfMoves = 1

        self.ChessBoardDrawStartX = 65
        self.ChessBoardDrawStartY = 65
        self.ChessBoardFieldSquareDimension = 65

        ColNum = 0
        RowNum = 0
        BackgroundColor = ChessInterfaceColors.BackgroundFirstColor

        for PosX in ChessInterfaceConstants.Cols:
            for PosY in ChessInterfaceConstants.RowsInverted:
                self.ChessBoard[PosX, PosY] = ChessArea(PosX + PosY, ChessInterfaceConstants.EMPTY_FIELD, BackgroundColor, self.ChessBoardDrawStartX + ColNum * self.ChessBoardFieldSquareDimension, self.ChessBoardDrawStartY + RowNum * self.ChessBoardFieldSquareDimension)
                RowNum += 1
                if BackgroundColor == ChessInterfaceColors.BackgroundFirstColor:
                    BackgroundColor = ChessInterfaceColors.BackgroundSecondColor
                else:
                    BackgroundColor = ChessInterfaceColors.BackgroundFirstColor
            if BackgroundColor == ChessInterfaceColors.BackgroundFirstColor:
                BackgroundColor = ChessInterfaceColors.BackgroundSecondColor
            else:
                BackgroundColor = ChessInterfaceColors.BackgroundFirstColor
            ColNum += 1
            RowNum = 0

    def EmptyBoard(self):
        for PosX in ChessInterfaceConstants.Cols:
            for PosY in ChessInterfaceConstants.Rows:
                self.ChessBoard[PosX, PosY].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)

    def NewGame(self, Player):

        self.Player = Player
        self.BitPassing = None
        self.NumberOfMoves = 1

        self.SelectedFigure = False
        self.SelectedPos = None

        self.EmptyBoard()
        self.ChessBoard["a", "1"].SetFigure(ChessInterfaceConstants.ROOK_WHITE)
        self.ChessBoard["a", "1"].CanCastling = True
        self.ChessBoard["b", "1"].SetFigure(ChessInterfaceConstants.KNIGHT_WHITE)
        self.ChessBoard["c", "1"].SetFigure(ChessInterfaceConstants.BISHOP_WHITE)
        self.ChessBoard["d", "1"].SetFigure(ChessInterfaceConstants.QUEEN_WHITE)
        self.ChessBoard["e", "1"].SetFigure(ChessInterfaceConstants.KING_WHITE)
        self.ChessBoard["e", "1"].CanCastling = True
        self.ChessBoard["f", "1"].SetFigure(ChessInterfaceConstants.BISHOP_WHITE)
        self.ChessBoard["g", "1"].SetFigure(ChessInterfaceConstants.KNIGHT_WHITE)
        self.ChessBoard["h", "1"].SetFigure(ChessInterfaceConstants.ROOK_WHITE)
        self.ChessBoard["h", "1"].CanCastling = True
        for PosX in ChessInterfaceConstants.Cols:
            self.ChessBoard[PosX, "2"].SetFigure(ChessInterfaceConstants.PAWN_WHITE)
            self.ChessBoard[PosX, "7"].SetFigure(ChessInterfaceConstants.PAWN_BLACK)
        self.ChessBoard["a", "8"].SetFigure(ChessInterfaceConstants.ROOK_BLACK)
        self.ChessBoard["a", "8"].CanCastling = True
        self.ChessBoard["b", "8"].SetFigure(ChessInterfaceConstants.KNIGHT_BLACK)
        self.ChessBoard["c", "8"].SetFigure(ChessInterfaceConstants.BISHOP_BLACK)
        self.ChessBoard["d", "8"].SetFigure(ChessInterfaceConstants.QUEEN_BLACK)
        self.ChessBoard["e", "8"].SetFigure(ChessInterfaceConstants.KING_BLACK)
        self.ChessBoard["e", "8"].CanCastling = True
        self.ChessBoard["f", "8"].SetFigure(ChessInterfaceConstants.BISHOP_BLACK)
        self.ChessBoard["g", "8"].SetFigure(ChessInterfaceConstants.KNIGHT_BLACK)
        self.ChessBoard["h", "8"].SetFigure(ChessInterfaceConstants.ROOK_BLACK)
        self.ChessBoard["h", "8"].CanCastling = True

    def SelectFigure(self, Pos):
        PosX = Pos[0]
        PosY = Pos[1]
        self.ChessBoard[PosX, PosY].Select()
        self.SelectedFigure = True
        self.SelectedPos = Pos

    def UnselectFigure(self, Pos):
        PosX = Pos[0]
        PosY = Pos[1]
        self.ChessBoard[PosX, PosY].Unselect()
        self.SelectedFigure = False

    def __MakeMove(self, PosFrom, PosTo, BitPassing, Castling, CheckPawnPromotion):
        SelectedPosX = PosFrom[0]
        SelectedPosY = PosFrom[1]
        PosX = PosTo[0]
        PosY = PosTo[1]

        if BitPassing:
            self.ChessBoard[self.BitPassing[0], self.BitPassing[1]].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)

        if Castling:
            self.MakeCastling(PosTo)

        self.BitFigure = self.ChessBoard[PosX, PosY].GetFigure()
        self.ChessBoard[PosX, PosY].SetFigure(self.ChessBoard[SelectedPosX, SelectedPosY].GetFigure())
        self.ChessBoard[PosX, PosY].CanCastling = False
        self.ChessBoard[SelectedPosX, SelectedPosY].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)
        self.SelectedFigure = False

        # Pawn Promotion
        if CheckPawnPromotion is True:
            if self.ChessBoard[PosX, PosY].CheckFigure(ChessInterfaceConstants.PAWN_WHITE) and PosY == "8":
                self.ChessBoard[PosX, PosY].SetFigure(ChessInterfaceConstants.QUEEN_WHITE)
            elif self.ChessBoard[PosX, PosY].CheckFigure(ChessInterfaceConstants.PAWN_BLACK) and PosY == "1":
                self.ChessBoard[PosX, PosY].SetFigure(ChessInterfaceConstants.QUEEN_BLACK)

    def UnmakeCastling(self, PosTo):
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        if PosXTo == "c":
            self.ChessBoard["d", PosYTo].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)
            if PosYTo == "1":
                self.ChessBoard["a", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_WHITE)
            else:
                self.ChessBoard["a", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_BLACK)
        else:
            self.ChessBoard["f", PosYTo].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)
            if PosYTo == "1":
                self.ChessBoard["h", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_WHITE)
            else:
                self.ChessBoard["h", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_BLACK)

    def __UnmakeMove(self, PosFrom, PosTo, BitPassing, Castling, CanCastling = False):
        SelectedPosX = PosFrom[0]
        SelectedPosY = PosFrom[1]
        PosX = PosTo[0]
        PosY = PosTo[1]

        if BitPassing:
            if self.Player == ChessInterfaceConstants.WHITE:
                Pawn = ChessInterfaceConstants.PAWN_BLACK
            else:
                Pawn = ChessInterfaceConstants.PAWN_WHITE
            self.ChessBoard[self.BitPassing[0], self.BitPassing[1]].SetFigure(Pawn)

        self.ChessBoard[SelectedPosX, SelectedPosY].SetFigure(self.ChessBoard[PosX, PosY].GetFigure())
        self.ChessBoard[PosX, PosY].SetFigure(self.BitFigure)

        if self.ChessBoard[SelectedPosX, SelectedPosY].GetFigureName() == "KING" and CanCastling is True:
            self.ChessBoard[SelectedPosX, SelectedPosY].CanCastling = True

        if Castling:
            self.UnmakeCastling(PosTo)

    def MakeCastling(self, PosTo):
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        if PosXTo == "c":
            self.ChessBoard["a", PosYTo].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)
            if PosYTo == "1":
                self.ChessBoard["d", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_WHITE)
            else:
                self.ChessBoard["d", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_BLACK)
        else:
            self.ChessBoard["h", PosYTo].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)
            if PosYTo == "1":
                self.ChessBoard["f", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_WHITE)
            else:
                self.ChessBoard["f", PosYTo].SetFigure(ChessInterfaceConstants.ROOK_BLACK)

    def CheckIfNumberOfMovesExceededNumberForStalemate(self, ChessInterfaceFrameObject):
        return (self.NumberOfMoves > int(ChessInterfaceFrameObject.ForceStalemateAfterNumberOfMovesEntryboxVar.get()) and ((ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.server and ChessInterfaceFrameObject.FirstMoveColor == ChessInterfaceConstants.BLACK) or (ChessInterfaceFrameObject.Mode == ChessInterfaceConstants.client and ChessInterfaceFrameObject.FirstMoveColor == ChessInterfaceConstants.WHITE)))

    def CheckAdditionalStalemateCondition(self, ChessInterfaceFrameObject):
        if (len([Figure for Figure in [self.ChessBoard[PosX, PosY].GetEngineFigureNameFromInterfaceName()[0] for PosY in ChessInterfaceConstants.Rows for PosX in ChessInterfaceConstants.Cols]]) == 2) or self.CheckIfNumberOfMovesExceededNumberForStalemate(ChessInterfaceFrameObject) is True:
            ChessInterfaceFiles.MakeEndOfGame(ChessInterfaceFrameObject, ChessInterfaceConstants.STALEMATE)
            return True
        return False

    def CheckIfThereIsEndOfGame(self, ChessInterfaceFrameObject):
        if self.IsAnyMovePossible(ChessInterfaceConstants.WHITE, ChessInterfaceFrameObject) is False:
            if ChessRules.IsCheck(self.ChessBoard, ChessInterfaceColors.WhitePiecesColor) is True:
                ChessInterfaceFiles.MakeEndOfGame(ChessInterfaceFrameObject, ChessInterfaceConstants.MATE_BLACK_WINS)
            else:
                ChessInterfaceFiles.MakeEndOfGame(ChessInterfaceFrameObject, ChessInterfaceConstants.STALEMATE)
            return True

        if self.IsAnyMovePossible(ChessInterfaceConstants.BLACK, ChessInterfaceFrameObject) is False:
            if ChessRules.IsCheck(self.ChessBoard, ChessInterfaceColors.BlackPiecesColor) is True:
                ChessInterfaceFiles.MakeEndOfGame(ChessInterfaceFrameObject, ChessInterfaceConstants.MATE_WHITE_WINS)
            else:
                ChessInterfaceFiles.MakeEndOfGame(ChessInterfaceFrameObject, ChessInterfaceConstants.STALEMATE)
            return True

        return False

    def MakeOpponentMove(self, PosFrom, PosTo, BitPassing, ChessInterfaceFrameObject):
        PosXFrom = PosFrom[0]
        PosYFrom = PosFrom[1]
        PosXTo = PosTo[0]
        PosYTo = PosTo[1]

        self.BitPassing = ChessRules.IsBitInPassing(self.ChessBoard, PosFrom, PosTo)
        self.ChessBoard[PosXTo, PosTo[1]].SetFigure(self.ChessBoard[PosXFrom, PosYFrom].GetFigure())
        self.ChessBoard[PosXFrom, PosYFrom].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)
        self.ChessBoard[PosXTo, PosTo[1]].CanCastling = False
        if BitPassing is not None:
            # Castling
            if BitPassing == "o0":
                self.MakeCastling(PosTo)
            # bit in passing
            else:
                self.ChessBoard[BitPassing[0], BitPassing[1]].SetFigure(ChessInterfaceConstants.EMPTY_FIELD)

        # Pawn Promotion
        if self.ChessBoard[PosXTo, PosYTo].CheckFigure(ChessInterfaceConstants.PAWN_WHITE) and PosYTo == "8":
            self.ChessBoard[PosXTo, PosYTo].SetFigure(ChessInterfaceConstants.QUEEN_WHITE)
        elif self.ChessBoard[PosXTo, PosYTo].CheckFigure(ChessInterfaceConstants.PAWN_BLACK) and PosYTo == "1":
            self.ChessBoard[PosXTo, PosYTo].SetFigure(ChessInterfaceConstants.QUEEN_BLACK)

        self.NumberOfMoves += 1

        ChessInterfaceFrameObject.CurrentMovePlayerColorLabel.config(text = ChessInterfaceFrameObject.CurrentPlayer + " [Move " + str(self.NumberOfMoves) + "]")

    def SetNetworkHandle(self, net):
        self.ChessNetSocketObject = net

    def MakeMove(self, PosTo, ChessInterfaceFrameObject, SendMoveToNetwork):

        Result = False
        BitPassing = False
        Castling = False
        CanCastling = False
        ReturnValue = {"Result": False, "BitPassing": None, "Castling": False}

        if self.Player == ChessInterfaceConstants.WHITE:
            PlayerColor = ChessInterfaceColors.WhitePiecesColor
        else:
            PlayerColor = ChessInterfaceColors.BlackPiecesColor

        if ChessRules.IsTheOwnFigure(self.ChessBoard, self.SelectedPos, PosTo):
            return ReturnValue

        if ChessRules.IsTheKing(self.ChessBoard, PosTo):
            return ReturnValue

        Field = self.ChessBoard[self.SelectedPos[0], self.SelectedPos[1]]
        if Field.GetFigureName() == ChessInterfaceConstants.BISHOP:
            Result = ChessRules.Bishop(self.ChessBoard, self.SelectedPos, PosTo)
        elif Field.GetFigureName() == ChessInterfaceConstants.KNIGHT:
            Result = ChessRules.Knight(self.SelectedPos, PosTo)
        elif Field.GetFigureName() == ChessInterfaceConstants.ROOK:
            Result = ChessRules.Rook(self.ChessBoard, self.SelectedPos, PosTo)
        elif Field.GetFigureName() == ChessInterfaceConstants.QUEEN:
            Result = ChessRules.Queen(self.ChessBoard, self.SelectedPos, PosTo)
        elif Field.GetFigureName() == ChessInterfaceConstants.KING:
            CanCastling = Field.CanCastling
            Result, Castling = ChessRules.King(self.ChessBoard, self.SelectedPos, PosTo)
        elif Field.GetFigureName() == ChessInterfaceConstants.PAWN:
            Result, BitPassing = ChessRules.Pawn(self.ChessBoard, self.SelectedPos, PosTo, self.BitPassing)

        if Result:
            self.__MakeMove((self.SelectedPos[0], self.SelectedPos[1]), PosTo, BitPassing, Castling, True)
            IsCheck = ChessRules.IsCheck(self.ChessBoard, PlayerColor)
            ReturnValue["Result"] = True
            if not IsCheck:
                ReturnValue["Result"] = True

                ChessInterfaceFrameObject.UpdateFirstPlayerColorData(ChessInterfaceConstants.FIRST_MY_MOVE)
                ChessInterfaceFiles.SaveMoveInHistoryFiles(self.SelectedPos, PosTo, ChessInterfaceFrameObject)

                self.NumberOfMoves += 1

                if self.CheckAdditionalStalemateCondition(ChessInterfaceFrameObject) is True:
                    return ReturnValue

                if SendMoveToNetwork is True:
                    self.SendToNetwork(PosTo, BitPassing, Castling)
            else:
                self.__UnmakeMove((self.SelectedPos[0], self.SelectedPos[1]), PosTo, BitPassing, Castling, CanCastling)
        else:
            return ReturnValue

        if Castling:
            ReturnValue["Castling"] = True

        if BitPassing:
            ReturnValue["BitPassing"] = self.BitPassing

        ChessInterfaceFrameObject.FirstMove = False

        CheckIfThereIsEndOfGameResult = self.CheckIfThereIsEndOfGame(ChessInterfaceFrameObject)

        if CheckIfThereIsEndOfGameResult is False or (CheckIfThereIsEndOfGameResult is True and ChessInterfaceFrameObject.TournamentStopped is True):
            ChessInterfaceFrameObject.UpdateCurrentPlayerColorData()

        return ReturnValue

    def SendToNetwork(self, PosTo, BitPassing, Castling):
        if self.ChessNetSocketObject is not None:
            if BitPassing:
                self.ChessNetSocketObject.SendChessMove((self.SelectedPos, PosTo, self.BitPassing))
            elif Castling:
                self.ChessNetSocketObject.SendChessMove((self.SelectedPos, PosTo, "o0"))
            else:
                self.ChessNetSocketObject.SendChessMove((self.SelectedPos, PosTo))
        else:
            print("No network connection")


    def IsAnyMovePossible(self, Color, ChessInterfaceFrameObject):
        if Color == ChessInterfaceConstants.WHITE:
            PlayerColor = ChessInterfaceColors.WhitePiecesColor
        else:
            PlayerColor = ChessInterfaceColors.BlackPiecesColor

        BitPassing = False
        Castling = False
        CanCastling = False

        for PosY in ChessInterfaceConstants.Rows:
            for PosX in ChessInterfaceConstants.Cols:
                if self.ChessBoard[(PosX, PosY)].GetFigureName() != ChessInterfaceConstants.EMPTY and self.ChessBoard[(PosX, PosY)].GetFigureColor() == PlayerColor:

                    for PosYToMove in ChessInterfaceConstants.Rows:
                        for PosXToMove in ChessInterfaceConstants.Cols:
                            if not (PosX == PosXToMove and PosY == PosYToMove):

                                IsMovePossible = False

                                PosTo = (PosXToMove, PosYToMove)
                                if ChessRules.IsTheOwnFigure(self.ChessBoard, (PosX, PosY), PosTo) is True:
                                    continue
                                if ChessRules.IsTheKing(self.ChessBoard, PosTo) is True:
                                    continue

                                Field = self.ChessBoard[PosX, PosY]

                                if Field.GetFigureName() == ChessInterfaceConstants.BISHOP:
                                    if ChessRules.Bishop(self.ChessBoard, (PosX, PosY), PosTo) is True:
                                        IsMovePossible = True
                                elif Field.GetFigureName() == ChessInterfaceConstants.KNIGHT:
                                    if ChessRules.Knight((PosX, PosY), PosTo) is True:
                                        IsMovePossible = True
                                elif Field.GetFigureName() == ChessInterfaceConstants.ROOK:
                                    if ChessRules.Rook(self.ChessBoard, (PosX, PosY), PosTo) is True:
                                        IsMovePossible = True
                                elif Field.GetFigureName() == ChessInterfaceConstants.QUEEN:
                                    if ChessRules.Queen(self.ChessBoard, (PosX, PosY), PosTo) is True:
                                        IsMovePossible = True
                                elif Field.GetFigureName() == ChessInterfaceConstants.KING:
                                    CanCastling = Field.CanCastling
                                    IsMovePossibleK, Castling = ChessRules.King(self.ChessBoard, (PosX, PosY), PosTo)
                                    if IsMovePossibleK is True:
                                        IsMovePossible = True
                                elif Field.GetFigureName() == ChessInterfaceConstants.PAWN:
                                    IsMovePossibleP, BitPassing = ChessRules.Pawn(self.ChessBoard, (PosX, PosY), PosTo, self.BitPassing)
                                    if IsMovePossibleP is True:
                                        IsMovePossible = True

                                if IsMovePossible is True:
                                    self.__MakeMove((PosX, PosY), PosTo, BitPassing, Castling, False)
                                    if ChessRules.IsCheck(self.ChessBoard, PlayerColor) is False:
                                        self.__UnmakeMove((PosX, PosY), PosTo, BitPassing, Castling, CanCastling)
                                        ChessInterfaceFrameObject.RefreshFigure((PosX, PosY))
                                        ChessInterfaceFrameObject.RefreshFigure(PosTo)
                                        return True
                                    else:
                                        self.__UnmakeMove((PosX, PosY), PosTo, BitPassing, Castling, CanCastling)
                                        ChessInterfaceFrameObject.RefreshFigure((PosX, PosY))
                                        ChessInterfaceFrameObject.RefreshFigure(PosTo)

        ChessInterfaceFrameObject.RefreshAllFigures()
        return False