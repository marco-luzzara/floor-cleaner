from types import CellType
from typing import List
from PySide6 import QtCore, QtWidgets


class RealTimeCleaningWindow(QtWidgets.QDialog):
    def __init__(self, cells_grid: List[List[CellType]], parent: QtWidgets.QWidget):
        super().__init__(parent)

        self.cells_grid = cells_grid

        self.setLayout(QtWidgets.QGridLayout())
        self.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.setFrameStyle(QtWidgets.QFrame.Plain)
        self.ROW_COUNT = len(self.cells_grid)
        self.COLUMN_COUNT = len(self.cells_grid[0])

        self._cells = [[QtWidgets.QFrame() for c in range(self.COLUMN_COUNT)]
                       for r in range(self.ROW_COUNT)]

        self.layout().setContentsMargins(0, 0, 0, 0)
        self.layout().setSpacing(0)

        for r in range(self.ROW_COUNT):
            for c in range(self.COLUMN_COUNT):
                self.layout().addWidget(self._cells[r][c], r, c)  # type:ignore
