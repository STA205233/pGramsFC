import logging
from pGramsComSender import MainWindow


class GUIRunner:
    def __init__(self, executable_prefix="") -> None:
        self.root = None
        self.executable_path = executable_prefix

    def run(self):
        logger = logging.getLogger("pGramsComSender")
        MainWindow.MainWindow(self.root, logger, executable_prefix=self.executable_path).run()
