"""Validate final 224 cohort and full 615-function architecture inventory."""
from __future__ import annotations

import csv
import io
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))

import build_remaining_architecture as builder


class FinalArchitectureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.outputs = builder.build()
        cls.full = list(csv.DictReader(io.StringIO(
            cls.outputs[builder.ARCH / "architecture_615.csv"])))
        cls.final = list(csv.DictReader(io.StringIO(
            cls.outputs[builder.ARCH / "architecture_224.csv"])))
        cls.summary = json.loads(
            cls.outputs[builder.ARCH / "architecture_615_summary.json"])
        cls.reviewed_count = len(builder.common.read_csv(builder.REVIEWED))

    def test_exact_disjoint_coverage(self):
        self.assertEqual(len(self.full), 615)
        self.assertEqual(len(self.final), 224)
        self.assertEqual(len({r["address"] for r in self.full}), 615)
        self.assertEqual(len({r["address"] for r in self.final}), 224)
        self.assertEqual(
            self.summary["cohort_counts"],
            {
                "architecture_batch_200": 200,
                "architecture_batch_224": 224,
                "previous_source_written": 191,
            },
        )
        self.assertEqual(self.summary["architecture_mapped"], 615)
        self.assertEqual(self.summary["source_written"], self.reviewed_count)
        self.assertEqual(self.summary["architecture_only"], 615 - self.reviewed_count)
        self.assertEqual(self.summary["cpp_compiles"], sum(
            r["source_stage"] == "compiles" for r in self.full))
        self.assertEqual(self.summary["behavior_verified"], 0)

    def test_final_224_are_runtime_only(self):
        self.assertTrue(all(
            r["module_candidate"] == "static_runtime" for r in self.final))
        # Later C++ reconstruction must not change original architecture cohort.
        self.assertTrue(all(
            r["source_stage"] in ("unreviewed", "source_written", "compiles", "behavior_verified")
            for r in self.final))
        self.assertTrue(all(
            r["mapping_cohort"] == "architecture_batch_224" for r in self.final))

    def test_provenance_and_dependency_integrity(self):
        by_address = {r["address"]: r for r in self.full}
        for r in self.full:
            self.assertEqual(len(r["pseudocode_sha256"]), 64)
            self.assertTrue(r["pseudocode"].endswith(".c"))
            self.assertEqual(r["disclaimer"], "evidence_mapping_not_C++_implementation")
            for addr in r["callee_addresses"].split(";"):
                if addr:
                    self.assertIn(addr, by_address)

    def test_source_reconstruction_stages_preserved(self):
        mapped = [r for r in self.full if r["source_stage"] in ("source_written","compiles","behavior_verified")]
        self.assertEqual(len(mapped), self.reviewed_count)
        self.assertTrue(all(r["explicit_cpp_source"] for r in mapped))
        self.assertTrue(all(r["cpp_validation_test"] for r in mapped))
        self.assertEqual(
            sum(r["source_stage"] == "behavior_verified" for r in self.full), 0)

    def test_global_evidence_is_noncommittal(self):
        globals_data = json.loads(
            self.outputs[builder.ARCH / "architecture_615_globals.json"])
        self.assertGreater(len(globals_data), 1000)
        for row in globals_data:
            self.assertEqual(row["data_type"], "unknown")
            self.assertEqual(row["read_write_direction"], "not_inferred")
            self.assertTrue(row["original_global"].startswith("0x"))

    def test_cpp_registry_metadata_not_body(self):
        header = self.outputs[
            builder.ROOT / "reconstruction/include/btb/original_architecture_615.hpp"]
        self.assertIn("Not 615 source implementations", header)
        self.assertIn("kOriginalArchitecture615", header)


if __name__ == "__main__":
    unittest.main()
