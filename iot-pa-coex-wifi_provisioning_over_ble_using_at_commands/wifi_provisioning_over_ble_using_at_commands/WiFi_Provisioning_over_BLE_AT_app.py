import serial
import time
import tkinter
from tkinter import *
from tkinter import ttk
from tkinter import messagebox
import warnings
import serial.tools.list_ports
from PIL import Image,ImageTk
import platform
import glob
from sys import exit
import sys      
import os
opersys=platform.system()                                   # Checking operating system
baud_rate=115200                                            # Default Baud Rate
mport=''                                                    # Port Number
usbcount=0                                                 # Counter for every function

def resource_path(relative_path):
    base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(base_path, relative_path)
silabs_image = resource_path("silabs_icon.png")
background_image=resource_path("background.png")

    
def msgbox(inp1,inp2):                                      # Error message box
    roott = Tk()
    roott.geometry( "300x150" )
    roott.config(background="#023242")
    roott.title(inp1)
    roott.call('wm', 'iconphoto', roott._w, PhotoImage(file=silabs_image)) 
    label = Label(roott, text=inp2)
    label.config(bg="#023242", fg="white")
    label.pack(side="top", fill="both", expand=True, padx=20, pady=10)
    button = Button(roott,width=12, text="OK", command=lambda: roott.destroy()).pack(pady=20)
    roott.mainloop()
# Auto-Selecting the port
if(opersys == 'Darwin'):
    module_ports = [
        p.device
        for p in serial.tools.list_ports.comports()
        if  ('USB UART' in p.description) or ('Wireless USB-CDC Network Module' in p.description)
    ]
elif(opersys == 'Windows'):
    module_ports = [                
        p.device
        for p in serial.tools.list_ports.comports()
        if  ('USB Serial Port' in p.description) or ('USB Serial Device' in p.description)
    ]
elif(opersys == 'Linux'):

    result = glob.glob('/dev/tty[A-Za-z]*')
    module_ports = []
    for i in result:
        if ('/dev/ttyUSB0' in i or '/dev/ttyACM0' in i):
            module_ports.append(i)        
if not module_ports:
    msgbox("Error", "No module found!")
    exit()

def show(root,clicked,portclicked):                         
    global baud_rate
    global mport
    global usbcount
    temp = clicked.get()
    if temp == "USB-CDC" :
        baud_rate = 921600
        usbcount=10
    elif temp == "UART" :
        baud_rate = 115200
        usbcount=15
    mport = portclicked.get()
    root.destroy()
    
def interface():                                            # Window for selecting port and interface
    root = Tk()
    label = Label( root , text = " " )
    clicked = StringVar()
    portclicked = StringVar()
    root.geometry( "350x300" )
    root.title("Silicon Labs")
    root.call('wm', 'iconphoto', root._w, PhotoImage(file=silabs_image)) 
    Label(root, text="Select an Interface", font=('Arial 12')).pack(pady=5)  
      
    # Dropdown menu options
    options = [
        "UART",
        "USB-CDC"
    ]
    clicked.set("USB-CDC")
    menu = OptionMenu( root , clicked , *options )
    menu.config(bg="#023242", fg="white",width=20)
    menu.pack(pady=20, ipadx=20)
    Label(root, text="Select a Port", font=('Arial 12')).pack(pady=5) 
    portclicked.set(module_ports[0])
    portmenu = OptionMenu( root , portclicked , *module_ports )
    portmenu.config(bg="#023242", fg="white",width=20)
    portmenu.pack(pady=20, ipadx=20)
    
    # Create button, it will change label text
    button = Button(root, text= "OK", width= 20,command=lambda: show(root,clicked,portclicked)).pack(pady=10)
    # Create Label
    label.pack()
    root.mainloop()

interface()
if usbcount==0:
    exit()
sp=serial.Serial(port=mport,baudrate=baud_rate,timeout=0.05)
print("Port opened\n")


handle=''           # Service Handler Ptr
starthandle=0       # Handle of the Attribute Record
fwversion=""        # Firmware Version
ssid_selected=[]    # Selected SSID in the provisioning tool
sec_type=0          # Security Type of the selected SSID
psk_ssid=[]         # Password of the selected SSID
connected = 0       # wlan connection status; 0-->not connected , 1-->connected
resp="OK"           # For receiving response from the command
joinfailure =0      # Checking if there was any errors during join connection
remotemacadd=''     # MAC address of the remote device
wifiinitstatus=0    # Keeping a check whether to initialize Wi-Fi or not
handle_1aa1=0
handle_1bb1=0
handle_1cc1=0
 

