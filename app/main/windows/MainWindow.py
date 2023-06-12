import copy
from PySide6 import QtCore, QtWidgets

from main.util import MsgBoxUtil
from main.components import CellsGrid, SendMapDialog
from main.windows.RealTimeCleaningWindow import RealTimeCleaningWindow


class MainWindow(QtWidgets.QMainWindow):
    GRID_ROWS = 30
    GRID_COLUMNS = 30

    def __init__(self):
        super().__init__()
        self.setObjectName('main')

        self.setWindowTitle('Floor Cleaning Map Tool')
        self._create_grid()
        self._attach_menu()

    def _create_grid(self):
        self.cells_grid = CellsGrid.CellsGrid(
            MainWindow.GRID_ROWS, MainWindow.GRID_COLUMNS)
        self.setCentralWidget(self.cells_grid)

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
        dialog_send = SendMapDialog.SendMapDialog(self, self.cells_grid)
        sendResult = dialog_send.exec()

        if sendResult == QtWidgets.QDialog.DialogCode.Accepted:
            MsgBoxUtil.MsgBoxUtil.assert_with_timed_box('Send successful', 2000)

            realtime_map = copy.deepcopy(self.cells_grid.get_map())
            realtime_cleaning_window = RealTimeCleaningWindow(realtime_map, self)
            realtime_cleaning_window.exec()

    @QtCore.Slot()
    def _place_cleaner(self) -> None:
        cellsGrid: CellsGrid.CellsGrid = self.centralWidget()
        cellsGrid.start_cells_cleaner_positioning()

        MsgBoxUtil.MsgBoxUtil.info_box('Place the cleaner by clicking on a cell')
