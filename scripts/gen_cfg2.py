"""
Create config header file
"""
import sys
import configparser
import io
import binascii

H_HEADER = """/*
  Do not edit, auto generated
*/
#pragma once

"""

ini_file = sys.argv[1]
device_name = sys.argv[2]
op_file = sys.argv[3]

all_device_cfg = configparser.ConfigParser()
all_device_cfg.read(ini_file)

def get_device_cfg(key: str):
    for i in range(len(device_name), 3, -1):
        try:
            sdn = device_name[0:i]
            val = all_device_cfg[sdn][key]
            if len(val):
                return val
        except KeyError:
            pass
    return None

def bld_cfg_header():

    all_keys = set()

    for sec in all_device_cfg.sections():
        for k in all_device_cfg[sec]:
            all_keys.add(k)

    op = io.StringIO()
    op.write(H_HEADER)

    device_constant = 'WCH_' + device_name.upper()

    device_constant_val = hex(binascii.crc32(device_constant.encode('utf8')))
    op.write(f"#define {device_constant} {device_constant_val}\n")

    op.write(f'#define LIBCH32_DEVICE_ID {device_constant}\n')

    op.write(f'#define LIBCH32_DEVICE_NAME "{device_name.upper()}"\n')

    op.write('\n')

    for k in sorted(all_keys):
        cfg_val = get_device_cfg(k)
        if cfg_val is not None:
            try:
                _ = int(cfg_val, 16)
                op.write('#define LIBCH32_{0} {1}\n'.format(k.upper(), cfg_val))
            except ValueError:
                op.write('#define LIBCH32_{0} "{1}"\n'.format(k.upper(), cfg_val))

    str = op.getvalue()
    op.close()
    return str

if __name__ == '__main__':
    # get_device_cfg(sys.argv[1], sys.argv[2])
    hdr = bld_cfg_header()
    with open(op_file, 'w') as hf:
        hf.write(hdr)