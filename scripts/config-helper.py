"""
Generate combined config file
"""
import argparse
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

        self._args = parser.parse_args()

        print(self._args)

    def run(self):
        pass


if __name__ == '__main__':
    cfw = ConfigFileWriter()
    cfw.run()
