from enum import Enum


class CommandParameter:
    def __init__(self, name: str, description: str, range=None):
        self.name = name
        self.description = description
        self.range = range

    def __repr__(self) -> str:
        return f"CommandParameter(name={self.name}, description={self.description}, range={self.range})"

    def __str__(self) -> str:
        return f"{self.name} : {self.description}. {self.range}" if self.range else f"{self.name} : {self.description}"

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

    def __repr__(self) -> str:
        return f"CommandItem(name={self.name}, description={self.description}, parameters={self.parameters})"

    def __str__(self) -> str:
        str_ = f"{self.name}: {self.description}\nParameters:\n"
        if self.parameters != []:
            str_ += f"{'\t'.join([str(param) for param in self.parameters])}"
        else:
            str_ += "None"
        return str_


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
command_collection.add_command("Hub", CommandItem("Dummy2", "A dummy command for testing with argument", [CommandParameter("Arg1", "An integer argument")]))
command_collection.add_command("Hub", CommandItem("Prepare Shutdown", "Prepare the hub for shutdown"))
command_collection.add_command("Hub", CommandItem("Exec Shutdown", "Execute the shutdown of the hub"))
command_collection.add_command("Hub", CommandItem("Prepare Restart", "Prepare the hub for restart"))
command_collection.add_command("Hub", CommandItem("Exec Restart", "Execute the restart of the hub"))
command_collection.add_command("Hub", CommandItem("Reset Error", "Reset the error flags in the hub"))
command_collection.add_command("Hub", CommandItem("Set Link", "Set the link", [CommandParameter("Link state", "Used link, 0: iridium, 1: starlink", range=(0, 1))]))

# Orchestrator commands
command_collection.add_command("ORC", CommandItem("Exec CPU Restart", "Execute CPU restart"))
command_collection.add_command("ORC", CommandItem("Exec CPU Shutdown", "Execute CPU shutdown"))
command_collection.add_command("ORC", CommandItem("Boot All DAQ", "Boot all DAQ systems"))
command_collection.add_command("ORC", CommandItem("Shutdown All DAQ", "Shutdown all DAQ systems"))
command_collection.add_command("ORC", CommandItem("Start Computer Status", "Get the CPU status"))
command_collection.add_command("ORC", CommandItem("Stop Computer Status", "Stop the CPU status"))
command_collection.add_command("ORC", CommandItem("Init PCIe Driver", ""))
command_collection.add_command("ORC", CommandItem("Boot Monitor", ""))
command_collection.add_command("ORC", CommandItem("Shutdown Monitor", ""))
command_collection.add_command("ORC", CommandItem("Boot ToF DAQ", ""))
command_collection.add_command("ORC", CommandItem("Shutdown ToF DAQ", ""))
command_collection.add_command("ORC", CommandItem("Boot TPC DAQ", ""))
command_collection.add_command("ORC", CommandItem("Shutdown TPC DAQ", ""))


command_collection.add_command("TPC", CommandItem("Configure", "Configure the TPC readout system", [CommandParameter("Configuration type", "Configuration type", range=(0, 3))]))
command_collection.add_command("TPC", CommandItem("Start Run", "Start data acquisition"))
command_collection.add_command("TPC", CommandItem("Stop Run", "Stop data acquisition"))
command_collection.add_command("TPC", CommandItem("Reset Run", ""))
command_collection.add_command("TPC", CommandItem("Boot DAQ", "Boot the DAQ system"))
command_collection.add_command("TPC", CommandItem("Boot Monitor", "Boot the monitoring system"))
command_collection.add_command("TPCMonitor", CommandItem("Query LB Data", "Query the hardware status", [CommandParameter("Data type", "Type of data to query"), CommandParameter("", ""), CommandParameter("", ""), CommandParameter("", "")]))
command_collection.add_command("TPCMonitor", CommandItem("Query Event Data", "",[CommandParameter("Data type", "Type of data to query"), CommandParameter("", ""), CommandParameter("", ""), CommandParameter("", "")]))


command_collection.add_command("TOF", CommandItem("Start DAQ", "Start data acquisition"))
command_collection.add_command("TOF", CommandItem("Stop DAQ", "Stop data acquisition"))
command_collection.add_command("TOF", CommandItem("Reset DAQ", "Reset data acquisition system"))

command_collection.add_command("TOF", CommandItem("Run Init System", "Initialize the TOF system"))
command_collection.add_command("TOF", CommandItem("Run Make Bias Calib Table", ""))
command_collection.add_command("TOF", CommandItem("Run Make Simple Bias Set Table", ""))
command_collection.add_command("TOF", CommandItem("Run Make Simple Channel Map", ""))
command_collection.add_command("TOF", CommandItem("Run Make Simple Disc Set Table", ""))
command_collection.add_command("TOF", CommandItem("Run Read Temperature Sensors", ""))
command_collection.add_command("TOF", CommandItem("Run Acquire Threshold Calibration", ""))
command_collection.add_command("TOF", CommandItem("Run Acquire QDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Run Acquire TDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Run Acquire SiPM Data", ""))
command_collection.add_command("TOF", CommandItem("Run Acquire Threshold Calibration BN", ""))
command_collection.add_command("TOF", CommandItem("Run Acquire Threshold Calibration D", ""))
command_collection.add_command("TOF", CommandItem("Run Process Threshold Calibration", ""))
command_collection.add_command("TOF", CommandItem("Run Process TDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Run Process QDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Run Convert Raw To Raw", ""))
command_collection.add_command("TOF", CommandItem("Run Convert Raw To Singles", ""))
command_collection.add_command("TOF", CommandItem("Run Process Coin Evt QA", ""))
command_collection.add_command("TOF", CommandItem("Run Convert Stg1 To Stg2", ""))


command_collection.add_command("TOF", CommandItem("Bias ON", "Turn ON the bias voltage"))
command_collection.add_command("TOF", CommandItem("Bias OFF", "Turn OFF the bias voltage"))
command_collection.add_command("TOF", CommandItem("Bias Set Voltage", "Set the bias voltage", [CommandParameter("Voltage (V)", "Bias voltage in volts")]))
