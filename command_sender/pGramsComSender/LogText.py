from tkinter.scrolledtext import ScrolledText


class LogView(ScrolledText):
    def __init__(self, master, max_lines=500, **kwargs):
        super().__init__(master, **kwargs)
        self.max_lines = max_lines
        self.configure(state="disabled")

    def write(self, text, tag=None):
        self.configure(state="normal")

        if tag:
            self.insert("end", text, tag)
        else:
            self.insert("end", text)

        if not text.endswith("\n"):
            self.insert("end", "\n")

        self._trim_lines()
        self.see("end")

        self.configure(state="disabled")

    def _trim_lines(self):
        last_line = int(self.index("end-1c").split(".")[0])

        extra = last_line - self.max_lines
        if extra > 0:
            self.delete("1.0", f"{extra + 1}.0")
