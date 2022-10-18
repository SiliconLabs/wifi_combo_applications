# **AT Command Serial Terminal**


## About

This document provides information about the AT Command Serial Terminal application, a tool that helps in creating an opermode command, decoding an opermode command and to send commands to the evaluation board (EVB) via UART/USB-CDC interface.

## Prerequisites

This section details on the hardware and software required.

### Hardware

You should have the below mentioned hardware to run the GUI application.

1. Windows PC

2. RS911x-EVK along with a Micro USB cable (Needed only to run the third section of this application i.e. to send commands to EVB)

### Software

AT_Command_Serial_Terminal application file

## Application Architecture and Execution

The application consists of 3 independent sections,

1. Create an Opermode Command

2. Decode an Opermode Command

3. Sending commands to Evaluation Board


![introduction](resources/aintro.png)

You can select any of the three sections based on your requirement.

## Executing the GUI Application

1. Download ***AT_Command_Serial_Terminal*** application file present in ***iot-pa-wifi-at_command_serial_terminal*** folder.

   **Note:** You can download ***iot-pa-wifi-at_command_serial_terminal*** folder, to get a local copy of the readme file.
   ![folder structure](resources/bfolders.png)
**Note:** Right-click on the application and select ***Pin to taskbar*** for easier access.

   ![pin to taskbar](resources/bpin_bar.png)
   
3. The application will open as shown below.

   ![home page](resources/chome_page.png)

## Create Opermode Command

This section describes how to create an opermode command using the GUI (Graphical User Interface) of the application.

1. Click on the ***Create Opermode Command*** on the GUI Screen.

   ![home page](resources/dcreate_home.png)

2. You should select the mode of operation. The ***Opermode Command*** will get updated as you keep selecting features.

   ![create opermode page1](resources/dcreate_page2.png)

3. Next, you will be prompted to select the features under each bitmap. For example, in *Feature Bitmap*, you can select the relevant features and click on ***NEXT***.

   ![create feature bitmap](resources/dcreate_page3.png)

   **Note:** You can view a feature's description by placing cursor over it.

4. The *Opermode Command* gets updated with the *Feature Bitmap* features, and you can select the required *TCP IP Feature Bitmap* features.

   ![tcp ip feature bitmap](resources/dcreate_page4.png)

5. Similarly, you can select features related to different bitmaps.

6. Click on ***PREV*** to go to the previous page and re-select the features.

7. Once all the features are selected, you can see the framed opermode command as shown below.

   ![create opermode last page](resources/dcreate_page5.png)

8. At any stage of opermode creation, you can 
   
   a. Save the created opermode command in a computer folder of your choice, by clicking on ***Save***.

   ![save opermode](resources/ecreate_save.png)

   b.  Open a saved opermode command by clicking on ***Open*** and either view the command, or continue the creation.

   ![open saved opermode](resources/fcreate_open.png)
   **Note:** The opermode commands are saved with an extension of ***.astc*** by default indicating *AT Serial Terminal Create*.

   c. Return to Home page by clicking on *Home* 
   ![return to homepage](resources/fcreate_return.png)


## Decode Opermode Command

This section describes how the GUI decodes an opermode command.

1. Click on ***Decode Opermode Command*** on the GUI Home Screen.

   ![decode opermode home page](resources/gdecode_home.png)

2. Give the Opermode command you would like to decode in the box below the *Enter the opermode command* and Click on ***Decode***.

   ![input page decode opermode](resources/gdecode_input.png)

3. The opermode command will be decoded and you will see the features enabled in each bitmap. The below image is an example of the *WLAN_BLE power save* opermode command decoding.

   ![output page decode opermode](resources/gdecode_output.png)
## Sending Commands to Evaluation Board (EVB)

This section explains how the application can be used for sending commands to an evaluation board. Here we used RS9116 evaluation board.

1. Connect the RS9116 USB-CDC/UART Port to your PC using a Micro USB cable.

   ![setup diagram](resources/hsendcmd_setup.png)

2. Click on ***Sending Commands to EVB*** on the GUI Home Screen.

   ![sending commands home page](resources/isendcmd_home.png)

3. Select the COM Port of your EVB (here it is COM10).

   By default, Baud rate, Parity, Data bits and Stop bits are set to required values, you can edit them if required. Next, click on *OPEN*.

   ![port opening page](resources/isendcmd_openport.png)
**Note:** Please refer [troubleshoot](#troubleshoot) if you're unable to open COM port.

4. You can give the ABRD command directly through the command option available as shown below.

      ![ABRD1 sending commands](resources/isendcmd_page1.png)

5. For ABRD, after giving the command, select appropriate bootloader option and click on *SEND*. For example, below is an image reference.

      ![ABRD2 sending commands](resources/isendcmd_page2.png)


6. You can manually type each command and send to the EVB, as below.

      ![Enter Command Sending Commands](resources/isendcmd_page3.png)


7. To save the commands sent to EVB till now, click on ***Save***, choose a folder of your choice, give a name to the file and save it.

      ![Save commands](resources/isendcmd_save.png)


8. To load the saved commands, click on ***Open***.

      ![Open commands saved](resources/jsendcmd_open.png)

   **Note:** The commands sent to EVB are saved with ***.asts*** extension, which indicates *AT Serial Terminal Send*.

9. To send a command to the EVB, click on the corresponding command.

      ![Select command](resources/jsendcmd_select.png)

10. To edit the command, click on ***EDIT*** and the command will be seen in the ***Enter Command*** box. You can edit the command here and click on ***SEND***.

      ![Edit command](resources/ksendcmd_edit.png)

11. To clear the ***Communication Window***, click on ***Clear*** option in menu as shown below.
![Clear communication window](resources/lsendcmd_clear.png)

## <a id="references">References</a>

You can refer to the Program Reference Manuals available on our website, from [link](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-programming-reference-manuals/).

## <a id="troubleshoot">Troubleshoot</a>

- To trouble shoot the **Create Opermode Command** and **Decode Opermode Command**, refer the program reference manuals from https://docs.silabs.com/rs9116/wiseconnect/rs9116w-wifi-at-command-prm/latest/wlan-commands#rsi-opermode----operating-mode.

- To trouble shoot the **Sending Commands to EVB**, check the connection between PC and module. Reset the EVB and try again.
- If you're unable to open the  port, please see if the device is getting detected as serial port in your laptop *Device Manager*.
