import tkinter as tk
from tkinter import *
from tkinter import ttk
from tkinter import messagebox
from subprocess import call
from time import sleep
import threading
from mttkinter import mtTkinter
import sys
import glob
import serial
from serial import Serial
import time
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.backend_bases import key_press_handler
import serial.tools.list_ports
import re
import mplcursors
import datetime
from tkinter import filedialog
from matplotlib.widgets import Cursor
import numpy as np
import os
from PIL import Image,ImageTk
import webbrowser


class MyApp(Tk):

    def __init__(self):
        Tk.__init__(self)
        global antenna
        global module
        global comport
        global home_bool
        global string
        global lt,new_lt,bt_lt,ble_lt
        lt=[]
        bt_lt=[]
        ble_lt=[]
        new_lt=[]
        global x1,y1
        x1=[]
        y1=[]
        global p
        global n, n2, n3
        n, n2, n3=0, 0, 0
        p=""
        global l, abrddone,portopen
        l=[]
        global p1,f1
        p1=[]
        f1=[]
        abrddone=False
        portopen=False
        string=""
        global wifi_pass, wifi_fail, wifi_rssi
        wifi_pass, wifi_fail, wifi_rssi = [],[],[]
        global bt_pass, bt_fail, bt_rssi
        bt_pass, bt_fail, bt_rssi = [],[],[]
        global ble_pass, ble_fail, ble_rssi
        ble_pass, ble_fail, ble_rssi = [],[],[]
        antenna = tk.StringVar()
        module=tk.StringVar()
        comport=tk.StringVar()
        
        canva = tk.Canvas(self)
        fram = tk.Frame(canva)
        x_bar = tk.Scrollbar(self)
        y_bar = tk.Scrollbar(self)

        def updateScrollRegion():
            canva.update_idletasks()
            canva.config(scrollregion=fram.bbox())
            self.after(1000, updateScrollRegion)
            

        canva.config(xscrollcommand=x_bar.set,yscrollcommand=y_bar.set, highlightthickness=0)
        x_bar.config(orient=tk.HORIZONTAL, command=canva.xview)
        y_bar.config(orient=tk.VERTICAL, command=canva.yview)

        x_bar.pack(fill=tk.X, side=tk.BOTTOM, expand=tk.FALSE)
        y_bar.pack(fill=tk.Y, side=tk.RIGHT, expand=tk.FALSE)
        canva.pack(fill=tk.BOTH, side=tk.LEFT, expand=tk.TRUE)
        canva.create_window(0, 0, window=fram, anchor=tk.NW)
        canva.grid_rowconfigure(0, weight=1)
        canva.grid_columnconfigure(0, weight=1)
        self.frames = {}
        for F in (PageOne, PageTwo, PageThree, PageFour, PageFive, PageSix, PageSeven):
            frame = F(fram, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky='NSEW')
        updateScrollRegion()
        self.show_frame(PageOne)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()

class PageOne(ttk.Frame):

    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()
    def make_widget(self):
        
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        w = tk.Label(self.cvs, text="PHY PERFORMANCE TEST",fg="red",font="Times 20 bold")
        w.grid(row=1,columnspan=2,pady=10)
        
        self.protocol = tk.StringVar(self.cvs)
        self.receive = tk.StringVar(self.cvs)
        
        self.receive.set("Transmit")
        self.protocol.set("Wi-Fi")
        module.set("QMS")
        antenna.set("INTERNAL_ANTENNA")
        
        module_label = ttk.Label(self.cvs, text="Module Type",font = ("Times New Roman", 12))
        module_label.grid(column = 0,row = 9,padx = 50, pady = 20,sticky='W')
        

        protocol_label = ttk.Label(self.cvs, text="Protocol",font = ("Times New Roman", 12))
        protocol_label.grid(column = 0,row = 11,padx = 50, pady = 20,sticky='W')
        

        antenna_lable = ttk.Label(self.cvs, text="Antenna",font = ("Times New Roman", 12))
        antenna_lable.grid(column = 0,row = 12,padx = 50, pady = 20,sticky='W')
        
        
        receive_lable = ttk.Label(self.cvs, text="Transmit/Receive",font = ("Times New Roman", 12))
        receive_lable.grid(column = 0,row = 13,padx = 50, pady = 20,sticky='W')
        
        def callback(url):
            webbrowser.open_new_tab(url)
        def antennaChoice(event):
            if(module_menu.get()=="QMS"):
                antenna.set("INTERNAL_ANTENNA")
                antenna_menu['values']=["INTERNAL_ANTENNA"]
                link.bind("<Button-1>", lambda e:
                callback("https://www.silabs.com/wireless/wi-fi/rs9116-wi-fi-ncp-socs/device.rs9116w-sb00-qms"))
                link.config(text="Click - QMS Hyperlink")
            elif(module_menu.get()=="B00"):
                antenna.set("INTERNAL_ANTENNA")
                antenna_menu['values']=["INTERNAL_ANTENNA"]
                link.bind("<Button-1>", lambda e:
                callback("https://www.silabs.com/wireless/wi-fi/rs9116-wi-fi-ncp-modules/device.rs9116w-sb00-b00"))
                link.config(text="Click - B00 Hyperlink")
            elif(module_menu.get()=="CC0"):
                antenna.set("RF_PORT2/INTERNAL_ANTENNA")
                antenna_menu['values']=["RF_PORT2/INTERNAL_ANTENNA","RF_PORT1/U.FL"]
                link.bind("<Button-1>", lambda e:
                callback("https://www.silabs.com/wireless/wi-fi/rs9116-wi-fi-ncp-modules/device.rs9116w-db00-cc0"))
                link.config(text="Click - CC0 Hyperlink")
            else:
                antenna.set("RF_PORT2/INTERNAL_ANTENNA")
                antenna_menu['values']=["RF_PORT2/INTERNAL_ANTENNA","RF_PORT1/U.FL"]
                link.bind("<Button-1>", lambda e:
                callback("https://www.silabs.com/wireless/wi-fi/rs9116-wi-fi-ncp-modules/device.rs9116w-db00-cc1"))
                link.config(text="Click - CC1 Hyperlink")
            

        module_menu = ttk.Combobox(self.cvs, text=module, width=26,values=["QMS","B00","CC0","CC1"],state= "readonly")
        module_menu.bind("<<ComboboxSelected>>",antennaChoice)
        module_menu.grid(column = 1, row = 9,padx = 10 )
        link = Label(self.cvs, text="Click - QMS Hyperlink", fg="blue", cursor="hand2")
        link.bind("<Button-1>", lambda e:
                callback("https://www.silabs.com/wireless/wi-fi/rs9116-wi-fi-ncp-socs/device.rs9116w-sb00-qms"))
        link.grid(column = 1, row = 10,padx = 5 )
        
        protocol_menu = ttk.Combobox(self.cvs, text=self.protocol,width=26, values=["Wi-Fi", "BT", "BLE"],state= "readonly")
        protocol_menu.bind("<<>ComboboxSelected>")
        protocol_menu.grid(column = 1, row = 11,padx = 10)
        antenna_menu = ttk.Combobox(self.cvs, text=antenna,width=26,state= "readonly", values=["INTERNAL_ANTENNA"])
        antenna_menu.bind("<<>ComboboxSelected>")
        
        antenna_menu.grid(column = 1, row = 12,padx = 10)
        receive_menu = ttk.Combobox(self.cvs, text=self.receive,width=26, values=["Transmit", "Receive"],state= "readonly")
        receive_menu.bind("<<>ComboboxSelected>")
        receive_menu.grid(column = 1, row = 13,padx = 10)
        
        def scanning():
            global l
            l=[]
            ports = serial.tools.list_ports.comports()
            for port_only, desc, hwid in sorted(ports):
                port = port_only + " - " + desc
                l.append(port)
                #l.append(port_only)
            comport_menu['values']=l
            self.cvs.after(1000, scanning)
        
        
        comport_label=ttk.Label(self.cvs, text = "COM Port",font = ("Times New Roman", 12))
        comport_label.grid(column = 0,row = 8,padx = 50, pady = 20,sticky='W')
        comport_menu=ttk.Combobox(self.cvs, text= comport ,width=26, postcommand=scanning,state= "readonly") 
        comport_menu.grid(column=1, row=8, padx=10)
        
        button_set = tk.Button(self.cvs,text="NEXT",font = ("Times New Roman", 12, "bold"),fg='blue',command=self.ok)
        button_set.grid(row=20,columnspan=2 , pady=20)
        
                
        self.cvs.pack()
        
        Grid.columnconfigure(self.cvs, 0,weight=1)
        Grid.columnconfigure(self.cvs, 1,weight=1)
        def change_page(self):
            pass
        
    def ok(self):
        global p1,f1
        p1=[]
        f1=[]
        try:
            global string
            string=""
            global home_bool, wlan_tx_bool, wlan_rx_bool, bt_tx_bool, bt_rx_bool, ble_tx_bool, ble_rx_bool
            global wlan_tx_start, wlan_rx_start, bt_tx_start, bt_rx_start, ble_tx_start, ble_rx_start
            wlan_tx_start=False
            wlan_rx_start=False
            bt_tx_start=False
            bt_rx_start=False
            ble_tx_start=False
            ble_rx_start=False
            
            home_bool=True
            wlan_tx_bool=True
            wlan_rx_bool=True
            bt_tx_bool=True
            bt_rx_bool=True
            ble_tx_bool=True
            ble_rx_bool=True
                
            global p,sp
            
            if(p!=comport.get()):
                p=comport.get()
                split_p = p.split(' - ')
                sp=serial.Serial(split_p[0],baudrate=115200,timeout=0.05)
            def startreading():
                global portopen
                #print("In Progress")
                try:
                    temp= sp.readlines()
                    portopen=False
                except Exception as e:
                    #portopen=True
                    sp.close()
                    time.sleep(1)
                    sp.open()
                    
                self.cvs.after(100, startreading)
            #startreading()
            if(self.receive.get() == 'Transmit' and self.protocol.get() == 'Wi-Fi' and comport.get()):
                self.controller.show_frame(PageTwo)
            elif(self.receive.get() == 'Receive' and self.protocol.get() == 'Wi-Fi' and comport.get()):
                self.controller.show_frame(PageThree)
            elif(self.receive.get() == 'Transmit' and self.protocol.get() == 'BT' and comport.get()):
                self.controller.show_frame(PageFour)
            elif(self.receive.get() == 'Receive' and self.protocol.get() == 'BT' and comport.get()):
                self.controller.show_frame(PageFive)
            elif(self.receive.get() == 'Transmit' and self.protocol.get() == 'BLE' and comport.get()):
                self.controller.show_frame(PageSix)
            elif(self.receive.get() == 'Receive' and self.protocol.get() == 'BLE' and comport.get()):
                self.controller.show_frame(PageSeven)
            else:
                tk.messagebox.showinfo('Error','Select values from drop box')
        except Exception as e:
            pass
            messagebox.showinfo("Error","This COM port cannot be opened. Access Denied!")
            print("This COM port cannot be opened. Access Denied!")
            

