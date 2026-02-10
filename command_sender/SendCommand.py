#! /usr/bin/env python3

import os
import pGramsComSender.GUIManager
import logging
import argparse
logger = logging.getLogger(__name__)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="pGrams Command Sender GUI")
    parser.add_argument("--executable-prefix", type=str, default="", help="Prefix for the command executables (default: current directory's build/send_command)")
    parser.add_argument("--logfile", type=str, default="pGramsComSender.log", help="Log file path (default: pGramsComSender.log)")
    args = parser.parse_args()
    if args.executable_prefix:
        executable_prefix = args.executable_prefix
    else:
        executable_prefix = f"{os.path.dirname(__file__)}/build/send_command"
    gui_manager = pGramsComSender.GUIManager.GUIRunner(executable_prefix=executable_prefix, logfile=args.logfile)
    gui_manager.run()