main=Tk()
main.title("	WLAN_BLE_AT_mode_Provisioning",)
screen_width = main.winfo_screenwidth()
screen_height = main.winfo_screenheight()
main.geometry(f"{screen_width}x{screen_height}")
main.config(bg="white")
main.call('wm', 'iconphoto', main._w, PhotoImage(file=silabs_image)) 
style = ttk.Style()
style.theme_use('clam')
style.configure("Vertical.TScrollbar", gripcount=0,
                background="#DCDCDC",
                troughcolor="#023242", bordercolor="#0597AA", arrowcolor="#023242") 
image1 = Image.open(background_image)
image2 = ImageTk.PhotoImage(image1)
bglabel = Label( main, image = image2)
bglabel.place(x=0,y=0)
container = Frame(main)
canvas = Canvas(container,width=screen_width/2,height=3*(screen_height/4),bd=3,background="black")
scrollbar =ttk.Scrollbar(container, orient="vertical", command=canvas.yview, style="Vertical.TScrollbar")
scrollable_frame = Frame(canvas)
scrollable_frame.bind(
    "<Configure>",
    lambda e: canvas.configure(
        scrollregion=canvas.bbox("all")
    )
)
canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
canvas.configure(yscrollcommand=scrollbar.set)
container.pack()
canvas.pack(side="left", fill="both", expand=True)
scrollbar.pack(side="right", fill="y")

exit_button = Button(main, text="Exit", command=main.destroy,bd=1,height=2,width=30,bg="white",fg="black",font=("Verdana 10 bold",10))
exit_button.pack(pady=20)

def framelabel(scrollable_frame,inp) :
    if opersys=='Darwin':
    	lbl_mac=Label(scrollable_frame,text=inp,bd=2,height=3,width=int(screen_width/16),font=("Verdana 7 bold",10),command=None,bg="#023242",fg="white", relief=GROOVE)
    else: 
    	lbl_mac=Label(scrollable_frame,text=inp,bd=2,height=3,width=int(screen_width/16),font=("Verdana 7 bold",10),command=None,bg="#023242",fg="white", relief=GROOVE)
    lbl_mac.pack()
    canvas.update_idletasks()
    canvas.yview_moveto('1.0')

framelabel(scrollable_frame,"Port opened")
main.update()

def sendcmd(cmd,timecount):                                 # Encoding the command and returning the Response
    resp=""
    response=[]
    sp.write(cmd.encode())
    count=0
    while(resp!="\n"):
        resp=sp.readline()
        count+=1
        if count==timecount:                                # Delay to maintain flow control with Provisioning Tool 
            break
        elif resp=='\x00'or str(resp)=="b''" :
            continue
        elif resp[0:1].decode(errors="ignore")=='E':
            print(resp.decode(errors="ignore"))
            response.append(resp)
            return response
        else :  
            response.append(resp)
    return response
  


def opermode():                                             # Opermode for RS9116 Coex Mode
    if baud_rate == 115200:
        time.sleep(1)
    response=sendcmd('at+rsi_opermode=851968,0,4,2147483648,2150629376,3221225472,0,1966080\0\r\n',usbcount+10) 
    if(len(response) > 0):
        for resp in response:                               # response --> b'OK\r\n' #b'bt_loaded\r\n'
            if not(resp[0:2].decode(errors="ignore")=='OK' or resp[0:2].decode(errors="ignore")=='bt'):
                print("Opermode not set. "+str(resp.decode(errors="ignore").strip("\r\n")))
                framelabel(scrollable_frame,"Opermode not set. "+str(resp.decode(errors="ignore").strip("\r\n")))
                break
        else:
            print("Opermode set\n")
            framelabel(scrollable_frame,"Opermode set")
    else:
        framelabel(scrollable_frame,"Opermode not set. No response received")
    
    main.update()
    sp.write('at+rsi_fwversion?\0\r\n'.encode())            # Firmware Version Query
    response=sp.read(size=40)
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            global fwversion
            fwversion=response[2:17].decode(errors="ignore")
            print("Firmware version: "+str(fwversion)+"\n")
            framelabel(scrollable_frame,"Firmware version: "+str(fwversion))
        else:   
            print("Firmware version not found. "+str(response.decode(errors="ignore").strip("\r\n")))
            framelabel(scrollable_frame,"Firmware version not found. "+str(response.decode(errors="ignore").strip("\r\n")))
            distemp='at+rsibt_disconnect=' + str(remotemacadd) + '\0\r\n'
            sp.write(distemp.encode())
    else:
        print("Firmware version not found. No response received\n")
        messagebox.showerror("Module not responding", "Please reset the module and confirm whether connected to selected Interface!")
        exit()
   
    main.update()
