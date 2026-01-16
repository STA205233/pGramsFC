import tkinter as tk


class NumberEntryGroup(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master)

        self.vars: list[tk.StringVar] = []
        self.entries: list[tk.Entry] = []

        self._vcmd = (self.register(self._validate_int), "%P")
        self.kwargs = kwargs

    def set_count(self, n: int, *, clear=True):
        self.clear()
        for i in range(n):
            # tk.Label(self, text=f"{i + 1}:", **self.kwargs).grid(row=0, column=2 * i, padx=(0, 4), pady=2, sticky="e", )

            v = tk.StringVar(value="" if clear else "")
            e = tk.Entry(self, width=10, textvariable=v, validate="key", validatecommand=self._vcmd, **self.kwargs)
            e.grid(row=0, column=2 * i + 1, padx=(0, 10), pady=2, sticky="w")

            e.bind("<Return>", lambda ev, idx=i: self._focus_next(idx))

            self.vars.append(v)
            self.entries.append(e)

        if self.entries:
            self.entries[0].focus_set()

    def _focus_next(self, idx: int):
        if not self.entries:
            return
        nxt = (idx + 1) % len(self.entries)
        self.entries[nxt].focus_set()
        self.entries[nxt].selection_range(0, tk.END)

    def _validate_int(self, proposed: str) -> bool:
        if proposed == "":
            return True
        else:
            return proposed.isdigit()

    def get_numbers(self) -> list[int]:
        nums = []
        for i, v in enumerate(self.vars, start=1):
            s = v.get().strip()
            if s == "" or s == "-":
                raise ValueError(f"Number {i} is not input")
            nums.append(int(s))
        return nums

    def clear(self):
        for w in self.winfo_children():
            w.destroy()
        self.vars.clear()
        self.entries.clear()
