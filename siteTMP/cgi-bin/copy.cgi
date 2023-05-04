import shutil
import sys
import os

if len(sys.argv) < 3:
    print("Usage: python3 copy_file.py source_file destination_path")
    sys.exit(1)

source_file = sys.argv[1]
destination_path = sys.argv[2]

if not os.path.isfile(source_file):
    print(f"{source_file} is not a valid file.")
    sys.exit(1)

if not os.path.isdir(destination_path):
    print(f"{destination_path} is not a valid directory.")
    sys.exit(1)

shutil.copy(source_file, destination_path)

print(f"{source_file} copied to {destination_path}")
