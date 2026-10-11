import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import build_architecture_map as m

class ArchitectureMapTests(unittest.TestCase):
    def test_regions(self):
        self.assertEqual(m.module("0042a2c0"), "game_flow")
        self.assertEqual(m.module("0042ffc4"), "static_runtime")

    def test_global_address_evidence(self):
        globals_, assets, warnings, uncertainty, arrays = m.evidence(
            "DAT_0044DDD8 = 1; _DAT_0051c2e8; s_data_ui_004435d8; "
            "/* WARNING: register */ extraout_ECX")
        self.assertEqual(globals_, ["0044ddd8", "0051c2e8"])
        self.assertEqual(len(assets), 1)
        self.assertEqual(warnings, 1)
        self.assertIn("extraout_", uncertainty)

    def test_class_is_only_a_candidate(self):
        name, reason, confidence = m.owner("CSound_Play", "shared_platform", "void CSound_Play()")
        self.assertEqual(name, "CSound")
        self.assertEqual(confidence, "medium")
        name, reason, confidence = m.owner("LoadMaze", "maze", "void LoadMaze()")
        self.assertEqual(name, "MazeActivityModule")
        self.assertEqual(confidence, "low")

    def test_generate_from_exact_retail_corpus(self):
        generated = m.generate()
        s = generated[m.ARCH / "architecture_200.csv"]
        import csv
        import io
        rows = list(csv.DictReader(io.StringIO(s)))
        self.assertEqual(len(rows), 200)
        self.assertEqual(len({r["address"] for r in rows}), 200)
        self.assertTrue(all(r["disclaimer"] == "evidence_mapping_not_C++_implementation" for r in rows))
        summary = generated[m.ARCH / "architecture_200_summary.json"]
        self.assertEqual(__import__("json").loads(summary)["game_platform_mapped"], 181)

if __name__ == "__main__":
    unittest.main()
