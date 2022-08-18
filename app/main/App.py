import sys
from PySide6 import QtCore, QtWidgets

from main.components.mapping import MapManager
from main.util import MsgBoxUtil

from .components import CellsGrid, SendMapDialog


class App(QtWidgets.QMainWindow):
    GRID_ROWS = 30
    GRID_COLUMNS = 30

    def __init__(self):
        super().__init__()
        self.setObjectName('main')

        self.setWindowTitle('Floor Cleaning Map Tool')
        self._create_grid()
        self._attach_menu()

    def _create_grid(self):
        self.setCentralWidget(CellsGrid.CellsGrid(
            App.GRID_ROWS, App.GRID_COLUMNS))

    def _attach_menu(self):
        # file menu
        fileMenu = self.menuBar().addMenu('&File')
        actionNew = fileMenu.addAction('&New grid')
        actionNew.triggered.connect(self._new_grid)

        actionSend = fileMenu.addAction('&Send')
        actionSend.triggered.connect(self._send)

        actionExit = fileMenu.addAction('&Exit')
        actionExit.triggered.connect(self._exit)

        # robot menu
        cleanerMenu = self.menuBar().addMenu('&Cleaner')
        actionPlaceCleaner = cleanerMenu.addAction('&Place Cleaner')
        actionPlaceCleaner.triggered.connect(self._place_cleaner)

    @QtCore.Slot()
    def _new_grid(self) -> None:
        self._create_grid()

    @QtCore.Slot()
    def _exit(self) -> None:
        self.close()

    @QtCore.Slot()
    def _send(self) -> None:
        dialog_send = SendMapDialog.SendMapDialog(self, self.centralWidget())
        sendResult = dialog_send.exec()

        if sendResult == QtWidgets.QDialog.DialogCode.Accepted:
            MsgBoxUtil.MsgBoxUtil.info_box('Send successful')

    @QtCore.Slot()
    def _place_cleaner(self) -> None:
        cellsGrid: CellsGrid.CellsGrid = self.centralWidget()
        cellsGrid.start_cells_cleaner_positioning()

        MsgBoxUtil.MsgBoxUtil.info_box('Place the cleaner by clicking on a cell')


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = App()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(700, 700)
    widget.show()

    sys.exit(app.exec())
