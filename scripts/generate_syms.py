#! /usr/bin/python3

import sys                 
import syms_values

entry_size = 64            

# 8 for quad, 1 for trailing '\0'
max_sym_name_len = entry_size - 8 - 1

f = open(syms_values.symbol_area_file, "w") 

print("generating symbol area")
count = 0      

f.write(".section \"_symbols\", \"a\"\n")

for line in sys.stdin:
    line = line.rstrip()
    sym_info = line.split()
    sym_name_len = len(sym_info[2]) 

    assert sym_name_len <= max_sym_name_len, sym_info[2] + " is too long!"

    f.write(".quad 0x")    
    f.write(sym_info[0])   
    f.write('\n')

    f.write(".string \"")  
    f.write(sym_info[2])   
    f.write("\"")          
    f.write('\n')          

    f.write(".space ")     
    f.write(str(max_sym_name_len - sym_name_len))
    f.write('\n')          

    count += 1

# null symbol indicating end
f.write(".space 64\n")
count += 1
assert entry_size * count <= syms_values.pre_allocated_size, "too many symbols! please expand the pre allocated size!"

# calculate and add in space to fill in the remaining space
f.write(".space ")
f.write(str(syms_values.pre_allocated_size - entry_size * count))

print("symbol area: ", entry_size * count)
print("please be sure the pre_allocated_size == the .space value in the first pass!")

