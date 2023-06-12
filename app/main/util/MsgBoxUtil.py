from typing import Callable, Optional
from PySide6 import QtWidgets, QtCore


class MsgBoxUtil:
    @staticmethod
    def info_box(text: str, icon: QtWidgets.QMessageBox.Icon = QtWidgets.QMessageBox.Icon.NoIcon):
        infoBox = QtWidgets.QMessageBox()
        infoBox.setIcon(icon)
        infoBox.setText(text)
        infoBox.exec()

    @staticmethod
    def assert_with_box(assertion: bool, err_msg: str) -> bool:
        if not assertion:
            MsgBoxUtil.info_box(err_msg, QtWidgets.QMessageBox.Icon.Critical)
            raise AssertionError(err_msg)

    @staticmethod
    def assert_with_timed_box(text: str, timeout: int, icon: QtWidgets.QMessageBox.Icon = QtWidgets.QMessageBox.Icon.NoIcon):
        infoBox = TimerMessageBox(text, timeout)
        infoBox.setIcon = icon
        infoBox.exec()


class TimerMessageBox(QtWidgets.QMessageBox):
    def __init__(self, message: str, timeout: int, parent: Optional[QtWidgets.QWidget] = None):
        super().__init__(parent=parent)
        self.time_to_wait = timeout
        self.setText(message)
        self.setStandardButtons(QtWidgets.QMessageBox.Ok)
        QtCore.QTimer.singleShot(timeout, lambda: self.done(QtWidgets.QDialog.DialogCode.Accepted))
