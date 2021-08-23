#! /usr/bin/python3

import syms_values

f = open(syms_values.symbol_area_file, "w")
f.write(".section \"_symbols\", \"a\"\n")
f.write(".space ")
f.write(str(syms_values.pre_allocated_size))
f.write("\n")
