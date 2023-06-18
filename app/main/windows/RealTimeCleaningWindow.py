import ast
import asyncio
from collections import namedtuple
from dataclasses import dataclass
from typing import Any, Callable, Dict, List, ClassVar
from PySide6 import QtCore, QtWidgets
import serial

from main.components.SendMapDialog import SerialConnectionInfo
from main.components.mapping.CellType import CellType
from main.util import MsgBoxUtil

MapPosition = namedtuple('MapPosition', ['r', 'c'])


class Receiver:
    def move_cleaner(self, r: int, c: int):
        raise NotImplementedError()

    def mark_position_as_obstacle(self, r: int, c: int):
        raise NotImplementedError()


class Command:
    '''
    Each command has the following format:
    COMMAND_ID{"key": "value", ...}

    A command without key-value pairs is sent as 'COMMAND1{}',
    while a command with key-value pairs could be
    'COMMAND2{"r": 1, "c": 2}'
    '''
    id: str

    def __init__(self, receiver: Receiver):
        self.receiver = receiver

    def execute(self):
        raise NotImplementedError()


class StartCommand(Command):
    id: str = 'START'


class EndCommand(Command):
    id: str = 'END'


class WithPosition:
    def __init__(self, r: int, c: int):
        self.r = r
        self.c = c


class NewCleanerPositionCommand(Command, WithPosition):
    id: str = 'MOVE'

    def __init__(self, receiver: Receiver, r: int, c: int):
        Command.__init__(self, receiver)
        WithPosition.__init__(self, r, c)

    def execute(self):
        self.receiver.move_cleaner(self.r, self.c)


class ObstacleCommand(Command, WithPosition):
    id: str = 'OBSTACLE'

    def __init__(self, receiver: Receiver, r: int, c: int):
        Command.__init__(self, receiver)
        WithPosition.__init__(self, r, c)

    def execute(self):
        self.receiver.mark_position_as_obstacle(self.r, self.c)


@dataclass
class ProcessedData:
    command_id: str
    params: Dict[str, str]


class InvalidCommandException(Exception):
    "Raised when the command format is invalid"
    ...


class InvalidExecutionException(Exception):
    """Raised when the order of commands received is invalid. The only one allowed is:
    StartCommand -> [NewCleanerPositionCommand|ObstacleCommand...] -> EndCommand
    """
    ...


class UndefinedCommandException(Exception):
    "Raised when the command id does not exist"
    ...


class RealTimeCleaningWindow(QtWidgets.QDialog, Receiver):
    def __init__(self, cell_types: List[List[CellType]], serial_connection_info: SerialConnectionInfo, parent: QtWidgets.QWidget):
        super().__init__(parent=parent)
        self.setObjectName('realtime_map_dialog')
        self.setWindowTitle('Realtime Cleaning')

        self.cell_types = cell_types
        self.serial_connection_info = serial_connection_info

        self.setLayout(QtWidgets.QGridLayout())
        self.ROW_COUNT = len(self.cell_types)
        self.COLUMN_COUNT = len(self.cell_types[0])

        self._cells = [[QtWidgets.QFrame() for c in range(self.COLUMN_COUNT)]
                       for r in range(self.ROW_COUNT)]

        self.layout().setContentsMargins(0, 0, 0, 0)
        self.layout().setSpacing(0)

        self._initialize_grid()

        self.resize(self.COLUMN_COUNT * 20, self.ROW_COUNT * 20)

        self.threadpool = QtCore.QThreadPool()

    def _execute_command(self, command: Command):
        command.execute()

    def _cleaning_complete(self):
        MsgBoxUtil.MsgBoxUtil.info_box('The cleaner completed its job')
        self.close()

    def listen_for_cleaner_updates(self):
        worker = RealTimeCleaningWindowWorker(self.serial_connection_info, self)
        worker.signals.new_command.connect(self._execute_command)
        worker.signals.completed.connect(self._cleaning_complete)

        self.threadpool.start(worker)

    def _color_cell(self, cell: QtWidgets.QFrame, color: str):
        cell.setStyleSheet(f'background-color: {color}')

    def _mark_cell_as_to_clean(self, r: int, c: int) -> None:
        cell = self._cells[r][c]
        self._color_cell(cell, '#9BF3FF')
        # don't need to change the cell type because no cell can change type to "to clean"

    def _mark_cell_as_cleaned(self, r: int, c: int) -> None:
        cell = self._cells[r][c]
        self._color_cell(cell, '#A4F9C8')
        self.cell_types[r][c] = CellType.ALREADY_CLEANED

    def _mark_cell_as_unavailable(self, r: int, c: int) -> None:
        cell = self._cells[r][c]
        self._color_cell(cell, '#F5F5F5')
        # don't need to change the cell type because an unavailable cell remains unavailable

    def _mark_cell_as_cleaner_position(self, r: int, c: int) -> None:
        cell = self._cells[r][c]
        self._color_cell(cell, 'black')
        self.cell_types[r][c] = CellType.CLEANER_POSITION

    def _mark_cell_as_obstacle(self, r: int, c: int) -> None:
        cell = self._cells[r][c]
        self._color_cell(cell, 'red')
        self.cell_types[r][c] = CellType.UNAVAILABLE

    def _initialize_grid(self):
        for r in range(self.ROW_COUNT):
            for c in range(self.COLUMN_COUNT):
                cell = self.cell_types[r][c]
                match cell:
                    case CellType.TO_CLEAN:
                        self._mark_cell_as_to_clean(r, c)
                    case CellType.UNAVAILABLE:
                        self._mark_cell_as_unavailable(r, c)
                    case CellType.CLEANER_POSITION:
                        self._mark_cell_as_cleaner_position(r, c)
                        self.cleaner_position = MapPosition(r, c)
                    case _:
                        raise RuntimeError('the grid is invalid')

                self.layout().addWidget(self._cells[r][c], r, c)

    def move_cleaner(self, r: int, c: int) -> None:
        # the current position is cleaned
        self._mark_cell_as_cleaned(self.cleaner_position.r, self.cleaner_position.c)
        # the new position of the cleaner is (r, c)
        self._mark_cell_as_cleaner_position(r, c)

    def _read_command(self, serial: serial.Serial) -> Command:
        line = serial.readline()
        raw_command = line.decode('ascii')[:-1]
        return CommandFactory.build_command(raw_command, self)


