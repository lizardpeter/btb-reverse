"""Asset-free unit tests for the independent PCM16 comparison gate."""
from pathlib import Path
import struct
import tempfile
import unittest

from compare_retail_bink import compare_pcm


class PcmComparisonTest(unittest.TestCase):
    def test_exact_and_one_lsb(self):
        with tempfile.TemporaryDirectory() as directory:
            a, b = Path(directory) / 'a', Path(directory) / 'b'
            a.write_bytes(struct.pack('<5h', -32768, -1, 0, 1, 32767))
            b.write_bytes(struct.pack('<5h', -32767, -1, 0, 0, 32767))
            self.assertEqual(compare_pcm(a, b), {
                'samples': 5, 'changed_samples': 2, 'max_abs_lsb': 1, 'sum_squared_error': 2})
            self.assertEqual(compare_pcm(a, a)['changed_samples'], 0)

    def test_length_and_alignment(self):
        with tempfile.TemporaryDirectory() as directory:
            a, b = Path(directory) / 'a', Path(directory) / 'b'
            a.write_bytes(b'\x00\x00')
            for data in (b'', b'\x00', b'\x00\x00\x00\x00'):
                b.write_bytes(data)
                with self.assertRaises(ValueError):
                    compare_pcm(a, b)

    def test_no_signed_overflow(self):
        with tempfile.TemporaryDirectory() as directory:
            a, b = Path(directory) / 'a', Path(directory) / 'b'
            a.write_bytes(struct.pack('<h', -32768))
            b.write_bytes(struct.pack('<h', 32767))
            self.assertEqual(compare_pcm(a, b)['max_abs_lsb'], 65535)


if __name__ == '__main__':
    unittest.main()
