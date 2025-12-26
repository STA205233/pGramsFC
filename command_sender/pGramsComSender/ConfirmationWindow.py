import pGramsComSender.Window
import tkinter as tk
from tkinter import messagebox
from pGramsComSender import GUIgeometry


class ConfirmationWindow(pGramsComSender.Window.Window):
    def __init__(self, root, executer, subsystem, command_all, logger) -> None:
        super().__init__(root, "Notification", geometry=GUIgeometry.GUIGeometry(300, 150), grab_set=True)
        self.executer = executer
        self.command_all = command_all
        self.subsystem = subsystem
        self.logger = logger
        self._create_widgets()

    def _create_widgets(self):
        window = super()._getWindow()
        tk.Label(window, text=f"Command to send\n{self.command_all}\nConfirm?").pack()
        tk.Button(window, text="OK", command=lambda: self.__do_OK(self.subsystem, self.command_all)).pack()
        tk.Button(window, text="Cancel", command=self.__do_Cancel).pack()

    def __do_OK(self, subsystem, command_all):
        self.executer.execute_command(command_all, subsystem)
        window = super()._getWindow()
        window.destroy()

    def __do_Cancel(self):
        self.logger.info("Command sending cancelled by user. Command: " + self.command_all)
        self._getWindow().destroy()
