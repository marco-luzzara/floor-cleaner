import sys
from PySide6 import QtCore, QtWidgets, QtGui
from .components import Cell

class App(QtWidgets.QWidget):
    GRID_ROWS = 10
    GRID_COLUMNS = 10
    cell_pressed = QtCore.Signal()

    def __init__(self):
        super().__init__()

        self.cells = [[Cell.Cell(self, 'text') for c in range(App.GRID_ROWS)] 
            for r in range(App.GRID_COLUMNS)]

        self.layout = QtWidgets.QGridLayout(self)
        self.layout.setContentsMargins(0,0,0,0)
        self.layout.setSpacing(0)

        for r in range(App.GRID_ROWS):
            self.layout.setRowStretch(r, 1)

        for c in range(App.GRID_COLUMNS):
            self.layout.setColumnStretch(c, 1)
        
        for r in range(0, 10):
            for c in range(0, 10):
                self.layout.addWidget(self.cells[r][c], r, c)

        self.cell_pressed.connect(self._cell_pressed)

    @QtCore.Slot()
    def _cell_pressed(self) -> None:
        print("done")

if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = App()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(800, 600)
    widget.show()

    sys.exit(app.exec())