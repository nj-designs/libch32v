"""
Create config header file
"""
import sys
import configparser
import io
import binascii

ini_file = sys.argv[1]
device_name = sys.argv[2]

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

    device_constant = 'WCH_' + device_name.upper()

    device_constant_val = hex(binascii.crc32(device_constant.encode('utf8')))
    op.write(f"{device_constant}={device_constant_val} ")

    op.write(f'LIBCH32_DEVICE_ID={device_constant} ')

    op.write(f'LIBCH32_DEVICE_NAME="{device_name.upper()}" ')

    for k in sorted(all_keys):
        cfg_val = get_device_cfg(k)
        if cfg_val is not None:
            try:
                _ = int(cfg_val, 16)
                op.write('LIBCH32_{0}={1} '.format(k.upper(), cfg_val))
            except ValueError:
                op.write('LIBCH32_{0}="{1}" '.format(k.upper(), cfg_val))
            # Special handling
            if k == 'family':
                op.write('LIBCH32_{0}_FAMILY=1 '.format(cfg_val.upper()))


    str = op.getvalue()
    op.close()
    return str

if __name__ == '__main__':
    print(bld_cfg_header())
