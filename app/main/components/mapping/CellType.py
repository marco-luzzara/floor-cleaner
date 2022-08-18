from enum import Enum, auto


class CellType(Enum):
    UNAVAILABLE = auto()
    TO_CLEAN = auto()
    CLEANER_POSITION = auto()
