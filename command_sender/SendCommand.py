import pGramsComSender.GUIManager
import subprocess
import logging
import argparse
import tkinter as tk
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


if __name__ == "__main__":
    executable_prefix = "/Users/shotaarai/software/pGramsFC/command_sender/build/send_command"
    gui_manager = pGramsComSender.GUIManager.GUIRunner(executable_prefix=executable_prefix)
    gui_manager.run({"Hub": ["dummy1", "dummy2", "prepare_shutdown", "exec_shutdown"]},)
