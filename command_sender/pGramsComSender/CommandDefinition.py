from enum import Enum


class CommandParameter:
    def __init__(self, name: str, description: str | None = None, range=None):
        self.name = name
        self.description = description if description is not None else ""
        self.range = range

    def __repr__(self) -> str:
        return f"CommandParameter(name={self.name}, description={self.description}, range={self.range})"

    def __str__(self) -> str:
        return f"{self.name} : {self.description} {self.range}" if self.range else f"{self.name} : {self.description}"

    def get_range(self):
        return self.range

    def validate(self, value) -> bool:
        if not isinstance(value, (int, float)):
            return False
        if self.range:
            return self.range[0] <= value <= self.range[1]
        return True


class CommandItem:
    def __init__(self, name: str, description: str, parameters: list[CommandParameter] = [], file_loadable: bool = False):
        self.name = name
        self.description = description
        self.parameters = parameters
        self.file_loadable = file_loadable

    def __repr__(self) -> str:
        return f"CommandItem(name={self.name}, description={self.description}, parameters={self.parameters}, file_loadable={self.file_loadable})"

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
command_collection.add_command("Hub", CommandItem("Emergency DAQ Shutdown", ""))
command_collection.add_command("Hub", CommandItem("Dummy1", "A dummy command for testing"))
command_collection.add_command("Hub", CommandItem("Dummy2", "A dummy command for testing with argument", [CommandParameter("Arg1", "An integer argument")]))
command_collection.add_command("Hub", CommandItem("Prepare Shutdown", "Prepare the hub for shutdown"))
command_collection.add_command("Hub", CommandItem("Exec Shutdown", "Execute the shutdown of the hub"))
command_collection.add_command("Hub", CommandItem("Prepare Restart", "Prepare the hub for restart"))
command_collection.add_command("Hub", CommandItem("Exec Restart", "Execute the restart of the hub"))
command_collection.add_command("Hub", CommandItem("Reset Error", "Reset the error flags in the hub"))
command_collection.add_command("Hub", CommandItem("Set Link", "Set the link", [CommandParameter("Link state", "Used link, 0: iridium, 1: starlink", range=(0, 1))]))
command_collection.add_command("Hub", CommandItem("TB Bias ON", "Enable Bias Control", [CommandParameter("Ch", "Channel",range=(0, 7))]))
command_collection.add_command("Hub", CommandItem("TB Bias OFF", "Disable Bias Control", [CommandParameter("Ch", "Channel",range=(0, 7))]))
command_collection.add_command("Hub", CommandItem("TB Set V offset", "Set V Offset", [CommandParameter("Voffset", range=(0, 20))]))
command_collection.add_command("Hub", CommandItem("TB Set V def", "Set V def", [CommandParameter("Ch", "Channel",range=(0, 127)), CommandParameter("Vdef", range=(136, 172))]))
command_collection.add_command("Hub",CommandItem("TB Set Tmux", "Set T mux channel",[CommandParameter("Ch", range=(0, 15)), CommandParameter("Thermometer Select", "0: primary, 1: secondary", range=(0, 1))]))

