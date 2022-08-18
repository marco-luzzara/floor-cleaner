from typing import Callable
from PySide6 import QtWidgets


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