class PageTwo(ttk.Frame):
    
    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()
        
    def make_widget(self):
        def saveLog():
            try:
                global string
                name=filedialog.asksaveasfile(mode='w',defaultextension=".txt")
                ct = datetime.datetime.now()
                string= str(ct) +'\n' + string
                if not name is None: 
                    name.write(string)
                    name.close()
            except Exception as e:
                pass
        
        def sendcmd(cmd):
            try:
                p=comport.get()
                resp=""
                response=[]
                sp.write(cmd.encode())
                count=0
                while(resp!="\n"):
                    resp=sp.readline()
                    count+=1
                    if count==31:
                        break
                    if resp.decode()=='': 
                        continue
                    if resp[0:1].decode()=='E':
                        print('Error: '+resp.decode())
                        return []
                    response.append(resp)
                return response
            except Exception as e:
                pass
    
        def wifi_tx():
            global string, portopen, home_bool, wlan_tx_start
            string=""
            
            if(tx_mode_menu.get()=="Burst" and (int(self.my_spinbox.get())<24 or int(self.my_spinbox.get())>1500)):
                tk.messagebox.showinfo('Error','Enter valid packet length')
            elif(tx_mode_menu.get()=="Continuous" and (int(self.my_spinbox.get())<24 or int(self.my_spinbox.get())>260)):
                tk.messagebox.showinfo('Error','Enter valid packet length')
            elif(tx_mode_menu.get()=="CW Mode" and (int(self.my_spinbox.get())<24 or int(self.my_spinbox.get())>260)):
                tk.messagebox.showinfo('Error','Enter valid packet length')
            else:
                status.config(text='Wifi Transmit Started')
                try:
                    if(wlan_tx_bool or home_bool or wlan_tx_start):
                        home_bool= False
                        def softreset():
                            reset_data="at+rsi_reset\r\n"
                            sp.write(reset_data.encode())
                            
                        softreset()
                        time.sleep(0.5)
                        sp.close()
                        time.sleep(1)
                        sp.open()
                        sp.write('|'.encode())
                        time.sleep(0.05)
                        resp =sp.read(size=20)
                        string+=str(resp.decode())
                        if(resp.decode() == 'U'):
                            sp.write('U'.encode())
                            time.sleep(0.05)
                            resp=sp.read(size=400)
                            string+=str(resp.decode())
                            sp.write('1'.encode())
                            resp=sp.read(size=40)
                            time.sleep(1)
                            string+=str(resp.decode())
                            count=0
                            while(resp!="\n"):
                                resp=sp.readline()
                                count+=1
                                if count==30:
                                    break
                                if resp.decode()=='':
                                    continue
                                string+=str(resp.decode())
                                if(resp.decode()=="Loading Done\r\n"):
                                    print("ABRD done\n")
                                    string+="ABRD done\n"
                                    abrddone=True
                                    response=sendcmd('at+rsi_opermode=8\r\n')
                                    string+="at+rsi_opermode=8 \n"
                                    for resp in response:
                                        if not(resp[0:2].decode()=='OK'):
                                            print("Error"+str(resp.decode()))
                                            string+="Error-opermode: "
                                            string+=str(resp.decode())
                                            break
                                        else:
                                            string+="Opermode set\n"
                        
                                            if(band_menu.get()== '2.4GHz'):
                                                response=sendcmd('at+rsi_band=0\r\n')
                                                string+="at+rsi_band=0 \n"
                                            else:
                                                response=sendcmd('at+rsi_band=1\r\n')
                                                string+="at+rsi_band=1 \n"
                                            for resp in response:
                                                if not(resp[0:2].decode()=='OK'):
                                                    print("Error"+str(resp.decode()))
                                                    string+="Error-band: "
                                                    string+=str(resp.decode())
                                                    break
                                                else:
                                                    string+="Band set\n"
                                                    pll_mode_string=0
                                                    
                                                    
                                                    featframe_string='at+rsi_feat_frame='+str(pll_mode_string)+',1,0,0,1,0\r\n'
                                                    string+=featframe_string+'\n'
                                                    sp.write(featframe_string.encode())
                                                    resp=sp.read(size=40)
                                                    if not(resp[0:2].decode()=='OK'):
                                                        print("Error"+str(resp.decode()))
                                                        string+="Error- featframe: "
                                                        string+=str(resp.decode())
                                                        
                                                    else:
                                                        string+="Feat frame set\n"
                            
                                                        sp.write('at+rsi_init\r\n'.encode())
                                                        string+="at+rsi_init \n"
                                                        resp=sp.read(size=40)
                                                        if not(resp[0:2].decode()=='OK'):
                                                            print("Error"+str(resp.decode()))
                                                            string+="Error- init: "
                                                            string+=str(resp.decode())
                                                            
                                                        else:
                                                            string+="init set\n"
                                                            if(antenna.get()=='RF_PORT2/INTERNAL_ANTENNA' or antenna.get()=='INTERNAL_ANTENNA'):
                                                                response=sendcmd('at+rsi_antenna=0\r\n')
                                                                string+="at+rsi_antenna=0 \n"
                                                            else:
                                                                response=sendcmd('at+rsi_antenna=1\r\n')
                                                                string+="at+rsi_antenna=1 \n"
                                                            for resp in response:
                                                                if not(resp[0:2].decode()=='OK'):
                                                                    print("Error"+str(resp.decode()))
                                                                    string+="Error-antenna: "
                                                                    string+=str(resp.decode())
                                                                    break
                                                                else:
                                                                    string+="Antenna set\n"
                                                                    
                                                                    if(region_menu.get() == 'JP (Japan)'): 
                                                                        x=3
                                                                    elif(region_menu.get() == 'FCC (US)'):
                                                                        x=0
                                                                    elif(region_menu.get() == 'ETSI (Europe)'):
                                                                        x=2
                                                                    else:
                                                                        x=4
                                                                    region_string='at+rsi_setregion=1,'+str(x)+'\r\n'
                                                                    string+=region_string+'\n'
                                                                    sp.write(region_string.encode())
                                                                    resp=sp.read(size=100)
                                                                    if not(resp[0:2].decode()=='OK'):
                                                                        print("Error"+str(resp.decode()))
                                                                        string+="Error-region: "
                                                                        string+=str(resp.decode())
                                                                    else:
                                                                        string+="Region set\n"
                        
                    if(tx_mode_menu.get()=='Burst'): x=0
                    elif(tx_mode_menu.get()=='Continuous'): x=1
                    else: x=2
                    if(rate_menu.get()=='1'):
                        rate_string=0
                    elif(rate_menu.get()=='2'):
                        rate_string=2
                    elif(rate_menu.get()=='5.5'):
                        rate_string=4
                    elif(rate_menu.get()=='11'):
                        rate_string=6
                    elif(rate_menu.get()=='6'):
                        rate_string=139
                    elif(rate_menu.get()=='9'):
                        rate_string=143
                    elif(rate_menu.get()=='12'):
                        rate_string=138
                    elif(rate_menu.get()=='18'):
                        rate_string=142
                    elif(rate_menu.get()=='24'):
                        rate_string=137
                    elif(rate_menu.get()=='36'):
                        rate_string=141
                    elif(rate_menu.get()=='48'):
                        rate_string=136
                    elif(rate_menu.get()=='54'):
                        rate_string=140
                    elif(rate_menu.get()=='mcs0'):
                        rate_string=256
                    elif(rate_menu.get()=='mcs1'):
                        rate_string=257
                    elif(rate_menu.get()=='mcs2'):
                        rate_string=258
                    elif(rate_menu.get()=='mcs3'):
                        rate_string=259
                    elif(rate_menu.get()=='mcs4'):
                        rate_string=260
                    elif(rate_menu.get()=='mcs5'):
                        rate_string=261
                    elif(rate_menu.get()=='mcs6'):
                        rate_string=262
                    else:
                        rate_string=263
                    wlan_tx_start= True
                    if not(x==0):
                        per_string='at+rsi_per=1,'+ max_power_menu.get()+','+str(rate_string)+','+self.my_spinbox.get()+',1,'+chnl_list_menu.get()+',0,0,0,0\r\n'
                    else:
                        per_string='at+rsi_per=1,'+ max_power_menu.get()+','+str(rate_string)+','+self.my_spinbox.get()+',0,'+chnl_list_menu.get()+',0,0,' + noOfPackets_menu.get() +',0\r\n'
                    
                    if(x==0 or x==1):
                        print(per_string)
                        string+=per_string+'\n'
                        response=sendcmd(per_string)
                        for resp in response:
                            if not(resp[0:2].decode()=='OK'):
                                print("Error"+str(resp.decode()))
                                string+="Error-per: "
                                string+=str(resp.decode())
                                break
                            else:
                                if(x==0): 
                                    string+="Burst Mode Started\n"
                                else: 
                                    string+="Continuous Mode Started\n"
                    
                    if(x==2):
                        per_string='at+rsi_per=1,'+ max_power_menu.get()+','+str(rate_string)+','+self.my_spinbox.get()+',1,'+chnl_list_menu.get()+','+'0,0,0,0\r\n'
                        string+=per_string+'\n'
                        response=sendcmd(per_string)
                        for resp in response:
                            if not(resp[0:2].decode()=='OK'):
                                print("Error"+str(resp.decode()))
                                string+="Error-per: "
                                string+=str(resp.decode())
                                break
                            else:
                                string+="Continuous Mode Started\n"
                                response=sendcmd('at+rsi_per=0\r\n')
                                string+="at+rsi_per=0 \n"
                                for resp in response:
                                    if not(resp[0:2].decode()=='OK'):
                                        print("Error"+str(resp.decode()))
                                        string+="Error-stop: "
                                        string+=str(resp.decode())
                                        break
                                    else:
                                        per_string='at+rsi_per=1,'+ max_power_menu.get()+','+str(rate_string)+','+self.my_spinbox.get()+',2,'+chnl_list_menu.get()+','+'0,0,0,0\r\n'
                                        print(per_string)
                                        string+=per_string+'\n'
                                        response=sendcmd(per_string)
                                        for resp in response:
                                            if not(resp[0:2].decode()=='OK'):
                                                print("Error"+str(resp.decode()))
                                                string+="Error-per: "
                                                string+=str(resp.decode())
                                                break
                                            else:
                                                string+="CW Mode Started\n"

                    
                except Exception as e:
                    pass
                    
        def wifi_tx_stop():
            try:
                global wlan_tx_bool,string
                global wlan_tx_start #checks the most recent command sent - if True: Start Transmit, else Stop Transmit is sent
                wlan_tx_start= False
                wlan_tx_bool=False
                response=sendcmd('at+rsi_per=0\r\n')
                string+="at+rsi_per=0 \n"
                print("at+rsi_per=0 \n")
                for resp in response:
                    if not(resp[0:2].decode()=='OK'):
                        print("Error"+str(resp.decode()))
                        string+="Error-stop: "
                        string+=str(resp.decode())
                        break
                    else:
                        string+="Wi-Fi Transmit Stopped\n"
                        status.config(text='Wifi Transmit Stoppped')
            except Exception as e:
                pass
        
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        Grid.columnconfigure(self.cvs, 0,weight=1)
        Grid.columnconfigure(self.cvs, 1,weight=1)
        
        w = tk.Label(self.cvs, text="Wi-Fi TRANSMIT",fg="red",font="Times 20 bold")
        w.grid(row=1,columnspan=2,pady=30)
        self.max_power = tk.StringVar(self.cvs)
        self.rate = tk.StringVar(self.cvs)
        self.tx_mode = tk.StringVar(self.cvs)
        self.packet = tk.StringVar(self.cvs)
        self.band = tk.StringVar(self.cvs)
        self.region = tk.StringVar(self.cvs)
        self.channel = tk.StringVar(self.cvs)
        self.aggretionFeature = tk.StringVar(self.cvs)
        self.noOfPackets = tk.StringVar(self.cvs)
        self.delay = tk.StringVar(self.cvs)
        
        self.region.set("World Wide")  # default value
        self.rate.set("1")  # default value
        self.tx_mode.set("Continuous")  # default value
        self.max_power.set("127")  # default value
        self.packet.set("260") # default value
        self.band.set("2.4GHz") # default value
        self.channel.set("1") # default value
        self.aggretionFeature.set("0") # default value
        self.noOfPackets.set("0") # default value
        self.delay.set("0") # default value
        
        
        def mode_length(event):
            if(tx_mode_menu.get()=="Burst"):
                self.packet.set("1000") # default value
                label=ttk.Label(self.cvs, text = "(Min bytes: 24  Max bytes: 1500)",font = ("Times New Roman", 10)).grid(column = 1,row = 23)
                
                noOfPackets_label.grid(column = 0,row = 19, padx = 50, pady = 15,sticky='W')
                noOfPackets_menu.grid(column = 1, row = 19,padx = 10)
            else:
                self.packet.set("260") # default value
                label=ttk.Label(self.cvs, text = "(Min bytes: 24  Max bytes: 260)",font = ("Times New Roman", 10)).grid(column = 1,row = 23)
                
                noOfPackets_menu.grid_forget()
                noOfPackets_label.grid_forget()
            
        def rate_band():
            if(band_menu.get() =='2.4GHz'):
                rate_menu['values']=["1", "2", "5.5", "6", "9", "11", "12", "18", "24", "36", "48", "54", "mcs0", "mcs1", "mcs2", "mcs3", "mcs4", "mcs5", "mcs6", "mcs7"]
            else:
                rate_menu['values']=["6", "9", "12", "18", "24", "36", "48", "54", "mcs0", "mcs1", "mcs2", "mcs3", "mcs4", "mcs5", "mcs6", "mcs7"]
                
        max_power_label = ttk.Label(self.cvs, text="Transmit Power ",font = ("Times New Roman", 12))
        max_power_label.grid(column = 0,row = 15, padx = 50, pady = 15,sticky='W')
        max_power_menu = ttk.Combobox(self.cvs, textvariable=self.max_power,width=26, values=["2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "127"],state= "readonly")
        max_power_menu.bind("<<>ComboboxSelected>")
        max_power_menu.grid(column = 1, row = 15,padx = 10)
        
        rate_label = ttk.Label(self.cvs, text="Rate",font = ("Times New Roman", 12))
        rate_label.grid(column = 0,row = 16, padx = 50, pady = 15,sticky='W')
        rate_menu = ttk.Combobox(self.cvs, textvariable=self.rate,width=26, values=["1", "2", "5.5", "6", "9", "11", "12", "18", "24", "36", "48", "54", "mcs0", "mcs1", "mcs2", "mcs3", "mcs4", "mcs5", "mcs6", "mcs7"],postcommand=rate_band ,state= "readonly")
        rate_menu.bind("<<ComboboxSelected>>")
        rate_menu.grid(column = 1, row = 16, padx = 10)
        
        tx_mode_label = ttk.Label(self.cvs, text="Mode",font = ("Times New Roman", 12))
        tx_mode_label.grid(column = 0,row = 17, padx = 50, pady = 15,sticky='W')
        tx_mode_menu = ttk.Combobox(self.cvs, textvariable=self.tx_mode,width=26, values=["Continuous", "Burst","CW Mode"],state= "readonly")
        tx_mode_menu.bind("<<ComboboxSelected>>", mode_length)
        tx_mode_menu.grid(column = 1, row = 17,padx = 10)
        
        noOfPackets_label = ttk.Label(self.cvs, text="No Of Packets",font = ("Times New Roman", 12))
        noOfPackets_label.grid(column = 0,row = 19, padx = 50, pady = 15,sticky='W')
        noOfPackets_menu = ttk.Entry(self.cvs, textvariable=self.noOfPackets,width=29)
        noOfPackets_menu.grid(column = 1, row = 19,padx = 10)
        noOfPackets_menu.grid_forget()
        noOfPackets_label.grid_forget()

        packet_label = ttk.Label(self.cvs, text="Packet Length",font = ("Times New Roman", 12))
        packet_label.grid(column = 0,row = 22, padx = 50, pady = 15,sticky='W')
        self.my_spinbox = tk.Entry(self.cvs, textvariable=self.packet, width=29)
        self.my_spinbox.grid(column = 1, row = 22)
        def changeband():
            if(module.get() =="CC0" or module.get() =="CC1"):
                band_menu['values']=["2.4GHz","5GHz"]
                
            else: 
                band_menu['values']=["2.4GHz"]
            
        def channelDefault(event):
            if(band_menu.get()=='5GHz'):
                self.channel.set("36")
                rate_menu.set("6")
            else: 
                self.channel.set("1")
                rate_menu.set("1")
        def changeChannel():
        
            if(band_menu.get() =='2.4GHz'):
                if(region_menu.get() == 'JP (Japan)'): 
                    x=3
                    chnl_list_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11','12','13','14')
                elif(region_menu.get() == 'FCC (US)'):
                    x=0
                    chnl_list_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11')
                elif(region_menu.get() == 'ETSI (Europe)'):
                    x=2
                    chnl_list_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11','12','13')
                else:
                    x=4
                    chnl_list_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11','12','13','14')  
            else:
                
                chnl_list_menu['values'] =('36','40','44','48','52','56','60','64','100','104','108','112','116','120','124','128','132','136','140','149','153','157','161','165')
        
        
        band_label = ttk.Label(self.cvs, text="Band",font = ("Times New Roman", 12))
        band_label.grid(column = 0,row = 25, padx = 50, pady = 15,sticky='W')
        band_menu = ttk.Combobox(self.cvs, textvariable=self.band,width=26, postcommand=changeband,state= "readonly")
        band_menu.bind("<<ComboboxSelected>>", channelDefault)
        band_menu.grid(column = 1, row = 25)
        
        region_label = ttk.Label(self.cvs, text="Region ",font = ("Times New Roman", 12))
        region_label.grid(column = 0,row = 26, padx = 50, pady = 15,sticky='W')
        region_menu = ttk.Combobox(self.cvs, textvariable=self.region,width=26, values=["FCC (US)", "ETSI (Europe)", "JP (Japan)", "World Wide"],state= "readonly")
        region_menu.bind("<<ComboboxSelected>>")
        region_menu.grid(column = 1, row = 26,padx = 10)

        channel_label = ttk.Label(self.cvs, text="Channel",font = ("Times New Roman", 12))
        channel_label.grid(column = 0,row = 27, padx = 50, pady = 15,sticky='W')
        chnl_list_menu = ttk.Combobox(self.cvs, textvariable=self.channel,width=26, postcommand=changeChannel,state= "readonly")
        chnl_list_menu.bind("<<ComboboxSelected>>")
        chnl_list_menu.grid(column = 1, row = 27,padx = 10)

        button_transmit_start = tk.Button(self.cvs, text="Wi-Fi Transmit Start",font = ("Times New Roman", 10, "bold"),fg='black', command=wifi_tx)
        button_transmit_start.grid(column=0, row=30, padx=50,pady=10,sticky='W')
        
        button_transmit_stop = tk.Button(self.cvs, text="Wi-Fi Transmit Stop",font = ("Times New Roman", 10, "bold"),fg='black', command=wifi_tx_stop)
        button_transmit_stop.grid(column=1, row=30,pady=10)
        
        save = Button(self.cvs, text="SAVE LOG",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveLog)
        save.grid(column=0, row=31, pady=10,sticky='W', padx=50)
        
        btnChange = Button(self.cvs, text="HOME",font = ("Times New Roman", 12, "bold"),fg='black',command=lambda: self.controller.show_frame(PageOne),bg="#a0ccda")
        btnChange.grid(column=1, row=31)
        
        status= Label(self.cvs,text='', fg="brown",font = ("Times New Roman", 10, "bold"))
        status.grid(column=0, row=33, columnspan=2)
        
        self.cvs.pack()
        def change_page(self):
            pass
    
    
class PageThree(ttk.Frame):

    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()

    def make_widget(self):
        
        def scanning():
            try:
                global string, new_rssi
                global wifi_pass, wifi_fail, wifi_rssi
                if running:  # Only do this if the Stop button has not been clicked
                    resp=sp.read(size=139)
                    count=0
                    global var
                    global lt, new_lt
                    resp_str=bytearray()
                    if str(resp)=="b''": 
                        count+=1
                    else:
                        string+=str(resp)+'\n'
                        print(resp)
                        print("\n")
                        
                        resp1=hex(resp[24])+hex(resp[23])
                        t1=f"0x{resp[24]:02x}" + f"0x{resp[23]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp_p=(int(n1,16))
                        print("crc_pass is:",resp_p)
                        string+=str(resp_p)+'\n'
                        wifi_pass.append(resp_p)
                        print("\n")
                        
                        resp4=hex(resp[26])+hex(resp[25])
                        t1=f"0x{resp[26]:02x}" + f"0x{resp[25]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp_f=(int(n1,16))
                        print("crc_fail is:",resp_f)
                        string+=str(resp_f)+'\n'
                        wifi_fail.append(resp_f)
                        print("\n")
                        
                        resp4=hex(resp[28])+hex(resp[27])
                        t1=f"0x{resp[28]:02x}" + f"0x{resp[27]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp6=(int(n1,16))
                        print("cca_stk is:",resp6)
                        string+=str(resp6)+'\n'
                        print("\n")
                        
                        resp4=hex(resp[30])+hex(resp[29])
                        t1=f"0x{resp[30]:02x}" + f"0x{resp[29]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp6=(int(n1,16))
                        print("cca_not_stk is:",resp6)
                        string+=str(resp6)+'\n'
                        print("\n")
                        
                        resp4=hex(resp[32])+hex(resp[31])
                        t1=f"0x{resp[32]:02x}" + f"0x{resp[31]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp6=(int(n1,16))
                        print("pkt_abort is:",resp6)
                        string+=str(resp6)+'\n'
                        print("\n")
                        
                        resp4=hex(resp[34])+hex(resp[33])
                        t1=f"0x{resp[34]:02x}" + f"0x{resp[33]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp6=(int(n1,16))
                        print("fls_rx_start is:",resp6)
                        string+=str(resp6)+'\n'
                        print("\n")
                        
                        resp4=hex(resp[36])+hex(resp[35])
                        t1=f"0x{resp[36]:02x}" + f"0x{resp[35]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp6=(int(n1,16))
                        print("cca_idle is:",resp6)
                        string+=str(resp6)+'\n'
                        print("\n")

                        resp4=hex(resp[68])+hex(resp[67])
                        t1=f"0x{resp[68]:02x}" + f"0x{resp[67]:02x}"
                        n1=(t1[2:4]+t1[6:8])
                        resp6=(int(n1,16))
                        print("cal_rssi is:",resp6)
                        string+=str(resp6)+'\n'
                        wifi_rssi.append(resp6)
                        print("\n")
                        
                        if(resp_p == 0):
                            print("PER [%]: -")
                        else:
                            print("PER [%]: ", end='')
                            print('%.3f' % (resp_f*100/(resp_p + resp_f)))

                        
                        resp_str=resp
                        lt.append(list(resp_str))
                        
                self.cvs.after(1000, scanning)
            except Exception as e:
                pass
        def saveLog():
            try:
                global string
                name=filedialog.asksaveasfile(mode='w',defaultextension=".txt")
                ct = datetime.datetime.now()
                string= str(ct) +'\n' + string
                if not name is None: 
                    name.write(string)
                    name.close()
            except Exception as e:
                pass
            
        def sendcmd(cmd):
            try:
                resp=""
                response=[]
                sp.write(cmd.encode())
                count=0
                while(resp!="\n"):
                    resp=sp.readline()
                    count+=1
                    if count==31:
                        break
                    if resp.decode()=='': 
                        continue
                    if resp[0:1].decode()=='E':
                        print('Error: '+resp.decode())
                        return []
                    response.append(resp)
                return response
            except Exception as e:
                pass
                        
    
        def wifi_rx():
            try:
                status.config(text="Fetching Wi-Fi Rx Stats")
                global home_bool,string,portopen, wlan_rx_start
                string=""
                if(wlan_rx_bool or home_bool or wlan_rx_start):
                
                    home_bool= False
                    def softreset():
                        reset_data="at+rsi_reset\r\n"
                        sp.write(reset_data.encode())
                        response=sp.read(size=100)
                        
                    softreset()
                    time.sleep(0.5)
                    sp.close()
                    time.sleep(1)
                    sp.open()
                    sp.write('|'.encode())
                    time.sleep(0.05)
                    resp =sp.read(size=20)
                    string+=str(resp.decode())
                    if(resp.decode() == 'U'):
                        sp.write('U'.encode())
                        time.sleep(0.05)
                        resp=sp.read(size=20)
                        time.sleep(1)
                        string+=str(resp.decode())
                        sp.write('1'.encode())
                        resp=sp.read(size=40)
                        string+=str(resp.decode())
                        count=0
                        while(resp!="\n"):
                            resp=sp.readline()
                            count+=1
                            if count==30:
                                break
                            if resp.decode()=='':
                                continue
                            string+=str(resp.decode())
                            if(resp.decode()=="Loading Done\r\n"):
                                print("ABRD done\n")
                                string+="ABRD done\n"
                                
                                response=sendcmd('at+rsi_opermode=8\r\n')
                                string+="at+rsi_opermode=8 \n"
                                for resp in response:
                                    if not(resp[0:2].decode()=='OK'):
                                        print("Error"+str(resp.decode()))
                                        string+="Error-opermode: "
                                        string+=str(resp.decode())
                                        break
                                    else:
                                        string+="Opermode set\n"
                        
                                        if(wirelessBand_menu.get()== '2.4GHz'):
                                            response=sendcmd('at+rsi_band=0\r\n')
                                            string+="at+rsi_band=0 \n"
                                        else:
                                            response=sendcmd('at+rsi_band=1\r\n')
                                            string+="at+rsi_band=1 \n"
                                        for resp in response:
                                            if not(resp[0:2].decode()=='OK'):
                                                print("Error"+str(resp.decode()))
                                                string+="Error-band: "
                                                string+=str(resp.decode())
                                                break
                                            else:
                                                string+="Band set\n"
                                                pll_mode_string=0
                                                
                                                
                                                featframe_string='at+rsi_feat_frame='+str(pll_mode_string)+',1,0,0,1,0\r\n'
                                                string+=featframe_string
                                                sp.write(featframe_string.encode())
                                                resp=sp.read(size=40)
                                                if not(resp[0:2].decode()=='OK'):
                                                    print("Error"+str(resp.decode()))
                                                    string+="Error-featframe: "
                                                    string+=str(resp.decode())
                                                else:
                                                    string+="Feat frame set\n"
                                                    sp.write('at+rsi_init\r\n'.encode())
                                                    string+="at+rsi_init \n"
                                                    resp=sp.read(size=40)
                                                    if not(resp[0:2].decode()=='OK'):
                                                        print("Error"+str(resp.decode()))
                                                        string+="Error-init: "
                                                        string+=str(resp.decode())
                                                        
                                                    else:
                                                        
                                                        string+="init set\n"
                        
                                                        if(region_menu.get() == 'JP (Japan)'): 
                                                            x=3
                                                        elif(region_menu.get() == 'FCC (US)'):
                                                            x=0
                                                        elif(region_menu.get() == 'ETSI (Europe)'):
                                                            x=2
                                                        else:
                                                            x=4
                                                        region_string='at+rsi_setregion=1,'+str(x)+'\r\n'
                                                        string+=region_string
                                                        
                                                        sp.write(region_string.encode())
                                                        resp=sp.read(size=100)
                                                        
                                                        if not(resp[0:2].decode()=='OK'):
                                                            print("Error"+str(resp.decode()))
                                                            string+="Error-region: "
                                                            string+=str(resp.decode())
                                                        else:
                                                            
                                                            string+="Region set\n"
                global running, new_rssi
                new_rssi =[]
                running=True
                wlan_rx_start=True
                per_string='at+rsi_per_stats=0,'+channel_menu.get() +'\r\n'
                string+=per_string
                print(per_string)
                
                string+="Fetching Wi-Fi Rx Stats\n"
                sp.write(per_string.encode())
                self.cvs.after(1000, scanning)  # After 1 second, call scanning
            except Exception as e:
                pass
                
                
        
        def wifi_rx_stop():
            try:
                global running,string, wlan_rx_start
                global wifi_pass, wifi_fail, wifi_rssi
                a,b,c = [],[],[]
                a,b,c = wifi_pass, wifi_fail, wifi_rssi
                wifi_pass, wifi_fail, wifi_rssi = [],[],[]
                if(len(a)==len(b) and len(a)==len(c)):
                    t=list(range(len(a)))
                    
                wlan_rx_start=False
                running = False
                global wlan_rx_bool
                wlan_rx_bool=False
                resp=""
                response=sendcmd('at+rsi_per_stats=1\r\n')
                string+="at+rsi_per_stats=1 \n"
                print("at+rsi_per_stats=1")
                for resp in response:
                    if not(resp[0:2].decode()=='OK'):
                        print("Error"+str(resp.decode()))
                        string+="Error-stop: "
                        string+=str(resp.decode())
                        break
                    else:
                        string+="Stop Receive Stats\n"
                        status.config(text="Stopped Receiving Wi-Fi Rx Stats")

                global p1,f1
                p1=[]
                f1=[]
                for i in range(len(a)):
                    if(a[i]+b[i]  == 0):
                        p1.append(0)
                        f1.append(100)
                    else:
                        p1.append(a[i]*100/(a[i]+b[i]))
                        f1.append(b[i]*100/(a[i]+b[i]))
                
                
                
                fig=Figure(figsize=(7,3), dpi=100)
                ax=fig.add_subplot(111)
                ax.plot(t, p1 , c='g' ,marker=(8,2,0),ls='--',label='CRC Pass',fillstyle='none')
                ax.plot(t, f1 ,c='r' ,marker=(8,2,0),ls='--',label='CRC Fail')
                
                mplcursors.cursor()
                ax.legend(['CRC Pass%', 'CRC Fail%'])
                #ax1=fig.add_subplot(212)
                #ax1.plot(t,c,'b',marker=(8,2,0),ls='--',label='RSSI val')
                #ax1.legend(['RSSI Val'])
                
                cursor = Cursor(ax, horizOn=True, vertOn=True, useblit=True,color = 'r', linewidth = 1)
                annot = ax.annotate("", xy=(0,0), xytext=(0,0),textcoords="offset points",bbox=dict(boxstyle='round4', fc='linen',ec='k',lw=1),arrowprops=dict(arrowstyle='-|>'))
                annot.set_visible(False)
                
                coord = []
                def onclick(event):
                    coord.append((event.xdata, event.ydata))
                    x = event.xdata
                    y = event.ydata
                    annot.xy = (x,y)
                    try:
                        text = "({:.2g}, {:.2g})".format(x,y)
                        annot.set_text(text)
                        annot.set_visible(True)
                        fig.canvas.draw() #redraw the figure
                    except TypeError as e:
                        pass
                    

                fig.canvas.mpl_connect('button_press_event', onclick)
                
                    
                canvas = FigureCanvasTkAgg(fig, master=self.cvs)  # A tk.DrawingArea.
                canvas.draw()
                canvas.get_tk_widget().grid(row=34,columnspan=2)
                
                yaxis_label = tk.Label(self.cvs, text="  Duration(in seconds)  ", bg="white").grid(row=35, columnspan=2)
                
                def saveGraph():
                    try:
                        global n
                        fig.savefig("wlan_graph{}.png".format(n))
                        n+=1
                    except Exception as e:
                        pass
                
                save = Button(self.cvs, text="Save Graph",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveGraph)
                save.grid(column=1, row=36, pady=20,sticky='W', padx=50)
            except Exception as e:
                pass            
       
        
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        w = tk.Label(self.cvs, text="Wi-Fi RECEIVE",fg="red",font="Times 20 bold").grid(columnspan=2,row=1,pady=10)
        def changeband():
            if(module.get() =="CC0" or module.get() =="CC1"):
                wirelessBand_menu['values']=["2.4GHz","5GHz"]
                
            else: 
                wirelessBand_menu['values']=["2.4GHz"]
        
        def delete():
            fig=Figure(figsize=(7,3), dpi=100, facecolor="#F0F0F0")
            canvas = FigureCanvasTkAgg(fig, master=self.cvs)  # A tk.DrawingArea.
            canvas.draw()
            self.dummy = canvas.get_tk_widget().grid(row=34, column=0, columnspan=2)
            self.dummy = Label(self.cvs, text="                                                  ").grid(row=35, column=0, columnspan=2)
            self.dummy = Label(self.cvs, text="                                                                                  ", height=10).grid(row=36, column=1, columnspan=2)
            self.controller.show_frame(PageOne)

        def band_channel():
            if(wirelessBand_menu.get() =='2.4GHz'):
                if(region_menu.get() == 'JP (Japan)'): 
                    channel_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11','12','13','14')
                elif(region_menu.get() == 'FCC (US)'):
                    channel_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11')
                elif(region_menu.get() == 'ETSI (Europe)'):
                    channel_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11','12','13')
                else:
                    channel_menu['values'] =('1','2','3','4','5','6','7','8','9','10','11','12','13','14')  
            else:
                channel_menu['values'] =('36','40','44','48','52','56','60','64','100','104','108','112','116','120','124','128','132','136','140','149','153','157','161','165')
        
    
        def channelDefault(event):
            if(wirelessBand_menu.get()=='5GHz'):
                self.channel.set("36")
            else: self.channel.set("1")        
        self.wirelessBand = tk.StringVar(self.cvs)
        
        self.channel = tk.StringVar(self.cvs)
        self.region = tk.StringVar(self.cvs)
        
        self.wirelessBand.set("2.4GHz")  # default value
        self.channel.set("1")  # default value
        self.region.set("World Wide")  # default value
        
        wirelessBand_label = ttk.Label(self.cvs, text="Wireless Band",font = ("Times New Roman", 12))
        wirelessBand_label.grid(column = 0,row = 15, padx = 50, pady = 20,sticky='W')
        wirelessBand_menu = ttk.Combobox(self.cvs, textvariable=self.wirelessBand, width=26, postcommand=changeband,state= "readonly")
        wirelessBand_menu.bind("<<ComboboxSelected>>", channelDefault)
        wirelessBand_menu.grid(column = 1, row = 15)
        
        region_label = ttk.Label(self.cvs, text="Region ",font = ("Times New Roman", 12))
        region_label.grid(column = 0,row = 17, padx = 50, pady = 15,sticky='W')
        region_menu = ttk.Combobox(self.cvs, textvariable=self.region, width=26,values=["FCC (US)", "ETSI (Europe)", "JP (Japan)", "World Wide"],state= "readonly")
        #region_menu.bind("<<ComboboxSelected>>",band_channel)
        region_menu.grid(column = 1, row = 17,padx = 10)
        
        channel_label = ttk.Label(self.cvs, text="Channel",font = ("Times New Roman", 12))
        channel_label.grid(column = 0,row = 18, padx = 50, pady = 20,sticky='W')
        channel_menu = ttk.Combobox(self.cvs, textvariable=self.channel,width=26,postcommand=band_channel,state= "readonly")
        channel_menu.bind("<<>ComboboxSelected>")
        channel_menu.grid(column = 1, row = 18)

        self.button_transmit_start = tk.Button(self.cvs, text="Wi-Fi Receive Start",font = ("Times New Roman", 10, "bold"),fg='black',command=wifi_rx)
        self.button_transmit_start.grid(column=0, row=24, pady=20,sticky='W', padx=50)

        button_transmit_stop = tk.Button(self.cvs, text="Wi-Fi Receive Stop",font = ("Times New Roman", 10, "bold"),fg='black',command=wifi_rx_stop)
        button_transmit_stop.grid(column=1, row=24, pady=20)
        
        save = Button(self.cvs, text="SAVE LOG",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveLog)
        save.grid(column=0, row=29, pady=20,sticky='W', padx=50)
        
        btnChange = Button(self.cvs, text="HOME",font = ("Times New Roman", 12, "bold"), fg='black', command=delete, bg="#a0ccda")
        btnChange.grid(column=1, row=29)
        status= Label(self.cvs,text='', fg="brown",font = ("Times New Roman", 10, "bold"))
        status.grid(column=0, row=30, columnspan=2)

        
        self.cvs.pack()

        def change_page(self):
            pass

    

class PageFour(ttk.Frame):

    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()

    def make_widget(self):
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        w = tk.Label(self.cvs, text="BT TRANSMIT",fg="red",font="Times 20 bold").grid(columnspan=2,row=1,pady=10)
        def saveLog():
            try:
                global string
                name=filedialog.asksaveasfile(mode='w',defaultextension=".txt")
                ct = datetime.datetime.now()
                string= str(ct) +'\n' + string
                if not name is None: 
                    name.write(string)
                    name.close()
            except Exception as e:
                pass
            
        def sendcmd(cmd):
            try:
                global string
                resp=""
                response=[]
                sp.write(cmd.encode())
                count=0
                while(resp!="\n"):
                    resp=sp.readline()
                    count+=1
                    if count==31:
                        break
                    if resp.decode()=='': 
                        continue
                    if resp[0:1].decode()=='E':
                        print('Error: '+resp.decode())
                        return []
                    response.append(resp)
                return response
            except Exception as e:
                pass
        
        def bt_tx():
            try:
                status.config(text="BT Transmit Started")
                pattern =  r'^([A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2})$'
                if not(re.search(pattern,BDAddress.get())):
                    tk.messagebox.showinfo('Error','Enter valid BDAddress pattern')
                if not (packet_type_menu.get()):
                    tk.messagebox.showinfo('Error','Select different BR/EDR and link type values. This combination does not support any packet type.')
                else:
                    global home_bool,string,portopen, bt_tx_start
                    string=""
                    if(bt_tx_bool or home_bool or bt_tx_start):
                        home_bool= False
                        def softreset():
                            reset_data="at+rsi_reset\r\n"
                            sp.write(reset_data.encode())
                            response=sp.read(size=100)
                        softreset()
                        time.sleep(0.5)
                        sp.close()
                        time.sleep(1)
                        sp.open()
                        sp.write('|'.encode())
                        time.sleep(0.05)
                        resp =sp.read(size=20)
                        #print(resp.decode())
                        string+=str(resp.decode())
                        if(resp.decode() == 'U'):
                            sp.write('U'.encode())
                            time.sleep(0.05)
                            resp=sp.read(size=20)
                            time.sleep(1)
                            #print(resp.decode())
                            string+=str(resp.decode())
                            sp.write('1'.encode())
                            resp=sp.read(size=40)
                            #print(resp.decode())
                            string+=str(resp.decode())
                            count=0
                            while(resp!="\n"):
                                resp=sp.readline()
                                count+=1
                                if count==30:
                                    break
                                if resp.decode()=='':
                                    continue
                                #print(resp)
                                string+=str(resp.decode())
                                if(resp.decode()=="Loading Done\r\n"):
                                    print("ABRD done\n")
                                    string+="ABRD done\n"
                                    sp.write('at+rsi_opermode=327680,0,2147483648,2147483648,2150629376,1073741824\r\n'.encode())  
                                    string+="at+rsi_opermode=327680,0,2147483648,2147483648,2150629376,1073741824 \n"      
                                    
                                    for i in range(1):
                                        time.sleep(1)
                                    resp=sp.read(size=100)
                                    #for resp in response:
                                    #print(resp.decode()+'1')
                                    if not(resp[0:2].decode()=='OK'):
                                        print("Error: "+str(resp.decode()))
                                        string+="Error-opermode: "
                                        string+=str(resp.decode())
                                    else:
                                        #print("Opermode set\n")
                                        string+="Opermode set\n"
                        
                    if(packet_type_menu.get()=="DM1"):
                        type=3
                        len=17
                    elif(packet_type_menu.get()=="DH1"):
                        type=4
                        len=27
                    elif(packet_type_menu.get()=="DM3"):
                        type=10
                        len=121
                    elif(packet_type_menu.get()=="DH3"):
                        type=11
                        len=183
                    elif(packet_type_menu.get()=="DM5"):
                        type=14
                        len=224
                    elif(packet_type_menu.get()=="DH5"):
                        type=15
                        len=339
                    elif(packet_type_menu.get()=="2-DH1"):
                        type=4
                        len=54
                    elif(packet_type_menu.get()=="2-DH3"):
                        type=10
                        len=367
                    elif(packet_type_menu.get()=="2-DH5"):
                        type=14
                        len=679
                    elif(packet_type_menu.get()=="3-DH1"):
                        type=8
                        len=83
                    elif(packet_type_menu.get()=="3-DH3"):
                        type=11
                        len=552
                    elif(packet_type_menu.get()=="3-DH5"):
                        type=15
                        len=1021
                    elif(packet_type_menu.get()=="HV1"):
                        type=5
                        len=10
                    elif(packet_type_menu.get()=="HV2"):
                        type=6
                        len=20
                    elif(packet_type_menu.get()=="HV3"):
                        type=7
                        len=30
                    elif(packet_type_menu.get()=="DV"):
                        type=8
                        len=19  
                    elif(packet_type_menu.get()=="EV3"):
                        type=7
                        len=30
                    elif(packet_type_menu.get()=="2-EV3"):
                        type=6
                        len=60
                    elif(packet_type_menu.get()=="3-EV3"):
                        type=7
                        len=90
                    elif(packet_type_menu.get()=="EV4"):
                        type=12
                        len=120
                    elif(packet_type_menu.get()=="2-EV5"):
                        type=12
                        len=360
                    elif(packet_type_menu.get()=="EV5"):
                        type=13
                        len=180
                    elif(packet_type_menu.get()=="3-EV5"):
                        type=13
                        len=540
                   
                    if(mode_menu.get()=='Burst'): 
                        tx_packets=0
                        tx_md=0
                    elif(mode_menu.get()=='Continuous'):
                        tx_packets=0
                        tx_md=1
                    else:
                        tx_packets=0
                        tx_md=2
                    
                        
                    if(self.pyld_menu.get()=='PRBS9'):
                        py_type=0
                    elif(self.pyld_menu.get()=='0xF0'):
                        py_type=1
                    elif(self.pyld_menu.get()=='0xAA'):
                        py_type=2
                    elif(self.pyld_menu.get()=='PBRS15'):
                        py_type=3
                    elif(self.pyld_menu.get()=='0xFF'):
                        py_type=4
                    elif(self.pyld_menu.get()=='0x00'):
                        py_type=5
                    elif(self.pyld_menu.get()=='0x0F'):
                        py_type=6
                    elif(self.pyld_menu.get()=='0x55'):
                        py_type=7
                    
                    
                    if(self.hopping_menu.get()=='No Hopping'):
                        hp_string=0
                    elif(self.hopping_menu.get()=='Fixed Hopping'):
                        hp_string=1
                    else:
                        hp_string=2
                    
                    if(antenna.get()=="RF_PORT2/INTERNAL_ANTENNA" or antenna.get()=='INTERNAL_ANTENNA' ):
                        antenna_string=2
                    else:
                        antenna_string=3
                    pll_mode=0
                    if(self.rf_chain_menu.get()=="HP"):
                        rf_chain_string=2
                    else:
                        rf_chain_string=3
                    rf_type=1
                    if(mode_menu.get()=='CW Mode'):
                        scrambler_seed=5
                    else:
                        scrambler_seed=0
                    bt_tx_start = True
                    bt_string='at+rsibt_bredrtransmit=1,'+BDAddress.get()+','+str(len)+','+str(type)+','+BREDR_menu.get()+','+channel_menu.get()+','+channel_menu.get()+','+ link_type_menu.get() +','+str(scrambler_seed)+','+str(hp_string)+','+str(antenna_string)+','+ str(pll_mode)+','+str(rf_type)+','+str(rf_chain_string)+','+ str(py_type)+','+self.tx_power.get()+','+str(tx_md)+',0,'+ str(tx_packets)+'\r\n'
                    print(bt_string)
                    string+=bt_string
                    response=sendcmd(bt_string)
                    for resp in response:
                        if not(resp[0:2].decode()=='OK'):
                            print("Error"+str(resp.decode()))
                            string+="Error-bt_tx: "
                            string+=str(resp.decode())
                            break
                        else:
                            string+="BT Transmit Set\n"
            except Exception as e:
                pass
                
        def bt_tx_stop():
            global bt_tx_bool,string, bt_tx_start
            bt_tx_start = False
            bt_tx_bool=False
            response=sendcmd('at+rsibt_bredrtransmit=0\r\n')
            string+="at+rsibt_bredrtransmit=0 \n"
            print("at+rsibt_bredrtransmit=0")
            for resp in response:
                if not(resp[0:2].decode()=='OK'):
                    print("Error"+str(resp.decode()))
                    string+="Error-stop: "
                    string+=str(resp.decode())
                    break
                else:
                    string+="BT Transmit Stop\n"
                    status.config(text='BT Transmit Stopped')
        
        def packetAvailability(event):
            if(link_type_menu.get()=="0" and BREDR_menu.get()=="1"):
                self.packet_type.set("DM1") # default value
                packet_type_menu['values']=["DM1", "DM3", "DM5", "HV1", "HV2", "HV3"]
            if(link_type_menu.get()=="0" and BREDR_menu.get()=="2"):
                self.packet_type.set("") # default value
                packet_type_menu['values']=[]
                tk.messagebox.showinfo('Error','Select different BR/EDR and link type values. This combination does not support any packet type.')
            if(link_type_menu.get()=="0" and BREDR_menu.get()=="3"):
                self.packet_type.set("") # default value
                packet_type_menu['values']=[]
                tk.messagebox.showinfo('Error','Select different BR/EDR and link type values. This combination does not support any packet type.')
            if(link_type_menu.get()=="1" and BREDR_menu.get()=="1"):
                self.packet_type.set("DM1") # default value
                packet_type_menu['values']=["DM1", "DH1", "DH3", "DM3", "DH5", "DM5"]
            if(link_type_menu.get()=="1" and BREDR_menu.get()=="2"):
                self.packet_type.set("2-DH1") # default value
                packet_type_menu['values']=["2-DH1", "2-DH3", "2-DH5"]
            if(link_type_menu.get()=="1" and BREDR_menu.get()=="3"):
                self.packet_type.set("3-DH1") # default value
                packet_type_menu['values']=["3-DH1", "3-DH3", "3-DH5"]
            if(link_type_menu.get()=="2" and BREDR_menu.get()=="1"):
                self.packet_type.set("DM1") # default value
                packet_type_menu['values']=["DM1", "DM3", "DM5", "EV3", "EV4", "EV5"]
            if(link_type_menu.get()=="2" and BREDR_menu.get()=="2"):
                self.packet_type.set("2-EV3") # default value
                packet_type_menu['values']=["2-EV3", "2-EV5"]
            if(link_type_menu.get()=="2" and BREDR_menu.get()=="3"):
                self.packet_type.set("3-EV3") # default value
                packet_type_menu['values']=["3-EV3", "3-EV5"]
                
        self.channel = tk.StringVar(self.cvs)
        self.packet_type = tk.StringVar(self.cvs)
        self.mode = tk.StringVar(self.cvs)
        self.hopping = tk.StringVar(self.cvs)
        self.pyld = tk.StringVar(self.cvs)
        self.rf_chain = tk.StringVar(self.cvs)
        self.BDAddress = tk.StringVar(self.cvs)
        self.tx_power = tk.StringVar(self.cvs)
        self.BREDR = tk.StringVar(self.cvs)
        self.link_type = tk.StringVar(self.cvs)
        
        self.packet_type.set("DM1") # default value
        self.hopping.set("No Hopping")  # default value
        self.channel.set("0")  # default value
        self.mode.set("Continuous")  # default value
        self.pyld.set("0x00") # default
        self.rf_chain.set("HP") # default
        self.tx_power.set("127") # default
        self.BREDR.set("1") # default
        self.link_type.set("0") # default

        channel_label = ttk.Label(self.cvs, text="Channel",font = ("Times New Roman", 12))
        channel_label.grid(column = 0,row = 15, padx = 50, pady = 15,sticky='W')
        channel_menu = ttk.Combobox(self.cvs, textvariable=self.channel,width=26,values=["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34",
                                                                                 "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78"],state= "readonly")
        channel_menu.bind("<<>ComboboxSelected>")
        channel_menu.grid(column = 1, row = 15)

        BREDR_label = ttk.Label(self.cvs, text="BR/EDR Mode",font = ("Times New Roman", 12))
        BREDR_label.grid(column = 0,row = 16, padx = 50, pady = 15,sticky='W')
        BREDR_menu = ttk.Combobox(self.cvs, textvariable=self.BREDR,width=26, values=["1", "2", "3"],state= "readonly")
        BREDR_menu.bind("<<ComboboxSelected>>", packetAvailability)
        BREDR_menu.grid(column = 1, row = 16)

        link_type_label = ttk.Label(self.cvs, text="Link Type",font = ("Times New Roman", 12))
        link_type_label.grid(column = 0,row = 17, padx = 50, pady = 15,sticky='W')
        link_type_menu = ttk.Combobox(self.cvs, textvariable=self.link_type,width=26, values=["0", "1", "2"],state= "readonly")
        link_type_menu.bind("<<ComboboxSelected>>", packetAvailability)
        link_type_menu.grid(column = 1, row = 17)

        packet_type_label = ttk.Label(self.cvs, text="Packet Type",font = ("Times New Roman", 12))
        packet_type_label.grid(column = 0,row = 18, padx = 50, pady = 15,sticky='W')
        packet_type_menu = ttk.Combobox(self.cvs, textvariable=self.packet_type,width=26, values=["DM1", "DM3", "DM5", "HV1", "HV2", "HV3"],state= "readonly")
        packet_type_menu.bind("<<>ComboboxSelected>")
        packet_type_menu.grid(column = 1, row = 18)
        
        mode_label = ttk.Label(self.cvs, text="Mode",font = ("Times New Roman", 12))
        mode_label.grid(column = 0,row = 19, padx = 50, pady = 15,sticky='W')
        mode_menu = ttk.Combobox(self.cvs, textvariable=self.mode,width=26, values=["Continuous", "Burst", "CW Mode"],state= "readonly")
        mode_menu.bind("<<>ComboboxSelected>")
        mode_menu.grid(column = 1, row = 19)

        self.hopping_label = ttk.Label(self.cvs, text="Hopping Type",font = ("Times New Roman", 12))
        self.hopping_label.grid(column = 0,row = 20, padx = 50, pady = 15,sticky='W')
        self.hopping_menu = ttk.Combobox(self.cvs, textvariable=self.hopping, width=26,values=["No Hopping","Fixed Hopping","Random Hopping"],state= "readonly")
        self.hopping_menu.bind("<<ComboboxSelected>>")
        self.hopping_menu.grid(column = 1, row = 20)
      
        BDAddress_label = ttk.Label(self.cvs, text="BD Address",font = ("Times New Roman", 12))
        BDAddress_label.grid(column = 0,row = 21, padx = 50, pady = 15,sticky='W')
        BDAddress = ttk.Entry(self.cvs, textvariable=self.BDAddress,width=29)
        BDAddress.insert(0,"11-11-11-11-11-11")
        BDAddress.bind("<<>ComboboxSelected>")
        BDAddress.grid(column = 1, row = 21)
        
        l0=tk.Label(self.cvs, text = "(BD Address should be same for Tx & Rx)", fg="red").grid(column=1,row = 22)
        l=tk.Label(self.cvs, text = "(Format: XX-XX-XX-XX-XX-XX)", fg="red").grid(column=0,row = 22)
        
        self.pyld_label = ttk.Label(self.cvs, text="Payload Type",font = ("Times New Roman", 12))
        self.pyld_label.grid(column = 0,row = 23, padx = 50, pady = 15,sticky='W')
        self.pyld_menu = ttk.Combobox(self.cvs, textvariable=self.pyld,width=26, values=['0x00','0xFF','0xF0','0x55','PRBS9','PBRS15','0x0F','0xAA'],state= "readonly")
        self.pyld_menu.bind("<<ComboboxSelected>>")
        self.pyld_menu.grid(column = 1,row = 23)

        def rf_tx(event):
            if(self.rf_chain_menu.get() == "HP"):
                self.tx_power.set("127") # default
                self.tx_power_menu['values']=('127','0','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18')
            else:
                self.tx_power.set("31") # default
                self.tx_power_menu['values']=('31')
            
        self.rf_chain_label = ttk.Label(self.cvs, text="RF Chain",font = ("Times New Roman", 12))
        self.rf_chain_label.grid(column = 0,row = 24, padx = 50, pady = 15,sticky='W')
        self.rf_chain_menu = ttk.Combobox(self.cvs, textvariable=self.rf_chain,width=26, values=["HP", "LP"],state= "readonly")
        self.rf_chain_menu.grid(column = 1,row = 24)

        self.tx_power_label = ttk.Label(self.cvs, text="Transmit Power Index",font = ("Times New Roman", 12))
        self.tx_power_label.grid(column = 0,row = 25, padx = 50, pady = 15,sticky='W')
        self.tx_power_menu = ttk.Combobox(self.cvs, textvariable=self.tx_power,width=26, values=['127','0','1','2','3','4','5','6','7','8','9','10'],state= "readonly")
        self.tx_power_menu.bind("<<ComboboxSelected>>")
        self.tx_power_menu.grid(column = 1,row = 25)


        self.button_transmit_start = tk.Button(self.cvs, text="BT Transmit Start",font = ("Times New Roman", 10, "bold"),fg='black',command=bt_tx)
        self.button_transmit_start.grid(column=0, row=26, pady=20,sticky='W', padx=50)

        button_transmit_stop = tk.Button(self.cvs, text="BT Transmit Stop",font = ("Times New Roman", 10, "bold"),fg='black',command=bt_tx_stop)
        button_transmit_stop.grid(column=1, row=26, pady=20)

        save = Button(self.cvs, text="SAVE LOG",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveLog)
        save.grid(column=0, row=27)
        
        btnChange = Button(self.cvs, text="HOME", font = ("Times New Roman", 12, "bold"),fg='black',
                           command=lambda: self.controller.show_frame(PageOne),
                           bg="#a0ccda")
        btnChange.grid(column=1, row=27)
        status= Label(self.cvs,text='', fg="brown",font = ("Times New Roman", 10, "bold"))
        status.grid(column=0, row=28, columnspan=2)
        self.cvs.pack()

        def change_page(self):
            pass

    

    


class PageFive(ttk.Frame):

    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()

    def make_widget(self):
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        w = tk.Label(self.cvs, text="BT RECEIVE",fg="red",font="Times 20 bold").grid(columnspan=2,row=1,pady=10)
        def saveLog():
            try:
                global string
                name=filedialog.asksaveasfile(mode='w',defaultextension=".txt")
                ct = datetime.datetime.now()
                string= str(ct) +'\n' + string
                if not name is None: 
                    name.write(string)
                    name.close()
            except Exception as e:
                pass
            
        def bt_scanning():
            try:
                global string, bt_pass, bt_fail, bt_rssi
                if bt_running:
                    sp.write('at+rsibt_perstats\r\n'.encode())
                    resp=sp.read(size=100)
                    print(str(resp))

                    print("\n")
                    resp1=hex(resp[4])+hex(resp[3])
                    t1=f"0x{resp[4]:02x}" + f"0x{resp[3]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp_f=(int(n1,16))
                    print("crc_fail_count is:",resp_f)
                    bt_fail.append(resp_f)
                    print("\n")
                    
                    resp1=hex(resp[6])+hex(resp[5])
                    t1=f"0x{resp[6]:02x}" + f"0x{resp[5]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp_p=(int(n1,16))
                    print("crc_pass_count is:",resp_p)
                    bt_pass.append(resp_p)
                    print("\n")
                    
                    resp1=hex(resp[8])+hex(resp[7])
                    t1=f"0x{resp[8]:02x}" + f"0x{resp[7]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("tx_abort_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[10])+hex(resp[9])
                    t1=f"0x{resp[10]:02x}" + f"0x{resp[9]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_drop_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[12])+hex(resp[11])
                    t1=f"0x{resp[12]:02x}" + f"0x{resp[11]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_cca_idle_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[14])+hex(resp[13])
                    t1=f"0x{resp[14]:02x}" + f"0x{resp[13]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_start_idle_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[16])+hex(resp[15])
                    t1=f"0x{resp[16]:02x}" + f"0x{resp[15]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_abrt_count is:",resp3)
                    print("\n")
                    
                    resp4=hex(resp[18])+hex(resp[17])
                    t1=f"0x{resp[18]:02x}" + f"0x{resp[17]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp6=(int(n1,16))
                    print("tx_dones is:",resp6)
                    print("\n")
                    
                    resp7=hex(resp[20])+hex(resp[19])
                    t1 = f"0x{resp[19]:02x}"
                    n1=(t1[2:4])
                    resp10=(int(n1,16))
                    print("RSSI is :",resp10)
                    bt_rssi.append(resp10)
                    print("\n")
                    
                    resp7=hex(resp[22])+hex(resp[21])
                    t1=f"0x{resp[21]:02x}" + f"0x{resp[20]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp10=(int(n1,16))
                    print("id_pkts_rcvd is :",resp10)
                    print("\n")

                    if(resp_p == 0):
                        print("PER [%]: -")
                    else:
                        print("PER [%]: ", end='')
                        print('%.3f' % (resp_f*100/(resp_p + resp_f)))
                        
                    string+=str(resp)+'\n'
                    count=0
                    resp_str=bytearray()
                    if str(resp)=="b''": 
                        count+=1
                    else:
                        resp_str=resp
                        global bt_lt
                        bt_lt.append(list(resp_str))
                self.cvs.after(1000, bt_scanning)
            except Exception as e:
                pass
        def sendcmd(cmd):
            try:
                resp=""
                response=[]
                sp.write(cmd.encode())
                count=0
                while(resp!="\n"):
                    resp=sp.readline()
                    count+=1
                    if count==31:
                        break
                    if resp.decode()=='': 
                        continue
                    if resp[0:1].decode()=='E':
                        print('Error: '+resp.decode())
                        return []
                    response.append(resp)
                return response
            except Exception as e:
                pass
        def bt_rx():
            try:
                status.config(text='Fetching BT Receive Stats')
                pattern =  r'^([A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2}-[A-F0-9]{2})$'
                if not(re.search(pattern,BDAddress.get())):
                    tk.messagebox.showinfo('Error','Enter valid BDAddress pattern')
                if not(packet_type_menu.get()):
                    tk.messagebox.showinfo('Error','Select different BR/EDR and link type values. This combination does not support any packet type.')
                else:
                    global home_bool,string,portopen, bt_rx_start
                    string=""
                    if(bt_rx_bool or home_bool or bt_rx_start):
                        home_bool= False
                        def softreset():
                            reset_data="at+rsi_reset\r\n"
                            sp.write(reset_data.encode())
                            response=sp.read(size=20)
                        softreset()
                        time.sleep(0.5)
                        sp.close()
                        time.sleep(1)
                        sp.open()
                        sp.write('|'.encode())
                        time.sleep(0.05)
                        resp =sp.read(size=20)
                        string+=str(resp.decode())
                        if(resp.decode() == 'U'):
                            sp.write('U'.encode())
                            time.sleep(0.05)
                            resp=sp.read(size=20)
                            time.sleep(1)
                            string+=str(resp.decode())
                            sp.write('1'.encode())
                            resp=sp.read(size=40)
                            string+=str(resp.decode())
                            string+=str(resp.decode())
                            count=0
                            while(resp!="\n"):
                                resp=sp.readline()
                                count+=1
                                if count==30:
                                    break
                                if resp.decode()=='':
                                    continue
                                string+=str(resp.decode())
                                if(resp.decode()=="Loading Done\r\n"):
                                    print("ABRD done\n")
                                    string+="ABRD done\n"
                                    
                                    sp.write('at+rsi_opermode=327680,0,2147483648,2147483648,2150629376,1073741824\r\n'.encode())  #1
                                    string+="at+rsi_opermode=327680,0,2147483648,2147483648,2150629376,1073741824 \n" #1
                                    for i in range(1):
                                        time.sleep(1)
                                    resp=sp.read(size=100)
                                    if not(resp[0:2].decode()=='OK'):
                                        print("Error: "+str(resp.decode()))
                                        string+="Error-opermode: "+ str(resp.decode())
                                        
                                    else:
                                        string+="Opermode set\n"
                    if(packet_type_menu.get()=="DM1"):
                        type=3
                        len=17
                    elif(packet_type_menu.get()=="DH1"):
                        type=4
                        len=27
                    elif(packet_type_menu.get()=="DM3"):
                        type=10
                        len=121
                    elif(packet_type_menu.get()=="DH3"):
                        type=11
                        len=183
                    elif(packet_type_menu.get()=="DM5"):
                        type=14
                        len=224
                    elif(packet_type_menu.get()=="DH5"):
                        type=15
                        len=339
                    elif(packet_type_menu.get()=="2-DH1"):
                        type=4
                        len=54
                    elif(packet_type_menu.get()=="2-DH3"):
                        type=10
                        len=367
                    elif(packet_type_menu.get()=="2-DH5"):
                        type=14
                        len=679
                    elif(packet_type_menu.get()=="3-DH1"):
                        type=8
                        len=83
                    elif(packet_type_menu.get()=="3-DH3"):
                        type=11
                        len=552
                    elif(packet_type_menu.get()=="3-DH5"):
                        type=15
                        len=1021
                    elif(packet_type_menu.get()=="HV1"):
                        type=5
                        len=10
                    elif(packet_type_menu.get()=="HV2"):
                        type=6
                        len=20
                    elif(packet_type_menu.get()=="HV3"):
                        type=7
                        len=30
                    elif(packet_type_menu.get()=="DV"):
                        type=8
                        len=19
                    elif(packet_type_menu.get()=="EV3"):
                        type=7
                        len=30
                    elif(packet_type_menu.get()=="2-EV3"):
                        type=6
                        len=60
                    elif(packet_type_menu.get()=="3-EV3"):
                        type=7
                        len=90
                    elif(packet_type_menu.get()=="EV4"):
                        type=12
                        len=120
                    elif(packet_type_menu.get()=="2-EV5"):
                        type=12
                        len=360
                    elif(packet_type_menu.get()=="EV5"):
                        type=13
                        len=180
                    elif(packet_type_menu.get()=="3-EV5"):
                        type=13
                        len=540
                   
                    if(self.hopping_menu.get()=='Fixed Hopping'):
                        hp_string=1
                    elif(self.hopping_menu.get()=='No Hopping'):
                        hp_string=0
                    else:
                        hp_string=2
                    
                    if(antenna.get()=="RF_PORT2/INTERNAL_ANTENNA"  or antenna.get()=='INTERNAL_ANTENNA'):
                        antenna_string=2
                    else:
                        antenna_string=3
                    
                    if(self.RFChain_menu.get()=="HP"):
                        rf_chain_string=2
                    else:
                        rf_chain_string=3
                    pll_mode=0
                    rf_type=1
                    loop_back=0
                    if(mode_menu.get()=='CW Mode'):
                        scrambler_seed=5
                    else:
                        scrambler_seed=0
                    if(loop_back_menu.get()=="0: Disable"): loopBack=0
                    else: loopBack=1
                    bt_rx_start = True
                    bt_string='at+rsibt_bredrreceive=1,'+BDAddress.get()+','+str(len)+','+str(type)+','+ BREDR_menu.get() +','+channel_menu.get()+','+channel_menu.get()+','+ link_type_menu.get() +','+str(scrambler_seed)+','+str(hp_string)+','+str(antenna_string)+','+ str(pll_mode)+','+str(rf_type)+','+str(rf_chain_string)+','+str(loopBack)+'\r\n'
                    print(bt_string)
                    string+=bt_string
                    response=sendcmd(bt_string)
                    for resp in response:
                        if not(resp[0:2].decode()=='OK'):
                            print("Error1"+str(resp.decode()))
                            string+="Error-bt_rx: "+ str(resp.decode())
                            break
                        else:
                            
                            string+="Fetching BT Receive Start \n"
                            
                            global bt_running
                            bt_running=True
                            self.cvs.after(1000, bt_scanning)
            except Exception as e:
                pass
                
        def bt_rx_stop():
            try:
                global bt_rx_bool, bt_running,string, bt_rx_start
                bt_rx_start = False
                bt_running=False
                bt_rx_bool=False
                response=sendcmd('at+rsibt_bredrreceive=0\r\n')
                string+="at+rsibt_bredrreceive=0 \n"
                print("at+rsibt_bredrreceive=0")
                for resp in response:
                    if not(resp[0:2].decode()=='OK'):
                        print("Error"+str(resp.decode()))
                        string+="Error-stop: "+str(resp.decode())
                        break
                    else:
                        #print("BT Receive Stop\n")
                        string+="BT Receive Stop\n"
                        status.config(text='BT Receive Stopped')
                
                global bt_pass, bt_fail, bt_rssi
                a,b,c=[],[],[]
                a,b,c=bt_pass, bt_fail, bt_rssi
                bt_pass, bt_fail, bt_rssi=[],[],[]
                if(len(a)==len(b) and len(a)==len(c)):
                    t=list(range(len(a)))

                p=[]
                f=[]
                for i in range(len(a)):
                    if(a[i]+b[i]  == 0):
                        p.append(0)
                        f.append(100)
                    else:
                        p.append(a[i]*100/(a[i]+b[i]))
                        f.append(b[i]*100/(a[i]+b[i]))
                
                
                fig=Figure(figsize=(7,3), dpi=100)
                ax=fig.add_subplot(111)
                
                ax.plot(t, p , c='g' ,marker=(8,2,0),ls='--',label='CRC Pass',fillstyle='none')
                ax.plot(t, f ,c='r' ,marker=(8,2,0),ls='--',label='CRC Fail')
                mplcursors.cursor()
                ax.legend(['CRC Pass%', 'CRC Fail%'])

                #ax1=fig.add_subplot(212)
                #ax1.plot(t, c ,c='b' ,marker=(8,2,0))
                #ax1.legend(['RSSI Val'])
                
                cursor = Cursor(ax, horizOn=True, vertOn=True, useblit=True,color = 'r', linewidth = 1)
                annot = ax.annotate("", xy=(0,0), xytext=(0,0),textcoords="offset points",bbox=dict(boxstyle='round4', fc='linen',ec='k',lw=1),arrowprops=dict(arrowstyle='-|>'))
                annot.set_visible(False)
                
                coord = []
                def onclick(event):
                    try:
                        coord.append((event.xdata, event.ydata))
                        x = event.xdata
                        y = event.ydata
                        annot.xy = (x,y)
                        text = "({:.2g}, {:.2g})".format(x,y)
                        annot.set_text(text)
                        annot.set_visible(True)
                        fig.canvas.draw() #redraw the figure
                    except Exception as e:
                        pass

                fig.canvas.mpl_connect('button_press_event', onclick)


                canvas = FigureCanvasTkAgg(fig, master=self.cvs)  # A tk.DrawingArea.
                canvas.draw()
                canvas.get_tk_widget().grid(row=33,columnspan=2)
                yaxis_label = tk.Label(self.cvs, text="  Duration(in seconds)  ", bg="white").grid(row=34, columnspan=2)
                
                def saveGraph():
                    try:
                        global n2
                        fig.savefig("bt_graph{}.png".format(n2))
                        n2+=1
                    except Exception as e:
                        pass
                
                save = Button(self.cvs, text="Save Graph",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveGraph)
                save.grid(column=1, row=35, pady=20,sticky='W', padx=50)
            except Exception as e:
                pass
        def delete():
            fig=Figure(figsize=(7,3), dpi=100, facecolor="#F0F0F0")
            canvas = FigureCanvasTkAgg(fig, master=self.cvs)  # A tk.DrawingArea.
            canvas.draw()
            self.dummy = canvas.get_tk_widget().grid(row=33, column=0, columnspan=2)
            self.dummy = Label(self.cvs, text="                                                  ").grid(row=34, column=0, columnspan=2)
            self.dummy = Label(self.cvs, text="                                                                                        ", height=10).grid(row=35, column=1, columnspan=2)
            self.controller.show_frame(PageOne)    
        def packetAvailability(event):
            if(link_type_menu.get()=="0" and BREDR_menu.get()=="1"):
                self.packet_type.set("DM1") # default value
                packet_type_menu['values']=["DM1", "DM3", "DM5", "HV1", "HV2", "HV3"]
            if(link_type_menu.get()=="0" and BREDR_menu.get()=="2"):
                self.packet_type.set("") # default value
                packet_type_menu['values']=[]
                tk.messagebox.showinfo('Error','Select different BR/EDR and link type values. This combination does not support any packet type.')
            if(link_type_menu.get()=="0" and BREDR_menu.get()=="3"):
                self.packet_type.set("") # default value
                packet_type_menu['values']=[]
                tk.messagebox.showinfo('Error','Select different BR/EDR and link type values. This combination does not support any packet type.')
            if(link_type_menu.get()=="1" and BREDR_menu.get()=="1"):
                self.packet_type.set("DM1") # default value
                packet_type_menu['values']=["DM1", "DH1", "DH3", "DM3", "DH5", "DM5"]
            if(link_type_menu.get()=="1" and BREDR_menu.get()=="2"):
                self.packet_type.set("2-DH1") # default value
                packet_type_menu['values']=["2-DH1", "2-DH3", "2-DH5"]
            if(link_type_menu.get()=="1" and BREDR_menu.get()=="3"):
                self.packet_type.set("3-DH1") # default value
                packet_type_menu['values']=["3-DH1", "3-DH3", "3-DH5"]
            if(link_type_menu.get()=="2" and BREDR_menu.get()=="1"):
                self.packet_type.set("DM1") # default value
                packet_type_menu['values']=["DM1", "DM3", "DM5", "EV3", "EV4", "EV5"]
            if(link_type_menu.get()=="2" and BREDR_menu.get()=="2"):
                self.packet_type.set("2-EV3") # default value
                packet_type_menu['values']=["2-EV3", "2-EV5"]
            if(link_type_menu.get()=="2" and BREDR_menu.get()=="3"):
                self.packet_type.set("3-EV3") # default value
                packet_type_menu['values']=["3-EV3", "3-EV5"]
        
        self.BDAddress = tk.StringVar(self.cvs)
        self.rf_chain = tk.StringVar(self.cvs)
        self.channel = tk.StringVar(self.cvs)
        self.packet_type = tk.StringVar(self.cvs)
        self.hopping = tk.StringVar(self.cvs)
        self.mode = tk.StringVar(self.cvs)
        self.BREDR = tk.StringVar(self.cvs)
        self.link_type = tk.StringVar(self.cvs)
        self.loop_back = tk.StringVar(self.cvs)
        
        self.rf_chain.set("HP")  # default value
        self.channel.set("0")  # default value
        self.packet_type.set("DM1")  # default value
        self.hopping.set("No Hopping")  # default value
        self.mode.set("Continuous")  # default value
        self.BREDR.set("1")  # default value
        self.link_type.set("0")  # default value
        self.loop_back.set("0: Disable")  # default value
        
        
        channel_label = ttk.Label(self.cvs, text="Channel",font = ("Times New Roman", 12))
        channel_label.grid(column = 0,row = 13, padx = 50, pady = 15,sticky='W')
        channel_menu = ttk.Combobox(self.cvs, textvariable=self.channel,width=26, values=["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34",
                                                                                 "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78"],state= "readonly")
        channel_menu.bind("<<>ComboboxSelected>")
        channel_menu.grid(column = 1, row = 13)
        
        BREDR_label = ttk.Label(self.cvs, text="BR/EDR Mode",font = ("Times New Roman", 12))
        BREDR_label.grid(column = 0,row = 14, padx = 50, pady = 15,sticky='W')
        BREDR_menu = ttk.Combobox(self.cvs, textvariable=self.BREDR,width=26, values=["1", "2"],state= "readonly")
        BREDR_menu.bind("<<ComboboxSelected>>", packetAvailability)
        BREDR_menu.grid(column = 1, row = 14)

        link_type_label = ttk.Label(self.cvs, text="Link Type",font = ("Times New Roman", 12))
        link_type_label.grid(column = 0,row = 15, padx = 50, pady = 15,sticky='W')
        link_type_menu = ttk.Combobox(self.cvs, textvariable=self.link_type,width=26, values=["0", "1", "2"],state= "readonly")
        link_type_menu.bind("<<ComboboxSelected>>", packetAvailability)
        link_type_menu.grid(column = 1, row = 15)
        
        packet_type_label = ttk.Label(self.cvs, text="Packet Type",font = ("Times New Roman", 12))
        packet_type_label.grid(column = 0,row = 16, padx = 50, pady = 15,sticky='W')
        packet_type_menu = ttk.Combobox(self.cvs, textvariable=self.packet_type,width=26, values=["DM1", "DM3", "DM5", "HV1", "HV2", "HV3"],state= "readonly")
        packet_type_menu.bind("<<>ComboboxSelected>")
        packet_type_menu.grid(column = 1, row = 16)
        
        self.hopping_label = ttk.Label(self.cvs, text="Hopping Type",font = ("Times New Roman", 12))
        self.hopping_label.grid(column = 0,row = 17, padx = 50, pady = 15,sticky='W')
        self.hopping_menu = ttk.Combobox(self.cvs, textvariable=self.hopping,width=26, values=["No Hopping", "Fixed Hopping","Random Hopping"],state= "readonly")
        self.hopping_menu.bind("<<ComboboxSelected>>")
        self.hopping_menu.grid(column = 1, row = 17)
       
        BDAddress_label = ttk.Label(self.cvs, text="BD Address",font = ("Times New Roman", 12))
        BDAddress_label.grid(column = 0,row = 18, padx = 50, pady = 15,sticky='W')
        BDAddress = ttk.Entry(self.cvs, textvariable=self.BDAddress,width=29)
        BDAddress.insert(0,"11-11-11-11-11-11")
        BDAddress.bind("<<>ComboboxSelected>")
        BDAddress.grid(column = 1, row = 18)
        
        l=tk.Label(self.cvs, text = "(Format: XX-XX-XX-XX-XX-XX)", fg="red").grid(column=0,row = 19)
        l0=tk.Label(self.cvs, text = "(BD Address should be same for Tx & Rx)", fg="red").grid(column=1,row = 19)
        
        
        mode_label = ttk.Label(self.cvs, text="Mode",font = ("Times New Roman", 12))
        mode_label.grid(column = 0,row = 20, padx = 50, pady = 15,sticky='W')
        mode_menu = ttk.Combobox(self.cvs, textvariable=self.mode,width=26, values=["Continuous", "Burst", "CW Mode"],state= "readonly")
        mode_menu.bind("<<>ComboboxSelected>")
        mode_menu.grid(column = 1, row = 20)
        
        RFChain_label = ttk.Label(self.cvs, text="RF Chain",font = ("Times New Roman", 12))
        RFChain_label.grid(column = 0,row = 21, padx = 50, pady = 15,sticky='W')
        self.RFChain_menu = ttk.Combobox(self.cvs, textvariable=self.rf_chain, width=26,values=('HP','LP'),state= "readonly")
        self.RFChain_menu.grid(column = 1, row = 21)
        
        loop_back_label = ttk.Label(self.cvs, text="Loop Back",font = ("Times New Roman", 12))
        loop_back_label.grid(column = 0,row = 22, padx = 50, pady = 15,sticky='W')
        loop_back_menu = ttk.Combobox(self.cvs, textvariable=self.loop_back,width=26, values=["0: Disable","1: Enable"],state= "readonly")
        loop_back_menu.bind("<<>ComboboxSelected>")
        loop_back_menu.grid(column = 1, row = 22)
        
        button_transmit_start = tk.Button(self.cvs, text="BT Receive Start",font = ("Times New Roman", 10, "bold"),fg='black',command=bt_rx)
        button_transmit_start.grid(column=0, row=24,pady=20,sticky='W', padx=50)
        
        button_transmit_stop = tk.Button(self.cvs, text="BT Receive Stop",font = ("Times New Roman", 10, "bold"),fg='black',command=bt_rx_stop)
        button_transmit_stop.grid(column=1, row=24,pady=20)

        save = Button(self.cvs, text="SAVE LOG",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveLog)
        save.grid(column=0, row=27, pady=20,sticky='W', padx=50)
        
        btnChange = Button(self.cvs, text="HOME", font = ("Times New Roman", 12, "bold"),fg='black', command=delete, bg="#a0ccda")
        btnChange.grid(column=1, row=27)
        status= Label(self.cvs,text='', fg="brown",font = ("Times New Roman", 10, "bold"))
        status.grid(column=0, row=28, columnspan=2)
        self.cvs.pack()

        def change_page(self):
            pass

    
    


