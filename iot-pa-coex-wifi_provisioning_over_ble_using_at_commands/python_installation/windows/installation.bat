curl -o %CD%\py379.exe  https://www.python.org/ftp/python/3.7.9/python-3.7.9-amd64.exe
py379.exe /quiet /passive InstallAllUsers=0 PrependPath=1 Include_tests=0
pip install pyserial
pip install --upgrade --force-reinstall pyserial
pip install pillow	