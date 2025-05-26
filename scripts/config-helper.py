"""
Generate combined config file
"""
import argparse
import configparser
import pathlib


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
        self._device_cfg = configparser.ConfigParser()
        self._app_cfg = configparser.ConfigParser()

    def _get_device_value(self):
        pass

    def _get_device_cfg(self):
        # all_device_keys = set()
        # for sec in self._device_cfg.sections():
        #     for k in self._device_cfg[sec]:
        #         all_device_keys.add(k)
        # print(all_device_keys)
        device_id = self._args['device_id']
        key = self._args['cfg_name']
        for i in range(len(device_id), 3, -1):
            try:
                sdn = device_id[0:i]
                val = self._device_cfg[sdn][key]
                if len(val):
                    print(val, end='')
                    return
            except KeyError:
                pass

    def _get_app_cfg(self):
        print('Get App File')

    def _gen_header_file(self):
        print('Gen Header file')

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
    cfw = ConfigFileWriter()
    cfw.run()
