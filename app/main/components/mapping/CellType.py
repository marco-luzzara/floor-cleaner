from enum import Enum


class CellType(Enum):
    UNAVAILABLE = 0
    TO_CLEAN = 1
    ALREADY_CLEANED = 2
    CLEANER_POSITION = 3
