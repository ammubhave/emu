#!/usr/bin/env python3

import xml.etree.ElementTree as ET
import os

whitelisted_opcodes = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d,
                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
                       0x8c, 0x8e,
                       0x9e, 0x9f,
                       0xa8, 0xa9,
                       0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
                       0xd0, 0xd1, 0xd2, 0xd3,
                       0xe4, 0xe5, 0xe6, 0xe7, 0xe9, 0xea, 0xee, 0xef,
                       0xf4, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd}

if __name__ == "__main__":
    tree = ET.parse(os.path.join(os.path.dirname(
        os.path.realpath(__file__)), 'x86reference.xml'))
    root = tree.getroot()
    assert(root.tag == 'x86reference')

    mnems = set()
    for pri_opcd, of in [(pri_opcd, of) for opcodes, of in [(root.find('./one-byte'), False), (root.find('./two-byte'), True)] for pri_opcd in opcodes]:
        po = int(pri_opcd.attrib['value'], 16)
        if po not in whitelisted_opcodes or of == True:
            continue

        for entry in pri_opcd.findall('./entry'):
            grp1 = entry.find('./grp1')
            if grp1 is not None and grp1.text == 'prefix':
                continue

            syntaxs = entry.findall('./syntax')
            if len(syntaxs) > 1:
                if po in (0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x8c, 0xd0, 0xd1, 0xd2, 0xd3):
                    syntaxs = [syntaxs[0]]
            assert(len(syntaxs) == 1)

            for syntax in syntaxs:
                mnem = syntax.find('./mnem')
                if mnem is not None:
                    mnems.add(mnem.text)

    for mnem in sorted(list(mnems)):
        print(
            f"#include \"cpu/insts/gen/{mnem.lower().capitalize()}.h\"")

    print("#include \"cpu/insts/Illegal.h\"")