def softreset():
    reset_data="at+rsi_reset\r\n"
    sp.write(reset_data.encode())
    response=sp.read(size=100)
    
def abrd_usbcdc():                                          # Auto Baud Rate Detection for USB-CDC
    resp=""
    sp.write('|'.encode())
    sp.write('U'.encode())
    sp.write('U'.encode())
    sp.write('1'.encode())
    time.sleep(1)
    resp=sp.read(size=500)                                  # Loading Default Wireless Firmware
    print("ABRD done\n") 
    framelabel(scrollable_frame,"ABRD done")
    main.update()
    opermode()
      
def abrd_uart():                                            # Auto Baud Rate Detection for UART
    resp=""
    sp.write('|'.encode())
    time.sleep(0.05)
    sp.write('U'.encode())
    time.sleep(0.05)
    sp.write('U'.encode())
    time.sleep(0.05)
    sp.write('1'.encode())
    time.sleep(1)
    resp=sp.read(size=500)                                  # Loading Default Wireless Firmware
    print("ABRD done\n")  
    framelabel(scrollable_frame,"ABRD done")
    main.update()
    opermode()
    
def getlocalbdaddr():                                       # Bluetooth Device Address
    resp=sendcmd('at+rsibt_getlocalbdaddr?\0\r\n',usbcount)
    if(len(resp) > 0):
        if(len(resp[0]) >=2  and resp[0][0:2].decode(errors="ignore")=='OK'):
            print("BD address of Local device : ",resp[0][2:20].decode(errors="ignore"))
            framelabel(scrollable_frame,"BD address of Local device : " + str(resp[0][2:20].decode(errors="ignore")))
        else:
            print("Getting Local BD address Unsuccessful. "+str(resp[0].decode(errors="ignore").strip("\r\n")))
            framelabel(scrollable_frame,"Getting Local BD address Unsuccessful.  "+str(resp[0].decode(errors="ignore").strip("\r\n")))
    else:
        print("Getting Local bd address Unsuccessful. No response received\n")
        framelabel(scrollable_frame,"Getting Local BD address Unsuccessful. No response received")
    main.update()
    
def addservice():
    sp.write('at+rsibt_addservice=2,aabb,3,30\0\r\n'.encode())
    response=sp.read(size=20)
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            global handle
            handle=response[3:8].decode(errors="ignore")
            global starthandle
            starthandle=response[9]
            print("Service added with handle: ",handle,chr(starthandle),"\n")
            framelabel(scrollable_frame,"Service added with handle: " + handle + " " +chr(starthandle))
        else:   
            print("Adding Service Unsuccessful. "+str(response.decode(errors="ignore").strip("\r\n")))
            framelabel(scrollable_frame,"Adding Service Unsuccessful. "+str(response.decode(errors="ignore").strip("\r\n")))
    else:
        print("Adding Service Unsuccessful. No response received!\n")
        framelabel(scrollable_frame,"Adding Service Unsuccessful. No response received!")  
    main.update()
        
