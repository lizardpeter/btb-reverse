"""Deterministic registry tests; require only the checked-in Ghidra corpus."""
from __future__ import annotations

import csv
import importlib.util
import json
import pathlib
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("build_reconstruction_index", ROOT / "tools/build_reconstruction_index.py")
module = importlib.util.module_from_spec(spec)
assert spec.loader is not None
spec.loader.exec_module(module)


class ReconstructionIndexTests(unittest.TestCase):
    def test_disjoint_verified_function_inventory(self):
        with (ROOT / "analysis/ghidra_main_exe/functions.csv").open(newline="", encoding="utf-8") as file:
            rows = list(csv.DictReader(file))
        self.assertEqual(615, len(rows))
        self.assertEqual(615, len({r["address"].lower() for r in rows}))
        self.assertTrue(all(r["decompiled"] == "1" for r in rows))

    def test_module_ranges_are_tentative_but_stable(self):
        self.assertEqual("dinosaur", module.subsystem(0x0040A440))
        self.assertEqual("game_flow", module.subsystem(0x0042A2C0))
        self.assertEqual("unclassified", module.subsystem(0x00414410))

    def test_dependency_sccs_identify_cycles_and_singletons(self):
        vertices = ["00000001", "00000002", "00000003", "00000004"]
        calls = {"00000001": {"00000002"}, "00000002": {"00000001"},
                 "00000003": {"00000002"}, "00000004": set()}
        self.assertEqual(
            [["00000001", "00000002"], ["00000003"], ["00000004"]],
            module.components(vertices, calls),
        )

    def test_generated_registry_matches_ghidra(self):
        module.run(check=True)
        summary = json.loads((ROOT / "analysis/reconstruction/summary.json").read_text())
        self.assertEqual(615, summary["ghidra_function_count"])
        self.assertEqual(1550, summary["direct_internal_edges"])
        self.assertEqual(296, summary["direct_external_edges"])
        self.assertEqual(615, sum(summary["stage_counts"].values()))

    def test_queue_does_not_claim_unsourced_implementations(self):
        with (ROOT / "analysis/reconstruction/work_queue.csv").open(newline="", encoding="utf-8") as file:
            rows = list(csv.DictReader(file))
        self.assertEqual(615, len(rows))
        self.assertEqual(list(range(1, 616)), sorted(int(r["rank"]) for r in rows))
        self.assertTrue(all(r["pseudocode"].startswith("analysis/ghidra_main_exe/functions/") for r in rows))
        self.assertTrue(all(r["stage"] in module.STAGES for r in rows))


if __name__ == "__main__":
    unittest.main()
