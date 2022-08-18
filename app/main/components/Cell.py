from PySide6 import QtCore, QtWidgets, QtGui

from main.components.mapping import CellType

from . import CellsGrid


class Cell(QtWidgets.QFrame):
    def __init__(self):
        super().__init__()
        self.setObjectName('cell')

        self.type: CellType.CellType = CellType.CellType.UNAVAILABLE
        self._cleaner_positioning = False

    def is_point_inside(self, point: QtCore.QPoint) -> bool:
        return (self.x() <= point.x() <= self.x() + self.width()) and \
            (self.y() <= point.y() <= self.y() + self.height())

    def toggle_selection(self):
        self.type = CellType.CellType.TO_CLEAN if self.type is not CellType.CellType.TO_CLEAN \
            else CellType.CellType.UNAVAILABLE

        if self.type is CellType.CellType.TO_CLEAN:
            self.setStyleSheet('background-color: #9BF3FF')  # light blue
        else:
            self.setStyleSheet('background-color: #F5F5F5')  # default

    def toggle_cleaner_positioning(self):
        self._cleaner_positioning = not self._cleaner_positioning

    def mouseReleaseEvent(self, ev: QtGui.QMouseEvent) -> None:
        parent: CellsGrid.CellsGrid = self.parent()  # types:ignore
        if self._cleaner_positioning:
            self.type = CellType.CellType.CLEANER_POSITION
            self.setStyleSheet('background-color: black')

            parent.end_cells_cleaner_positioning()
            return

        parent.selection_end.emit(self, ev.globalPosition())