def addcharacteristic():
    global handle_1aa1
    global handle_1bb1
    global handle_1cc1
    
    resp=""
    temp = 'at+rsibt_addattribute=' + str(handle) + ','
    
    temp2 = temp + hex(starthandle + 1 -55).lstrip('0x') + ',2,2803,2,6,0,8,0,0C,00,a1,1a\0\r\n'
    sp.write(temp2.encode())
    time.sleep(0.1)                                           # Delay added so that attribute is recognized by Provisioning tool
    
    temp2 = temp + hex(starthandle + 2 -55).lstrip('0x') + ',2,1aa1,8,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\0\r\n'
    handle_1aa1=starthandle + 2 -55
    sp.write(temp2.encode())
    time.sleep(0.1)
    
    temp2 = temp + hex(starthandle + 3 -55).lstrip('0x') + ',2,2803,2,6,0,A,0,E,00,b1,1b\0\r\n'
    sp.write(temp2.encode())
    time.sleep(0.1)
    
    temp2 = temp + hex(starthandle + 4 -55).lstrip('0x') + ',2,1bb1,A,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\0\r\n'
    handle_1bb1=starthandle + 4 -55
    print("handle_1bb1: ",handle_1bb1)
    sp.write(temp2.encode())
    time.sleep(0.1)
    
    temp2 = temp + hex(starthandle + 5 -55).lstrip('0x') + ',2,2803,2,6,0,1A,0,10,00,c1,1c\0\r\n'
    sp.write(temp2.encode())
    time.sleep(0.1)
    
    temp2 = temp + hex(starthandle + 6 -55).lstrip('0x') + ',2,1cc1,1A,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\0\r\n'
    handle_1cc1=starthandle + 6 -55
    sp.write(temp2.encode())
    time.sleep(0.1)
    
    temp2 = temp + hex(starthandle + 7 -55).lstrip('0x') + ',2,2902,A,2,0,0,0\0\r\n'
    sp.write(temp2.encode())
    time.sleep(0.1)
    
    count=0
    while(resp!="\n"):
        resp=sp.readline()
        if count==7 :
            print("Characteristics added\n")
            framelabel(scrollable_frame,"Characteristics added")
            break
        if resp.decode(errors="ignore")=='':  
            continue
        if (resp[0:2].decode(errors="ignore")=='OK'):
            count+=1
        else :
            print("Characteristics not added. "+str(resp.decode(errors="ignore").strip("\r\n")))
            framelabel(scrollable_frame,"Characteristics not added. "+str(resp.decode(errors="ignore").strip("\r\n")))
            break   
    main.update()

def setlocalname():                                         # Module name set to RS9116W_BLE 
    sp.write('at+rsibt_setlocalname=11,RS9116W_BLE\0\r\n'.encode())
    response=sp.read(size=10)
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            print("Localname set\n")
            framelabel(scrollable_frame,"Localname set")
        else:   
            print("Localname Not set. "+str(response.decode(errors="ignore").strip("\r\n")))
            framelabel(scrollable_frame,"Localname Not set. "+str(response.decode(errors="ignore").strip("\r\n")))
    else:
        print("Localname Not set. No response received\n")
        framelabel(scrollable_frame,"Localname Not set. No response received!")    
    main.update()
        
def band():
    sp.write('at+rsi_band=0\r\n'.encode())
    response=sp.read(size=10)
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            print("Band set\n")
            framelabel(scrollable_frame,"Band set")
        else:   
            print("Setting Band Unsuccessful. "+str(response.decode(errors="ignore").strip("\r\n")))
            framelabel(scrollable_frame,"Setting Band Unsuccessful. "+str(response.decode(errors="ignore").strip("\r\n")))
    else:
        print("Setting Band Unsuccessful. No response received!\n")
        framelabel(scrollable_frame,"Setting Band Unsuccessful. No response received!")    
    main.update()
            
def wifiinit():
    global wifiinitstatus
    if(wifiinitstatus==0):
        sp.write('at+rsi_init\r\n'.encode())
        response=sp.read(size=10)
        if(len(response)>0):
            if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
                print("Wi-Fi Initialization\n")
                framelabel(scrollable_frame,"Wi-Fi Initialization Successful")
                wifiinitstatus=1
            else:   
                print("Wi-Fi Initialization Unsuccessful. "+str(response.decode(errors="ignore").strip("\r\n")))
                framelabel(scrollable_frame,"Wi-Fi Initialization Unsuccessful. "+str(response.decode(errors="ignore").strip("\r\n")))
        else:
            print("Wi-Fi Initialization Unsuccessful. No response received!\n")
            framelabel(scrollable_frame,"Wifi Initialization Unsuccessful. No response received!")   
        main.update()   

