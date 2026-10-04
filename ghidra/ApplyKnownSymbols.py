# Apply known Bob Builds a Park function names from known_symbols.csv.
#@category BTB

import csv
from ghidra.program.model.symbol import SourceType

csv_file = askFile("Select known_symbols.csv", "Open")
addr_space = currentProgram.getAddressFactory().getDefaultAddressSpace()
fm = currentProgram.getFunctionManager()

renamed = 0
created = 0
skipped = 0

with open(csv_file.getAbsolutePath(), "rb") as fh:
    reader = csv.DictReader(fh)
    for row in reader:
        addr_text = row["address"].strip()
        name = row["name"].strip()
        if not addr_text or not name:
            continue

        addr = addr_space.getAddress(addr_text)
        func = fm.getFunctionAt(addr)

        if func is None:
            func = createFunction(addr, name)
            if func is None:
                print("SKIP %s %s: could not create function" % (addr_text, name))
                skipped += 1
                continue
            created += 1
        else:
            try:
                func.setName(name, SourceType.USER_DEFINED)
                renamed += 1
            except Exception as exc:
                print("SKIP %s %s: %s" % (addr_text, name, exc))
                skipped += 1
                continue

        print("%s -> %s" % (addr_text, name))

print("Done: %d renamed, %d created, %d skipped" % (renamed, created, skipped))
