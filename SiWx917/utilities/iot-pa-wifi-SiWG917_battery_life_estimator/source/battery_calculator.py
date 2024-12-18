#Stable till 20-11-2024
import time as t
import json
import tkinter as tk
from tkinter import ttk
from tkinter import *
import sys, os
import math
from math import gcd
from functools import reduce
from ttkbootstrap import ttk
from ttkbootstrap.constants import *
from ttkbootstrap.tooltip import ToolTip
# import messagebox from tkinter module 
import tkinter.messagebox 
from PIL import Image, ImageTk

root = tk.Tk()
root.title("SiWG917 Battery Life Calculator")
root.geometry("800x700")

'''mainframe=Frame(root,width=1155,height=700,bd=1,bg='white')
mainframe.grid_propagate(0)
mainframe.pack(anchor='center')'''

def on_mouse_wheel(event):
    canvas.yview_scroll(int(-1*(event.delta/120)), "units")
    
# Create a canvas with scrollbars
canvas = tk.Canvas(root, width=400, height=300, scrollregion=(0, 0, 800, 1400))
canvas.grid(column=0, row=0, sticky="nsew")

vscrollbar = ttk.Scrollbar(root, orient="vertical", command=canvas.yview)
vscrollbar.grid(column=1, row=0, sticky="ns")

hscrollbar = ttk.Scrollbar(root, orient="horizontal", command=canvas.xview)
hscrollbar.grid(column=0, row=1, sticky="ew")

canvas.configure(yscrollcommand=vscrollbar.set, xscrollcommand=hscrollbar.set)
canvas.bind_all("<MouseWheel>",on_mouse_wheel)

# Create a frame inside the canvas
frame_sb = tk.Frame(canvas)
canvas.create_window((0, 0), window=frame_sb, anchor='nw')

root.rowconfigure(0, weight=1)
root.columnconfigure(0, weight=1)

#root.configure(bg='#333333')  # Change the background color to a darker shade
def resource_path(relative_path):
    base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(base_path, relative_path)
    

# Set the initial theme
root.tk.call("source", resource_path("./azure.tcl"))
root.tk.call("set_theme", "light")

heading_frame = tk.Frame(frame_sb)
heading_frame.pack(pady=10)
# heading_frame.grid(column=0,row=0)

heading_label = tk.Label(heading_frame, text="SiWG917 Battery Life Calculator", font=("Helvetica", 20))
heading_label.pack()

entry_frame = tk.Frame(frame_sb)  # Match the background color of the frame to the window
entry_frame.pack(expand=True)
# entry_frame.grid(column=0,row=1)

# toggle_button = ttk.Checkbutton(button_frame,  text='Unlock', style='Toggle.TButton')
# toggle_button.pack(side="left", padx=10, pady=10)
# status_frame = tk.Frame(root)
# status_frame.pack(pady=10)
# status_label = tk.Label(status_frame, text="", font=("Helvetica", 10))
# status_label.pack()

tooltip_img= Image.open(resource_path("tooltip_2.png"))
tooltip_image= ImageTk.PhotoImage(tooltip_img)

