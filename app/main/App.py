import sys
from PySide6 import QtWidgets

from main.windows import MainWindow

if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = MainWindow.MainWindow()
    with open("stylesheet.qss", "r") as f:
        _style = f.read()
        app.setStyleSheet(_style)

    widget.resize(700, 700)
    widget.show()

    sys.exit(app.exec())
