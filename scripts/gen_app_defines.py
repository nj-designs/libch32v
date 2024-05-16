"""
Create app config header file
"""
import sys
import configparser
import io
import binascii


def bld_cfg_defines(ini_file: str):

    op = io.StringIO()

    all_device_cfg = configparser.ConfigParser(allow_no_value=True)
    all_device_cfg.read(ini_file)

    for k, v in all_device_cfg.defaults().items():
            if v is None:
                 op.write('APP_{0} '.format(k.upper()))
            else:
                try:
                    _ = int(v, 16)
                    op.write('APP_{0}={1} '.format(k.upper(), v))
                except ValueError:
                    op.write('APP_{0}="{1}" '.format(k.upper(), v))

    str = op.getvalue()
    op.close()
    return str

if __name__ == '__main__':
    print(bld_cfg_defines(sys.argv[1]))
    