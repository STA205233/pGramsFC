from pGramsComSender.Window import Window
from pGramsComSender.LogText import LogView
import tkinter as tk
from pGramsComSender.GUIgeometry import GUIGeometry
from pGramsComSender.CommandExecuter import CommandExecuter
from pGramsComSender.ConfirmationWindow import ConfirmationWindow
from pGramsComSender import ToolTip
from pGramsComSender.NumberEntryGroup import NumberEntryGroup
from tkinter import messagebox
import tkinter.ttk as ttk
import logging
from pGramsComSender.CommandDefinition import command_collection as command_collection


class TextHandler(logging.Handler):
    def __init__(self, text_widget: LogView):
        super().__init__()
        self.text = text_widget

    def emit(self, record: logging.LogRecord):
        msg = self.format(record)
        level = self.check_level(msg)
        self.text.write(msg, tag=level)

    @staticmethod
    def check_level(msg):
        if "[DEBUG]" in msg:
            return "DEBUG"
        elif "[INFO]" in msg:
            return "INFO"
        elif "[WARNING]" in msg:
            return "WARNING"
        elif "[ERROR]" in msg:
            return "ERROR"
        elif "[CRITICAL]" in msg:
            return "CRITICAL"
        else:
            return "INFO"


class MainWindow(Window):
    def __init__(self, root, logger, executable_prefix="") -> None:
        super().__init__(root, "CommandSender", geometry=GUIGeometry(1200, 700), grab_set=False)
        self.__current_command = None
        self.logger = logger
        self.__executer = CommandExecuter(logger=logger, executable_prefix=executable_prefix)
        self.__label = None
        self.current_subsystem = "Hub"
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
        top_frame_style = ttk.Style(self._getWindow())
        top_frame_style.theme_use("clam")
        top_frame_style.configure("modifiedTop.TFrame", background="green")
        top_frame = ttk.Frame(window, style="modifiedTop.TFrame")
        top_frame.place(relheight=top_height_ratio, relwidth=top_width_ratio, relx=blank_width_ratio / 2, rely=blank_height_ratio / 2, anchor="nw")
        top_frame_label_style = ttk.Style(self._getWindow())
        top_frame_label_style.theme_use("clam")
        top_frame_label_style.configure("modifiedTop.TLabel", background="green", foreground="white", font=("Arial", 16))
        self.__label = ttk.Label(top_frame, text=f"Select a command to send\nSelected command: {self.__current_command.name if self.__current_command else 'None'}", style="modifiedTop.TLabel", anchor="center", justify="center")
        self.__label.pack()

        mid_left_frame = ttk.Frame(window)
        mid_left_frame.place(relheight=mid_height_ratio, relwidth=mid_left_width_ratio, relx=blank_width_ratio / 2, rely=top_height_ratio + blank_height_ratio / 2, anchor="nw")

        self.tabs = ttk.Notebook(mid_left_frame)
        for subsystem in command_collection.commands.keys():
            tab_container, tab_inner = make_scrollable_tab(self.tabs)
            for i in range(len(command_collection.commands[subsystem])):
                cmd = command_collection.commands[subsystem][i]
                b1 = ttk.Button(tab_inner, text=cmd.name, command=lambda c=cmd: self._on_command_click(c))
                ToolTip.ToolTip(b1, text=str(cmd))
                b1.pack(fill="x", padx=50, pady=2)
            self.tabs.add(tab_container, text=subsystem)
        self.tabs.bind("<<NotebookTabChanged>>", self._on_click_tabs)
        self.tabs.place(relheight=1.0, relwidth=1.0, relx=0.0, rely=0.0, anchor="nw")

        mid_right_frame = ttk.Frame(window)
        mid_right_frame.place(relheight=mid_height_ratio, relwidth=mid_right_width_ratio, relx=mid_left_width_ratio + blank_width_ratio / 2, rely=top_height_ratio + blank_height_ratio / 2, anchor="nw")
        self.log = LogView(mid_right_frame, fg="black", font=("Arial", 12), bg="white", max_lines=500)
        self.log.tag_config("INFO", foreground="black")
        self.log.tag_config("DEBUG", foreground="gray")
        self.log.tag_config("WARNING", foreground="orange")
        self.log.tag_config("ERROR", foreground="red")
        self.log.tag_config("CRITICAL", foreground="red", background="black")

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
        bottom_subframe = tk.Frame(bottom_frame, bg=bottom_bg)
        bottom_subframe.place(relheight=0.65, relwidth=0.6, relx=0.05, rely=0.3, anchor="nw")
        self.__entry = NumberEntryGroup(bottom_subframe, bg="white", fg="black", font=("Arial", 16), cursor="xterm")
        self.__entry.pack(fill="both", expand=True)
        tk.Button(bottom_frame, text="Send", command=self._on_send_command, width=10, bg=bottom_bg).place(relx=0.7, rely=0.3, anchor="w")
        tk.Button(bottom_frame, text="Exit", command=window.quit, width=10, bg=bottom_bg).place(relx=0.7, rely=0.7, anchor="w")
        tk.Button(bottom_frame, text="Reset", command=self.reset_command, width=10, bg=bottom_bg).place(relx=0.85, rely=0.3, anchor="w")
        self.logger.info("GUI initialized successfully.")

    @property
    def current_command(self):
        return self.__current_command

    @current_command.setter
    def current_command(self, command):
        raise RuntimeError("current_command is read-only")

    def _on_command_click(self, command):
        self.logger.debug(f"Command selected: {command.name}")
        self.__current_command = command
        if self.__label:
            self.__label.config(text=f"Select a command to send\nSelected command: {command.name}")
        self.__entry.set_count(len(command.parameters), clear=True)

    @staticmethod
    def compile_command(command, args):
        return f"{command.name.replace(' ', '_').lower()} {args.replace(',', ' ')}"

    def reset_command(self):
        self.__current_command = None
        if self.__label:
            self.__label.config(text=f"Select a command to send\nSelected command: {self.__current_command.name if self.__current_command else 'None'}")
        self.__entry.clear()

    def _on_send_command(self):
        if self.__current_command is None:
            self.logger.warning("No command selected to send")
            messagebox.showwarning("No Command Selected", "Please select a command before sending.")
            return
        try:
            args = self.__entry.get_numbers()
            args_str = ",".join(str(arg) for arg in args)
        except ValueError as e:
            self.logger.warning(f"Invalid input for command arguments on command {self.__current_command.name}: {e}")
            messagebox.showerror("Invalid Input", str(e))
            return
        command_all = self.compile_command(self.__current_command, args_str)
        if self.__current_command:
            window = super()._getWindow()
            ConfirmationWindow(window, self.__executer, self.current_subsystem, command_all, self.logger)
            self.reset_command()

    def run(self):
        window = super()._getWindow()
        window.mainloop()

    def _on_click_tabs(self, event):
        note = event.widget
        self.current_subsystem = note.tab(note.select(), "text")
        self.logger.debug(f"Tab changed: {self.current_subsystem}")
        tab = event.widget.nametowidget(event.widget.select())
        for child in tab.winfo_children():
            try:
                child.state(['!disabled'])
            except Exception:
                pass


