from PySide6 import QtCore, QtWidgets, QtGui

class Cell(QtWidgets.QLabel):
    def __init__(self, parent, text: str):
        super().__init__()

        self.parent = parent
        self.setParent(parent)
        self.setText(text)
        self.selected = False

    def is_point_inside(self, point: QtCore.QPoint) -> bool:
        return (self.x() <= point.x() <= self.x() + self.width()) and \
            (self.y() <= point.y() <= self.y() + self.height())

    def toggle_selection(self):
        self.selected = not self.selected
        if self.selected:
            self.setStyleSheet('background-color: #9BF3FF') # light blue
        else:
            self.setStyleSheet('background-color: #F5F5F5') # default

    def mouseReleaseEvent(self, ev: QtGui.QMouseEvent) -> None:
        self.parent.selection_end.emit(self, ev.scenePosition())