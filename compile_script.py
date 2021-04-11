import sys
import os
import subprocess
import random
import operator
import csv
import logging
import tempfile

def run_command(command):
    print('----------------------------------------')
    print('Executing: '+command)
    pipe = subprocess.Popen(command, shell=True, bufsize=1, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True)
    out, err = pipe.communicate()
    print (out)
    
print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

files_to_compile = ['analogTemp', 'HD44780']
file_to_compile = 'analogTemp'
com_port = '/dev/cu.usbmodem14101'

level_of_compilation = int(sys.argv[1])

if level_of_compilation >= 1: 
    for fs in files_to_compile:
        cmd = 'avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o'+fs+'.o '+fs+'.c'
        run_command(cmd)
    
if level_of_compilation >= 2:
    cmd = 'avr-gcc -mmcu=atmega328p '
    for fs in files_to_compile:
        cmd = cmd+fs+'.o ' 
    cmd = cmd +'-o '+file_to_compile
    run_command(cmd)

if level_of_compilation >= 3:
    cmd = 'avr-objcopy -O ihex -R .eeprom '+file_to_compile+' '+file_to_compile+'.hex'
    
    run_command(cmd)
if level_of_compilation >= 4:
    cmd = 'avrdude -c stk500v1 -P'+com_port+' -p atmega328p -U flash:w:'+file_to_compile+'.hex:i -F'
    run_command(cmd)
