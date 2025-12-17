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
    gui_manager.run({"Hub": ["Dummy1", "Dummy2", "Prepare Shutdown", "Exec Shutdown", "Prepare Shutdown", "Exec Restart", "Reset Error", "Set Link"], "Orc": ["Exec CPU Restart", "Exec CPU Shutdown", "Boot All DAQ", "Shutdown All DAQ", "CPU Status"]})
