import os
import pGramsComSender.GUIManager
import logging
logger = logging.getLogger(__name__)

if __name__ == "__main__":
    executable_prefix = f"{os.path.dirname(__file__)}/build/send_command"
    gui_manager = pGramsComSender.GUIManager.GUIRunner(executable_prefix=executable_prefix)
    gui_manager.run()
