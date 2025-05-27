"""
Generate a minimal compile_commands.json file for LIBCH32V
"""
import argparse
import json
import os
import os.path
import pathlib
import sys


class CompileCommandsHelper(object):
    def __init__(self):
        parser = argparse.ArgumentParser()

        parser.add_argument(
            '--family',
            required=True,
            type=str,
            help='family of device. example v203',
        )

        parser.add_argument(
            '--out-file',
            type=pathlib.Path,
            required=True,
            help='path of output file',
        )

        parser.add_argument(
            '--base-dir',
            type=pathlib.Path,
            required=True,
            help='base dir of LIBCH32V',
        )

        parser.add_argument(
            '--app-name',
            type=str,
            required=True,
            help='name of application',
        )

        parser.add_argument(
            '--cc',
            type=str,
            required=True,
            help='compile commnad',
        )

        parser.add_argument(
            '--bld-dir',
            type=pathlib.Path,
            default='build',
            help='build directory',
        )

        parser.add_argument(
            '--verbose', action='store_true', help='verbose output'
        )

        args = parser.parse_args()

        self.family = args.family
        self.out_file_path = args.out_file
        self.base_dir = args.base_dir
        self.app_name = args.app_name
        self.cc = args.cc
        self.bld_dir = args.bld_dir
        self.verbose = args.verbose

    def _get_translation_units(self, tul: list, tud: str):
        for tup in os.listdir(os.path.join(self.base_dir, tud)):
            fp = os.path.join(tud, tup)
            if os.path.isfile(fp) and fp.lower().endswith('.c'):
                tul.append(fp)
        return tul

    def _add_tu_entry(self, ccj: list, tu: str):
        args = self.cc.split()
        args.append('-o')
        of = os.path.join(self.bld_dir, tu)
        args.append(f'{of}')
        args.append(tu)
        e = {
            'arguments': args,
            'directory': str(self.base_dir),
            'file': tu,
        }

        ccj.append(e)

    def run(self):
        ccj = []

        tul = []
        self._get_translation_units(tul, os.path.join('app', self.app_name))
        self._get_translation_units(tul, os.path.join('lib', 'src'))
        self._get_translation_units(
            tul, os.path.join('lib', 'src', self.family)
        )

        for tu in tul:
            self._add_tu_entry(ccj, tu)

        with open(self.out_file_path, mode='wt', encoding='utf-8') as f:
            json.dump(ccj, f, indent=4)


if __name__ == '__main__':
    cch = CompileCommandsHelper()
    try:
        res = cch.run()
    except Exception as ex:
        print(ex, file=sys.stderr)
        sys.exit(1)