def advertise():
    global remotemacadd
    remotemacadd=''
    resp=""
    sp.write('at+rsibt_setadvertisedata=15,2,1,6,11,9,42,4C,45,5F,43,4F,4E,46,49,47,55,52,41,54,4F,52\0\r\n'.encode())
    response=sp.read(size=10)                               #b'OK \r\n'
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            print("Advertising data set\n")
            framelabel(scrollable_frame,"Advertising data set")
            sp.write('at+rsibt_advertise=1,128,0,0,0,32,32,0,7\0\r\n'.encode())
            resp=sp.read(size=10)                           #b'OK \r\n'
            if(len(resp) > 0):
                if(len(resp) >= 2 and resp[0:2].decode(errors="ignore")=='OK'):
                    print("Advertising started\n")
                    print("Check 'BLE_CONFIGURATOR' for BLE connection......")
                    framelabel(scrollable_frame,"Advertising started")
                    framelabel(scrollable_frame,"Check 'BLE_CONFIGURATOR' for BLE connection......")
                else :
                    print("Advertising not started. "+str(resp.decode(errors="ignore").strip("\r\n"))) 
                    framelabel(scrollable_frame,"Advertising not started. "+str(resp.decode(errors="ignore").strip("\r\n")))
            else:
                print("Advertising not started. No response received!") 
                framelabel(scrollable_frame,"Advertising not started. No response received!")
        else:   
            print("Advertising data not set.\n")
            framelabel(scrollable_frame,"Advertising data not set. " + str(response.decode(errors="ignore")))
    else:
        print("Advertising data not set. No response received!\n")
        framelabel(scrollable_frame,"Advertising data not set. No response received!")
    main.update()
         
def errormsg():
    temp='at+rsibt_setlocalattvalue='+ hex(handle_1bb1).lstrip('0x') + ',14,' + hex(2).lstrip('0x') + ',' + hex(0).lstrip('0x') + '\0\r\n'
    sp.write(temp.encode())
    response=sp.read(size=10)
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            print("Set_Local_Attvalue\n")
        else:   
            print("Error: "+str(response.decode(errors="ignore")))
    else:
        print("Local_Attvalue not Set. No response received!\n")   
        
def defaultvalues() :
    global ssid_selected
    ssid_selected.clear()
    global psk_ssid
    psk_ssid.clear()
    global connected
    connected=0 
    global sec_type
    sec_type=0
    
def scan():
    print("wifiinitstatus ",wifiinitstatus)
    wifiinit()
    
    response=sendcmd('at+rsi_scan=0\0\r\n',35)                  # Scan Command
    if(len(response) > 0):
        var=[]                                                  # Creating a list containing the response
        for element in response:
            var=var + list(element)                             # Concatenating list with list                      
        for i in range(len(var)):                               # List of hexadecimal values
            var[i] = hex(var[i]).lstrip('0x')
        r = int(var[2],16)                                      # Rows --> Number of SSIDs # if 0 no 1cc1 
        c = 46                                                  # Columns --> Data Length of each SSID , BUG!!!!!
        arr = [[0 for i in range(c)] for j in range(r)]         # 2D array for easy traversal of individual SSID data
        itr = 10                                                # Iterating variable --> Initial 10 values--> OK, Data Length, Padding
        for i in range(r):
            for j in range(c):
                arr[i][j] = var[itr]
                itr = itr + 1    
            
        Security_type = [0 for i in range(r)]                   # Array Containing Security Type of SSIDs
        for i in range(r) :
            if(arr[i][1]==''):
                Security_type[i] = '0'
            else:
                Security_type[i] = arr[i][1]
        SSID = []                                               # Array of Hex Value of SSID
        for i in range(r) :
            temp_list=[]
            for j in range(4,38):                               # BUG
                if arr[i][j] == '':
                    break
                else :
                    temp_list.append(arr[i][j])
            SSID.append(temp_list)
        temp = 'at+rsibt_setlocalattvalue='+ hex(handle_1bb1).lstrip('0x') + ',14,33,' + hex(r).lstrip("0x") + "\0\r\n" # Sending Number of SSIDs on 1bb1
        response2=sendcmd(temp,usbcount)
        if len(response2)>0 :
            if(len(response2[0])>=2 and response2[0][0:2].decode(errors="ignore")=='OK'):
                print("\nNumber of SSIDs = " + str(r))
                framelabel(scrollable_frame,"Number of SSIDs = " + str(r))
                if r>0:
                    i=0        
                    for listitr in SSID :
                        temp = 'at+rsibt_setlocalattvalue=' + hex(handle_1cc1).lstrip('0x') + ',14,' + str(Security_type[i]) + ',2c'  # Sending SSID data on 1cc1   
                        i=i+1
                        for inditr in range(len(listitr)):
                            temp = temp + "," + str(listitr[inditr])  
                        temp+="\0\r\n"
                        response1=sendcmd(temp,usbcount)
                        if len(response1)>0 :
                            if(len(response1[0])>=2 and response1[0][0:2].decode(errors="ignore")=='OK'):
                                print("Set_Local_Attvalue : SSID sent on 1cc1\n") 
                                framelabel(scrollable_frame,"SSID " + str(i) +" sent")
                                main.update()
                            else:
                                print("SSID not sent. 1CC1 Error: "+str(response1[0].decode(errors="ignore"))) 
                                framelabel(scrollable_frame,"SSID not sent. "+str(response1[0].decode(errors="ignore")))

                        else:
                            print("No response received while sending SSID on 1cc1")
                            framelabel(scrollable_frame,"No response received while sending SSID on 1cc1")
            else:
                print("Number of SSIDs not sent. 1BB1 Error: "+str(response2[0].decode(errors="ignore")))
                framelabel(scrollable_frame,"Number of SSIDs not sent. "+str(response2[0].decode(errors="ignore")))
        else:
            print("No response received while sending number of SSIDs on 1bb1")
            framelabel(scrollable_frame,"No response received while sending number of SSIDs on 1bb1")  
    else:
        framelabel(scrollable_frame,"No response received for Scan Command!")
    main.update()

