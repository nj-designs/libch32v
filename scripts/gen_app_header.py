"""
Create app config header file
"""
import sys
import configparser
import io
import binascii

H_HEADER = """/*
  Application config header file
  Do not edit, auto generated
*/
#pragma once

"""

# def get_device_cfg(key: str):
#     for i in range(len(device_name), 3, -1):
#         try:
#             sdn = device_name[0:i]
#             val = all_device_cfg[sdn][key]
#             if len(val):
#                 return val
#         except KeyError:
#             pass
#     return None

def bld_cfg_header(ini_file: str):

    op = io.StringIO()
    op.write(H_HEADER)

    all_device_cfg = configparser.ConfigParser(allow_no_value=True)
    all_device_cfg.read(ini_file)

    for k, v in all_device_cfg.defaults().items():
            if v is None:
                 op.write('#define APP_{0}\n'.format(k.upper()))
            else:
                try:
                    _ = int(v, 16)
                    op.write('#define APP_{0} {1}\n'.format(k.upper(), v))
                except ValueError:
                    op.write('#define APP_{0} "{1}"\n'.format(k.upper(), v))

    str = op.getvalue()
    op.close()
    return str

if __name__ == '__main__':
    hdr = bld_cfg_header(sys.argv[1])
    with open(sys.argv[2], 'w') as hf:
        hf.write(hdr)