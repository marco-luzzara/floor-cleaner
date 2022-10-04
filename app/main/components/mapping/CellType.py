from enum import Enum


class CellType(Enum):
    UNAVAILABLE = 0
    TO_CLEAN = 1
    # 2 is missing because ALREADY_CLEANED can be set by the cleaner only
    CLEANER_POSITION = 3
