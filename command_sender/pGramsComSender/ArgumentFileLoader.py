from os import PathLike
from shutil import copy2
from datetime import datetime
from pathlib import Path

class ArgumentFileLoader:
    def __init__(self, logging) -> None:
        self.args = []
        self.logging = logging
    def load(self, filepath:str | PathLike[str] , copy=True):
        filepath = Path(filepath)
        if copy:
            copy_filename = datetime.now().strftime(f"{filepath.parent}/{filepath.stem}_%Y%m%d_%H%M%S.txt")
            copy2(filepath, copy_filename)
            self.logging.info(f"Copied argument file {filepath} to {copy_filename}")
        with open(filepath, "r") as f:
            lines = f.readlines()
        self.args = [int(line.strip()) for line in lines if line.strip() != ""]
        return self.args
    