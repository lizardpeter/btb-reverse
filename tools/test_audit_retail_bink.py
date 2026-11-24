import json
import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import audit_retail_bink as audit


def movie(tracks=0):
    start = 44 + 12 * tracks + 12
    end = start + 8
    prefix = b"SJPi" + struct.pack("<10I", end - 8, 2, 4, 0, 4, 4, 15, 1, 0, tracks)
    track_table = b"".join(struct.pack("<III", 4096, 22050, i) for i in range(tracks))
    return prefix + track_table + struct.pack("<3I", start | 1, start + 4, end) + b"12345678"


class AuditTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.addCleanup(self.tmp.cleanup)

    def test_inventory_and_aliases(self):
        (self.root / "one.BIK").write_bytes(movie(1))
        (self.root / "two.bik").write_bytes(movie(1))
        (self.root / "other.wav").write_bytes(b"ignored")
        report = audit.audit(self.root, None, None, 10, None)
        self.assertEqual(report["movie_count"], 2)
        self.assertEqual(report["unique_valid_movies"], 1)
        self.assertEqual(report["partial_paths"], 2)
        self.assertEqual(report["movies"][1]["identical_to"], "one.BIK")
        self.assertEqual(report["movies"][0]["header"]["audio_tracks"][0]["track_id"], 0)
        self.assertEqual(report["movies"][0]["header"]["keyframes"], 1)

    def test_fail_closed_on_bad_container(self):
        (self.root / "bad.bik").write_bytes(movie()[:-1])
        report = audit.audit(self.root, None, None, 10, None)
        self.assertEqual(report["failed_paths"], 1)
        self.assertIn("declared length", report["movies"][0]["error"])

    def test_native_decode_requires_both_video_and_audio(self):
        (self.root / "one.bik").write_bytes(movie(2))
        def fake(command, timeout):
            if command[0] == "video":
                return {"status": "passed", "output_tail": "SUMMARY decoded=2/2 frames"}
            return {"status": "passed", "output_tail": "Native Bink1 audio structural audit: 2/2 packets\nPCM16 validated bytes=4096"}
        with patch.object(audit, "run_tool", side_effect=fake) as run:
            report = audit.audit(self.root, Path("video"), Path("audio"), 10, None)
            self.assertEqual(report["fully_decoded_paths"], 1)
            self.assertEqual(run.call_count, 3)
        with patch.object(audit, "run_tool", side_effect=fake):
            report = audit.audit(self.root, Path("video"), None, 10, None)
            self.assertEqual(report["partial_paths"], 1)

    def test_strict_cli_gate(self):
        (self.root / "one.bik").write_bytes(movie(1))
        with tempfile.TemporaryDirectory() as output:
            out = Path(output) / "report.json"
            video = Path(output) / "video"
            audio = Path(output) / "audio"
            video.write_bytes(b"video tool")
            audio.write_bytes(b"audio tool")
            base = [str(self.root), "--video-tool", str(video),
                    "--require-complete", "--out", str(out)]
            def fake(command, timeout):
                if command[0] == str(video):
                    return {"status": "passed", "output_tail": "SUMMARY decoded=2/2 frames"}
                return {"status": "passed", "output_tail":
                        "Native Bink1 audio structural audit: 2/2 packets\nPCM16 validated bytes=4096"}
            with patch.object(audit, "run_tool", side_effect=fake):
                self.assertEqual(audit.main(base), 1)  # audio missing
                self.assertFalse(json.loads(out.read_text())["complete_decode"])
                both = base + ["--audio-tool", str(audio)]
                self.assertEqual(audit.main(both + ["--expected-count", "2"]), 1)
                self.assertEqual(audit.main(both + ["--expected-count", "1"]), 0)
                report = json.loads(out.read_text())
                self.assertTrue(report["complete_decode"])
                self.assertTrue(report["inventory_count_matches"])
                self.assertEqual(report["video_frames_validated"], 2)
                self.assertEqual(report["audio_tracks_validated"], 1)
                self.assertEqual(report["native_tools_sha256"]["video"], audit.digest(video))

    def test_incomplete_audio_is_failure(self):
        (self.root / "one.bik").write_bytes(movie(1))
        def fake(command, timeout):
            if command[0] == "video":
                return {"status": "passed", "output_tail": "SUMMARY decoded=2/2 frames"}
            return {"status": "passed", "output_tail": "Native Bink1 audio structural audit: 1/2 packets"}
        with patch.object(audit, "run_tool", side_effect=fake):
            report = audit.audit(self.root, Path("video"), Path("audio"), 10, None)
            self.assertEqual(report["failed_paths"], 1)

    def test_missing_native_summary_is_failure(self):
        (self.root / "one.bik").write_bytes(movie())
        with patch.object(audit, "run_tool", return_value={"status": "passed", "output_tail": ""}):
            report = audit.audit(self.root, Path("video"), None, 10, None)
            self.assertEqual(report["failed_paths"], 1)


if __name__ == "__main__":
    unittest.main()