def rb_func():
    #m4WakesupWithoutTA Conditions
    if(m4WakesupWithoutTA.get() == 1):
        m4SensingPeriodicity_label.grid(column = 0,row = 22, padx = 70, pady = 15,sticky='W')
        m4SensingPeriodicity_entry.grid(column = 1, row = 22)
        m4SensingDuration_label.grid(column = 0,row = 23, padx = 70, pady = 15,sticky='W')
        m4SensingDuration_entry.grid(column = 1, row = 23)
    else:
        m4SensingPeriodicity_label.grid_forget()
        m4SensingPeriodicity_entry.grid_forget()
        m4SensingDuration_label.grid_forget()
        m4SensingDuration_entry.grid_forget()
    
    #TAconnectedSleep Conditions
    if(TAconnectedSleep.get() == 1) or (TAconnectedSleep.get() == 3):
        TAwakeupPeriodicity_label.grid_forget()
        TAwakeupPeriodicity_entry.grid_forget()
        
        #radio_frame3.grid(column = 0,row = 28, padx = 70, pady = 15,sticky='W')

        if(TAconnectedSleep.get() == 1):
            TAlistenInterval_label.grid(column = 0,row = 26, padx = 70, pady = 15,sticky='W')
            TAlistenInterval_entry.grid(column = 1,row = 26)

            TAwlanKeepAliveInterval_label.grid(column = 0,row = 30, padx = 70, pady = 15,sticky='W')
            TAwlanKeepAliveInterval_entry.grid(column = 1,row = 30)

            TAtwtWakeInterval_label.grid_forget()
            TAtwtWakeInterval_entry.grid_forget()
            TAtwtWakeDuration_label.grid_forget()
            TAtwtWakeDuration_entry.grid_forget()
        else:
            TAtwtWakeInterval_label.grid(column = 0,row = 26, padx = 70, pady = 15,sticky='W')
            TAtwtWakeInterval_entry.grid(column = 1,row = 26)

            TAtwtWakeDuration_label.grid(column = 0,row = 27, padx = 70, pady = 15,sticky='W')
            TAtwtWakeDuration_entry.grid(column = 1,row = 27)

            TAwlanKeepAliveInterval_label.grid(column = 0,row = 30, padx = 70, pady = 15,sticky='W')
            TAwlanKeepAliveInterval_entry.grid(column = 1,row = 30)
            
            TAlistenInterval_label.grid_forget()
            TAlistenInterval_entry.grid_forget()
            #TAwlanKeepAliveInterval_label.grid_forget()
            #TAwlanKeepAliveInterval_entry.grid_forget()

        radio_frame4.grid(column = 0,row = 31, padx = 70,sticky='W', columnspan=4)
        if(TAdataTransferNeeded.get() == 1):
        
            TAsocketKeepAliveInterval_label.grid(column = 0,row = 32, padx = 70, pady = 15,sticky='W')
            TAsocketKeepAliveInterval_entry.grid(column = 1, row = 32)
            
            radio_frame5.grid(column = 0,row = 33, padx = 70, pady = 15,sticky='W', columnspan=4)
            #radio_frame6.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')

            if(TAdataTxNeeded.get() ==1):
                TAdataRxPeriodicity_label.grid_forget()
                TAdataRxPeriodicity_entry.grid_forget()
                TAdataRxPacketSize_label.grid_forget()
                TAdataRxPacketSize_entry.grid_forget()
                TAdataTxPeriodicity_label.grid_forget()
                TAdataTxPeriodicity_entry.grid_forget()
                TAdataTxPacketSize_label.grid_forget()
                TAdataTxPacketSize_entry.grid_forget()
                
            elif (TAdataTxNeeded.get() ==2):
                TAdataTxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
                TAdataTxPeriodicity_entry.grid(column = 1,row = 34)

                TAdataTxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
                TAdataTxPacketSize_entry.grid(column = 1,row = 35)

                TAdataRxPeriodicity_label.grid_forget()
                TAdataRxPeriodicity_entry.grid_forget()
                TAdataRxPacketSize_label.grid_forget()
                TAdataRxPacketSize_entry.grid_forget()
            
            elif (TAdataTxNeeded.get() ==3):
                TAdataRxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
                TAdataRxPeriodicity_entry.grid(column = 1,row = 34)

                TAdataRxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
                TAdataRxPacketSize_entry.grid(column = 1,row = 35)

                TAdataTxPeriodicity_label.grid_forget()
                TAdataTxPeriodicity_entry.grid_forget()
                TAdataTxPacketSize_label.grid_forget()
                TAdataTxPacketSize_entry.grid_forget()
                
            elif(TAdataTxNeeded.get() ==4):
                TAdataTxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
                TAdataTxPeriodicity_entry.grid(column = 1,row = 34)

                TAdataTxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
                TAdataTxPacketSize_entry.grid(column = 1,row = 35)
                
                TAdataRxPeriodicity_label.grid(column = 0,row = 36, padx = 70, pady = 15,sticky='W')
                TAdataRxPeriodicity_entry.grid(column = 1,row = 36)

                TAdataRxPacketSize_label.grid(column = 0,row = 37, padx = 70, pady = 15,sticky='W')
                TAdataRxPacketSize_entry.grid(column = 1,row = 37)
                            
        else:
            TAsocketKeepAliveInterval_label.grid_forget()
            TAsocketKeepAliveInterval_entry.grid_forget()
            
            radio_frame5.grid_forget()
            #radio_frame6.grid_forget()

            TAdataTxPeriodicity_label.grid_forget()
            TAdataTxPeriodicity_entry.grid_forget()

            TAdataTxPacketSize_label.grid_forget()
            TAdataTxPacketSize_entry.grid_forget()

            TAdataRxPeriodicity_label.grid_forget()
            TAdataRxPeriodicity_entry.grid_forget()

            TAdataRxPacketSize_label.grid_forget()
            TAdataRxPacketSize_entry.grid_forget()

        '''else:
            TAsocketKeepAliveInterval_label.grid_forget()
            TAsocketKeepAliveInterval_entry.grid_forget()
            
            radio_frame4.grid_forget()
            radio_frame5.grid_forget()
            radio_frame6.grid_forget()

            TAdataTxPeriodicity_label.grid_forget()
            TAdataTxPeriodicity_entry.grid_forget()

            TAdataRxPeriodicity_label.grid_forget()
            TAdataRxPeriodicity_entry.grid_forget()

            TAdataTxPacketSize_label.grid_forget()
            TAdataTxPacketSize_entry.grid_forget()

            TAdataRxPacketSize_label.grid_forget()
            TAdataRxPacketSize_entry.grid_forget()'''

    elif(TAconnectedSleep.get() == 2):
        TAwakeupPeriodicity_label.grid(column = 0,row = 25, padx = 70, pady = 15,sticky='W')
        TAwakeupPeriodicity_entry.grid(column = 1,row = 25)

        #radio_frame3.grid_forget()

        TAlistenInterval_label.grid_forget()
        TAlistenInterval_entry.grid_forget()
        
        TAtwtWakeInterval_label.grid_forget()
        TAtwtWakeInterval_entry.grid_forget()
        TAtwtWakeDuration_label.grid_forget()
        TAtwtWakeDuration_entry.grid_forget()

        TAwlanKeepAliveInterval_label.grid_forget()
        TAwlanKeepAliveInterval_entry.grid_forget()

        TAsocketKeepAliveInterval_label.grid_forget()
        TAsocketKeepAliveInterval_entry.grid_forget()

        TAdataTxPeriodicity_label.grid_forget()
        TAdataTxPeriodicity_entry.grid_forget()
        TAdataRxPeriodicity_label.grid_forget()
        TAdataRxPeriodicity_entry.grid_forget()

        radio_frame4.grid(column = 0,row = 31, padx = 70,sticky='W', columnspan=4)
        
        if(TAdataTransferNeeded.get() == 1):
            
            TAsocketKeepAliveInterval_label.grid_forget()
            TAsocketKeepAliveInterval_entry.grid_forget()
            
            radio_frame5.grid(column = 0,row = 33, padx = 70, pady = 15,sticky='W', columnspan=4)
            #radio_frame6.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W') #
            
            if(TAdataTxNeeded.get() ==1):
                TAdataRxPeriodicity_label.grid_forget()
                TAdataRxPeriodicity_entry.grid_forget()
                TAdataRxPacketSize_label.grid_forget()
                TAdataRxPacketSize_entry.grid_forget()
                TAdataTxPeriodicity_label.grid_forget()
                TAdataTxPeriodicity_entry.grid_forget()
                TAdataTxPacketSize_label.grid_forget()
                TAdataTxPacketSize_entry.grid_forget()
                
            elif (TAdataTxNeeded.get() ==2):
                TAdataTxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
                TAdataTxPeriodicity_entry.grid(column = 1,row = 34)

                TAdataTxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
                TAdataTxPacketSize_entry.grid(column = 1,row = 35)

                TAdataRxPeriodicity_label.grid_forget()
                TAdataRxPeriodicity_entry.grid_forget()
                TAdataRxPacketSize_label.grid_forget()
                TAdataRxPacketSize_entry.grid_forget()
            
            elif (TAdataTxNeeded.get() ==3):
                TAdataRxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
                TAdataRxPeriodicity_entry.grid(column = 1,row = 34)

                TAdataRxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
                TAdataRxPacketSize_entry.grid(column = 1,row = 35)

                TAdataTxPeriodicity_label.grid_forget()
                TAdataTxPeriodicity_entry.grid_forget()
                TAdataTxPacketSize_label.grid_forget()
                TAdataTxPacketSize_entry.grid_forget()
                
            elif(TAdataTxNeeded.get() ==4):
                TAdataTxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
                TAdataTxPeriodicity_entry.grid(column = 1,row = 34)

                TAdataTxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
                TAdataTxPacketSize_entry.grid(column = 1,row = 35)
                
                TAdataRxPeriodicity_label.grid(column = 0,row = 36, padx = 70, pady = 15,sticky='W')
                TAdataRxPeriodicity_entry.grid(column = 1,row = 36)

                TAdataRxPacketSize_label.grid(column = 0,row = 37, padx = 70, pady = 15,sticky='W')
                TAdataRxPacketSize_entry.grid(column = 1,row = 37)                    
        else:
            radio_frame5.grid_forget()
            #radio_frame6.grid_forget()

            TAdataTxPacketSize_label.grid_forget()
            TAdataTxPacketSize_entry.grid_forget()
            
            TAdataRxPacketSize_label.grid_forget()
            TAdataRxPacketSize_entry.grid_forget()
def rb_func2():
    if(int(TAmemoryRetention.get())==2):
        M4memoryRetention_drop.grid_forget()
        M4memoryRetention_label.grid_forget()
    else:
        M4memoryRetention_label.grid(column = 0,row = 4, sticky='W', pady = 15,padx=20)
        M4memoryRetention_drop.grid(column = 1,row = 4,columnspan=2,sticky='W', padx=27)
        if(int(TAmemoryConfig.get())==1):
            menu= M4memoryRetention_drop["menu"]
            menu.delete(0,"end")
            for option in ["320 KB","256 KB","192 KB","64 KB"]:
                menu.add_command(label=option,command=lambda value=option: M4memoryRetention.set(value))
            M4memoryRetention.set("320 KB")
        elif(int(TAmemoryConfig.get())==2):
            menu= M4memoryRetention_drop["menu"]
            menu.delete(0,"end")
            for option in ["256 KB","192 KB","64 KB"]:
                menu.add_command(label=option,command=lambda value=option: M4memoryRetention.set(value))
            M4memoryRetention.set("256 KB")
        elif(int(TAmemoryConfig.get())==3):
            menu= M4memoryRetention_drop["menu"]
            menu.delete(0,"end")
            for option in ["192 KB","64 KB"]:
                menu.add_command(label=option,command=lambda value=option: M4memoryRetention.set(value))
            M4memoryRetention.set("192 KB")
        
