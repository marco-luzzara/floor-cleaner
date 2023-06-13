from dataclasses import dataclass
from PySide6 import QtWidgets
import serial
from serial.tools import list_ports, list_ports_common

from main.components.mapping import MapManager


@dataclass
class SerialConnectionInfo:
    port_name: str
    baudrate: int


class SendMapDialog(QtWidgets.QDialog):
    def __init__(self, parent: QtWidgets.QWidget, grid_widget: MapManager.MapManager):
        super().__init__(parent=parent)
        self.setObjectName('serial_id_dialog')
        self.setWindowTitle('Choose Serial port')

        self._grid_widget = grid_widget
        self.setLayout(QtWidgets.QVBoxLayout())

        # combobox serial config
        self._combo_box_serial = QtWidgets.QComboBox()
        for i, serial_data in enumerate(list_ports.comports()):
            self._combo_box_serial.insertItem(
                i, serial_data.description, userData=serial_data)

        # button send config
        self._button_send = QtWidgets.QPushButton('Send')
        self._button_send.clicked.connect(self.accept)

        # button cancel config
        self._button_cancel = QtWidgets.QPushButton('Cancel')
        self._button_cancel.clicked.connect(self.reject)

        self.layout().addWidget(self._combo_box_serial)
        self.layout().addWidget(self._button_send)
        self.layout().addWidget(self._button_cancel)

    def accept(self) -> None:
        cleaning_map = self._grid_widget.get_map()
        serial_data: list_ports_common.ListPortInfo = self._combo_box_serial.currentData()
        self.serial_connection_info = SerialConnectionInfo(serial_data.device, 9600)
        with serial.Serial(port=self.serial_connection_info.port_name, baudrate=self.serial_connection_info.baudrate) as cleaner_serial:
            cleaner_serial.write(b'&')
            cleaner_serial.write(bytes(str(len(cleaning_map)), encoding='ascii'))
            cleaner_serial.write(b'|')
            cleaner_serial.write(bytes(str(len(cleaning_map[0])), encoding='ascii'))
            cleaner_serial.write(b'|')

            for row in cleaning_map:
                for cell in row:
                    cleaner_serial.write(cell.value.to_bytes(1, byteorder='big'))

            cleaner_serial.write(b'&')

        super().accept()
