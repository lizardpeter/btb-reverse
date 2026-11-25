import struct
import unittest
from check_pe_imports import imports

def fixture(pe64=True, delay=True):
    data=bytearray(0x800)
    data[:2]=b"MZ"
    struct.pack_into("<I",data,0x3c,0x80)
    data[0x80:0x84]=b"PE\0\0"
    opt=0x98
    size=240 if pe64 else 224
    struct.pack_into("<H",data,0x86,1)
    struct.pack_into("<H",data,0x94,size)
    struct.pack_into("<H",data,opt,0x20b if pe64 else 0x10b)
    struct.pack_into("<I",data,opt+60,0x200)
    struct.pack_into("<I",data,opt+(108 if pe64 else 92),16)
    dirs=opt+(112 if pe64 else 96)
    struct.pack_into("<II",data,dirs+8,0x1000,40)
    struct.pack_into("<IIII",data,opt+size+8,0x600,0x1000,0x600,0x200)
    struct.pack_into("<5I",data,0x200,0,0,0,0x1100,0)
    data[0x300:0x30d]=b"KERNEL32.dll\0"
    if delay:
        struct.pack_into("<II",data,dirs+13*8,0x1040,64)
        struct.pack_into("<8I",data,0x240,1,0x1120,0,0,0,0,0,0)
        data[0x320:0x32a]=b"DDRAW.dll\0"
    return data
class ImportTests(unittest.TestCase):
    def test_pe32_and_pe64(self):
        for pe64 in (True,False):
            self.assertEqual(imports(fixture(pe64)),{"kernel32.dll","ddraw.dll"})
            self.assertEqual(imports(fixture(pe64,False)),{"kernel32.dll"})
    def test_bad_image(self):
        with self.assertRaises(ValueError): imports(b"no")
        with self.assertRaises(ValueError): imports(fixture()[:0x201])
    def test_unmapped_name(self):
        data=fixture()
        struct.pack_into("<I",data,0x20c,0xffff0000)
        with self.assertRaises(ValueError): imports(data)
if __name__ == "__main__":
    unittest.main()
