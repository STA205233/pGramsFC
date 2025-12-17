from pGramsComSender.Window import Window
import tkinter as tk
from pGramsComSender.GUIgeometry import GUIGeometry
from pGramsComSender.CommandExecuter import CommandExecuter
from pGramsComSender.ConfirmationWindow import ConfirmationWindow
from tkinter import messagebox
from tkinter.scrolledtext import ScrolledText
import logging


class TextHandler(logging.Handler):
    def __init__(self, text_widget: tk.Text):
        super().__init__()
        self.text = text_widget

    def emit(self, record: logging.LogRecord):
        msg = self.format(record)
        self.text.configure(state="normal")
        self.text.insert("end", msg + "\n")
        self.text.see("end")
        self.text.configure(state="disabled")


class MainWindow(Window):
    def __init__(self, root, command_list, logger, executable_prefix="") -> None:
        super().__init__(root, "CommandSender", geometry=GUIGeometry(1000, 700), grab_set=False)
        self.command_list = command_list
        self.__current_command = None
        self.logger = logger
        self.__executer = CommandExecuter(logger=logger, executable_prefix=executable_prefix)
        self.__label = None
        self.current_subsystem = "Hub"  # Currently assuming only one subsystem
        self._create_widgets()

    def _create_widgets(self):
        # parameters for layout
        blank_height_ratio = 0.01
        top_height_ratio = 0.1 - blank_height_ratio
        mid_height_ratio = 0.8 - blank_height_ratio
        bottom_height_ratio = 0.1 - blank_height_ratio

        blank_width_ratio = 0.02
        top_width_ratio = 1.0 - blank_width_ratio
        mid_left_width_ratio = 0.5 - blank_width_ratio / 2
        mid_right_width_ratio = 0.5 - blank_width_ratio / 2
        bottom_width_ratio = 1.0 - blank_width_ratio

        window = super()._getWindow()
        top_frame = tk.Frame(window, bg="lightgray")
        top_frame.place(relheight=top_height_ratio, relwidth=top_width_ratio, relx=blank_width_ratio / 2, rely=blank_height_ratio / 2, anchor="nw")
        self.__label = tk.Label(top_frame, text=f"Select a command to send\nSelected command: {self.__current_command}", font=("Arial", 16,), background="lightgray", fg="black")
        self.__label.pack()

        mid_left_frame = tk.Frame(window)
        mid_left_frame.place(relheight=mid_height_ratio, relwidth=mid_left_width_ratio, relx=blank_width_ratio / 2, rely=top_height_ratio + blank_height_ratio / 2, anchor="nw")
        num_commands = len(self.command_list)
        num_columns = 2
        num_rows = max(num_commands / num_columns, 20)
        for cmd in self.command_list[self.current_subsystem]:
            index = self.command_list[self.current_subsystem].index(cmd)
            tk.Button(mid_left_frame, text=cmd, command=lambda c=cmd: self._on_command_click(c)).place(relwidth=0.4, relheight=1.0 / num_rows, anchor="nw", relx=(index // num_columns) * 0.4 + 0.1, rely=index % 2 * (1.0 / num_rows))

        mid_right_frame = tk.Frame(window)
        mid_right_frame.place(relheight=mid_height_ratio, relwidth=mid_right_width_ratio, relx=mid_left_width_ratio + blank_width_ratio / 2, rely=top_height_ratio + blank_height_ratio / 2, anchor="nw")
        self.log = ScrolledText(mid_right_frame, fg="black", font=("Arial", 12), bg="white", )

        self.logger.setLevel(logging.DEBUG)
        text_handler = TextHandler(self.log)
        text_handler.setFormatter(logging.Formatter(
            "%(asctime)s [%(levelname)s] %(message)s",
            datefmt="%H:%M:%S"
        ))
        self.logger.addHandler(text_handler)

        self.log.place(relheight=1 - 0.01, relwidth=1.0, relx=0.01, rely=0.0, anchor="nw")
        self.log.configure(state=tk.DISABLED)
        self.log.see("end")

        bottom_bg = "yellow"
        bottom_frame = tk.Frame(window, bg=bottom_bg)
        bottom_frame.place(relheight=bottom_height_ratio, relwidth=bottom_width_ratio, relx=blank_width_ratio / 2, rely=top_height_ratio + mid_height_ratio + blank_height_ratio / 2, anchor="nw")

        label = tk.Label(bottom_frame, text="Arguments:", fg="black", bg=bottom_bg, font=("Arial", 12), cursor="xterm", anchor="w")
        label.place(relx=0, rely=0, relheight=0.25, relwidth=0.6, anchor="nw")
        self.__entry = tk.Entry(bottom_frame, background="white", fg="black", font=("Arial", 16), cursor="xterm")
        self.__entry.place(relheight=0.65, relwidth=0.6, relx=0.05, rely=0.3, anchor="nw")
        tk.Button(bottom_frame, text="Send", command=self._on_send_command, width=10, bg=bottom_bg).place(relx=0.7, rely=0.3, anchor="w")
        tk.Button(bottom_frame, text="Exit", command=window.quit, width=10, bg=bottom_bg).place(relx=0.7, rely=0.7, anchor="w")
        tk.Button(bottom_frame, text="Reset", command=self.reset_command, width=10, bg=bottom_bg).place(relx=0.85, rely=0.3, anchor="w")

    @property
    def current_command(self):
        return self.__current_command

    @current_command.setter
    def current_command(self, command):
        raise RuntimeError("current_command is read-only")

    def _on_command_click(self, command):
        self.logger.info(f"Command selected: {command}")
        self.__current_command = command
        if self.__label:
            self.__label.config(text=f"Select a command to send\nSelected command: {command}")
        self.__entry.delete(0, tk.END)

    @staticmethod
    def compile_command(command, args):
        return f"{command} {args.replace(',', ' ')}"

    def reset_command(self):
        self.__current_command = None
        if self.__label:
            self.__label.config(text=f"Select a command to send\nSelected command: {self.__current_command}")
        self.__entry.delete(0, tk.END)

    def _on_send_command(self):
        args = self.__entry.get()
        command_all = self.compile_command(self.__current_command, args)
        if self.__current_command:
            window = super()._getWindow()
            ConfirmationWindow(window, self.__executer, self.current_subsystem, command_all)
        else:
            messagebox.showwarning("No Command Selected", "Please select a command before sending.")

    def run(self):
        window = super()._getWindow()
        window.mainloop()
