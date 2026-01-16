import tkinter as tk
from tkinter import ttk


class ToolTip:
    def __init__(self, widget, text='widget info'):
        self.widget = widget
        self.text = text
        self.tipwindow = None
        self.delay = 200  # ミリ秒
        self._after_id = None
        self.id = None
        self.x = self.y = 0
        self.widget.bind('<Enter>', self._on_enter)
        self.widget.bind('<Leave>', self._on_leave)

    def _on_enter(self, event):
        self._last_event = event
        self._schedule()

    def _on_leave(self, event):
        self._unschedule()
        self.hidetip()

    def _schedule(self):
        self._unschedule()
        self._after_id = self.widget.after(self.delay, self._show_now)

    def _unschedule(self):
        if self._after_id:
            try:
                self.widget.after_cancel(self._after_id)
            except Exception:
                pass
            self._after_id = None

    def _show_now(self):
        if self.tipwindow or not self.text:
            return
        e = self._last_event
        x = e.x_root + 12
        y = e.y_root + 12
        tw = self.tipwindow = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(True)  # ウィンドウ装飾を消す
        tw.attributes("-topmost", True)  # 前面に（環境によって効かないことも）
        tw.wm_geometry(f"+{x}+{y}")

        label = tk.Label(tw, text=self.text, justify=tk.LEFT,
                         background="#ffffe0", relief=tk.SOLID, borderwidth=1,
                         font=("tahoma", "10", "normal"), foreground="black")
        label.pack(ipadx=1)

    def hidetip(self):
        tw = self.tipwindow
        self.tipwindow = None
        if tw:
            tw.destroy()
