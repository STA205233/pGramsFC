import logging
import logging.handlers
from pGramsComSender import MainWindow


class GUIRunner:
    def __init__(self, executable_prefix="", logfile="pGramsComSender.log") -> None:
        self.root = None
        self.executable_path = executable_prefix
        self.logfile = logfile

    def run(self):
        self.logger = logging.getLogger("pGramsComSender")
        self.logger.setLevel(logging.INFO)
        stream_handler = logging.StreamHandler()
        stream_handler.setFormatter(logging.Formatter(
            "%(asctime)s [%(levelname)s] %(message)s",
            datefmt="%Y-%m-%d %H:%M:%S"))
        self.logger.addHandler(stream_handler)
        file_handler = logging.handlers.RotatingFileHandler(self.logfile, maxBytes=1_000_000, backupCount=1000)
        file_handler.setFormatter(logging.Formatter(
            "%(asctime)s [%(levelname)s] %(message)s",
            datefmt="%Y-%m-%d %H:%M:%S"))
        self.logger.addHandler(file_handler)
        MainWindow.MainWindow(self.root, self.logger, executable_prefix=self.executable_path).run()
        self.on_close()

    def on_close(self):
        self.logger.info("Window closed. Bye!")