command_collection.add_command("PDU", CommandItem("TOF Bias ON", ""))
command_collection.add_command("PDU", CommandItem("TOF Bias OFF", ""))
command_collection.add_command("PDU", CommandItem("Cold TPC HV ON", ""))
command_collection.add_command("PDU", CommandItem("Cold TPC HV OFF", ""))
command_collection.add_command("PDU", CommandItem("Cold Charge PreAmp ON", ""))
command_collection.add_command("PDU", CommandItem("Cold Charge PreAmp OFF", ""))
command_collection.add_command("PDU", CommandItem("Cold SiPM PreAmp ON", ""))
command_collection.add_command("PDU", CommandItem("Cold SiPM PreAmp OFF", ""))
command_collection.add_command("PDU", CommandItem("Warm TPC Shaper ON", ""))
command_collection.add_command("PDU", CommandItem("Warm TPC Shaper OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM 0 ON", ""))
command_collection.add_command("PDU", CommandItem("SiPM 0 OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM 1 ON", ""))
command_collection.add_command("PDU", CommandItem("SiPM 1 OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM 2 ON", ""))
command_collection.add_command("PDU", CommandItem("SiPM 2 OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM 3 ON", ""))
command_collection.add_command("PDU", CommandItem("SiPM 3 OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM 4 ON", ""))
command_collection.add_command("PDU", CommandItem("SiPM 4 OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM 5 ON", ""))
command_collection.add_command("PDU", CommandItem("SiPM 5 OFF", ""))
command_collection.add_command("PDU", CommandItem("Tof ON", ""))
command_collection.add_command("PDU", CommandItem("Tof OFF", ""))
command_collection.add_command("PDU", CommandItem("CAEN P3V3 ON", ""))
command_collection.add_command("PDU", CommandItem("CAEN P3V3 OFF", ""))
command_collection.add_command("PDU", CommandItem("CAEN PM5V ON", ""))
command_collection.add_command("PDU", CommandItem("CAEN PM5V OFF", ""))
command_collection.add_command("PDU", CommandItem("CAEN P12V ON", ""))
command_collection.add_command("PDU", CommandItem("CAEN P12V OFF", ""))
command_collection.add_command("PDU", CommandItem("DAQ CPU ON", ""))
command_collection.add_command("PDU", CommandItem("DAQ CPU OFF", ""))
command_collection.add_command("PDU", CommandItem("SiPM0 VSET", "", [CommandParameter("Voltage")]))
command_collection.add_command("PDU", CommandItem("SiPM1 VSET", "", [CommandParameter("Voltage")]))
command_collection.add_command("PDU", CommandItem("SiPM2 VSET", "", [CommandParameter("Voltage")]))
command_collection.add_command("PDU", CommandItem("SiPM3 VSET", "", [CommandParameter("Voltage")]))
command_collection.add_command("PDU", CommandItem("SiPM4 VSET", "", [CommandParameter("Voltage")]))
command_collection.add_command("PDU", CommandItem("SiPM5 VSET", "", [CommandParameter("Voltage")]))
command_collection.add_command("PDU", CommandItem("PressureReg VSET", "", [CommandParameter("voltage")]))


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
command_collection.add_command("ORC", CommandItem("Start PPS", ""))
command_collection.add_command("ORC", CommandItem("Send Pulse Train", ""))
command_collection.add_command("ORC", CommandItem("Stop PPS", ""))
command_collection.add_command("ORC", CommandItem("Restart Orchestrator", ""))
command_collection.add_command("ORC", CommandItem("Clear Errors", ""))
command_collection.add_command("ORC", CommandItem("Set Data SSD0", "write to SSD 0"))
command_collection.add_command("ORC", CommandItem("Set Data SSD1", "write to SSD 1"))


command_collection.add_command("TPC", CommandItem("Configure", "Configure the TPC readout system", [CommandParameter("Configuration type", "Configuration type", range=(0, 3))], file_loadable=True))
command_collection.add_command("TPC", CommandItem("Start Run", "Start data acquisition"))
command_collection.add_command("TPC", CommandItem("Stop Run", "Stop data acquisition"))
command_collection.add_command("TPC", CommandItem("Reset Run", ""))
command_collection.add_command("TPC", CommandItem("Boot DAQ", "Boot the DAQ system"))
command_collection.add_command("TPC", CommandItem("Boot Monitor", "Boot the monitoring system"))
command_collection.add_command("TPCMonitor", CommandItem("Query LB Data", "Query the hardware status", [CommandParameter("Data type", "Type of data to query"), CommandParameter("", ""), CommandParameter("", ""), CommandParameter("", "")])) # deprecated
command_collection.add_command("TPCMonitor", CommandItem("Query Event Data", "",[CommandParameter("Data type", "Type of data to query"), CommandParameter("", ""), CommandParameter("", ""), CommandParameter("", "")])) # deprecated
command_collection.add_command(
    "TPCMonitor",
    CommandItem(
        "Send Full Event Data",
        "One event: FEM headers, charge [248,520), all light ROIs, then 0x4005",
        [
            CommandParameter("Run", "99999 = latest closed run", range=(0, 99999)),
            CommandParameter("File", "99999 = latest closed file", range=(0, 99999)),
            CommandParameter("Event", "99999 = last event in file", range=(0, 99999)),
            CommandParameter("L lag", "LFEM header from event+n, ADC from event+(n-1); 99999 = auto-match LFEM f#", range=(0, 99999)),
        ],
    ),
)
command_collection.add_command(
    "TPCMonitor",
    CommandItem(
        "Start Continuous LBW",
        "Background thread: each new closed file → one 0x4001 average (no resend). 99999 run/file independent. Lives until Stop.",
        [
            CommandParameter("Period sec", "Poll interval while waiting for the next closed file", range=(1, 3600)),
            CommandParameter("Run", "99999 = latest run, then newer runs; else only this run", range=(0, 99999)),
            CommandParameter("File", "99999 = latest closed, then newer closed files; else that file# once", range=(0, 99999)),
            CommandParameter("N event", "Decode events 0..N-1 and average; 99999 = whole file", range=(1, 99999)),
        ],
    ),
)
command_collection.add_command(
    "TPCMonitor",
    CommandItem("Stop Continuous LBW", "Stop periodic LBW telemetry"),
)


