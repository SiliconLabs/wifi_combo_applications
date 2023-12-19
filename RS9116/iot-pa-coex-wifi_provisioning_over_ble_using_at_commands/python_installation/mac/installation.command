#!/bin/sh
curl -o py3108.pkg https://www.python.org/ftp/python/3.10.8/python-3.10.8-macos11.pkg
installer -pkg py3108.pkg -target CurrentUserHomeDirectory
pip3 install tkmacosx
pip3 install serial
pip3 install pyserial
pip3 install pillow
