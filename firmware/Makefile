CC = avr-gcc
CC_ARGS = -Wall -O1 -g -flto
SRC_DIR = src
BUILD_DIR = bin
SIM_DIR = sim_src
#Programmer device type
PG_TYPE = stk500
#Programmer serial device
PG_PORT = /dev/ttyUSB0
#AVR mmcu
MMCU = atmega8

#firmware path
FIRMW_PATH = $(BUILD_DIR)/firmware.elf
#simulation path
SIM_PATH = $(BUILD_DIR)/simulation

$(FIRMW_PATH): $(SRC_DIR)/*.h $(SRC_DIR)/*.c
	@test -d $(BUILD_DIR) || (mkdir $(BUILD_DIR) && echo -e \
	    "Created $(BUILD_DIR)/ directory")
	@#it would be nice to have avr-gcc warn about declared/defined
	@#functions that aren't used anywhere. Seems like that's hard to
	@#achieve...
	$(CC) $(CC_ARGS) -mmcu=$(MMCU) -o $@ $(SRC_DIR)/*.c

burn: $(FIRMW_PATH)
	avrdude -p m8 -c $(PG_TYPE) -P $(PG_PORT) \
	    -U flash:w:$(FIRMW_PATH)

check:
	avrdude -p m8 -c $(PG_TYPE) -P $(PG_PORT)

$(SIM_PATH): $(SIM_DIR)/*.c $(SIM_DIR)/*.h
	gcc -o $(SIM_PATH) -lsimavr -lncurses $(SIM_DIR)/*.c \
	    -DFIRMW_PATH=\"$(FIRMW_PATH)\" -DMMCU=\"$(MMCU)\"

simulator: $(SIM_PATH)

simulate: $(SIM_PATH) $(FIRMW_PATH)
	$<

clean:
	rm -rf $(BUILD_DIR)/*