class RealTimeCleaningWindowWorkerSignals(QtCore.QObject):
    new_command = QtCore.Signal(Command)
    completed = QtCore.Signal(int)


class RealTimeCleaningWindowWorker(QtCore.QRunnable):
    def __init__(self, serial_connection_info: SerialConnectionInfo, owner: RealTimeCleaningWindow):
        super().__init__()
        self.serial_connection_info = serial_connection_info
        self.owner = owner

        self.signals = RealTimeCleaningWindowWorkerSignals()

    def _read_command(self, serial: serial.Serial) -> Command:
        line = serial.readline()
        raw_command = line.decode('ascii')[:-1]
        print(raw_command)
        return CommandFactory.build_command(raw_command, self.owner)

    async def _listen_for_new_commands(self) -> None:
        # Initially, there is no timeout because I have to wait for the start command
        with serial.Serial(port=self.serial_connection_info.port_name, baudrate=self.serial_connection_info.baudrate, timeout=None) as realtime_serial:
            command = self._read_command(realtime_serial)

            if not isinstance(command, StartCommand):
                raise InvalidExecutionException()

            # after the start command, I want to keep the data-read delay in check
            realtime_serial.timeout = 10

            command = self._read_command(realtime_serial)
            while not isinstance(command, EndCommand):
                self.signals.new_command.emit(command)
                await asyncio.sleep(0.3)
                command = self._read_command(realtime_serial)

            # TODO: return correct result code
            self.signals.completed.emit(0)

    @QtCore.Slot()
    def run(self):
        asyncio.run(self._listen_for_new_commands())


class CommandFactory:
    @staticmethod
    def _process_command(raw_command: str) -> ProcessedData:
        params_start = raw_command.find('{')
        if params_start == -1:
            raise InvalidCommandException()
        command_id = raw_command[:params_start]
        params = ast.literal_eval(raw_command[params_start:])

        return ProcessedData(command_id, params)

    @staticmethod
    def build_command(raw_command: str, window: RealTimeCleaningWindow) -> Command:
        command_data = CommandFactory._process_command(raw_command)
        match command_data.command_id:
            case StartCommand.id:
                return StartCommand(window)
            case EndCommand.id:
                return EndCommand(window)
            case NewCleanerPositionCommand.id:
                return NewCleanerPositionCommand(window, int(command_data.params['r']), int(command_data.params['c']))
            case ObstacleCommand.id:
                return ObstacleCommand(window, int(command_data.params['r']), int(command_data.params['c']))
            case _:
                raise UndefinedCommandException()
