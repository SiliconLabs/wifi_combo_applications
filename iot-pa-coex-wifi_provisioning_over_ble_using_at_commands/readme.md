<h1> Wi-Fi Provisioning over BLE using AT commands</h1>

## About

This project helps the user to securely send Wi-Fi network credentials from a Provisioning Tool to RS9116 NCP module over BLE using AT commands via UART/USB-CDC interface with PC as the Host Platform.
<br><br>

## Terminologies

   1.  WLAN    : Wireless Local Area Network

   2.  BLE    : Bluetooth Low Energy

   3.  EVK    : Evaluation Kit

   4.  GUI    : Graphical User Interface

   5.  AP      : Access Point
<br><br>

## Application Design Flow

This section demonstrates the Services and Characteristics created in the project to exchange data between RS9116 NCP module and Provisioning App.

###  **Services & Characteristics**

   A customized service, **0xAABB** is created for communication between RS9116 NCP module and Provisioning App. Below are the **Characteristics** defined for the 0xAABB service:

1. **1AA1**

   *Definition*: This characteristic is used by Provisioning App to send commands to the RS9116 NCP module.

   *Property*: This characteristic has **Write with response** property.

2. **1BB1**

   *Definition*: This characteristic is used by RS9116 NCP module to respond to the Provisioning App.

   *Property*: This characteristic has **Read** and **Write with response** properties.

3. **1CC1**

   *Definition*: This characteristic is used by RS9116 NCP module to notify list of APs to the Provisioning App.

   *Property*: This characteristic has **Read**, **Write with response** and **Notification** properties.
<br><br>
## **Communication between RS9116 NCP module & Provisioning App**

   The Provisioning tool sends the following Commands on 1AA1 characteristic while communicating with the RS9116 NCP module:

| Command ID | Command Type | Description (Data exchange between RS9116 NCP & Provisioning App) |
|------------|--------------|---------------------------------------------------------------|
| 8 | Firmware query | This Command is used to get the firmware version. |
| 7 | WLAN Status | This Command is used to know whether RS9116 NCP module is already connected to an AP or not. |
| 3 | WLAN Scan | This Command request is used to get the list of APs available nearby. |
| 5 | Security Type | This Command is used to get the security type of the AP. Below are the supported security types:<br>1. Open →0 <br>2. WPA → 1<br>3. WPA2 →2<br>4. WPA/WPA2 Mixed →6<br>**Note**:  If “security type” is “0”, the Provisioning Tool directly issues join command instead of requesting the password. |
| 6 | PSK | This Command requests to enter the password of the network (AP) to connect to. |
| 2 | Join | This Command is used for joining the RS9116 NCP module to a network (AP). |
| 4 | WLAN Disconnect | This Command disconnects the RS9116 NCP module with the AP. |

### **Message Sequence Chart**

Below figure explains about the data communication between RS9116 NCP module and Provisioning App:<br>
<img src="resources/image1.png" alt="" width=600><br>

## Prerequisites

This section details on the hardware and software required.

### **Hardware**

You should have the below mentioned hardware to run the GUI application.

1. Windows PC
    <img src="resources/image2.png" alt="" width=600>

2. RS9116 NCP EVK along with a Micro USB cable

   <img src="resources/image3.png" alt="" width=600>

3. Android Mobile

4. Access Point

### **Software**

The below mentioned software is required for the GUI application.

1. Programming Language: Python 3.7.9

- Python packages: Tkinter, pillow, pyserial
<br><br>

## Python Installation

To execute the AT command application, python and its packages need to be installed. However, skip this section if the application is being executed directly from.exe file.

 The python packages required are:
   1. Tkinter
   2. Pyserial
   3. Pillow

**Note:** Make sure python default version is **3.7.9** and pip is installed.<br><br>
  *Steps to install python and its packages:*

1. Run the batch file **Installation.bat**. Refer to the below images, showing the installation log.<br>
<img src="resources/image5.png" alt=""><br>
<img src="resources/image42.png" alt=""><br>

2. While **Python 3.7.9** is being installed, a pop-up will appear. Click on **Add Python 3.7 to PATH** checkbox, followed by **Install Now** button.<br><br>
<img src="resources/image10.png" alt="" width=30%><br><br>
 Following image can be referred after clicking **Install Now** button:<br>
   <img src="resources/image31.png" alt="" width=30%><br>

   **Note**: The **Installation.bat** file will automatically install the **pyserial** and **pillow** packages.

