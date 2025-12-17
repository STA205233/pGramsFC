class GUIGeometry:
    def __init__(self, width, height) -> None:
        self.width = width
        self.height = height

    def __call__(self):
        return f"{self.width}x{self.height}"
