# Apply labels to RunMainGameFlow case-handler addresses from gameflow_states.csv.
#@category BTB

import csv
import re
from ghidra.program.model.symbol import SourceType

csv_file = askFile("Select gameflow_states.csv", "Open")
space = currentProgram.getAddressFactory().getDefaultAddressSpace()
symbols = currentProgram.getSymbolTable()

count = 0
with open(csv_file.getAbsolutePath(), "rb") as fh:
    for row in csv.DictReader(fh):
        state = row["state"].strip()
        handler = row["handler"].strip()
        role = row["role"].strip()
        module = row["module"].strip()

        if not handler:
            continue

        addr = space.getAddress(handler)
        state_token = state.lower().replace("0x", "")
        role_token = re.sub(r"[^A-Za-z0-9_]+", "_", role)
        module_token = re.sub(r"[^A-Za-z0-9_]+", "_", module)
        label = "gameflow_state_%s_%s_%s" % (state_token, module_token, role_token)

        try:
            symbols.createLabel(addr, label, SourceType.USER_DEFINED)
            count += 1
            print("%s -> %s" % (handler, label))
        except Exception as exc:
            print("SKIP %s: %s" % (handler, exc))

print("Applied %d game-flow labels" % count)
