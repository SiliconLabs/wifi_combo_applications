#!/usr/bin/python

import sys

def read_file(filename):
    f = open(filename, 'r')
    txt = f.read()
    f.close()
    return txt

def main(filename):
    cert = read_file(filename)
    cert_binary = bytearray(cert)

    out_file = "".join(filename.split('-'))
    f = open(out_file, 'w')

    f.write('unsigned char ' + (out_file.split('.')[0]) + '[] = {')
    f.write(",".join([("'" + (chr(char) if char != 10 else '\\n') + "'") for char in cert_binary]))
    f.write(", 0};")
    f.close()


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print ('Usage: %s <filename>' % sys.argv[0])