class PageSix(ttk.Frame):

    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()

    def make_widget(self):
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        w = tk.Label(self.cvs, text="BLE TRANSMIT",fg="red",font="Times 20 bold").grid(columnspan=2,row=7,pady=10)
        def saveLog():
            try:
                global string
                name=filedialog.asksaveasfile(mode='w',defaultextension=".txt")
                ct = datetime.datetime.now()
                string= str(ct) +'\n' + string
                if not name is None: 
                    name.write(string)
                    name.close()
            except Exception as e:
                pass
        def sendcmd(cmd):
            try:
                resp=""
                response=[]
                sp.write(cmd.encode())
                count=0
                while(resp!="\n"):
                    resp=sp.readline()
                    count+=1
                    if count==31:
                        break
                    if resp.decode()=='': 
                        continue
                    if resp[0:1].decode()=='E':
                        print('Error: '+resp.decode())
                        return []
                    response.append(resp)
                return response
            except Exception as e:
                pass
                
        def ble_tx():
            try:
                status.config(text='BLE Transmit Started')
                pattern =  r'^([A-F0-9]{8})$'
                if not(re.search(pattern,AccessCode_menu.get())):
                    tk.messagebox.showinfo('Error','Enter valid Access Address pattern')
                else:
                    global home_bool,string,portopen, ble_tx_start
                    string=""
                    if(ble_tx_bool or home_bool or ble_tx_start):
                        home_bool= False
                        def softreset():
                            reset_data="at+rsi_reset\r\n"
                            sp.write(reset_data.encode())
                            response=sp.read(size=100)
                        softreset()
                        time.sleep(0.5)
                        sp.close()
                        time.sleep(1)
                        sp.open()
                        sp.write('|'.encode())
                        time.sleep(0.05)
                        resp =sp.read(size=20)
                        string+=str(resp.decode())
                        if(resp.decode() == 'U'):
                            sp.write('U'.encode())
                            time.sleep(0.05)
                            resp=sp.read(size=20)
                            time.sleep(1)
                            string+=str(resp.decode())
                            sp.write('1'.encode())
                            resp=sp.read(size=40)
                            string+=str(resp.decode())
                            count=0
                            while(resp!="\n"):
                                resp=sp.readline()
                                count+=1
                                if count==30:
                                    break
                                if resp.decode()=='':
                                    continue
                                string+=str(resp.decode())
                                if(resp.decode()=="Loading Done\r\n"):
                                    print("ABRD done\n")
                                    string+="ABRD done\n"
                                    sp.write('at+rsi_opermode=851968,0,1,2147483648,2150629376,3221225472,0,275349504,2048\r\n'.encode())  
                                    string+="at+rsi_opermode=851968,0,1,2147483648,2150629376,3221225472,0,275349504,2048 \n"
                                    for i in range(1):
                                        time.sleep(1)
                                    resp=sp.read(size=100)
                                    if not(resp[0:2].decode()=='OK'):
                                        print("Error: "+str(resp.decode()))
                                        string+="Error-opermode: "+str(resp.decode())
                                        #break
                                    else:
                                        string+="Opermode set\n"
                    if(rate_menu.get()=='1Mbps'):
                        rate_string=1
                    elif(rate_menu.get()=='2Mbps'):
                        rate_string=2
                    elif(rate_menu.get()=='125Kbps'):
                        rate_string=4
                    else:
                        rate_string=8
                    if(hopping_menu.get()=='Fixed Hopping'):
                        hopping_string=1
                    elif(hopping_menu.get()=='No Hopping'):
                        hopping_string=0
                    else:
                        hopping_string=2
                    if(antenna.get()=='RF_PORT2/INTERNAL_ANTENNA' or antenna.get()=='INTERNAL_ANTENNA'):
                        antenna_string=2
                    else: antenna_string=3
                    if(RFChain_menu.get()=='HP'): rf_chain_string=2
                    else: rf_chain_string=3
                    if(pyld_menu.get()=='PRBS9'):
                        py_type=0
                    elif(pyld_menu.get()=='0xF0'):
                        py_type=1
                    elif(pyld_menu.get()=='0xAA'):
                        py_type=2
                    elif(pyld_menu.get()=='PBRS15'):
                        py_type=3
                    elif(pyld_menu.get()=='0xFF'):
                        py_type=4
                    elif(pyld_menu.get()=='0x00'):
                        py_type=5
                    elif(pyld_menu.get()=='0x0F'):
                        py_type=6
                    elif(pyld_menu.get()=='0x55'):
                        py_type=7
                    if(mode_menu.get()=='Burst'):
                        tx_md=0
                        tx_packets=0
                    elif(mode_menu.get()=='Continuous'):
                        tx_md=1
                        tx_packets=0
                    else:
                        tx_md=2
                        tx_packets=0
                    
                    if(mode_menu.get()=='CW Mode'):
                        scrambler_seed=5
                    else:
                        scrambler_seed=0
                    if(le_channel_menu.get()=='0: Advertising Channel'):
                        le_channel_string=0
                    else:
                        le_channel_string=1
                    ble_tx_start = True
                    ble_string='at+rsibt_bletransmit=1,'+ AccessCode_menu.get()+','+str(rate_string)+','+channel_menu.get()+','+channel_menu.get()+','+str(scrambler_seed)+','+str(le_channel_string)+','+str(hopping_string)+','+str(antenna_string)+','+'0,1,'+str(rf_chain_string)+',240,'+str(py_type)+','+self.tx_power_menu.get()+','+str(tx_md)+',0,'+str(tx_packets)+'\r\n'
                    print(ble_string)
                    string+=ble_string
                    response=sendcmd(ble_string)
                    for resp in response:
                        if not(resp[0:2].decode()=='OK'):
                            print("Error"+str(resp.decode()))
                            string+="Error-ble_tx: "+str(resp.decode())
                            break
                        else:
                            string+="BLE Transmit Start\n"
            except Exception as e:
                pass
                
        def ble_tx_stop():
            global ble_tx_bool,string, ble_tx_start
            ble_tx_start = False
            ble_tx_bool=False
            ble_string='at+rsibt_bletransmit=0\r\n'
            string+="at+rsibt_bletransmit=0 \n"
            print("at+rsibt_bletransmit=0")
            response=sendcmd(ble_string)
            for resp in response:
                if not(resp[0:2].decode()=='OK'):
                    print("Error"+str(resp.decode()))
                    string+="Error-stop: "+str(resp.decode())
                    break
                else:
                    string+="BLE Transmit Stop\n"
                    status.config(text='BLE Transmit Stopped')
        
        def channelAvailability(event):
            if(le_channel_menu.get()=="0: Advertising Channel"):
                self.channel.set("37")  # default value
                channel_menu['values']=["37","38","39"]
            else:
                self.channel.set("0")  # default value
                channel_menu['values']=["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16",
                                                                                 "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36"]

        self.le_channel = tk.StringVar(self.cvs)
        self.channel = tk.StringVar(self.cvs)
        self.rate = tk.StringVar(self.cvs)
        self.mode = tk.StringVar(self.cvs)
        self.hopping = tk.StringVar(self.cvs)
        self.pyld = tk.StringVar(self.cvs)
        self.rf_chain= tk.StringVar(self.cvs)
        self.tx_power= tk.StringVar(self.cvs)
        self.AccessCode= tk.StringVar()
        
        self.le_channel.set("0: Advertising Channel")  # default value
        self.channel.set("37")  # default value
        self.rate.set("1Mbps")  # default value
        self.mode.set("Continuous")  # default value
        self.hopping.set("No Hopping")  # default value
        self.pyld.set("0x00")  # default value
        self.rf_chain.set("LP")  # default value
        self.tx_power.set("31")  # default value
        
        le_channel_label = ttk.Label(self.cvs, text="LE Channel Type",font = ("Times New Roman", 12))
        le_channel_label.grid(column = 0,row = 14, padx = 50, pady = 15,sticky='W')
        le_channel_menu = ttk.Combobox(self.cvs, textvariable=self.le_channel,width=26, values=["0: Advertising Channel","1: Data Channel"],state= "readonly")
        le_channel_menu.bind("<<ComboboxSelected>>", channelAvailability)
        le_channel_menu.grid(column = 1, row = 14)
        
        channel_label = ttk.Label(self.cvs, text="Channel",font = ("Times New Roman", 12))
        channel_label.grid(column = 0,row = 15, padx = 50, pady = 15,sticky='W')
        channel_menu = ttk.Combobox(self.cvs, textvariable=self.channel,width=26, values=["37","38","39"],state= "readonly")
        channel_menu.bind("<<>ComboboxSelected>")
        channel_menu.grid(column = 1, row = 15)
        
        rate_label = ttk.Label(self.cvs, text="Data Rate",font = ("Times New Roman", 12))
        rate_label.grid(column = 0,row = 16, padx = 50, pady = 15,sticky='W')
        rate_menu = ttk.Combobox(self.cvs, textvariable=self.rate, width=26,values=('1Mbps','2Mbps','125Kbps','500Kbps'),state= "readonly")
        rate_menu.bind("<<>ComboboxSelected>")
        rate_menu.grid(column = 1, row = 16)
        
        mode_label = ttk.Label(self.cvs, text="Mode",font = ("Times New Roman", 12))
        mode_label.grid(column = 0,row = 17, padx = 50, pady = 15,sticky='W')
        mode_menu = ttk.Combobox(self.cvs, textvariable=self.mode,width=26, values=('Burst','Continuous','CW Mode'),state= "readonly")
        mode_menu.bind("<<>ComboboxSelected>")
        mode_menu.grid(column = 1, row = 17)
        
        hopping_label = ttk.Label(self.cvs, text="Hopping Type",font = ("Times New Roman", 12))
        hopping_label.grid(column = 0,row = 18, padx = 50, pady = 15,sticky='W')
        hopping_menu = ttk.Combobox(self.cvs, textvariable=self.hopping,width=26, values=('No Hopping','Fixed Hopping','Random Hopping'),state= "readonly")
        hopping_menu.bind("<<>ComboboxSelected>")
        hopping_menu.grid(column = 1, row = 18)
        
        AccessCode_label = ttk.Label(self.cvs, text="Access Address",font = ("Times New Roman", 12))
        AccessCode_label.grid(column = 0,row = 19, padx = 50, pady = 15,sticky='W')
        AccessCode_menu = ttk.Entry(self.cvs, textvariable=self.AccessCode,width=29)
        AccessCode_menu.insert(0,"12345678")
        AccessCode_menu.bind("<<>ComboboxSelected>")
        AccessCode_menu.grid(column = 1, row = 19)
        
        l=tk.Label(self.cvs, text = "(Enter 8 Characters ONLY)", fg="red").grid(column=0,row = 20)
        l0=tk.Label(self.cvs, text = "(Access Code should be same for Tx & Rx)", fg="red").grid(column=1,row = 20)
        
        
        pyld_label = ttk.Label(self.cvs, text="Payload Type",font = ("Times New Roman", 12))
        pyld_label.grid(column = 0,row = 21, padx = 50, pady = 15,sticky='W')
        pyld_menu = ttk.Combobox(self.cvs, textvariable=self.pyld,width=26, values=('0x00','0xFF','0xF0','0x55','PRBS9','PBRS15','0x0F','0xAA'),state= "readonly")
        pyld_menu.bind("<<>ComboboxSelected>")
        pyld_menu.grid(column = 1, row = 21)
        def rf_tx(event):
            if(RFChain_menu.get() == "HP"):
                self.tx_power.set("127")  # default value
                self.tx_power_menu['values']=('127','1','2','3','4','5','6','7','8','9','10','11','12')
            else:
                self.tx_power.set("31")  # default value
                self.tx_power_menu['values']=('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30','31','33','34','35','36','37','38','39','40','41','42','43','44','45','46','47','48','49','50','51','52','53','54','55','56','57','58','59','60','61','62','63') 
        RFChain_label = ttk.Label(self.cvs, text="RF Chain",font = ("Times New Roman", 12))
        RFChain_label.grid(column = 0,row = 22, padx = 50, pady = 15,sticky='W')
        RFChain_menu = ttk.Combobox(self.cvs, textvariable=self.rf_chain,width=26, values=('HP','LP'),state= "readonly")
        RFChain_menu.bind("<<ComboboxSelected>>",rf_tx)
        RFChain_menu.grid(column = 1, row = 22)
        
        tx_power_label = ttk.Label(self.cvs, text="Transmit Power Index",font = ("Times New Roman", 12))
        tx_power_label.grid(column = 0,row = 23, padx = 50, pady = 15,sticky='W')
        self.tx_power_menu = ttk.Combobox(self.cvs, textvariable=self.tx_power,values=['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30','31','33','34','35','36','37','38','39','40','41','42','43','44','45','46','47','48','49','50','51','52','53','54','55','56','57','58','59','60','61','62','63'],width=26,state= "readonly")
        self.tx_power_menu.bind("<<ComboboxSelected>>")
        self.tx_power_menu.grid(column = 1, row = 23)
       
        button_receive_start = tk.Button(self.cvs, text="BLE Transmit Start",font = ("Times New Roman", 10, "bold"),fg='black',command=ble_tx)
        button_receive_start.grid(column=0, row=24, pady=20,sticky='W', padx=50)

        button_receive_stop = tk.Button(self.cvs, text="BLE Transmit Stop",font = ("Times New Roman", 10, "bold"),fg='black',command=ble_tx_stop)
        button_receive_stop.grid(column=1, row=24, pady=20)

        save = Button(self.cvs, text="SAVE LOG",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveLog)
        save.grid(column=0, row=27, pady=20,sticky='W', padx=50)
        
        btnChange = Button(self.cvs, text="HOME", font = ("Times New Roman", 12, "bold"),fg='black',
        command=lambda: self.controller.show_frame(PageOne),
        bg="#a0ccda")
        btnChange.grid(column=1, row=27)
        status= Label(self.cvs,text='', fg="brown",font = ("Times New Roman", 10, "bold"))
        status.grid(column=0, row=28, columnspan=2)
        self.cvs.pack()


