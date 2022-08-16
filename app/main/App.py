from collections import namedtuple
import sys
from typing import Tuple
from PySide6 import QtCore, QtWidgets, QtGui

from .components import CellsGrid

class App(QtWidgets.QMainWindow):
    GRID_ROWS = 30
    GRID_COLUMNS = 30

    def __init__(self):
        super().__init__()
        self.setObjectName('main')
        
        # self.addToolBar(QtWidgets.QToolBar(self))

        self.setWindowTitle('Floor Cleaning Map Tool')
        self.setCentralWidget(CellsGrid.CellsGrid(App.GRID_ROWS, App.GRID_COLUMNS))
        self._attach_menu()

        # self.layout = QtWidgets.QVBoxLayout(self)
        # self.layout.setContentsMargins(0, 0, 0, 0)
        # self.layout.setSpacing(0)
        # self.layout.addWidget()

    def _attach_menu(self):
        menu = QtWidgets.QMenu(self)
        menu.addAction('test')
        self.menuBar().addMenu('&File')


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = App()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(700, 700)
    widget.show()

    sys.exit(app.exec())