def battery_life():
    #-----------------------------------------#
    #----------Default Parameters-------------#
    #-----------------------------------------#
    global TAbeaconReceptionTime, TAbeaconReceptionCurrent, TAwlanKAtime, TAwlanKAcurrent, TAtcpKAtime, TAtcpKAcurrent
    global TAtcpTxTime, TAtcpTxCurrent, TAtcpRxTime, TAtcpRxCurrent, M4activeCurrent, RetentionSleepCurrent, WOretentionSleepCurrent
    global TAscanJoinIPconfigTime, TAscanJoinIPconfigCurrent, TAscanJoinIPconfigEnergy, bootupToInitTime, bootupToInitCurrent
    #Low Power Parameters
    TAbeaconReceptionTime= 4.4  #4.1              #msec
    TAbeaconReceptionCurrent= 9 #10.3           #mA

    TAfullBeaconReceptionTime= 7.5  #8.5          #msec
    TAfullBeaconReceptionCurrent= 15    #17.2        #mA
    
    TAtxActiveTime= 0.031   #msec
    TAtxActiveCurrent= 173  #mA
    TArxActiveTime= 2.02    #msec
    TArxActiveCurrent= 10.50  #mA                   #Review current
    
    TAwlanKAtime= 6.38  #0.03                      #msec
    TAwlanKAcurrent= 21.9   #173                   #mA

    TAtcpKAtime= 9.7                        #msec
    TAtcpKAcurrent= 33.2                    #mA

    #This data is taken for a packet of 1460 bytes TCP socket
    TAtcpTxTime= 60                         #msec
    TAtcpTxCurrent= 45                      #mA
    TAtcpRxTime= 19.1                       #msec
    TAtcpRxCurrent= 18.3                    #mA

    #M4 Low Power Parameters
    M4activeCurrent= 8                      #mA
    RetentionSleepCurrent= 0.019            #mA
    WOretentionSleepCurrent= 0.0038          #mA

    #Unconnected Powersave Parameters
    TAscanJoinIPconfigTime= 2000            #msec
    TAscanJoinIPconfigCurrent= 55           #mA
    TAscanJoinIPconfigEnergy= 0.067         #mAH

    #Without Retention Powersave Parameters
    bootupToInitTime= 3000                  #msec
    bootupToInitCurrent= 16                 #mA


    #-----------------------------------------#
    #----------Global   Variables-------------#
    #-----------------------------------------#
    global total_wifi_time, total_wifi_energy, total_m4_time, total_m4_energy, total_sleep_time, total_sleep_energy, currentConsumption
    total_wifi_time, total_wifi_energy= 0, 0
    total_m4_time, total_m4_energy=0, 0
    total_sleep_time, total_sleep_energy= 0, 0
    currentConsumption= 0                       #mA

    def lcm(a, b):
        return a * b // gcd(a, b)

    def lcm_of_list(numbers):
        return reduce(lcm, numbers)
        
    def calculate_cycle_time():
        #default cycle_time as ten_minutes
        default_cycle_time= 10*60*1000
        if (TAconnectedSleep.get() == 1):
            default_cycle_time= lcm_of_list([int(m4SensingPeriodicity_entry.get()),int(TAwlanKeepAliveInterval_entry.get())*1000,int(TAdataTxPeriodicity_entry.get())*1000,int(TAdataRxPeriodicity_entry.get())*1000,int(TAsocketKeepAliveInterval_entry.get())*1000])
        else:
            default_cycle_time= lcm_of_list([int(m4SensingPeriodicity_entry.get()),int(TAdataTxPeriodicity_entry.get())*1000,int(TAdataRxPeriodicity_entry.get())*1000])
        return default_cycle_time
        
    def calculate_battery_life(capacity, current_consumption):
        # Calculate the battery life in hours
        battery_life_hours = capacity / current_consumption
        return battery_life_hours

    def calculate_beacon_energy():
        global total_wifi_energy, total_wifi_time
             
        #Energy spent in beacon reception wakeup
        beacon_wakeup_count= cycleTime/int(TAlistenInterval_entry.get()) 
        
        full_beacon_wakeup_count= cycleTime/5000            #Full beacon for every 5 seconds
        full_beacon_wakeup_energy= TAfullBeaconReceptionTime*TAfullBeaconReceptionCurrent
        full_beacon_wakeup_total_time= full_beacon_wakeup_count*TAfullBeaconReceptionTime
        full_beacon_wakeup_total_energy= full_beacon_wakeup_count*full_beacon_wakeup_energy
        short_beacon_wakeup_count= beacon_wakeup_count-full_beacon_wakeup_count # Add for beacon missed + ((cycleTime/(60*1000))*5)
        short_beacon_wakeup_energy= TAbeaconReceptionTime*TAbeaconReceptionCurrent
        short_beacon_wakeup_total_time= short_beacon_wakeup_count*TAbeaconReceptionTime
        short_beacon_wakeup_total_energy= short_beacon_wakeup_count*short_beacon_wakeup_energy
        
        beacon_wakeup_total_time= full_beacon_wakeup_total_time+short_beacon_wakeup_total_time
        beacon_wakeup_total_energy= full_beacon_wakeup_total_energy+short_beacon_wakeup_total_energy        
        total_wifi_time+= beacon_wakeup_total_time
        total_wifi_energy+= beacon_wakeup_total_energy
        print("Cycle time:",cycleTime,"\nfb wkp count:",full_beacon_wakeup_count,"\npbwkp cout:",short_beacon_wakeup_count)

    def calculate_wlan_ka_energy():
        global total_wifi_energy, total_wifi_time
        
        #Energy spent in WLAN KA wakeup
        wlan_ka_wakeup_count= cycleTime/(int(TAwlanKeepAliveInterval_entry.get())*1000)
        wlan_ka_wakeup_energy= TAwlanKAtime*TAwlanKAcurrent
        wlan_ka_wakeup_total_time= wlan_ka_wakeup_count*TAwlanKAtime
        wlan_ka_wakeup_total_energy= wlan_ka_wakeup_count*wlan_ka_wakeup_energy
        
        total_wifi_time+= wlan_ka_wakeup_total_time
        total_wifi_energy+= wlan_ka_wakeup_total_energy
        
    def calculate_bootup_energy():
        global total_wifi_energy, total_wifi_time
        wakeup_count= cycleTime/(int(TAwakeupPeriodicity_entry.get())*1000)
        total_wifi_time+=bootupToInitTime        
        total_wifi_energy+=wakeup_count*bootupToInitTime*bootupToInitCurrent
        
    def calculate_wlan_connection_energy():
        global total_wifi_energy, total_wifi_time
        wakeup_count= cycleTime/(int(TAwakeupPeriodicity_entry.get())*1000)
        total_wifi_time+=TAscanJoinIPconfigTime        
        total_wifi_energy+=(wakeup_count*TAscanJoinIPconfigTime*TAscanJoinIPconfigCurrent)
        
    def calculate_wlan_data_transfer_energy():
        global total_wifi_energy, total_wifi_time
        if((TAdataTxNeeded.get() == 2)or(TAdataTxNeeded.get() == 4)):
            #Energy spent in data transfer wakeup
            tcp_tx_wakeup_count= cycleTime/(int(TAdataTxPeriodicity_entry.get())*1000)
            tcp_tx_packet_count= math.ceil(int(TAdataTxPacketSize_entry.get())/1460)    #Considering 1460 is the general TCP Packet size
            tcp_tx_packet_energy= TAtcpTxTime*TAtcpTxCurrent    #energy for one packet
            tcp_tx_total_energy= tcp_tx_packet_count*tcp_tx_packet_energy   #energy for one iteration of transferring configured amount of data
            total_wifi_time+= (tcp_tx_packet_count*TAtcpTxTime)*tcp_tx_wakeup_count
            total_wifi_energy+= (tcp_tx_total_energy)*tcp_tx_wakeup_count
        if((TAdataTxNeeded.get() == 3)or(TAdataTxNeeded.get() == 4)):
            #Energy spent in data transfer wakeup
            tcp_rx_wakeup_count= cycleTime/(int(TAdataRxPeriodicity_entry.get())*1000)
            tcp_rx_packet_count= math.ceil(int(TAdataRxPacketSize_entry.get())/1460)    #Considering 1460 is the general TCP Packet size
            tcp_rx_packet_energy= TAtcpRxTime*TAtcpRxCurrent
            tcp_rx_total_energy= tcp_rx_packet_count*tcp_rx_packet_energy
            total_wifi_time+= (tcp_rx_packet_count*TAtcpRxTime)*tcp_rx_wakeup_count
            total_wifi_energy+= (tcp_rx_total_energy)*tcp_rx_wakeup_count
        
    def calculate_wlan_socket_ka_energy():
        global total_wifi_energy, total_wifi_time
        
        if (TAconnectedSleep.get() == 1) or (TAconnectedSleep.get() == 3): #KA cannot be counted in unassociated sleep
            if(int(TAdataTxNeeded.get())==1):
                #Energy spent in TCP KA wakeup
                tcp_ka_wakeup_count= cycleTime/(int(TAsocketKeepAliveInterval_entry.get())*1000)
                tcp_ka_wakeup_energy= TAwlanKAtime*TAwlanKAcurrent
                tcp_ka_wakeup_total_energy= tcp_ka_wakeup_count*tcp_ka_wakeup_energy
                total_wifi_time+= tcp_ka_wakeup_count*TAwlanKAtime
                total_wifi_energy+= tcp_ka_wakeup_total_energy
                print("ka energy:",tcp_ka_wakeup_total_energy)
            elif(((int(TAsocketKeepAliveInterval_entry.get()) < int(TAdataTxPeriodicity_entry.get()))and ((TAdataTxNeeded.get() == 2)or(TAdataTxNeeded.get() == 4))) or \
            ((int(TAsocketKeepAliveInterval_entry.get()) < int(TAdataRxPeriodicity_entry.get()))and ((TAdataTxNeeded.get() == 3)or(TAdataTxNeeded.get() == 4)))):
                #Energy spent in TCP KA wakeup
                tcp_ka_wakeup_count= cycleTime/(int(TAsocketKeepAliveInterval_entry.get())*1000)
                tcp_ka_wakeup_energy= TAwlanKAtime*TAwlanKAcurrent
                tcp_ka_wakeup_total_energy= tcp_ka_wakeup_count*tcp_ka_wakeup_energy
                total_wifi_time+= tcp_ka_wakeup_count*TAwlanKAtime
                total_wifi_energy+= tcp_ka_wakeup_total_energy
    def calculate_wlan_twt_energy():
        global total_wifi_energy, total_wifi_time
        awake_prep_time= 2.6    #msec
        awake_prep_current= 21.4    #mA
        wakeup_hp_current= 48   #mA
        sleep_prep_time= 0.640  #msec
        sleep_prep_current= 14.8    #mA
        
        twt_wakeup_count= cycleTime/(int(TAtwtWakeInterval_entry.get())*1000)
        full_beacon_wakeup_count= twt_wakeup_count            #Full beacon for every 5 seconds
        full_beacon_wakeup_energy= TAfullBeaconReceptionTime*TAfullBeaconReceptionCurrent
        full_beacon_wakeup_total_time= full_beacon_wakeup_count*TAfullBeaconReceptionTime
        full_beacon_wakeup_total_energy= full_beacon_wakeup_count*full_beacon_wakeup_energy
        total_wifi_time+=full_beacon_wakeup_total_time
        total_wifi_energy+=full_beacon_wakeup_total_energy
        
        #twt_sp_calculation
        twt_sp_time= awake_prep_time+int(TAtwtWakeDuration_entry.get())+sleep_prep_time
        twt_sp_energy= (awake_prep_time*awake_prep_current)+(int(TAtwtWakeDuration_entry.get())*wakeup_hp_current)+(sleep_prep_time*sleep_prep_current)
        total_wifi_energy+=twt_sp_energy*twt_wakeup_count
        total_wifi_time+=twt_sp_time*twt_wakeup_count
        
        #WLAN KA energy
        #Energy spent in WLAN KA wakeup
        if(int(TAwlanKeepAliveInterval_entry.get())*1000)>(int(TAtwtWakeInterval_entry.get())*1000):
            wlan_ka_wakeup_count= cycleTime/(int(TAwlanKeepAliveInterval_entry.get())*1000)
        else:
            wlan_ka_wakeup_count= cycleTime/(int(TAwlanKeepAliveInterval_entry.get())*1000) #(int(TAtwtWakeInterval_entry.get())*1000)
        wlan_ka_wakeup_energy= TAtxActiveTime*TAtxActiveCurrent
        wlan_ka_wakeup_total_time= wlan_ka_wakeup_count*TAtxActiveTime
        wlan_ka_wakeup_total_energy= wlan_ka_wakeup_count*wlan_ka_wakeup_energy
        
        total_wifi_time+= wlan_ka_wakeup_total_time
        total_wifi_energy+= wlan_ka_wakeup_total_energy
        
    def calculate_wlan_twt_data_transfer_energy():
        global total_wifi_energy, total_wifi_time
        if((TAdataTxNeeded.get() == 2)or(TAdataTxNeeded.get() == 4)):
            #Energy spent in data transfer wakeup
            tcp_tx_wakeup_count= cycleTime/(int(TAdataTxPeriodicity_entry.get())*1000)
            tcp_tx_packet_count= math.ceil(int(TAdataTxPacketSize_entry.get())/1460)    #Considering 1460 is the general TCP Packet size
            tcp_tx_packet_energy= TAtcpTxTime*TAtcpTxCurrent    #energy for one packet
            tcp_tx_total_energy= tcp_tx_packet_count*tcp_tx_packet_energy   #energy for one iteration of transferring configured amount of data
            total_wifi_time+= (tcp_tx_packet_count*TAtcpTxTime)*tcp_tx_wakeup_count
            total_wifi_energy+= (tcp_tx_total_energy)*tcp_tx_wakeup_count
        if((TAdataTxNeeded.get() == 3)or(TAdataTxNeeded.get() == 4)):
            #Energy spent in data transfer wakeup
            tcp_rx_wakeup_count= cycleTime/(int(TAdataRxPeriodicity_entry.get())*1000)
            tcp_rx_packet_count= math.ceil(int(TAdataRxPacketSize_entry.get())/1460)    #Considering 1460 is the general TCP Packet size
            tcp_rx_packet_energy= TAtcpRxTime*TAtcpRxCurrent
            tcp_rx_total_energy= tcp_rx_packet_count*tcp_rx_packet_energy
            total_wifi_time+= (tcp_rx_packet_count*TAtcpRxTime)*tcp_rx_wakeup_count
            total_wifi_energy+= (tcp_rx_total_energy)*tcp_rx_wakeup_count
        
    #calculate energy spent in wi-fi activities
    def calculate_wifi_energy():  
        global total_wifi_energy, total_wifi_time
        if (TAconnectedSleep.get() == 1):
            calculate_beacon_energy()
            calculate_wlan_ka_energy()
            
        elif (TAconnectedSleep.get() == 2):
            if(int((TAwakeupPeriodicity_entry.get()))>0):
                calculate_wlan_connection_energy()
                if (TAmemoryRetention.get() == 2):  # TAmemoryRetention is False
                    calculate_bootup_energy()
            
        elif (TAconnectedSleep.get() == 3):
            calculate_wlan_twt_energy()
            
        if(TAdataTransferNeeded.get() == 1):
            calculate_wlan_socket_ka_energy()
            calculate_wlan_data_transfer_energy()
            
    def calculate_m4_energy():
        global total_m4_energy, total_m4_time
        if(m4WakesupWithoutTA.get() == 1):
            m4_alone_wakeup_count= cycleTime/int(m4SensingPeriodicity_entry.get())
            m4_alone_wakeup_current= int(m4SensingDuration_entry.get()) * M4activeCurrent
            total_m4_time+= m4_alone_wakeup_count * int(m4SensingDuration_entry.get())
            total_m4_energy+= m4_alone_wakeup_count * m4_alone_wakeup_current
        
    def calculate_sleep_energy():
        global total_sleep_time, total_sleep_energy, RetentionSleepCurrent
        
        if(M4memoryRetention.get()=="320 KB"):
            RetentionSleepCurrent= 0.019    #mA 12.9+9-2.9
        if(M4memoryRetention.get()=="216 KB"):
            RetentionSleepCurrent= 0.017    #mA 11+9-2.9
        if(M4memoryRetention.get()=="192 KB"):
            RetentionSleepCurrent= 0.015    #mA 9+9-2.9
        if(M4memoryRetention.get()=="64 KB"):
            RetentionSleepCurrent= 0.011    #mA 4.7+9-2.9
        
        
        total_sleep_time+=cycleTime-total_wifi_time-total_m4_time
        if(TAmemoryRetention.get() == 1):
            total_sleep_energy+=total_sleep_time*RetentionSleepCurrent
        else:
            total_sleep_energy+=total_sleep_time*WOretentionSleepCurrent

    def calculate_average_current():
        #Calculate overall average current consumption for the duration of cycle time
        calculate_wifi_energy()
        calculate_m4_energy()
        calculate_sleep_energy()
        total_energy= total_m4_energy+total_sleep_energy+total_wifi_energy
        return total_energy/cycleTime
        
    if(m4WakesupWithoutTA.get() == 1 and int(m4SensingPeriodicity_entry.get()) < 100):
        tkinter.messagebox.showinfo("Warning",  "Periodicity should be >= 100ms") 
    elif(m4WakesupWithoutTA.get() == 1 and int(m4SensingDuration_entry.get()) >= int(m4SensingPeriodicity_entry.get())):
        tkinter.messagebox.showinfo("Warning",  "Duration should be less than Periodicity")
    elif(TAconnectedSleep.get() == 1 and int(TAlistenInterval_entry.get()) % 100 != 0):
        tkinter.messagebox.showinfo("Warning",  "NWP Listen Interval should be multiples of 100")
    else:
        # Calculate cycle time in msec
        cycleTime= calculate_cycle_time()

        # Calculate average current in mA
        # parameters: cycleTime         //Considered as reference time for sleep-wakeup cycles
        currentConsumption= calculate_average_current()

        # Calculate battery life in hours
        battery_life_hours= calculate_battery_life(int(batteryCapacity_entry.get()), currentConsumption)
        battery_life_days= battery_life_hours/24
        battery_life_months= battery_life_days/30

        print(f"Average current consumption: {currentConsumption:.4f} mA, Expected battery life: {battery_life_hours:.3f} hours, or {battery_life_days:.2f} days, or {battery_life_months:.2f} months")
        avgCurrentConsumptionString = f"Average current consumption: {currentConsumption:.4f} mA"
        batteryLifeString = f"Expected battery life: {battery_life_months:.2f} months, or {battery_life_days:.2f} days, or {battery_life_hours:.3f} hours"
        ouput_label1.config(text = avgCurrentConsumptionString)
        ouput_label2.config(text = batteryLifeString)

