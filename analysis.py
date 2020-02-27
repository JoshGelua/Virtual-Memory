import sys
import re
import collections


# Instructions for how to run it on teach.cs machines:
# The following commands will produce the output required for each address trace file
# and store it in:
# simpleloop.txt for addr-simpleloop.ref
# blocked.txt for addr-blocked.ref
# matmul.txt for addr-matmul.ref

# Commands:
# python3 analysis.py addr-simpleloop.ref > simpleloop.txt
# python3 analysis.py addr-blocked.ref > blocked.txt
# python3 analysis.py addr-matmul.ref > matmul.txt

# Note: For use in other trace files simply replace trace file use and pipe
# the output into a text file with a name of your choosing.
# E.g. : python3 analysis.py new_trace_file.ref > new_trace_file.txt


tracefile = open(sys.argv[1])

instruction_dict = collections.defaultdict(int)
data_dict = collections.defaultdict(int)
num_instructions = 0
num_loads = 0
num_stores = 0
num_modifies = 0
found = False
access_info = ""
address = ""
size = ""

for line in tracefile:
    line = line.strip()
    access_info = line.split()[0]
    address = line.split()[1].split(',')[0]
    size = line.split()[1].split(',')[1]
    if access_info == "I":
        num_instructions += 1
        instruction_dict[address[1:-3]] += 1
    elif access_info == "L":
        num_loads += 1
        data_dict[address[1:-3]] += 1
    elif access_info == "S":
        num_stores += 1
        data_dict[address[1:-3]] += 1
    elif access_info == "M":
        num_modifies += 1
        data_dict[address[1:-3]] += 1
    

print("""Counts:
  Instructions {}
  Loads        {}
  Stores       {}
  Modifies     {}\n""".format(num_instructions,
                                num_loads,
                                num_stores,
                                num_modifies))

print("Instructions:")
for key in instruction_dict:
    print("0x{}000, {}".format(key, instruction_dict[key]))

print("\nData:")
for key in data_dict:
    print("0x{}000, {}".format(key, data_dict[key]))



