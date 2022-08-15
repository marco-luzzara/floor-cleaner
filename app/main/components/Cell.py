from PySide6 import QtCore, QtWidgets, QtGui

from ..App import App

class Cell(QtWidgets.QLabel):
    def __init__(self, parent: App, text: str):
        super().__init__()

        self.parent = parent
        self.setParent(parent)
        self.setText(text)
        self.selected = False

    def mousePressEvent(self, ev: QtGui.QMouseEvent) -> None:
        self.selected = not self.selected
        if self.selected:
            self.setStyleSheet('background-color: #9BF3FF') # default
        else:
            self.setStyleSheet('background-color: #F5F5F5') # light blue

        self.parent.cell_pressed.emit()