class PageSeven(ttk.Frame):

    def __init__(self, parent, controller):
        self.controller = controller
        ttk.Frame.__init__(self, parent)
        self.make_widget()

    def make_widget(self):
        self.cvs = Canvas(self, width="725", height="700",bd=0, highlightthickness=0)
        w = tk.Label(self.cvs, text="BLE RECEIVE",fg="red",font="Times 20 bold").grid(columnspan=2,row=7,pady=10)
        def saveLog():
            try:
                global string
                name=filedialog.asksaveasfile(mode='w',defaultextension=".txt")
                ct = datetime.datetime.now()
                string= str(ct) +'\n' + string
                if not name is None: 
                    name.write(string)
                    name.close()
            except Exception as e:
                pass
        def ble_scanning():
            try:
                global string, ble_pass, ble_fail, ble_rssi
                if ble_running:
                    sp.write('at+rsibt_perstats\r\n'.encode())
                    resp=sp.read(size=100)
                    print(str(resp))

                    print("\n")
                    resp1=hex(resp[4])+hex(resp[3])
                    t1=f"0x{resp[4]:02x}" + f"0x{resp[3]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp_f=(int(n1,16))
                    print("crc_fail_count is:",resp_f)
                    ble_fail.append(resp_f)
                    print("\n")
                    
                    resp1=hex(resp[6])+hex(resp[5])
                    t1=f"0x{resp[6]:02x}" + f"0x{resp[5]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp_p=(int(n1,16))
                    ble_pass.append(resp_p)
                    print("\n")
                    
                    resp1=hex(resp[8])+hex(resp[7])
                    t1=f"0x{resp[8]:02x}" + f"0x{resp[7]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("tx_abort_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[10])+hex(resp[9])
                    t1=f"0x{resp[10]:02x}" + f"0x{resp[9]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_drop_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[12])+hex(resp[11])
                    t1=f"0x{resp[12]:02x}" + f"0x{resp[11]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_cca_idle_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[14])+hex(resp[13])
                    t1=f"0x{resp[14]:02x}" + f"0x{resp[13]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_start_idle_count is:",resp3)
                    print("\n")
                    
                    resp1=hex(resp[16])+hex(resp[15])
                    t1=f"0x{resp[16]:02x}" + f"0x{resp[15]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp3=(int(n1,16))
                    print("rx_abrt_count is:",resp3)
                    print("\n")
                    
                    resp4=hex(resp[18])+hex(resp[17])
                    t1=f"0x{resp[18]:02x}" + f"0x{resp[17]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp6=(int(n1,16))
                    print("tx_dones is:",resp6)
                    print("\n")
                    
                    resp7=hex(resp[20])+hex(resp[19])
                    t1=f"0x{resp[19]:02x}"
                    n1=(t1[2:4])
                    resp10=(int(n1,16))
                    print("RSSI is :",resp10)
                    ble_rssi.append(resp10)
                    print("\n")
                    
                    resp7=hex(resp[21])+hex(resp[20])
                    t1=f"0x{resp[21]:02x}" + f"0x{resp[20]:02x}"
                    n1=(t1[2:4]+t1[6:8])
                    resp10=(int(n1,16))
                    print("id_pkts_rcvd is :",resp10)
                    print("\n")

                    if(resp_p == 0):
                        print("PER [%]: -")
                    else:
                        print("PER [%]: ", end='')
                        print('%.3f' % (resp_f*100/(resp_p + resp_f)))
                        
                    string+=str(resp)+'\n'
                    count=0
                    resp_str=bytearray()
                    if str(resp)=="b''": 
                        count+=1
                    else:
                        resp_str=resp
                        global ble_lt
                        ble_lt.append(list(resp_str))
                self.cvs.after(1000, ble_scanning)
            except Exception as e:
                pass    
        def sendcmd(cmd):
            try:
                resp=""
                response=[]
                sp.write(cmd.encode())
                count=0
                while(resp!="\n"):
                    resp=sp.readline()
                    count+=1
                    if count==31:
                        break
                    if resp.decode()=='': 
                        continue
                    if resp[0:1].decode()=='E':
                        print('Error: '+resp.decode())
                        return []
                    response.append(resp)
                return response
            except Exception as e:
                pass
                
        def ble_rx():
            try:
                status.config(text='Fetching BLE Receive Stats')
                pattern =  r'^([A-F0-9]{8})$'
                if not(re.search(pattern,AccessCode_menu.get())):
                    tk.messagebox.showinfo('Error','Enter valid Access Address pattern')
                else:
                    global home_bool,string,portopen, ble_rx_start
                    string=""
                    if(ble_rx_bool or home_bool or ble_rx_start):
                        home_bool= False
                        def softreset():
                            reset_data="at+rsi_reset\r\n"
                            sp.write(reset_data.encode())
                            response=sp.read(size=100)
                        softreset()
                        time.sleep(0.5)
                        sp.close()
                        time.sleep(1)
                        sp.open()
                        sp.write('|'.encode())
                        time.sleep(0.05)
                        resp =sp.read(size=20)
                        string+=str(resp.decode())
                        if(resp.decode() == 'U'):
                            sp.write('U'.encode())
                            time.sleep(0.05)
                            resp=sp.read(size=20)
                            time.sleep(1)
                            string+=str(resp.decode())
                            sp.write('1'.encode())
                            resp=sp.read(size=40)
                            string+=str(resp.decode())
                            count=0
                            while(resp!="\n"):
                                resp=sp.readline()
                                count+=1
                                if count==30:
                                    break
                                if resp.decode()=='':
                                    continue
                                string+=str(resp.decode())
                                if(resp.decode()=="Loading Done\r\n"):
                                    print("ABRD done\n")
                                    string+="ABRD done\n"
                                    sp.write('at+rsi_opermode=851968,0,1,2147483648,2150629376,3221225472,0,275349504,2048\r\n'.encode())  #376012800
                                    string+="at+rsi_opermode=851968,0,1,2147483648,2150629376,3221225472,0,275349504,2048 \n"             #376012800
                                    for i in range(1):
                                        time.sleep(1)
                                    resp=sp.read(size=100)
                                    if not(resp[0:2].decode()=='OK'):
                                        print("Error: "+str(resp.decode()))
                                        string+="Error-opermode: "+str(resp.decode())
                                        
                                    else:
                                        string+="Opermode set\n"
                        
                    if(rate_menu.get()=='1Mbps'):
                        rate_string=1
                    elif(rate_menu.get()=='2Mbps'):
                        rate_string=2
                    elif(rate_menu.get()=='125Kbps'):
                        rate_string=2
                    else:
                        rate_string=8
                    
                    if(hopping_menu.get()=='Fixed Hopping'):
                        hopping_string=1
                    elif(hopping_menu.get()=='No Hopping'):
                        hopping_string=0
                    else:
                        hopping_string=2
                    if(antenna.get()=='RF_PORT2/INTERNAL_ANTENNA' or antenna.get()=='INTERNAL_ANTENNA'):
                        antenna_string=2
                    else: antenna_string=3
                    if(rf_chain_menu.get()=='HP'): rf_chain_string=2
                    else: rf_chain_string=3
                    if(mode_menu.get()=='CW Mode'):
                        scrambler_seed=5
                    else:
                        scrambler_seed=0
                    if(loop_back_menu.get()=="0: Disable"): loopBack=0
                    else: loopBack=1
                    
                    if(extDataLength_menu.get()=="0"): extDataLength_string=27
                    else: extDataLength_string=240
                    
                    if(dutyCycling_menu.get()=="0: Disable"): dutyCycling_string=0
                    else: dutyCycling_string=1
                    
                    if(le_channel_menu.get()=='0: Advertising Channel'):
                        le_channel_string=0
                    else:
                        le_channel_string=1
                    ble_rx_start = True
                    ble_string='at+rsibt_blereceive=1,'+ AccessCode_menu.get()+','+str(rate_string)+','+channel_menu.get()+','+channel_menu.get()+','+str(scrambler_seed)+','+str(le_channel_string)+','+str(hopping_string)+','+str(antenna_string)+','+'0,1,'+str(rf_chain_string)+','+str(extDataLength_string)+','+str(loopBack)+','+ str(dutyCycling_string) +'\r\n'
                    print(ble_string)
                    string+=ble_string
                    response=sendcmd(ble_string)
                    for resp in response:
                        if not(resp[0:2].decode()=='OK'):
                            print("Error"+str(resp.decode()))
                            string+="Error-ble_rx: "+str(resp.decode())
                            break
                        else:
                            string+="Fetching BLE Receive Start \n"
                            global ble_running
                            ble_running=True
                            self.cvs.after(1000, ble_scanning)
            except Exception as e:
                pass
                
        def ble_rx_stop():
            try:
                global ble_rx_bool, ble_running,string,ble_rx_start
                ble_rx_start= False
                ble_running=False
                ble_rx_bool=False
                ble_string='at+rsibt_blereceive=0\r\n'
                string+="at+rsibt_blereceive=0 \n"
                print("at+rsibt_blereceive=0")
                response=sendcmd(ble_string)
                for resp in response:
                    if not(resp[0:2].decode()=='OK'):
                        print("Error"+str(resp.decode()))
                        string+="Error-stop: "+str(resp.decode())
                        break
                    else:
                        string+="BLE Receive Stop\n"
                        status.config(text='BLE Receive Stopped')
                
                global ble_pass, ble_fail, ble_rssi
                a,b,c=[],[],[]
                a,b,c=ble_pass, ble_fail, ble_rssi
                ble_pass, ble_fail, ble_rssi=[],[],[]
                if(len(a)==len(b) and len(a)==len(c)):
                    t=list(range(len(a)))
                    
                
                p=[]
                f=[]
                for i in range(len(a)):
                    if(a[i]+b[i]  == 0):
                        p.append(0)
                        f.append(100)
                    else:
                        p.append(a[i]*100/(a[i]+b[i]))
                        f.append(b[i]*100/(a[i]+b[i]))
                
                
                fig=Figure(figsize=(7,3), dpi=100)
                ax=fig.add_subplot(111)
                
                
                ax.plot(t, p , c='g' ,marker=(8,2,0),ls='--',label='CRC Pass',fillstyle='none')
                ax.plot(t, f ,c='r' ,marker=(8,2,0),ls='--',label='CRC Fail')
                mplcursors.cursor()
                ax.legend(['CRC Pass%', 'CRC Fail%'])

                #ax1=fig.add_subplot(212)
                #ax1.plot(t, c ,c='b' ,marker=(8,2,0))
                #ax1.legend(['RSSI Val'])
                
                cursor = Cursor(ax, horizOn=True, vertOn=True, useblit=True,color = 'r', linewidth = 1)
                annot = ax.annotate("", xy=(0,0), xytext=(0,0),textcoords="offset points",bbox=dict(boxstyle='round4', fc='linen',ec='k',lw=1),arrowprops=dict(arrowstyle='-|>'))
                annot.set_visible(False)
                
                coord = []
                def onclick(event):
                    try:
                        coord.append((event.xdata, event.ydata))
                        x = event.xdata
                        y = event.ydata
                        annot.xy = (x,y)
                        text = "({:.2g}, {:.2g})".format(x,y)
                        annot.set_text(text)
                        annot.set_visible(True)
                        fig.canvas.draw() #redraw the figure
                    except Exception as e:
                        pass

                fig.canvas.mpl_connect('button_press_event', onclick)

                canvas = FigureCanvasTkAgg(fig, master=self.cvs)  # A tk.DrawingArea.
                canvas.draw()
                canvas.get_tk_widget().grid(row=33,columnspan=2)
                yaxis_label = tk.Label(self.cvs, text="  Duration(in seconds)  ", bg="white").grid(row=34, columnspan=2)

                def saveGraph():
                    try:
                        global n3
                        fig.savefig("ble_graph{}.png".format(n3))
                        n3+=1
                    except Exception as e:
                        pass
                
                save = Button(self.cvs, text="Save Graph",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveGraph)
                save.grid(column=1, row=35, pady=20,sticky='W', padx=50)
            except Exception as e:
                pass
        def channelAvailability(event):
            if(le_channel_menu.get()=="0: Advertising Channel"):
                self.channel.set("37")  # default value
                channel_menu['values']=["37","38","39"]
            else:
                self.channel.set("0")  # default value
                channel_menu['values']=["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16",
                                                                                 "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36"]
        def delete():
            fig=Figure(figsize=(7,3), dpi=100, facecolor="#F0F0F0")
            canvas = FigureCanvasTkAgg(fig, master=self.cvs)  # A tk.DrawingArea.
            canvas.draw()
            self.dummy = canvas.get_tk_widget().grid(row=33, column=0, columnspan=2)
            self.dummy = Label(self.cvs, text="                                                  ").grid(row=34, column=0, columnspan=2)
            self.dummy = Label(self.cvs, text="                                                                                      ", height=10).grid(row=35, column=1, columnspan=2)
            self.controller.show_frame(PageOne)

        self.rate = tk.StringVar(self.cvs)
        self.AccessCode =tk.StringVar(self.cvs)
        self.rf_chain = tk.StringVar(self.cvs)
        self.channel = tk.StringVar(self.cvs)
        self.le_channel = tk.StringVar(self.cvs)
        self.hopping = tk.StringVar(self.cvs)
        self.mode = tk.StringVar(self.cvs)
        self.loop_back = tk.StringVar(self.cvs)
        self.dutyCycling = tk.StringVar(self.cvs)
        self.extDataLength = tk.StringVar(self.cvs)

        self.rate.set("1Mbps")
        self.mode.set("Continuous")
        self.channel.set("37")
        self.le_channel.set("0: Advertising Channel")
        self.hopping.set("No Hopping")
        self.rf_chain.set("LP")
        self.loop_back.set("0: Disable")
        self.dutyCycling.set("0")
        self.extDataLength.set("0")
        
        le_channel_label = ttk.Label(self.cvs, text="LE Channel Type",font = ("Times New Roman", 12))
        le_channel_label.grid(column = 0,row = 14, padx = 50, pady = 15,sticky='W')
        le_channel_menu = ttk.Combobox(self.cvs, textvariable=self.le_channel,width=26, values=["0: Advertising Channel","1: Data Channel"],state= "readonly")
        le_channel_menu.bind("<<ComboboxSelected>>",channelAvailability)
        le_channel_menu.grid(column = 1, row = 14)
       
        channel_label = ttk.Label(self.cvs, text="Channel",font = ("Times New Roman", 12))
        channel_label.grid(column = 0,row = 15, padx = 50, pady = 15,sticky='W')
        channel_menu = ttk.Combobox(self.cvs, textvariable=self.channel,width=26, values=["37","38","39"],state= "readonly")
        channel_menu.bind("<<>ComboboxSelected>")
        channel_menu.grid(column = 1, row = 15)
        
        hopping_label = ttk.Label(self.cvs, text="Hopping Type",font = ("Times New Roman", 12))
        hopping_label.grid(column = 0,row =16, padx = 50, pady = 15,sticky='W')
        hopping_menu = ttk.Combobox(self.cvs, textvariable=self.hopping, width=26,values=('No Hopping','Fixed Hopping','Random Hopping'),state= "readonly")
        hopping_menu.bind("<<>ComboboxSelected>")
        hopping_menu.grid(column = 1, row = 16)
        
        rate_label = ttk.Label(self.cvs, text="Data Rate",font = ("Times New Roman", 12))
        rate_label.grid(column = 0,row = 17, padx = 50, pady = 15,sticky='W')
        rate_menu = ttk.Combobox(self.cvs, textvariable=self.rate,width=26, values=('1Mbps','2Mbps','125Kbps','500Kbps'),state= "readonly")
        rate_menu.bind("<<>ComboboxSelected>")
        rate_menu.grid(column = 1, row = 17)
        
        AccessCode_label = ttk.Label(self.cvs, text="Access Address",font = ("Times New Roman", 12))
        AccessCode_label.grid(column = 0,row = 18, padx = 50, pady = 15,sticky='W')
        AccessCode_menu = ttk.Entry(self.cvs, textvariable=self.AccessCode,width=29)
        AccessCode_menu.insert(0,"12345678")
        AccessCode_menu.bind("<<>ComboboxSelected>")
        AccessCode_menu.grid(column = 1, row = 18)
        
        l=tk.Label(self.cvs, text = "(Enter 8 Characters ONLY)", fg="red").grid(column=0,row = 19, padx = 50, pady = 15,sticky='W')
        l0=tk.Label(self.cvs, text = "(Access Code should be same for Tx & Rx)", fg="red").grid(column=1,row = 19)
        
        mode_label = ttk.Label(self.cvs, text="Mode",font = ("Times New Roman", 12))
        mode_label.grid(column = 0,row = 20, padx = 50, pady = 15,sticky='W')
        mode_menu = ttk.Combobox(self.cvs, textvariable=self.mode,width=26, values=["Continuous", "Burst", "CW Mode"],state= "readonly")
        mode_menu.bind("<<>ComboboxSelected>")
        mode_menu.grid(column = 1, row = 20)
        
        rf_chain_label = ttk.Label(self.cvs, text="Choose RF Chain ",font = ("Times New Roman", 12))
        rf_chain_label.grid(column = 0,row = 21, padx = 50, pady = 15,sticky='W')
        rf_chain_menu = ttk.Combobox(self.cvs, textvariable=self.rf_chain,width=26, values=["HP", "LP"],state= "readonly")
        rf_chain_menu.bind("<<>ComboboxSelected>")
        rf_chain_menu.grid(column = 1, row = 21)
        
        loop_back_label = ttk.Label(self.cvs, text="Loop Back ",font = ("Times New Roman", 12))
        loop_back_label.grid(column = 0,row = 22, padx = 50, pady = 15,sticky='W')
        loop_back_menu = ttk.Combobox(self.cvs, textvariable=self.loop_back,width=26, values=["0: Disable", "1: Enable"],state= "readonly")
        loop_back_menu.bind("<<>ComboboxSelected>")
        loop_back_menu.grid(column = 1, row = 22)
        
        extDataLength_label = ttk.Label(self.cvs, text="External Data Length Indicator",font = ("Times New Roman", 12))
        extDataLength_label.grid(column = 0,row = 23, padx = 50, pady = 15,sticky='W')
        extDataLength_menu = ttk.Combobox(self.cvs, textvariable=self.extDataLength,width=26, values=["0", "1"],state= "readonly")
        extDataLength_menu.bind("<<>ComboboxSelected>")
        extDataLength_menu.grid(column = 1, row = 23)
        
        dutyCycling_label = ttk.Label(self.cvs, text="Duty Cycling",font = ("Times New Roman", 12))
        dutyCycling_label.grid(column = 0,row = 24, padx = 50, pady = 15,sticky='W')
        dutyCycling_menu = ttk.Combobox(self.cvs, textvariable=self.dutyCycling,width=26, values=["0: Disable","1: Enable"],state= "readonly")
        dutyCycling_menu.bind("<<>ComboboxSelected>")
        dutyCycling_menu.grid(column = 1, row = 24)
        
        button_receive_start = tk.Button(self.cvs, text="BLE Receive Start",font = ("Times New Roman", 10, "bold"),fg='black', command=ble_rx)
        button_receive_start.grid(column=0, row=25,pady=20,sticky='W', padx=50)

        button_receive_stop = tk.Button(self.cvs, text="BLE Receive Stop",font = ("Times New Roman", 10, "bold"),fg='black',command=ble_rx_stop)
        button_receive_stop.grid(column=1, row=25,pady=20)
        
        save = Button(self.cvs, text="SAVE LOG",font = ("Times New Roman", 12, "bold"),fg='black',bg="#a0ccda",command=saveLog)
        save.grid(column=0, row=27, pady=20,sticky='W', padx=50)
        
        btnChange = Button(self.cvs, text="HOME", font = ("Times New Roman", 12, "bold"),fg='black',
        command=delete,
        bg="#a0ccda")
        btnChange.grid(column=1, row=27)
        status= Label(self.cvs,text='', fg="brown",font = ("Times New Roman", 10, "bold"))
        status.grid(column=0, row=28, columnspan=2)
        self.cvs.pack()
	
        def change_page(self):
            pass

running = True    
home_bool=False 
wlan_tx_bool=True
wlan_rx_bool=True
bt_tx_bool=True
bt_rx_bool=True
ble_tx_bool=True
ble_rx_bool=True
wlan_tx_start=False
wlan_rx_start=False
bt_tx_start=False
bt_rx_start=False
ble_tx_start=False
ble_rx_start=False

x=0
if __name__ == '__main__':
    app = MyApp()
    app.geometry('700x750')
    def resource_path1(relative_path):
        try:
            base_path = sys._MEIPASS
            
        except Exception:
            base_path = os.path.abspath(".")
        
        return os.path.join(base_path, relative_path)
    def resource_path(relative_path):
        base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
        return os.path.join(base_path, relative_path)
    

    Logo = resource_path("logo.ico")
    app.iconbitmap(Logo)
    app.title('RS9116W PHY Performance Test')
    app.resizable(0,0)
    app.mainloop()