#!/usr/bin/env python3
'''Helper script for checking that a submission ZIP file:
- contains all the necessary files
- does not contain unnecessary files
- compiles correctly (with provided files)
- passes basic tests
'''

import glob
import os
import shutil
import subprocess
import sys
import tempfile
import zipfile

### settings: assignement files ###

PROVIDED_PART1_FILES = ['amount_set_str.h']
PROVIDED_PART2_FILES = ['matamikya.h', 'matamikya_print.h', 'matamikya_print.c',
                        'list.h', 'set.h', 'amount_set.h', 'libmtm.a', 'libas.a']
PROVIDED_TEST_FILES = ['test_utilities.h',
                       'matamikya_tests.c', 'matamikya_tests.h', 'matamikya_main.c',
                       'expected_best_selling.txt', 'expected_no_selling.txt',
                       'expected_inventory.txt', 'expected_order.txt']
TESTS_DIR = 'tests'
PROVIDED_MISC_FILES = ['final_check.py']
PROVIDED_FILES = PROVIDED_PART2_FILES + PROVIDED_PART1_FILES + PROVIDED_TEST_FILES + PROVIDED_MISC_FILES
PROVIDED_FILES_DIR = os.path.dirname(__file__)

MINIMUM_REQUIRED_TOP_DIR_FILES = ['matamikya.c', 'amount_set_str.c']
MINIMUM_REQUIRED_TOP_DIR_FILES_CASE_INSENSITIVE = ['makefile', 'dry.pdf']

### settings: compilation ###

BASIC_GCC_ARGS = ['-std=c99', '-Wall', '-Werror', '-pedantic-errors']

PART1_NAME = 'amount_set_str'
EXEC_NAME_PART1 = 'amount_set_str'
GCC_ARGS_PART1 = BASIC_GCC_ARGS.copy()
# list of pairs: (subdirectory, glob pattern)
GCC_SOURCES_PART1 = [('', 'amount_set_str*.c'), (TESTS_DIR, 'amount_set_str*.c')]
GCC_ARGS_SUFFIX_PART1 = []

PART2_NAME = 'matamikya'
EXEC_NAME_PART2 = 'matamikya'
GCC_ARGS_PART2 = BASIC_GCC_ARGS.copy()
# list of pairs: (subdirectory, glob pattern)
GCC_SOURCES_PART2 = [('', 'matamikya*.c' ), (TESTS_DIR, 'matamikya*.c')]
GCC_ARGS_SUFFIX_PART2 = ['-L.', '-lm', '-lmtm', '-las']



def main():
    if len(sys.argv) != 2:
        print("Usage: %s <zipfile>" % sys.argv[0])
        return

    with tempfile.TemporaryDirectory() as path:
        print("Extracting zip to temporary folder:", path)
        zip_ref = zipfile.ZipFile(sys.argv[1], 'r')
        zip_ref.extractall(path)
        zip_ref.close()
        if not check_folder(path):
            print("Submission has errors, please fix.")
            return
        copy_provided_files(path)
        if not build_and_test(path):
            print("Submission has errors, please fix.")
            return
        print("Congratulations, you passed the provided tests!!!")

def check_folder(path):
    print("Verifying contents...")
    files = os.listdir(path)
    found_errors = check_folder_contents(files,
                                         MINIMUM_REQUIRED_TOP_DIR_FILES,
                                         MINIMUM_REQUIRED_TOP_DIR_FILES_CASE_INSENSITIVE,
                                         PROVIDED_FILES)
    if found_errors:
        print("There are errors in contents of the ZIP file.")
    else:
        print("The ZIP file contains all the necessary files.")
    return not found_errors

def check_folder_contents(contents, required_files, required_files_case_insensitive,
                          forbidden_files, prefix=''):
    found_errors = False
    for filename in required_files:
        pretty_name = os.path.join(prefix, filename)
        if filename in contents:
            print('Found required file/directory:', pretty_name)
        else:
            found_errors = True
            print('ERROR: Missing required file/directory:', pretty_name)

    for filename in required_files_case_insensitive:
        pretty_name = os.path.join(prefix, filename)
        if filename.lower() in map(str.lower, contents):
            print('Found required file/directory:', pretty_name)
        else:
            found_errors = True
            print('ERROR: Missing required file/directory:', pretty_name)

    for filename in forbidden_files:
        if filename in contents:
            found_errors = True
            print('ERROR: Found provided file/directory:', os.path.join(prefix, filename))

    for filename in contents:
        pretty_name = os.path.join(prefix, filename)
        if (filename in required_files or
            filename in forbidden_files or
            filename.lower() in required_files_case_insensitive):
            # already checked above
            continue

        if filename.endswith('.c') or filename.endswith('.h'):
            print('Found C file:', pretty_name)
            continue

        found_errors = True
        print('ERROR: Found unexpected file/directory:', pretty_name)

    return found_errors

def copy_provided_files(dst_dir):
    src_dir = PROVIDED_FILES_DIR
    src_tests_dir = os.path.join(src_dir, TESTS_DIR)
    dst_tests_dir = os.path.join(dst_dir, TESTS_DIR)
    os.makedirs(dst_tests_dir)
    for f in PROVIDED_PART1_FILES + PROVIDED_PART2_FILES:
        shutil.copy(os.path.join(src_dir, f), dst_dir)
    for f in PROVIDED_TEST_FILES:
        shutil.copy(os.path.join(PROVIDED_FILES_DIR, TESTS_DIR, f), dst_tests_dir)

def relative_glob(pattern, path, subdir=''):
    return [os.path.join(subdir, os.path.basename(f)) for f in
            glob.glob(os.path.join(path, subdir, pattern))]

def build_and_test(path):
    part1_sources = sum([relative_glob(pattern, path, subdir=subdir)
                         for (subdir, pattern) in GCC_SOURCES_PART1],
                        [])
    part1_compiled = compile_test(path, PART1_NAME, EXEC_NAME_PART1,
                                  GCC_ARGS_PART1 + part1_sources + GCC_ARGS_SUFFIX_PART1)
    part2_sources = sum([relative_glob(pattern, path, subdir=subdir)
                         for (subdir, pattern) in GCC_SOURCES_PART2],
                        [])
    part2_compiled = compile_test(path, PART2_NAME, EXEC_NAME_PART2,
                                  GCC_ARGS_PART2 + part2_sources + GCC_ARGS_SUFFIX_PART2)
    if not (part1_compiled and part2_compiled):
        return False
    if not run_test(path, PART1_NAME, EXEC_NAME_PART1):
        return False
    if not run_test(path, PART2_NAME, EXEC_NAME_PART2):
        return False
    return True

def compile_test(path, test_name, exec_file, gcc_args):
    command = ['gcc'] + gcc_args + ['-o', exec_file]
    print("Compiling %s... (command: %s)" % (test_name, ' '.join(command)))
    if subprocess.call(command, shell=False, cwd=path):
        print("ERROR: couldn't compile %s" % test_name)
        return False
    return True

def run_test(path, test_name, exec_file):
    print("Testing %s..." % test_name)
    proc = subprocess.Popen(os.path.join('.', exec_file), shell=False, cwd=path,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            universal_newlines=True)
    out, _ = proc.communicate()
    print(out)
    if proc.returncode != 0:
        print("ERROR: %s test errored" % test_name)
        return False
    if 'FAIL' in out or 'Failed' in out:
        print("ERROR: %s test failed" % test_name)
        return False
    return True

if __name__ == '__main__':
    main()
