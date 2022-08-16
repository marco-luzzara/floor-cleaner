from collections import namedtuple
import sys
from typing import Tuple
from PySide6 import QtCore, QtWidgets, QtGui

from .components import Cell

GridCoord = namedtuple('GridCoord', ['x', 'y'])

class App(QtWidgets.QWidget):
    GRID_ROWS = 10
    GRID_COLUMNS = 10

    selection_end = QtCore.Signal(Cell.Cell, QtCore.QPointF)

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

        self.selection_end.connect(self._selection_end_handler)

    @QtCore.Slot(Cell.Cell)
    def _selection_end_handler(self, cell: Cell.Cell, pos: QtCore.QPointF) -> None:
        selection_end_cell = self._get_cell_from_pos(pos)
        selection_start_grid_coord = self._get_cell_grid_coordinates(cell)
        selection_end_grid_coord = self._get_cell_grid_coordinates(selection_end_cell)
        self._toggle_multiple_cells(selection_start_grid_coord, selection_end_grid_coord)
        

    # get Cell object from position
    def _get_cell_from_pos(self, pos: QtCore.QPoint) -> Cell.Cell:
        return next(cell for row in self.cells 
            for cell in row if cell.is_point_inside(pos))

    # get coordinates of the target cell in the grid
    def _get_cell_grid_coordinates(self, target_cell: Cell.Cell) -> GridCoord:
        grid_coord_raw = next((ir, ic) for ir, row in enumerate(self.cells)
            for ic, cell in enumerate(row) if cell is target_cell)
        return GridCoord(x=grid_coord_raw[1], y=grid_coord_raw[0])

    def _toggle_multiple_cells(self, 
            grid_coord1: GridCoord, 
            grid_coord2: GridCoord):
        for ir, row in enumerate(self.cells):
            for ic, cell in enumerate(row):
                if min(grid_coord1.x, grid_coord2.x) <= ic <= max(grid_coord1.x, grid_coord2.x) and \
                        min(grid_coord1.y, grid_coord2.y) <= ir <= max(grid_coord1.y, grid_coord2.y):
                    cell.toggle_selection()


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = App()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(800, 600)
    widget.show()

    sys.exit(app.exec())