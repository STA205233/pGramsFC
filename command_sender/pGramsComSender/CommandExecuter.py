import subprocess
import pathlib


class CommandExecuter:
    def __init__(self, logger, executable_prefix="") -> None:
        self.logger = logger
        self.executable_prefix = executable_prefix

    def execute_command(self, command_all, subsystem) -> subprocess.CompletedProcess | None:
        try:
            command_executed = f"{self.executable_prefix}_{subsystem.lower()} {command_all}".strip()
            self.logger.info(f"Executing command: {command_executed}")
            try:
                result = subprocess.run(command_executed, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=pathlib.Path(f"{self.executable_prefix}_{subsystem}").parent, timeout=3)
                self.logger.info(f"Command executed successfully:\n{result.stdout.decode()}")
            except subprocess.TimeoutExpired:
                self.logger.error("Command execution timed out.")
                return None
            return result
        except subprocess.CalledProcessError as e:
            self.logger.error(f"An error occurred while executing the command:\n{e.stderr.decode()}")
            return None