command_collection.add_command("TOF", CommandItem("Start DAQ", "Start data acquisition"))
command_collection.add_command("TOF", CommandItem("Stop DAQ", "Stop data acquisition"))
command_collection.add_command("TOF", CommandItem("Reset DAQ", "Reset data acquisition system"))
command_collection.add_command("TOF", CommandItem("Reconnect Network", "Reset data acquisition system"))

command_collection.add_command("TOF", CommandItem("Init System", "Initialize the TOF system"))
command_collection.add_command("TOF", CommandItem("Make Bias Calib Table", ""))
command_collection.add_command("TOF", CommandItem("Make Simple Bias Set Table", ""))
command_collection.add_command("TOF", CommandItem("Make Simple Channel Map", ""))
command_collection.add_command("TOF", CommandItem("Make Simple Disc Set Table", ""))
command_collection.add_command("TOF", CommandItem("Read Temperature Sensors", ""))
command_collection.add_command("TOF", CommandItem("Acquire Threshold Calibration", ""))
command_collection.add_command("TOF", CommandItem("Acquire QDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Acquire TDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Acquire SiPM Data", ""))
command_collection.add_command("TOF", CommandItem("Acquire Threshold Calibration BN", ""))
command_collection.add_command("TOF", CommandItem("Acquire Threshold Calibration D", ""))
command_collection.add_command("TOF", CommandItem("Set FEM Power Off", ""))
command_collection.add_command("TOF", CommandItem("Set FEM Power On", ""))
command_collection.add_command("TOF", CommandItem("Start Asic Temp Record", ""))
command_collection.add_command("TOF", CommandItem("Stop Asic Temp Record", ""))
command_collection.add_command("TOF", CommandItem("Read Temperature Sensors Single", ""))

command_collection.add_command("TOF", CommandItem("Monitor Data Stream", ""))
command_collection.add_command("TOF", CommandItem("Logger Data Stream", ""))

command_collection.add_command("TOF", CommandItem("Process Threshold Calibration", ""))
command_collection.add_command("TOF", CommandItem("Process TDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Process QDC Calibration", ""))
command_collection.add_command("TOF", CommandItem("Convert Raw To Raw", ""))
command_collection.add_command("TOF", CommandItem("Convert Raw To Singles", ""))

command_collection.add_command("TOF", CommandItem("Convert Stg1 To Stg2", ""))
command_collection.add_command("TOF", CommandItem("Process QA Coin", ""))
command_collection.add_command("TOF", CommandItem("Process QA Coin Iridium", ""))
command_collection.add_command("TOF", CommandItem("Macro Thermal Calib", ""))
command_collection.add_command("TOF", CommandItem("Macro Stage0 Prebreakdown BN", ""))
command_collection.add_command("TOF", CommandItem("Macro Stage1 Unbiased TDC", ""))
command_collection.add_command("TOF", CommandItem("Macro Stage2 Prebreakdown QDC", ""))
command_collection.add_command("TOF", CommandItem("Macro Stage3 Operational D", ""))
command_collection.add_command("TOF", CommandItem("Macro Auto Run Cycle", ""))