def dissociate():                                            # Wi-Fi dissociation
    global joinfailure
    global wifiinitstatus
    defaultvalues()
    sp.write('at+rsi_disassoc=0\0\r\n'.encode())
    response=sp.read(size=10)
    if(len(response)>0):
        if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
            print("WLAN Disconnection Successful\n")
            framelabel(scrollable_frame,"WLAN Disconnection Successful")
            defaultvalues()
            wifiinitstatus=0
            joinfailure = 0
            temp1='at+rsibt_setlocalattvalue=' + hex(handle_1bb1).lstrip('0x') + ',14,' + hex(4).lstrip('0x') + ',' + hex(1).lstrip('0x') + '\0\r\n'
            sp.write(temp1.encode())
            response1=sp.read(size=10)
            if(len(response1)>0):
                if(len(response1)>=2 and response1[0:2].decode(errors="ignore")=='OK'):
                    print("Set_Local_Attvalue\n")
                else:   
                    print("Dissociation Status not sent. Error: "+str(response1.decode(errors="ignore")))
                    framelabel(scrollable_frame,"Dissociation Status not sent. "+str(response1.decode(errors="ignore")))
            else:
                print("No response received while sending Dissociation Status")
        else:   
            print("Dissociation Unsuccessful "+str(response.decode(errors="ignore")))
            framelabel(scrollable_frame,"Dissociation Unsuccessful. "+str(response.decode(errors="ignore")))
    else:
        print("Dissociation Unsuccessful.No response received!\n")
        framelabel(scrollable_frame,"Dissociation Unsuccessful. No response received!")
    main.update()        
    
def wrongpskscan():
    wifiinit()
    response = sendcmd('at+rsi_scan=0\0\r\n',100)             # Scan Command
    time.sleep(1)
    
