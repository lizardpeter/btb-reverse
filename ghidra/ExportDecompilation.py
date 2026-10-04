# Export a function index and bulk Ghidra decompiler output for Bob Builds a Park.
#@category BTB

import csv
import os
from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

out_dir = askDirectory("Choose export directory", "Export")
out_path = out_dir.getAbsolutePath()
monitor = ConsoleTaskMonitor()

decomp = DecompInterface()
decomp.toggleCCode(True)
decomp.toggleSyntaxTree(True)
if not decomp.openProgram(currentProgram):
    raise RuntimeError("Could not initialize Ghidra decompiler")

functions_path = os.path.join(out_path, "functions.csv")
decompiled_path = os.path.join(out_path, "decompiled.c")

fm = currentProgram.getFunctionManager()
funcs = fm.getFunctions(True)

rows = []
blocks = []
count = 0

for func in funcs:
    if monitor.isCancelled():
        break

    entry = func.getEntryPoint()
    name = func.getName()
    body_size = func.getBody().getNumAddresses()

    callers = 0
    callees = 0
    try:
        callers = len(list(func.getCallingFunctions(monitor)))
    except:
        pass
    try:
        callees = len(list(func.getCalledFunctions(monitor)))
    except:
        pass

    result = decomp.decompileFunction(func, 60, monitor)
    ok = result.decompileCompleted()
    c_text = ""
    if ok and result.getDecompiledFunction() is not None:
        c_text = result.getDecompiledFunction().getC()

    rows.append([
        str(entry),
        name,
        str(body_size),
        str(callers),
        str(callees),
        "1" if ok else "0",
    ])

    blocks.append("/* ============================================================ */\n")
    blocks.append("/* %s @ %s | bytes=%s callers=%s callees=%s */\n" %
                  (name, entry, body_size, callers, callees))
    blocks.append("/* ============================================================ */\n")
    if c_text:
        blocks.append(c_text)
        if not c_text.endswith("\n"):
            blocks.append("\n")
    else:
        blocks.append("/* decompilation failed */\n")
    blocks.append("\n")

    count += 1
    if count % 100 == 0:
        print("Decompiled %d functions..." % count)

with open(functions_path, "wb") as fh:
    writer = csv.writer(fh)
    writer.writerow(["address", "name", "body_bytes", "caller_count", "callee_count", "decompiled"])
    writer.writerows(rows)

with open(decompiled_path, "wb") as fh:
    for block in blocks:
        if isinstance(block, unicode):
            block = block.encode("utf-8")
        fh.write(block)

decomp.dispose()

print("Exported %d functions" % count)
print("Function index: %s" % functions_path)
print("Decompiler output: %s" % decompiled_path)
