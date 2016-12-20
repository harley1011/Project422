avr-gcc -Os -DF_CPU=1000000UL -mmcu=atmega328p -c -o $1.o $1.c
avr-gcc -mmcu=atmega328p $1.o -o $1
avr-objcopy -O ihex -R .eeprom $1 $1.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/cu.usbmodem1421 -b 115200 -U flash:w:$1.hex
rm $1.hex $1.o $1