def connect():
    global connected
    global joinfailure
    global wifiinitstatus
    time.sleep(0.5)
    if joinfailure==1: 
        joinfailure=0
        wrongpskscan()
        
    temp2='at+rsi_join='
    for i in range(len(ssid_selected)):
        temp2 = temp2 + chr(int(ssid_selected[i],16))
    temp2 = temp2 + ',0,2,' + chr(int(sec_type,16)) + '\0\r\n'  # if sec_type==35 --> '5'
    resp2=sendcmd(temp2,100)                                     # More Delay is needed for this command
    if len(resp2) >0:                                           # For checking if wrong credentials are entered; if so, resp = []; so len(resp) = 0
        if(len(resp2[0])>=2 and resp2[0][0:2].decode(errors="ignore")=='OK'):
            time.sleep(1)
            resp3=sendcmd('at+rsi_ipconf=1,0,0,0\r\n',100)
            if(len(resp3)>0):
                if(len(resp3[0])>=2 and resp3[0][0:2].decode(errors="ignore")=='OK'):
                    var=[]
                    for element in resp3:
                        var=var + list(element[:-2])
                    for i in range(len(var)):
                        var[i] = hex(var[i]).lstrip('0x')
                    temp3 = 'at+rsibt_setlocalattvalue=' + hex(handle_1bb1).lstrip('0x') + ',14,' + hex(2).lstrip('0x') + ',' + hex(1).lstrip('0x') + ',' + '2c'
                    for i in range(2,8):
                        temp3= temp3 + ',' + str(var[i])
                    temp3+=',2C'
                    for i in range(8,12):
                        temp3 = temp3 + ',' + str(var[i])
                    temp3+='\0\r\n'
                    print("Sending IP and MAC address\n")
                    resp4=sendcmd(temp3,usbcount+40)
                    if(len(resp4)>0):
                        if(len(resp4[0])>=2 and resp4[0][0:2].decode(errors="ignore")=='OK'):
                            connected=1
                            print("AP Joined\n")
                            framelabel(scrollable_frame,"AP joined")
                            framelabel(scrollable_frame,"WLAN Status: Connected")
                            framelabel(scrollable_frame,"IP and MAC address sent")
                        else:
                            framelabel(scrollable_frame,"Error while sending IP Address to Provisioning Tool. "+str(resp4[0].decode(errors="ignore").strip("\r\n")))
                            print("Error while sending IP Address to Provisioning Tool. "+str(resp4[0].decode(errors="ignore")))
                            dissociate()
                            errormsg()
                    else:
                        framelabel(scrollable_frame,"No response received  after sending IP Address to Provisioning Tool.")
                        print("No response received  after sending IP Address to Provisioning Tool.\n") 
                        dissociate()
                        errormsg()                        
                        
                else:
                    framelabel(scrollable_frame,"IP Address Error. AP not connected! "+str(resp3[0].decode(errors="ignore").strip("\r\n")))
                    print("IP and Mac Address Error: "+str(resp3[0].decode(errors="ignore"))) 
                    
                    dissociate()
                    errormsg()
            else:
                framelabel(scrollable_frame,"No response received while sending IP configuration command to module")
                print("No response received while sending IP configuration command to module\n")
                dissociate() 
                errormsg()
        else:
            print("Join not established. "+str(resp2[0].decode(errors="ignore").strip("\r\n")))
            errormsg()
            joinfailure=1
            wifiinitstatus=0
            framelabel(scrollable_frame,"Join not established. "+str(resp2[0].decode(errors="ignore").strip("\r\n")))
    else:
        errormsg()
        print("Join not established. Incorrect Password\n")
        joinfailure=1
        wifiinitstatus=0
        framelabel(scrollable_frame,"Incorrect Password ")
    main.update()
    
if baud_rate == 115200:
    softreset()
    abrd_uart()
else:
    softreset()
    sp.close()
    time.sleep(1)
    available=serial.tools.list_ports.comports()
    sp=serial.Serial(port=mport,baudrate=baud_rate,timeout=0.05)
    abrd_usbcdc()
setlocalname()
getlocalbdaddr()
addservice()
addcharacteristic()
band()
advertise()

