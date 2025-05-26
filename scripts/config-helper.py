"""
Generate combined config file
"""
import argparse
import binascii
import configparser
import pathlib
import sys
import textwrap
from typing import TextIO

HEADER = textwrap.dedent(
    """\
/**
 * LIBCH32 auto generated configfile
 */
#pragma once
"""
)


class ConfigFileWriter(object):
    def __init__(self):
        parser = argparse.ArgumentParser(
            description='This tool extracts varions device and application config values'
        )

        parser.add_argument(
            '--verbose', action='store_true', help='verbose output'
        )

        sub_parser = parser.add_subparsers(dest='command', required=True)

        device_args = argparse.ArgumentParser(add_help=False)
        device_args.add_argument(
            '--device-file',
            type=pathlib.Path,
            required=True,
            help='path to device config INI file',
        )
        device_args.add_argument(
            '--device-id',
            type=str,
            required=True,
            help='full device id example:ch32v307vct6',
        )

        app_args = argparse.ArgumentParser(add_help=False)
        app_args.add_argument(
            '--app-file',
            type=pathlib.Path,
            required=True,
            help='path to app config INI file',
        )

        key_args = argparse.ArgumentParser(add_help=False)
        key_args.add_argument(
            '--cfg-name',
            type=str,
            required=True,
            help='name of config key to query',
        )

        sub_parser.add_parser(
            'get-device-cfg',
            help='return specific device cfg value',
            parents=[device_args, key_args],
        )

        sub_parser.add_parser(
            'get-app-cfg',
            help='return specific app cfg value',
            parents=[app_args, key_args],
        )

        gen_cfg_header_file = sub_parser.add_parser(
            'gen-header-file',
            help='generate config header file',
            parents=[app_args, device_args],
        )
        gen_cfg_header_file.add_argument(
            '--header-file',
            type=pathlib.Path,
            required=True,
            help='header output file',
        )

        self._args = vars(parser.parse_args())
        self._device_cfg = configparser.ConfigParser(allow_no_value=True)
        self._app_cfg = configparser.ConfigParser(allow_no_value=True)

    def _get_device_value(self, device_id: str, key: str):
        """Returns the most device specific value for given key

        Given an device_id of ch32v203c8t6, this function will search for k
        in the following sections.

        ch32v203c8t6
        ch32v203c8t
        ch32v203c8
        ch32v203c
        ch32v203
        ch32v20
        ch32v2
        ch32v

        Args:
          device_id: The full id of the CH32V device
          key: Cfg value key to look for

        Returns:
          A str of the value for supplied key
        """
        for i in range(len(device_id), 3, -1):
            try:
                sdn = device_id[0:i]
                val = self._device_cfg[sdn][key]
                if len(val):
                    return val
            except KeyError:
                pass
        return None

    def _get_device_cfg(self):
        device_id = self._args['device_id']
        key = self._args['cfg_name']
        v = self._get_device_value(device_id, key)
        if v is None:
            raise KeyError(f"Can't find {key} in any section")
        print(v, end='')

    def _print_device_defs(self, f: TextIO):
        print(
            f"// Device defines for {self._args['device_id']} from {self._args['app_file']}\n",
            file=f,
        )
        # Get all unique keys
        all_keys = set()
        for sec in self._device_cfg.sections():
            for k in self._device_cfg[sec]:
                all_keys.add(k)

        device_id = self._args['device_id']

        device_constant = 'WCH_' + device_id.upper()

        device_constant_val = hex(
            binascii.crc32(device_constant.encode('utf8'))
        )
        print(f'#define {device_constant}={device_constant_val} ', file=f)

        print(f'#define LIBCH32_DEVICE_ID={device_constant} ', file=f)

        print(f'#define LIBCH32_DEVICE_NAME="{device_id.upper()}" ', file=f)

        for k in sorted(all_keys):
            cfg_val = self._get_device_value(device_id, k)
            if cfg_val is not None:
                try:
                    _ = int(cfg_val, 16)
                    print(
                        '#define LIBCH32_{0}={1} '.format(k.upper(), cfg_val),
                        file=f,
                    )
                except ValueError:
                    print(
                        '#define LIBCH32_{0}="{1}" '.format(
                            k.upper(), cfg_val
                        ),
                        file=f,
                    )
                # Special handling
                if k == 'family':
                    print(
                        '#define LIBCH32_{0}_FAMILY=1 '.format(
                            cfg_val.upper()
                        ),
                        file=f,
                    )

    def _print_app_defs(self, f: TextIO):
        print(
            f"// Application defines from {self._args['app_file']}\n", file=f
        )
        for k, v in self._app_cfg.defaults().items():
            if v is None:
                print('#define APP_{0} '.format(k.upper()), file=f)
            else:
                try:
                    _ = int(v, 16)
                    print('#define APP_{0}={1} '.format(k.upper(), v), file=f)
                except ValueError:
                    print(
                        '#define APP_{0}="{1}" '.format(k.upper(), v), file=f
                    )

    def _get_app_cfg(self):
        print('Get App File')

    def _gen_header_file(self):
        print('Gen Header file')

        with open(self._args['header_file'], mode='wt', encoding='utf-8') as f:
            print(HEADER, file=f)
            self._print_app_defs(f)
            print('', file=f)
            self._print_device_defs(f)

    def run(self):

        if self._args.get('device_file'):
            self._device_cfg.read(self._args['device_file'])

        if self._args.get('app_file'):
            self._app_cfg.read(self._args['app_file'])

        {
            'get-device-cfg': self._get_device_cfg,
            'get-app-cfg': self._get_app_cfg,
            'gen-header-file': self._gen_header_file,
        }[self._args['command']]()


if __name__ == '__main__':
    try:
        cfw = ConfigFileWriter()
        cfw.run()
    except Exception as ex:
        print(ex, file=sys.stderr)
        sys.exit(1)
