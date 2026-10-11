# Deterministic per-function Ghidra export for the verified Bob Builds a Park PE32.
# Execute ONLY with analyzeHeadless -postScript (never prompts for input).
#@category BTB

import csv
import io
import json
import os
import re
import traceback
from ghidra.app.decompiler import DecompInterface
from ghidra.program.model.symbol import SourceType
from ghidra.util.task import ConsoleTaskMonitor

EXPECTED_SHA256 = "c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05"
EXPECTED_SIZE = 311296

args = getScriptArgs()
if len(args) != 2:
    raise RuntimeError("Usage: -postScript ExportFullHeadless.py OUTPUT_DIRECTORY KNOWN_SYMBOLS_CSV")
out_dir = os.path.abspath(args[0])
known_symbols_file = os.path.abspath(args[1])
functions_dir = os.path.join(out_dir, "functions")
if not os.path.isdir(functions_dir):
    os.makedirs(functions_dir)

monitor = ConsoleTaskMonitor()
program = currentProgram
manager = program.getFunctionManager()
address_space = program.getAddressFactory().getDefaultAddressSpace()

# Retain a stable symbol naming provenance; never pretend names are from a PDB.
applied = 0
created = 0
name_failures = []
with open(known_symbols_file, "rb") as fh:
    for row in csv.DictReader(fh):
        raw_address = row.get("address", "").strip()
        name = row.get("name", "").strip()
        if not raw_address or not name:
            continue
        try:
            addr = address_space.getAddress(raw_address)
            if addr is None:
                raise ValueError("invalid address")
            memory_block = program.getMemory().getBlock(addr)
            if memory_block is None or not memory_block.isExecute():
                raise ValueError("not executable memory")
            func = manager.getFunctionAt(addr)
            if func is None:
                func = createFunction(addr, name)
                if func is None:
                    raise RuntimeError("createFunction failed")
                created += 1
            else:
                func.setName(name, SourceType.USER_DEFINED)
                applied += 1
        except Exception as exc:
            name_failures.append([raw_address, name, str(exc)])

decompiler = DecompInterface()
decompiler.toggleCCode(True)
decompiler.toggleSyntaxTree(True)
if not decompiler.openProgram(program):
    raise RuntimeError("Unable to start Ghidra decompiler")

functions = list(manager.getFunctions(True))
rows = []
calls = []
failures = []
success = 0
try:
    for index, func in enumerate(functions):
        address = str(func.getEntryPoint())
        name = str(func.getName())
        size = long(func.getBody().getNumAddresses())
        try:
            callers = sorted([str(x.getEntryPoint()) for x in func.getCallingFunctions(monitor)])
        except Exception:
            callers = []
        try:
            callees = sorted([str(x.getEntryPoint()) for x in func.getCalledFunctions(monitor)])
        except Exception:
            callees = []
        for target in callees:
            calls.append([address, target])
        try:
            result = decompiler.decompileFunction(func, 120, monitor)
            ok = bool(result.decompileCompleted()) and result.getDecompiledFunction() is not None
            message = str(result.getErrorMessage() or "")
            source = unicode(result.getDecompiledFunction().getC()) if ok else u""
        except Exception as exc:
            ok = False
            message = str(exc)
            source = u""

        if ok:
            success += 1
        else:
            failures.append([address, name, message])
        filename = address.upper().replace("0X", "").replace(":", "_") + ".c"
        function_path = os.path.join(functions_dir, filename)
        with io.open(function_path, "w", encoding="utf-8") as output:
            output.write(u"/* Ghidra generated pseudocode. NOT verified original C/C++.\n")
            output.write(u" * source-exe-sha256: %s\n" % EXPECTED_SHA256)
            output.write(u" * address: %s; function: %s; body bytes: %s\n" % (address, name, size))
            output.write(u" * callers: %d; callees: %d; success: %s\n */\n\n" %
                         (len(callers), len(callees), ok))
            if ok:
                output.write(source)
                if not source.endswith(u"\n"):
                    output.write(u"\n")
            else:
                output.write(u"/* DECOMPILATION FAILED: %s */\n" % unicode(message))
        rows.append([address, name, str(size), str(len(callers)), str(len(callees)),
                     "1" if ok else "0", filename, message])
        if (index + 1) % 100 == 0:
            print("BTB_GHIDRA_PROGRESS %d/%d successful=%d" %
                  (index + 1, len(functions), success))
finally:
    decompiler.dispose()

def save_csv(filename, header, records):
    with open(os.path.join(out_dir, filename), "wb") as output:
        writer = csv.writer(output)
        writer.writerow(header)
        writer.writerows(records)

save_csv("functions.csv",
         ["address", "name", "body_bytes", "callers", "callees", "decompiled", "file", "error"],
         rows)
save_csv("callgraph.csv", ["caller", "callee"], sorted(set(tuple(c) for c in calls)))
save_csv("failures.csv", ["address", "name", "error"], failures)
save_csv("known_symbol_failures.csv", ["address", "name", "error"], name_failures)

external = []
try:
    itr = program.getSymbolTable().getExternalSymbols()
    while itr.hasNext():
        sym = itr.next()
        external.append([str(sym.getAddress()), str(sym.getName()),
                         str(sym.getParentNamespace())])
except Exception as exc:
    print("External symbol enumeration failed: %s" % exc)
save_csv("external_symbols.csv", ["address", "name", "namespace"], external)

manifest = {
    "source": "Bob the Builder: Bob Builds a Park (Windows 2002) main PE32",
    "expected_exe_sha256": EXPECTED_SHA256,
    "expected_exe_size": EXPECTED_SIZE,
    "program_name": str(program.getName()),
    "processor": str(program.getLanguageID()),
    "image_base": str(program.getImageBase()),
    "function_count": len(functions),
    "decompiled_count": success,
    "failed_count": len(failures),
    "known_names_applied": applied,
    "known_functions_created": created,
    "known_names_failed": len(name_failures),
    "callgraph_edges": len(calls),
    "note": "Coverage counts reflect Ghidra-discovered functions, not full program semantics or recompilable C++."
}
with io.open(os.path.join(out_dir, "manifest.json"), "w", encoding="utf-8") as out:
    out.write(unicode(json.dumps(manifest, sort_keys=True, indent=2)) + u"\n")
print("BTB_GHIDRA_COMPLETE functions=%d succeeded=%d failed=%d" %
      (len(functions), success, len(failures)))
if len(functions) == 0 or success == 0:
    raise RuntimeError("No complete pseudocode emitted; refusing to treat export as successful")