def mainloopp() :
    global remotemacadd
    global resp
    global fwversion
    global ssid_selected
    global sec_type
    global psk_ssid
    global connected
    global wifiinitstatus
    global joinfailure
    resp=sp.readline()
    if(len(resp)>0 and resp[0:1].decode(errors="ignore")=='A'):
        print("Response received is : ",resp.decode(errors="ignore"))
        print("\n")
        receive_data=resp.decode(errors="ignore").strip("\r\n")
        receive_data_split=receive_data.split(",")
        if len(receive_data_split) > 4: 
            main.update()
            if receive_data_split[4].strip("\r\n")=="38":                           # Firmware Query
                print("Received Index from app is  :",receive_data_split[4])
                print("Firmware version: "+str(fwversion)+"\n")
                temp = 'at+rsibt_setlocalattvalue='+ hex(handle_1bb1).lstrip('0x') +',14,8,8'
                for element in range(0, len(fwversion)):
                    temp = temp + ',' + hex(ord(str(fwversion[element]))).lstrip("0x")
                temp+="\0\r\n"
                sp.write(temp.encode())
                response=sp.read(size=10)
                if(len(response)>0):
                    if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
                        print("Set_Local_Attvalue\n")
                    else:   
                        print("Error: "+str(response.decode(errors="ignore")))      # dissoc + advertise
                        distemp='at+rsibt_disconnect=' + str(remotemacadd) + '\0\r\n'
                        disresponse=sendcmd(distemp,usbcount-2)
                        advertise()
                else:
                    print("No response received while setting Firmware Version.\n")
                    messagebox.showerror("No response received", "Firmware Version not sent.")
                    exit()
                    
            elif receive_data_split[4].strip("\r\n")=="37":                         # WLAN Status
                temp = 'at+rsibt_setlocalattvalue='+ hex(handle_1bb1).lstrip('0x') + ',14,' + hex(7).lstrip('0x')
                if connected==0 :    
                    temp+='\0\r\n'
                    framelabel(scrollable_frame,"WLAN Status: Not connected")
                elif connected==1:
                    temp = temp + ',' + hex(1).lstrip('0x') + '\0\r\n'
                    framelabel(scrollable_frame,"WLAN Status: Connected")
                main.update()
                sp.write(temp.encode())
                response=sp.read(size=10)
                if(len(response)>0):
                    if(len(response)>=2 and response[0:2].decode(errors="ignore")=='OK'):
                        print("Sending wlan status command\n")
                    else:   
                        print("WLAN status not sent. Error: "+str(response.decode(errors="ignore")))
                        messagebox.showerror("WLAN status Error ", "WLAN status not sent. "+str(response.decode(errors="ignore")))
                else:
                    messagebox.showerror("No response received", "WLAN status not sent.")
                    
            
            elif receive_data_split[4].strip("\r\n")=="33":                  # WLAN Scan 
                print("Received Index from app is  :",receive_data_split[4])
                scan()
            
            elif receive_data_split[4].strip("\r\n")=="35":                  # Security
                sec_type = receive_data_split[7].strip("\r\n")               # .strip("\r\n") is necessary else, 'if' statement won't work
                
                if int(sec_type) < 10:                                       # For chrome tool need this condition
                    sec_type = str(int(sec_type) + 30)
                if sec_type=="30" :
                    connect()
        
            elif receive_data_split[4].strip("\r\n")=="36":                  # PSK
                psk_ssid.clear()
                for i in range(7,len(receive_data_split)):
                    if(receive_data_split[i]=='0'):
                        break
                    psk_ssid.append(receive_data_split[i])
                temp = 'at+rsi_psk=4,' + str(len(psk_ssid)) + ','
                
                if(len(psk_ssid) < 8 or len(psk_ssid) > 17):
                    print("Invalid length of password! ")
                    errormsg()
                    framelabel(scrollable_frame,"Invalid length of password! ")
                else:
                    for i in range(len(psk_ssid)):
                        temp = temp + chr(int(psk_ssid[i],16))
                    temp+='\0\r\n'
                    wifiinit()
                    time.sleep(0.5)
                    resp1=sendcmd(temp,usbcount)
                    if(len(resp1) > 0) :
                        if(len(resp1[0])>=2 and resp1[0][0:2].decode(errors="ignore")=='OK'):
                            print("Password Set\n")
                            framelabel(scrollable_frame,"Password Set")
                            connect()
                        else:
                            print("Error in setting password. "+str(resp1[0].decode(errors="ignore")))
                            errormsg()
                            defaultvalues()
                            #joinfailure=1
                            wifiinitstatus=0
                            framelabel(scrollable_frame,"Error in setting password. " + str(resp1[0].decode(errors="ignore")))
                    else :
                        print("No response received for Password Command\n")
                        errormsg()
                        framelabel(scrollable_frame,"No response received for Password Command")
                        defaultvalues()
                main.update()    

            elif receive_data_split[4].strip("\r\n")=="32":                    # Join
                print("Received Index from app is  :",receive_data_split[4])
                ssid_selected.clear()
                for i in range(7,len(receive_data_split)):
                    if receive_data_split[i] == '0':
                        break
                    ssid_selected.append(receive_data_split[i])
                print("SSID selected")
                framelabel(scrollable_frame,"SSID selected")
                main.update()
                
            elif receive_data_split[4].strip("\r\n")=="34":                    # WLAN Disconnect
                print("Received Index from app is  :",receive_data_split[4])        
                dissociate()
                
        elif receive_data_split[0].find('AT+RSIBT_LE_DISCONNECTED') != -1 :    # BLE Disconnect
            framelabel(scrollable_frame,"BLE Disconnected")
            main.update()
            advertise()
        elif receive_data_split[0].find('AT+RSIBT_LE_DEVICE_CONNECTED') != -1 :# Setting BD address of the remote device
            remotemacadd=receive_data_split[1][0:18]
            print("Remote Device Connected. Its MAC address: ",remotemacadd)
            framelabel(scrollable_frame,"Remote Device Connected. Its MAC address: " + str(remotemacadd))
            main.update()
    scrollable_frame.after(1,mainloopp)
mainloopp()
main.mainloop()
