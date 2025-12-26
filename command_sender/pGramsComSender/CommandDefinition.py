from enum import Enum


class CommandParameter:
    def __init__(self, name: str, description: str, range=None):
        self.name = name
        self.description = description
        self.range = range

    def __repr__(self) -> str:
        return f"CommandParameter(name={self.name}, description={self.description}, range={self.range})"

    def __str__(self) -> str:
        return f"{self.name} ({self.range}): {self.description}"

    def get_range(self):
        return self.range

    def validate(self, value) -> bool:
        if not isinstance(value, (int, float)):
            return False
        if self.range:
            return self.range[0] <= value <= self.range[1]
        return True


class CommandItem:
    def __init__(self, name: str, description: str, parameters: list[CommandParameter] = []):
        self.name = name
        self.description = description
        self.parameters = parameters

    def execute(self, **kwargs):
        # Placeholder for command execution logic
        print(f"Executing command '{self.name}' with parameters: {kwargs}")

    def __repr__(self) -> str:
        return f"CommandItem(name={self.name}, description={self.description}, parameters={self.parameters})"

    def __str__(self) -> str:
        param_str = ", ".join([str(param) for param in self.parameters])
        return f"{self.name}: {self.description} | Parameters: [{param_str}]"


class CommandDefinition:
    def __init__(self):
        self.commands = {}

    def add_command(self, subsystem: str, command: CommandItem):
        if subsystem not in self.commands:
            self.commands[subsystem] = []
        self.commands[subsystem].append(command)

    def get_commands(self, subsystem: str) -> list[CommandItem]:
        return self.commands.get(subsystem, [])

    def __repr__(self) -> str:
        return f"CommandDefinition(commands={self.commands})"

    def __str__(self) -> str:
        result = ""
        for subsystem, commands in self.commands.items():
            result += f"Subsystem: {subsystem}\n"
            for command in commands:
                result += f"  {str(command)}\n"
        return result


command_collection = CommandDefinition()
command_collection.add_command("Hub", CommandItem("Dummy1", "A dummy command for testing"))
command_collection.add_command("Hub", CommandItem("Dummy2", "A dummy command for testing with argument", [CommandParameter("arg1", "An integer argument")]))
command_collection.add_command("Hub", CommandItem("Prepare Shutdown", "Prepare the hub for shutdown"))
command_collection.add_command("Hub", CommandItem("Exec Shutdown", "Execute the shutdown of the hub"))
command_collection.add_command("Hub", CommandItem("Prepare Restart", "Prepare the hub for restart"))
command_collection.add_command("Hub", CommandItem("Exec Restart", "Execute the restart of the hub"))
command_collection.add_command("Hub", CommandItem("Reset Error", "Reset the error flags in the hub"))
command_collection.add_command("Hub", CommandItem("Set Link", "Set the link", [CommandParameter("link_state", "Used link, 0: iridium, 1: starlink", range=(0, 1))]))

# Orchestrator commands
command_collection.add_command("ORC", CommandItem("Exec CPU Restart", "Execute CPU restart"))
command_collection.add_command("ORC", CommandItem("Exec CPU Shutdown", "Execute CPU shutdown"))
command_collection.add_command("ORC", CommandItem("Boot All DAQ", "Boot all DAQ systems"))
command_collection.add_command("ORC", CommandItem("Shutdown All DAQ", "Shutdown all DAQ systems"))
command_collection.add_command("ORC", CommandItem("CPU Status", "Get the CPU status"))

command_collection.add_command("TPC", CommandItem("Configure", "Configure the TPC readout system", [CommandParameter("configuration type", "Configuration type", range=(0, 3))]))
command_collection.add_command("TPC", CommandItem("Start Run", "Start data acquisition"))
command_collection.add_command("TPC", CommandItem("Stop Run", "Stop data acquisition"))
command_collection.add_command("TPC", CommandItem("Boot DAQ", "Boot the DAQ system"))
command_collection.add_command("TPC", CommandItem("Boot Monitors", "Boot the monitoring system"))
command_collection.add_command("TPC", CommandItem("Query Hardware Status", "Query the hardware status"))

command_collection.add_command("Hub", CommandItem("Set Time", "Set the hub time to the current system time", [CommandParameter("epoch time", "Epoch time to set"), CommandParameter("timezone offset", "Timezone offset in hours", range=(-12, 14)), CommandParameter("dst flag", "Daylight saving time flag, 0: no DST, 1: DST", range=(0, 1))]))
