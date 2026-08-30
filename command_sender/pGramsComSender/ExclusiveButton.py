import tkinter as tk


class ExclusiveCheckButton:
    def __init__(self, checked_fg=None, unchecked_fg=None) -> None:
        self.__check_button: dict[str, tk.Checkbutton] = {}
        self.__var: dict[str, tk.BooleanVar] = {}
        self.__checked_fg = checked_fg
        self.__unchecked_fg = unchecked_fg
        self.__current_index = None

    def __set_true(self, name: str):
        self.__var[name].set(True)
        if self.__checked_fg is not None:
            self.__check_button[name].config(fg=self.__checked_fg)

    def set_true(self, name: str):
        self.__run(name)

    def __set_false(self, name: str):
        self.__var[name].set(False)
        if self.__unchecked_fg is not None:
            self.__check_button[name].config(fg=self.__unchecked_fg)

    def __run(self, name: str):
        for n in self.__check_button.keys():
            if n == name:
                self.__set_true(n)
                continue
            self.__set_false(n)
        self.__current_index = name

    def produce(self, name: str, master, *args, **kwargs):
        if name in self.__check_button.keys():
            raise ValueError(f"{name} already exists")
        var = tk.BooleanVar()
        self.__var[name] = var
        but = tk.Checkbutton(master=master, command=lambda: self.__run(name), variable=var, *args, **kwargs)
        self.__check_button[name] = but
        return but

    @property
    def current_selection(self) -> str:
        assert (self.__current_index)
        return self.__current_index
