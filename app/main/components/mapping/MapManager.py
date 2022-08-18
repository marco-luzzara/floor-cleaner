from . import CellType


class MapManager:
    def get_map(self) -> list[list[CellType.CellType]]:
        raise TypeError('override get_map')

    def start_cells_cleaner_positioning(self):
        raise TypeError('override start_cells_cleaner_positioning')

    def end_cells_cleaner_positioning(self):
        raise TypeError('override end_cells_cleaner_positioning')
