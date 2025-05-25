"""
Simple wrapper around wlink to help with multiple WCH-Link devices
"""
import argparse
import errno
import pathlib
import re
import shlex
import subprocess
import sys


class FlashError(Exception):
    def __init__(self, error_code, msg):
        self.message = msg
        self.error_code = error_code

    def get_error_code(self):
        return self.error_code


class FlashHelper(object):
    WLINK_EXE = 'wlink'
    MAX_WCH_LINK_DEVICES = 10

    def __init__(self):
        parser = argparse.ArgumentParser()

        parser.add_argument(
            '--device',
            required=True,
            type=str,
            help='Full device name example:ch32v307vct6',
        )

        parser.add_argument(
            '--address',
            required=True,
            type=lambda x: int(x, 0),
            help='Flash bash address example:0x08000000',
        )

        parser.add_argument(
            '--file',
            required=True,
            type=pathlib.Path,
            help='Path to .bin file to flash',
        )

        parser.add_argument(
            '--verbose', action='store_true', help='Verbose output'
        )

        args = parser.parse_args()

        self.flash_device = args.device
        self.flash_address = args.address
        self.flash_file = args.file
        self.verbose = args.verbose

    def _run_wlink(self, args: str):
        arg_str = self.WLINK_EXE + ' ' + args
        if self.verbose:
            print('cmd: ' + arg_str)
        return subprocess.run(shlex.split(arg_str), capture_output=True)

    def _check_attached_chip(self, device_idx: int, chip_id: str):
        res = self._run_wlink(f'--device {device_idx} status')
        if len(res.stderr):
            raise FlashError(str(res.stderr, encoding='utf-8'), res.returncode)
        if re.search(
            chip_id, str(res.stdout, encoding='utf-8'), re.IGNORECASE
        ):
            return True
        else:
            return False

    def _get_wch_link_device_idx(self, chip_id: str):
        for idx in range(self.MAX_WCH_LINK_DEVICES):
            try:
                if self._check_attached_chip(idx, chip_id):
                    return idx
            except:
                pass
        return -1

    def _flash_device(self, device_idx: int, address: int, file_path: str):
        res = self._run_wlink(
            f'--device {device_idx} flash --address 0x{address:08X} {file_path}'
        )
        if len(res.stderr):
            raise FlashError(str(res.stderr, encoding='utf-8'), res.returncode)
        if self.verbose:
            print(str(res.stdout, encoding='utf-8'))

    def run(self):
        device_idx = self._get_wch_link_device_idx(self.flash_device)
        if device_idx == -1:
            raise FlashError(
                f"Can't find connected {self.flash_device}", errno.ENODEV
            )
        self._flash_device(device_idx, self.flash_address, self.flash_file)


if __name__ == '__main__':
    fh = FlashHelper()
    try:
        res = fh.run()
    except FlashError as rte:
        sys.exit(rte.get_error_code())
