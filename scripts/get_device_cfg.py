"""
Extract values from device config file
"""
import sys
import configparser

def get_device_cfg(cfg_file: str, device_name: str, key: str):

    all_device_cfg = configparser.ConfigParser()
    all_device_cfg.read(cfg_file)


    for i in range(len(device_name), 3, -1):
        try:
            sdn = device_name[0:i]
            val = all_device_cfg[sdn][key]
            if len(val):
                print(val,end="")
                return
        except KeyError:
            pass

if __name__ == '__main__':
    get_device_cfg(sys.argv[1],  sys.argv[2], sys.argv[3])
