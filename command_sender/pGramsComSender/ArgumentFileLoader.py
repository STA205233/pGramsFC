from os import PathLike

class ArgumentFileLoader:
    def __init__(self) -> None:
        self.args = []
    def load(self, filepath: int | str | bytes | PathLike[str] | PathLike[bytes]):
        with open(filepath, "r") as f:
            lines = f.readlines()
        self.args = [int(line.strip()) for line in lines if line.strip() != ""]
        return self.args
    