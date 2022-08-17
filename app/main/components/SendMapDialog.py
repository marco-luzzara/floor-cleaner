from PySide6 import QtCore, QtWidgets, QtGui
from serial.tools import list_ports


class SendMapDialog(QtWidgets.QDialog):
    def __init__(self, parent: QtWidgets.QWidget, grid_widget: QtWidgets.QWidget):
        super().__init__(parent=parent)
        self.setObjectName('serial_id_dialog')
        self.setWindowTitle('Choose Serial port')

        self._grid_widget = grid_widget
        self.setLayout(QtWidgets.QVBoxLayout())

        # combobox serial config
        self._combo_box_serial = QtWidgets.QComboBox()
        for i, serial_port in enumerate(list_ports.comports()):
            self._combo_box_serial.insertItem(
                i, serial_port.description, userData=serial_port)

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
        serial_info = self._combo_box_serial.currentData()

        super().accept()
