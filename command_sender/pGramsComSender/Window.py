import tkinter as tk
from pGramsComSender.GUIgeometry import GUIGeometry
import abc


class Window(abc.ABC):
    def __init__(self, root, title: str, geometry: GUIGeometry, grab_set: bool) -> None:
        if root is None:
            self.__window = tk.Tk()
        else:
            self.__window = tk.Toplevel(root)
        self.__window.resizable(False, False)
        self.__window.title(title)
        self.__window.geometry(geometry())
        if grab_set:
            self.__window.grab_set()

    @abc.abstractmethod
    def _create_widgets(self):
        pass

    def _getWindow(self):
        return self.__window
