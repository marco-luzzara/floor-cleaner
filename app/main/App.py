import sys
from PySide6 import QtCore, QtWidgets

from .components import CellsGrid

class App(QtWidgets.QMainWindow):
    GRID_ROWS = 30
    GRID_COLUMNS = 30


    def __init__(self):
        super().__init__()
        self.setObjectName('main')
        
        # self.addToolBar(QtWidgets.QToolBar(self))

        self.setWindowTitle('Floor Cleaning Map Tool')
        self._create_grid()
        self._attach_menu()

        # self.layout = QtWidgets.QVBoxLayout(self)
        # self.layout.setContentsMargins(0, 0, 0, 0)
        # self.layout.setSpacing(0)
        # self.layout.addWidget()


    def _create_grid(self):
        self.setCentralWidget(CellsGrid.CellsGrid(App.GRID_ROWS, App.GRID_COLUMNS))


    def _attach_menu(self):
        # file menu
        fileMenu = self.menuBar().addMenu('&File')
        newAction = fileMenu.addAction('&New grid')
        newAction.triggered.connect(self._new_grid)

        exitAction = fileMenu.addAction('&Exit')
        exitAction.triggered.connect(self._exit)


    @QtCore.Slot()
    def _new_grid(self) -> None:
        self._create_grid()


    @QtCore.Slot()
    def _exit(self) -> None:
        self.close()


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = App()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(700, 700)
    widget.show()

    sys.exit(app.exec())