def make_scrollable_tab(notebook: ttk.Notebook):
    tab_container = ttk.Frame(notebook)

    canvas = tk.Canvas(tab_container, highlightthickness=0)
    vbar = ttk.Scrollbar(tab_container, orient="vertical", command=canvas.yview)
    canvas.configure(yscrollcommand=vbar.set)

    vbar.pack(side="right", fill="y")
    canvas.pack(side="left", fill="both", expand=True)

    inner = ttk.Frame(canvas)
    window_id = canvas.create_window((0, 0), window=inner, anchor="nw")

    def update_scroll_state():
        """中身がはみ出る時だけスクロール可能にする"""
        tab_container.update_idletasks()

        content_h = inner.winfo_reqheight()
        view_h = canvas.winfo_height()

        # scrollregion は常に最新化
        canvas.configure(scrollregion=canvas.bbox("all"))

        if content_h <= view_h:
            # はみ出てない → スクロール不要
            vbar.pack_forget()              # スクロールバー非表示
            canvas.yview_moveto(0)          # 上に戻す（任意）
            tab_container._scroll_enabled = False
        else:
            # はみ出てる → スクロール必要
            if not vbar.winfo_ismapped():
                vbar.pack(side="right", fill="y")
            tab_container._scroll_enabled = True

    # innerサイズ変更時
    inner.bind("<Configure>", lambda e: update_scroll_state())

    # canvasサイズ変更時（ウィンドウリサイズなど）
    def on_canvas_configure(event):
        canvas.itemconfigure(window_id, width=event.width)
        update_scroll_state()

    canvas.bind("<Configure>", on_canvas_configure)

    # マウスホイール（必要な時だけ動かす）
    def on_mousewheel(event):
        if getattr(tab_container, "_scroll_enabled", False):
            canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")

    def bind_wheel(_):
        canvas.bind_all("<MouseWheel>", on_mousewheel)

    def unbind_wheel(_):
        canvas.unbind_all("<MouseWheel>")

    tab_container.bind("<Enter>", bind_wheel)
    tab_container.bind("<Leave>", unbind_wheel)

    # 初期状態
    tab_container._scroll_enabled = False
    tab_container.after(0, update_scroll_state)

    return tab_container, inner
