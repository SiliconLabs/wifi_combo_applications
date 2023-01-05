#!/usr/bin/python

import sys
import os

#check the file name and path 
if len(sys.argv) < 2:
    print(' Description : This python scripts converts RPS binary image into a C header file \n' )
    print('               which contains hex array bytes of an input RPS image file.\n')
    print("USAGE: Binary_file_to_C_hex_array.py <RPS file+path>");
    exit(0)

filename = sys.argv[1] #input file name
#header_file_name = sys.argv[2] #output file name
header_file_name = 'fw_file.h'

#find length or size of file
def find_filesize(f):
  cur = f.tell()    # save current position
  f.seek(0, os.SEEK_END)
  end = f.tell()    # find the size of file
  f.seek(cur, os.SEEK_SET)
  return end

# write file description header 
def file_desciption_write(): 
    header_file = open(header_file_name,'w') 
    header_file.write('/*\n' )
    header_file.write(' * '+ header_file_name + "\n" )
    header_file.write(' *' + "\n" )
    header_file.write(' * Its auto generated c array file \n')  
    header_file.write(' */' + "\n" )
    header_file.write('#ifndef FW_FILES_H_ \n')
    header_file.write('#define FW_FILES_H_ \n')
    header_file.write('__attribute__((__section__(".text"))) unsigned char fw_array[] = {' + "\n" )
    header_file.close()


def bin_to_hex_array():
    #for add new line in array
    newline_cnt = 0 
    fw_file = open(filename, 'rb')
    size = find_filesize(fw_file)
    print(filename)
    print("file size :")
    print(size)
    file_desciption_write()
    header_file = open(header_file_name,'a')
    while True:
        byte = fw_file.read(1).hex()
        byte = '0x'+ byte
        if byte == '0x':
            byte = '0x00'
        byte = byte + ","   
        header_file.write(byte)
        newline_cnt = newline_cnt + 1
        if newline_cnt == 15: # add new line after 15 bytes(need for editor limitattions)
            newline_cnt = 0
            header_file.write("\n")
        #print(byte)
        size = size - 1;
        if size < 0:
            break
    header_file.write("\n" + '};')
    header_file.write("\n")
    header_file.write("#endif /* FW_FILES_FW_FILES_H_ */")
    header_file.close()
    print("file write done")    

#app start here
def main():
    file_desciption_write()
    bin_to_hex_array()

if __name__ == "__main__":
    main()
	
	
	