radio_frame0 = tk.Frame(entry_frame)  
#radio_frame0.grid(column = 0,row = 18, padx = 50, pady = 15,sticky='W', columnspan=4)

ncp_soc = IntVar() 
ncp_soc_label = ttk.Label(radio_frame0, text="SiWx917 Variant", font = ("Helvetica", 12), width= 39)
ncp_soc_label.grid(column = 0,row = 1, sticky='W', columnspan=2)  
ncp_soc_label.grid_propagate(False)
ncp_soc_label_rb1 = ttk.Radiobutton(radio_frame0, text="SoC", variable=ncp_soc, value=1)  
ncp_soc_label_rb1.grid(column = 2,row = 1)   
ncp_soc_label_rb2 = ttk.Radiobutton(radio_frame0, text="NCP", variable=ncp_soc, value=2)  
ncp_soc_label_rb2.grid(column = 3,row = 1, padx=95)
ncp_soc.set(1)                                                        #Default value SoC

'''noOfBatteries = tk.StringVar()
noOfBatteries_label = ttk.Label(entry_frame, text="No. of Batteries",font = ("Helvetica", 12))
noOfBatteries_label.grid(column = 0,row = 19, padx = 50, pady = 15,sticky='W')
noOfBatteries_entry = ttk.Entry(entry_frame, textvariable=noOfBatteries, width=29)
noOfBatteries_entry.insert(0,"1")
noOfBatteries_entry.bind("<<>ComboboxSelected>")
noOfBatteries_entry.grid(column = 1, row = 19)
noOfBatteries_tooltip = ToolTip(noOfBatteries_label, text="This is the tooltip text!", bootstyle=(PRIMARY, INVERSE))
'''
batteryCapacity = tk.StringVar()
#batteryCapacity_label = ttk.Label(entry_frame, text="Capacity of Each Battery (in mAH)",font = ("Helvetica", 12))
batteryCapacity_label = ttk.Label(entry_frame, text="Battery Capacity (mAh)",font = ("Helvetica", 12), width= 33, image=tooltip_image, compound=RIGHT)
batteryCapacity_label.grid(column = 0,row = 20, padx = 50,sticky='W',pady=20)
batteryCapacity_entry = ttk.Entry(entry_frame, textvariable=batteryCapacity, width=29)
batteryCapacity_entry.insert(0,"2000")
batteryCapacity_entry.bind("<<>ComboboxSelected>")
batteryCapacity_entry.grid(column = 1, row = 20)
batteryCapacity_tooltip = ToolTip(batteryCapacity_label, text="Battery Capacity= Number of Batteries * Capacity of Each Battery")

