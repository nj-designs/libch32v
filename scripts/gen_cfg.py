"""
Generate config .h file from supplied .ini file
"""
import configparser
import sys
import os
import io
import hashlib


H_HEADER = """/*
  Do not edit, auto generated
*/
#pragma once
"""

ini_file_name = sys.argv[1]
h_file_name = sys.argv[2]

# Read & parse ini file
cfg_parser = configparser.ConfigParser()
cfg_parser.read(ini_file_name)

# Start output


def bld_h_file_contents():
    """
    Return contents as a string
    """
    op = io.StringIO()
    op.write(H_HEADER)
    for section_name in cfg_parser.sections():
        op.write("\n// [{0}]\n".format(section_name))
        items = cfg_parser.items(section_name)
        for ini_key, ini_value in items:
            cfg_name = "{0}_{1}".format(section_name, ini_key).upper()
            env_v = os.environ.get(cfg_name, ini_value)
            if env_v != ini_value:
                comment = "// Overridden From env"
            else:
                comment = "// From ini file"
            try:
                _ = int(env_v, 16)
                op.write("#define {0} ({1}) {2}\n".format(
                    cfg_name, env_v, comment))
            except ValueError:
                if not '"' in env_v:
                    env_v = '"{0}"'.format(env_v)
                op.write("#define {0} {1} {2}\n".format(
                    cfg_name, env_v, comment))
    str = op.getvalue()
    op.close()
    return str


# Generate potential new contents and hash
new_h_file_contents = bld_h_file_contents()
n_h_hash = hashlib.md5(new_h_file_contents.encode()).hexdigest()

# Read in original and hash
try:
    with open(h_file_name) as hf:
        old_h_file_contents = hf.read()
    o_h_hash = hashlib.md5(old_h_file_contents.encode()).hexdigest()
except FileNotFoundError:
    o_h_hash = ""
if n_h_hash != o_h_hash:
    print("cfg: updated")
    with open(h_file_name, 'w') as hf:
        hf.write(new_h_file_contents)
else:
    print("cfg: update skip")