<br><br>

## Project Execution

To run EFR Connect Application as the Provisioning Tool, follow the below steps:

1. Install **EFR Connect** Application from Play Store / App Store.

2. Run the **EFR Connect** Application. The following interface will appear:<br><br>
<img src="resources/image14.jpeg"  alt=""><br>

3. If BLE is not enabled, enable using the following option:<br><br>
<img src="resources/image144.jpg"  alt="" ><br>


4. Switch to Demo Section as shown below:<br><br>
<img src="resources/3.jpg" alt="" width=30% height=30%><br>

5. Connect RS9116 NCP module with the PC using a micro-USB cable either in **USB-CDC** or **UART** interface.<br><br>
<img src="resources/image15.png" alt="" ><br>


6. Execute the application in any one of the following ways:

  - Run **WiFi\_Provisioning\_over\_BLE\_AT\_app.exe** file

                        OR 

  - Run the batch file (**Application.bat**).

                        OR 

  - Open command prompt at the location where the python script is stored. Run the command: `python WiFi_Provisioning_over_BLE_AT_app.py`

  **Note:** For second and third method, keep all the associated images in the same folder.<br>

7. Select the desired **Interface** and **Port** (if multiple available). Click OK.<br>
<img src="resources/image17.png" alt="" ><br><br>
The Desktop GUI will appear as shown below:<br>
   <img src="resources/image18.png" alt="">

8. Select **Wi-Fi Commissioning** from the list of available options in the **EFR Connect** mobile Application as shown below:<br><br>
<img src="resources/image32.png" alt="" ><br><br>

9. Select **BLE_CONFIGURATOR** from the list of available options:<br>
<img src="resources/image20.jpeg" alt=""><br><br>
Once the provisioning tool is connected to the RS9116 NCP module, the Desktop GUI will be updated with the device MAC address.<br><br>
<img src="resources/image21.png" alt="" ><br><br>

10. The WLAN scan results will be fetched to the EFR Connect App as shown below. Wait unit all the SSIDs are updated on the EFR Connect App else some delay might be observed.<br><br>
<img src="resources/image22.jpeg" alt="" ><br><br>

11. Select the desired AP and enter the password (if any). Click on **Connect**.<br>
<img src="resources/image23.jpeg" alt="" ><br><br>

12. The password will be sent to the RS9116 NCP module. If correct credentials are provided, the module will be connected to the AP and WLAN MAC address and the IP address will be sent to the **EFR Connect** Application. The Desktop GUI will appear as shown below:<br><br>
<img src="resources/image24.png" alt=""><br><br>

13. The **EFR Connect** Application will be updated with a green symbol highlighted across the selected AP.<br><br>
<img src="resources/image25.jpeg" alt="" ><br><br>

14. For WLAN disconnection from the AP, select the connected AP in the **EFR Connect** Application and click **Yes**.
<br><br><img src="resources/image26.jpeg" alt=""><br><br>


## Troubleshooting

1. Make sure Bluetooth is enabled on the Windows PC / Laptop.

2. If using python script, make sure default python version should be **3.7.9**. Please enter the below command to verify the python version in command prompt: **python - -version**

3. Choose the correct interface (UART/USB-CDC) in the GUI as RS9116 NCP is connected to.

4. If any error occurs, terminate the application, reset the RS9116 NCP module, and re-run the application. **ERROR 4D09** typically occurs when RS9116 NCP module is not reset.

5. If any error comes saying **serial.tools.list\_ports.comports()** not found or anything related to **Serial** package (occurs when python script is run manually instead of using exe file), then follow the steps:
   - Uninstall serial (if it is installed), type: **pip uninstall serial** in the command prompt.
   - Upgrade pyserial, type: **pip install –upgrade –force-reinstall pyserial** in the command prompt.

6. If "Module not Responding" message pops up again and again, flash the the latest firmware in the RS9116 NCP module.

<br><br>

## Limitations
   1. Maximum 11 APs can be displayed at a time.

   2. Length of password should be greater than 7 and less than or equal to 17.

   3. If a password of length \> 17 is entered in EFR Connect App, it stops responding.

   4. The RS9116 NCP radio is configured only for 2.4 GHz band.

   5. The RS9116 NCP module works as a normal client that can connect to an Access Point with different security modes other than enterprise security.

