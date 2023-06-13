import abc
from collections import namedtuple
from PySide6 import QtCore, QtWidgets

from main.components import Cell
from main.components.mapping import CellType
from main.util import MsgBoxUtil
from .mapping import MapManager

GridCoord = namedtuple('GridCoord', ['x', 'y'])


class CellsGrid(MapManager.MapManager, QtWidgets.QFrame):
    selection_end = QtCore.Signal(Cell.Cell, QtCore.QPointF)

    def __init__(self, row_count: int, column_count: int):
        super().__init__()

        self.setObjectName('cell_grid')

        self.setLayout(QtWidgets.QGridLayout())
        self.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.setFrameStyle(QtWidgets.QFrame.Plain)
        self.ROW_COUNT = row_count
        self.COLUMN_COUNT = column_count

        self._cells = [[Cell.Cell() for c in range(self.COLUMN_COUNT)]
                       for r in range(self.ROW_COUNT)]

        self.layout().setContentsMargins(0, 0, 0, 0)
        self.layout().setSpacing(0)

        for r in range(self.ROW_COUNT):
            for c in range(self.COLUMN_COUNT):
                self.layout().addWidget(self._cells[r][c], r, c)  # type:ignore

        self.selection_end.connect(self._selection_end_handler)

    @QtCore.Slot(Cell.Cell)
    def _selection_end_handler(self, cell: Cell.Cell, pos: QtCore.QPointF) -> None:
        # this point represents the offset from the top-left corner of the window
        # the mouse position references the global position
        inner_top_left = self.mapToGlobal(QtCore.QPoint(0, 0))

        selection_end_cell = self._get_cell_from_pos(
            QtCore.QPoint(int(pos.x()) - inner_top_left.x(), int(pos.y()) - inner_top_left.y()))
        selection_start_grid_coord = self._get_cell_grid_coordinates(cell)
        selection_end_grid_coord = self._get_cell_grid_coordinates(
            selection_end_cell)

        self._toggle_multiple_cells(
            selection_start_grid_coord, selection_end_grid_coord)

    def _get_cell_from_pos(self, pos: QtCore.QPoint) -> Cell.Cell:
        '''get Cell object from position'''
        return next(cell for row in self._cells
                    for cell in row if cell.is_point_inside(pos))

    def _get_cell_grid_coordinates(self, target_cell: Cell.Cell) -> GridCoord:
        '''get coordinates of the target cell in the grid'''
        grid_coord_raw = next((ir, ic) for ir, row in enumerate(self._cells)
                              for ic, cell in enumerate(row) if cell is target_cell)
        return GridCoord(x=grid_coord_raw[1], y=grid_coord_raw[0])

    def _toggle_multiple_cells(self,
                               grid_coord1: GridCoord,
                               grid_coord2: GridCoord):
        for ir, row in enumerate(self._cells):
            for ic, cell in enumerate(row):
                if min(grid_coord1.x, grid_coord2.x) <= ic <= max(grid_coord1.x, grid_coord2.x) and \
                        min(grid_coord1.y, grid_coord2.y) <= ir <= max(grid_coord1.y, grid_coord2.y):
                    cell.toggle_selection()

    def get_map(self) -> list[list[CellType.CellType]]:

        min_selected_row = next((i for i, row in enumerate(self._cells)
                                 if any(map(
                                     lambda cell: cell.type is not CellType.CellType.UNAVAILABLE, row))
                                 ), None)

        MsgBoxUtil.MsgBoxUtil.assert_with_box(min_selected_row is not None,
                                              'there must be at least one cell selected')

        MsgBoxUtil.MsgBoxUtil.assert_with_box(any(any(True for c in row
                                                      if c.type is CellType.CellType.CLEANER_POSITION)
                                                  for row in self._cells), 'There must be one cleaner positioned')

        min_selected_col = next((ic for ic, _ in enumerate(self._cells[0])
                                 for ir, row in enumerate(self._cells)
                                 if row[ic].type is not CellType.CellType.UNAVAILABLE), None)

        max_selected_row = next((len(self._cells) - i - 1
                                 for i, row in enumerate(reversed(self._cells))
                                 if any(map(lambda cell: cell.type is not CellType.CellType.UNAVAILABLE, row))))

        max_selected_col = next((len(self._cells) - ic - 1 for ic, _ in enumerate(self._cells[0])
                                 for ir, row in enumerate(self._cells)
                                 if row[len(self._cells) - ic - 1].type is not CellType.CellType.UNAVAILABLE))

        return list(map(lambda row: list(map(lambda cell: cell.type, row[min_selected_col:max_selected_col + 1])),
                        self._cells[min_selected_row:max_selected_row + 1]))

    def start_cells_cleaner_positioning(self):
        for row in self._cells:
            for cell in row:
                if cell.type is CellType.CellType.CLEANER_POSITION:
                    cell.toggle_selection()
                cell.toggle_cleaner_positioning()

    def end_cells_cleaner_positioning(self):
        for row in self._cells:
            for cell in row:
                cell.toggle_cleaner_positioning()
