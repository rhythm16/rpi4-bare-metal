#! /usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${GREEN}CLEAN${NC}"
make clean
echo -e "${CYAN}Link kernel8.elf first pass${NC}"
make
echo -e "${CYAN}Save first pass symbols${NC}"
nm -n main_build/kernel8.elf | sort | grep -v '\$' > nmfirstpass
echo -e "${CYAN}Generate symbol area and overwrite symbol_area.S${NC}"
nm -n main_build/kernel8.elf | sort | grep -v '\$' | scripts/generate_syms.py
echo -e "${CYAN}Compile symbol area and link kernel8.elf second pass${NC}"
make
echo -e "${CYAN}Save second pass symbols${NC}"
nm -n main_build/kernel8.elf | sort | grep -v '\$' > nmsecondpass
echo -e "${RED}Show diff of symbols (should be nothing):${NC}"
diff nmfirstpass nmsecondpass
rm nmfirstpass nmsecondpass
echo -e "${YELLOW}Deploy to SD card?${NC}"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) make deploy; break;;
        No ) exit;;
    esac
done

