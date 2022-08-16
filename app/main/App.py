from collections import namedtuple
import sys
from typing import Tuple
from PySide6 import QtCore, QtWidgets, QtGui

from .components import CellsGrid

class App(QtWidgets.QWidget):
    GRID_ROWS = 10
    GRID_COLUMNS = 10

    def __init__(self):
        super().__init__()
        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(0)
        self.layout.addWidget(CellsGrid.CellsGrid(App.GRID_ROWS, App.GRID_COLUMNS))


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = App()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(800, 600)
    widget.show()

    sys.exit(app.exec())