radio_frame1 = tk.Frame(entry_frame)  
radio_frame1.grid(column = 0,row = 21, padx = 50, pady = 15,sticky='W', columnspan=4)

dummylabel = ttk.Label(radio_frame1, text="- - - - - - - - - - - - - - - - - - - - - Application Processor Configuration - - - - - - - - - - - - - - - - - - - - -", font = ("Helvetica", 10))
dummylabel.grid(column = 0,row = 1, columnspan=4, sticky='W', padx=100,pady=20)
dummylabel.grid_propagate(False)

TAmemoryConfig = IntVar() 
TAmemoryConfig_label = ttk.Label(radio_frame1, text="Memory Configuration", font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAmemoryConfig_label.grid(column = 0,row = 2, sticky='W', pady = 15)
TAmemoryConfig_rb1 = ttk.Radiobutton(radio_frame1, text="352-320KB", variable=TAmemoryConfig, value=1, command = rb_func2)  
TAmemoryConfig_rb1.grid(column = 1,row = 2)
TAmemoryConfig_rb2 = ttk.Radiobutton(radio_frame1, text="416-256KB", variable=TAmemoryConfig, value=2, command = rb_func2)  
TAmemoryConfig_rb2.grid(column = 2,row = 2, sticky='W',padx=5)
TAmemoryConfig_rb2 = ttk.Radiobutton(radio_frame1, text="480-192KB", variable=TAmemoryConfig, value=3, command = rb_func2)  
TAmemoryConfig_rb2.grid(column = 3,row = 2, sticky='W')
TAmemoryConfig.set(1)                                                        #Default value True
TAmemoryConfig_tooltip = ToolTip(TAmemoryConfig_label, text="M4-NWP memory configuration for SiWG917 common flash" )


TAmemoryRetention = IntVar() 
TAmemoryRetention_label = ttk.Label(radio_frame1, text="Memory Retention", font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAmemoryRetention_label.grid(column = 0,row = 3, sticky='W', pady = 15)
TAmemoryRetention_rb1 = ttk.Radiobutton(radio_frame1, text="Yes          ", variable=TAmemoryRetention, value=1, command = rb_func2)  
TAmemoryRetention_rb1.grid(column = 1,row = 3)
TAmemoryRetention_rb2 = ttk.Radiobutton(radio_frame1, text="No   ", variable=TAmemoryRetention, value=2, command = rb_func2)  
TAmemoryRetention_rb2.grid(column = 2,row = 3, sticky='W',padx=5)
TAmemoryRetention.set(1)                                                        #Default value True
TAmemoryRetention_tooltip = ToolTip(TAmemoryRetention_label, text="Complete M4 and NWP RAM retention for SiWG917" )

M4memoryRetention = StringVar() 
M4memoryRetentionOptions=["","320 KB","256 KB","192 KB","64 KB"]
M4memoryRetention_label = ttk.Label(radio_frame1, text="M4 Memory Retention", font = ("Helvetica", 12))
M4memoryRetention_label.grid(column = 0,row = 4, sticky='W', pady = 15,padx=20)
M4memoryRetention_drop = ttk.OptionMenu(radio_frame1, M4memoryRetention, *M4memoryRetentionOptions)  
M4memoryRetention_drop.grid(column = 1,row = 4,columnspan=2,sticky='W', padx=27)
M4memoryRetention.set("320 KB")                                                        #Default value True
style=ttk.Style()
style.configure('TMenubutton', background='blue', foreground='black')
M4memoryRetention_drop['menu'].config(background='white')
#M4memoryRetention_tooltip = ToolTip(M4memoryRetention_label, text="" )

m4WakesupWithoutTA = IntVar() 
m4WakesupWithoutTA_label = ttk.Label(radio_frame1, text="Application Processor Activity", font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
m4WakesupWithoutTA_label.grid(column = 0,row = 5, sticky='W',pady=15)  
m4WakesupWithoutTA_rb1 = ttk.Radiobutton(radio_frame1,text="Yes          ", variable=m4WakesupWithoutTA, value=1, command = rb_func)  
m4WakesupWithoutTA_rb1.grid(column = 1,row = 5)   
m4WakesupWithoutTA_rb2 = ttk.Radiobutton(radio_frame1, text="No", variable=m4WakesupWithoutTA, value=2, command = rb_func)  
m4WakesupWithoutTA_rb2.grid(column = 2,row = 5,sticky='W')
m4WakesupWithoutTA.set(2)                                                        #Default value True
m4WakesupWithoutTA_tooltip = ToolTip(m4WakesupWithoutTA_label, text="Example scenario: In case M4 peripheral sensing collects data for every 1 min, the collected data will be sent on-air for every 1 hour. Periodicity= 1 min, Duration= Time taken to collect data" )

m4SensingPeriodicity = tk.StringVar()
m4SensingPeriodicity_label = ttk.Label(entry_frame, text="Periodicity (msec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
m4SensingPeriodicity_label.grid(column = 0,row = 22, padx = 70, pady = 15,sticky='W')
m4SensingPeriodicity_entry = ttk.Entry(entry_frame, textvariable=m4SensingPeriodicity, width=29)
m4SensingPeriodicity_entry.insert(0,"60000")
m4SensingPeriodicity_entry.bind("<<>ComboboxSelected>")
m4SensingPeriodicity_entry.grid(column = 1, row = 22)
m4SensingPeriodicity_label.grid_forget()
m4SensingPeriodicity_entry.grid_forget()
m4SensingPeriodicity_tooltip = ToolTip(m4SensingPeriodicity_label, text="Periodicity of M4 alone wakeup" )

m4SensingDuration = tk.StringVar()
m4SensingDuration_label = ttk.Label(entry_frame, text="Duration (msec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
m4SensingDuration_label.grid(column = 0,row = 23, padx = 70, pady = 15,sticky='W')
m4SensingDuration_entry = ttk.Entry(entry_frame, textvariable=m4SensingDuration, width=29)
m4SensingDuration_entry.insert(0,"10")
m4SensingDuration_entry.bind("<<>ComboboxSelected>")
m4SensingDuration_entry.grid(column = 1, row = 23)
m4SensingDuration_label.grid_forget()
m4SensingDuration_entry.grid_forget()
m4SensingDuration_tooltip = ToolTip(m4SensingDuration_label, text="Wakeup Duration" )

radio_frame2 = tk.Frame(entry_frame)  
radio_frame2.grid(column = 0,row = 24, padx = 50, pady = 15,sticky='W', columnspan=4)

dummylabel = ttk.Label(radio_frame2, text="- - - - - - - - - - - - - - - - - - - - - Wireless Processor Configuration - - - - - - - - - - - - - - - - - - - - -", font = ("Helvetica", 10))
dummylabel.grid(column = 0,row = 1, columnspan=4, sticky='W', padx=100,pady=20)
dummylabel.grid_propagate(False)

TAconnectedSleep = IntVar() 
TAconnectedSleep_label = ttk.Label(radio_frame2, text="NWP Sleep Configuration", font = ("Helvetica", 12), width=30, image=tooltip_image, compound=RIGHT)
TAconnectedSleep_label.grid(column = 0,row = 2, sticky='W', pady = 10)  
TAconnectedSleep_rb1 = ttk.Radiobutton(radio_frame2, text="Associated", variable=TAconnectedSleep, value=1, command = rb_func)  
TAconnectedSleep_rb1.grid(column = 2,row = 2, sticky='W')   
TAconnectedSleep_rb2 = ttk.Radiobutton(radio_frame2, text="Unassociated", variable=TAconnectedSleep, value=2, command = rb_func)  
TAconnectedSleep_rb2.grid(column = 1,row = 2, sticky='W')
TAconnectedSleep_rb1 = ttk.Radiobutton(radio_frame2, text="TWT", variable=TAconnectedSleep, value=3, command = rb_func)  
TAconnectedSleep_rb1.grid(column = 3,row = 2, sticky='W')  
TAconnectedSleep.set(1)                                                        #Default value True
TAconnectedSleep_tooltip = ToolTip(TAconnectedSleep_label, text="Relates to SiWG917 Associating with an Access Point as a Wi-Fi station" )

TAwakeupPeriodicity = tk.StringVar()
TAwakeupPeriodicity_label = ttk.Label(entry_frame, text="Wakeup Periodicity (sec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAwakeupPeriodicity_label.grid(column = 0,row = 25, padx = 50, pady = 15,sticky='W')
TAwakeupPeriodicity_entry = ttk.Entry(entry_frame, textvariable=TAwakeupPeriodicity, width=29)
TAwakeupPeriodicity_entry.insert(0,"0")
TAwakeupPeriodicity_entry.bind("<<>ComboboxSelected>")
TAwakeupPeriodicity_entry.grid(column = 1, row = 25, padx = 50, pady = 15,sticky='W')
TAwakeupPeriodicity_label.grid_forget()
TAwakeupPeriodicity_entry.grid_forget()
TAwakeupPeriodicity_tooltip = ToolTip(TAwakeupPeriodicity_label, text="NWP wakeup periodicity in case of Unassociated sleep." )

TAlistenInterval = tk.StringVar()
TAlistenInterval_label = ttk.Label(entry_frame, text="Listen Interval (msec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAlistenInterval_label.grid(column = 0,row = 26, padx = 70, pady = 15,sticky='W')
TAlistenInterval_entry = ttk.Entry(entry_frame, textvariable=TAlistenInterval, width=29)
TAlistenInterval_entry.insert(0,"1000")
TAlistenInterval_entry.bind("<<>ComboboxSelected>")
TAlistenInterval_entry.grid(column = 1, row = 26)
TAlistenInterval_tooltip = ToolTip(TAlistenInterval_label, text="Duration between beacons in Associated sleep. Recommended value= 1000." )

TAwlanKeepAliveInterval = tk.StringVar()
TAwlanKeepAliveInterval_label = ttk.Label(entry_frame, text="WLAN Keep Alive Interval (sec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAwlanKeepAliveInterval_label.grid(column = 0,row = 30, padx = 70, pady = 5,sticky='W')
TAwlanKeepAliveInterval_entry = ttk.Entry(entry_frame, textvariable=TAwlanKeepAliveInterval, width=29)
TAwlanKeepAliveInterval_entry.insert(0,"30")
TAwlanKeepAliveInterval_entry.bind("<<>ComboboxSelected>")
TAwlanKeepAliveInterval_entry.grid(column = 1, row = 30)
TAwlanKeepAliveInterval_tooltip = ToolTip(TAwlanKeepAliveInterval_label, text="Duration for Keep-Alive packet transmission (Used to maintain WLAN connection)" )

TAtwtWakeInterval = tk.StringVar()
TAtwtWakeInterval_label = ttk.Label(entry_frame, text="TWT Wake Interval (sec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAtwtWakeInterval_label.grid(column = 0,row = 28, padx = 70, pady = 5,sticky='W')
TAtwtWakeInterval_entry = ttk.Entry(entry_frame, textvariable=TAtwtWakeInterval, width=29)
TAtwtWakeInterval_entry.insert(0,"60")
TAtwtWakeInterval_entry.bind("<<>ComboboxSelected>")
TAtwtWakeInterval_entry.grid(column = 1, row = 28)
TAtwtWakeInterval_tooltip = ToolTip(TAtwtWakeInterval_label, text="TWT Wake Interval" )
TAtwtWakeInterval_label.grid_forget()
TAtwtWakeInterval_entry.grid_forget()

TAtwtWakeDuration = tk.StringVar()
TAtwtWakeDuration_label = ttk.Label(entry_frame, text="TWT Wake Duration (msec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAtwtWakeDuration_label.grid(column = 0,row = 29, padx = 70, pady = 5,sticky='W')
TAtwtWakeDuration_entry = ttk.Entry(entry_frame, textvariable=TAtwtWakeDuration, width=29)
TAtwtWakeDuration_entry.insert(0,"8")
TAtwtWakeDuration_entry.bind("<<>ComboboxSelected>")
TAtwtWakeDuration_entry.grid(column = 1, row = 29)
TAtwtWakeDuration_tooltip = ToolTip(TAtwtWakeDuration_label, text="Duration of TWT Wakeup" )
TAtwtWakeDuration_label.grid_forget()
TAtwtWakeDuration_entry.grid_forget()

#radio_frame3 = tk.Frame(entry_frame)
#radio_frame3.grid(column = 0,row = 30, padx = 70, pady = 15,sticky='W', columnspan=4)

'''TAsocketConnectivityNeeded = IntVar()
TAsocketConnectivityNeeded_label = ttk.Label(radio_frame3, text="Socket Connectivity Needed", font = ("Helvetica", 12))
TAsocketConnectivityNeeded_label.grid(column = 0,row = 1, sticky='W', pady = 15)  
TAsocketConnectivityNeeded_rb1 = ttk.Radiobutton(radio_frame3, text="True", variable=TAsocketConnectivityNeeded, value=1, command = rb_func)  
TAsocketConnectivityNeeded_rb1.grid(column = 1,row = 1, padx=25)   
TAsocketConnectivityNeeded_rb2 = ttk.Radiobutton(radio_frame3, text="False", variable=TAsocketConnectivityNeeded, value=2, command = rb_func)  
TAsocketConnectivityNeeded_rb2.grid(column = 2,row = 1)
TAsocketConnectivityNeeded.set(1)                                                        #Default value True
TAsocketConnectivityNeeded_tooltip = ToolTip(TAsocketConnectivityNeeded_label, text="This is the tooltip text!" )'''

radio_frame4 = tk.Frame(entry_frame)  
radio_frame4.grid(column = 0,row = 31, padx = 70,sticky='W', columnspan=4)

TAdataTransferNeeded = IntVar() 
TAdataTransferNeeded_label = ttk.Label(radio_frame4, text="NWP Data Transfer Needed", font = ("Helvetica", 12), width=29, image=tooltip_image, compound=RIGHT)
TAdataTransferNeeded_label.grid(column = 0,row = 0, sticky='W', columnspan=2, pady=15)  
TAdataTransferNeeded_rb1 = ttk.Radiobutton(radio_frame4, text="Yes", variable=TAdataTransferNeeded, value=1, command = rb_func)  
TAdataTransferNeeded_rb1.grid(column = 2,row = 0, padx=80)   
TAdataTransferNeeded_rb2 = ttk.Radiobutton(radio_frame4, text="No", variable=TAdataTransferNeeded, value=2, command = rb_func)  
TAdataTransferNeeded_rb2.grid(column = 3,row = 0, sticky='E', padx=28)
TAdataTransferNeeded.set(1)                                                        #Default value True
TAdataTransferNeeded_tooltip = ToolTip(TAdataTransferNeeded_label, text="If selected Yes, NWP wakes up to perform the data transfer with configured periodicity" )

radio_frame5 = tk.Frame(entry_frame)  
radio_frame5.grid(column = 0,row = 33, padx = 70, pady = 15,sticky='W', columnspan=4)

TAsocketKeepAliveInterval = tk.StringVar()
TAsocketKeepAliveInterval_label = ttk.Label(entry_frame, text="Socket Keep Alive Interval (sec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAsocketKeepAliveInterval_label.grid(column = 0,row = 32, padx = 70, pady = 15,sticky='W')
TAsocketKeepAliveInterval_entry = ttk.Entry(entry_frame, textvariable=TAsocketKeepAliveInterval, width=30)
TAsocketKeepAliveInterval_entry.insert(0,"240")
TAsocketKeepAliveInterval_entry.bind("<<>ComboboxSelected>")
TAsocketKeepAliveInterval_entry.grid(column = 1, row = 32)
TAsocketKeepAliveInterval_tooltip = ToolTip(TAsocketKeepAliveInterval_label, text="Duration for Keep-Alive packet transmission (Used to maintain Socket Connectivity during connected sleep)" )

TAdataTxNeeded = IntVar() 
TAdataTxNeeded_label = ttk.Label(radio_frame5, text="Data Transfer Type", font = ("Helvetica", 12),width=36, image=tooltip_image, compound=RIGHT)
TAdataTxNeeded_label.grid(column = 0,row = 0, sticky='W', pady = 15)  
TAdataTxNeeded_rb4 = ttk.Radiobutton(radio_frame5, text="None", variable=TAdataTxNeeded, value=1, command = rb_func)  
TAdataTxNeeded_rb4.grid(column = 1,row = 0, padx=20)
TAdataTxNeeded_rb1 = ttk.Radiobutton(radio_frame5, text="Tx", variable=TAdataTxNeeded, value=2, command = rb_func)  
TAdataTxNeeded_rb1.grid(column = 2,row = 0, padx=20)   
TAdataTxNeeded_rb2 = ttk.Radiobutton(radio_frame5, text="Rx", variable=TAdataTxNeeded, value=3, command = rb_func)  
TAdataTxNeeded_rb2.grid(column = 3,row = 0, padx=20)
TAdataTxNeeded_rb3 = ttk.Radiobutton(radio_frame5, text="Both", variable=TAdataTxNeeded, value=4, command = rb_func)  
TAdataTxNeeded_rb3.grid(column = 4,row = 0, padx=20)
TAdataTxNeeded.set(1)                                                        #Default value True
TAdataTxNeeded_tooltip = ToolTip(TAdataTxNeeded_label, text="NWP Data Transfer Type" )

TAdataTxPeriodicity = tk.StringVar()
TAdataTxPeriodicity_label = ttk.Label(entry_frame, text="Data Tx Periodicity (sec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAdataTxPeriodicity_label.grid(column = 0,row = 34, padx = 70, pady = 15,sticky='W')
TAdataTxPeriodicity_entry = ttk.Entry(entry_frame, textvariable=TAdataTxPeriodicity, width=29)
TAdataTxPeriodicity_entry.insert(0,"60")
TAdataTxPeriodicity_entry.bind("<<>ComboboxSelected>")
TAdataTxPeriodicity_entry.grid(column = 1, row = 34)
#TAdataTxPeriodicity_tooltip = ToolTip(TAdataTxPeriodicity_label, text="Units: Seconds" )
TAdataTxPeriodicity_label.grid_forget()
TAdataTxPeriodicity_entry.grid_forget()

TAdataTxPacketSize = tk.StringVar()
TAdataTxPacketSize_label = ttk.Label(entry_frame, text="Data Size (bytes)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAdataTxPacketSize_label.grid(column = 0,row = 35, padx = 70, pady = 15,sticky='W')
TAdataTxPacketSize_entry = ttk.Entry(entry_frame, textvariable=TAdataTxPacketSize, width=29)
TAdataTxPacketSize_entry.insert(0,"1460")
TAdataTxPacketSize_entry.bind("<<>ComboboxSelected>")
TAdataTxPacketSize_entry.grid(column = 1, row = 35)
#TAdataTxPacketSize_tooltip = ToolTip(TAdataTxPacketSize_label, text="Units: Bytes" )
TAdataTxPacketSize_label.grid_forget()
TAdataTxPacketSize_entry.grid_forget()

radio_frame6 = tk.Frame(entry_frame)  
radio_frame6.grid(column = 0,row = 36, padx = 70, pady = 15,sticky='W', columnspan=4)

TAdataRxNeeded = IntVar() 
'''TAdataRxNeeded_label = ttk.Label(radio_frame6, text="NWP Data Rx Needed", font = ("Helvetica", 12))
TAdataRxNeeded_label.grid(column = 0,row = 0, sticky='W', pady = 15)
TAdataRxNeeded_rb1 = ttk.Radiobutton(radio_frame6, text="True", variable=TAdataRxNeeded, value=1, command = rb_func)  
TAdataRxNeeded_rb1.grid(column = 1,row = 0, padx=25)   
TAdataRxNeeded_rb2 = ttk.Radiobutton(radio_frame6, text="False", variable=TAdataRxNeeded, value=2, command = rb_func)  
TAdataRxNeeded_rb2.grid(column = 2,row = 0)
TAdataRxNeeded.set(2)                                                        #Default value True
TAdataRxNeeded_tooltip = ToolTip(TAdataRxNeeded_label, text="This is the tooltip text!" )'''

TAdataRxPeriodicity = tk.StringVar()
TAdataRxPeriodicity_label = ttk.Label(entry_frame, text="Data Rx periodicty (sec)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAdataRxPeriodicity_label.grid(column = 0,row = 37, padx = 70, pady = 15,sticky='W')
TAdataRxPeriodicity_entry = ttk.Entry(entry_frame, textvariable=TAdataRxPeriodicity, width=29)
TAdataRxPeriodicity_entry.insert(0,"60")
TAdataRxPeriodicity_entry.bind("<<>ComboboxSelected>")
TAdataRxPeriodicity_entry.grid(column = 1, row = 37)
#TAdataRxPeriodicity_tooltip = ToolTip(TAdataRxPeriodicity_label, text="Units: Seconds" )
TAdataRxPeriodicity_label.grid_forget()
TAdataRxPeriodicity_entry.grid_forget()

TAdataRxPacketSize = tk.StringVar()
TAdataRxPacketSize_label = ttk.Label(entry_frame, text="Data Size (bytes)",font = ("Helvetica", 12), image=tooltip_image, compound=RIGHT)
TAdataRxPacketSize_label.grid(column = 0,row = 38, padx = 70, pady = 15,sticky='W')
TAdataRxPacketSize_entry = ttk.Entry(entry_frame, textvariable=TAdataRxPacketSize, width=29)
TAdataRxPacketSize_entry.insert(0,"1460")
TAdataRxPacketSize_entry.bind("<<>ComboboxSelected>")
TAdataRxPacketSize_entry.grid(column = 1, row = 38)
#TAdataRxPacketSize_tooltip = ToolTip(TAdataRxPacketSize_label, text="Units: Bytes" )
TAdataRxPacketSize_label.grid_forget()
TAdataRxPacketSize_entry.grid_forget()

button_frame = tk.Frame(frame_sb)  # Match the background color of the frame to the window
button_frame.pack(expand=True)
# button_frame.grid(column=0, row=2)

generate_button = ttk.Button(button_frame,  text='CALCULATE', style='Outline.TButton', command = battery_life)      #fg='blue',
generate_button.pack(padx=10, pady=10)

ouput_label1 = ttk.Label(button_frame, text="",font = 'Helvetica 12 bold')   #Silabs Red: fg='#d91e2a'
ouput_label1.pack(pady=10)
ouput_label1.configure(foreground='#007fff')
ouput_label2 = ttk.Label(button_frame, text="", font = 'Helvetica 12 bold')   #Silabs Red: fg='#d91e2a'
ouput_label2.pack(pady=10)
ouput_label2.configure(foreground='#007fff')

Logo = resource_path("logo.ico")
root.iconbitmap(Logo)
root.resizable(0,0)
root.mainloop()