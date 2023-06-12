from types import CellType
from typing import List
from PySide6 import QtCore, QtWidgets


class MapCleaningWindow(QtWidgets.QtWidget):
    def __init__(self, cells_grid: List[List[CellType]], parent: QtWidgets.QWidget):
        super().__init__(parent)

        self.cells_grid = cells_grid
