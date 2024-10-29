# **BLE OTA Firmware Upgrade**

## Table of Contents
<!-- TOC -->

- [Introduction](#1-introduction)
- [Prerequisites](#2-prerequisites)
  - [Hardware Requirements](#21-hardware-requirements)
  - [Software Requirements](#22-software-requirements)
- [Setup Diagram](#3-setup-diagram)
- [Project Creation](#4-project-creation)
  - [SiWx917 Project](#41-siwx917-project)
  - [SiWx917 SoC Project](#42-siwx917-SoC-project)
  - [SiWx917 NCP Project](#43-siwx917-NCP-project)
- [Application Configuration](#5-application-configuration)
- [Testing the Application](#6-testing-the-application)
  - [Build the Application](#61-build-the-application)
  - [Application execution](#62-application-execution)
  - [Firmware Upgrade format change](#63-firmware-upgrade-format-change)
  - [Firmware Upgrade with EFR Connect Mobile App](#64-firmware-upgrade-with-efr-connect-mobile-app)
  - [TA Firmware Upgrade](#641-ta-firmware-upgrade)
  - [M4 Firmware Upgrade](#642-m4-firmware-upgrade)
  - [Firmware Upgrade with Python script](#65-firmware-upgrade-with-python-script)
   - [TA Firmware Upgrade](#651-ta-firmware-upgrade)
  - [M4 Firmware Upgrade](#652-m4-firmware-upgrade)
- [Appendix](#7-appendix)


<!-- /TOC -->

## **1. Introduction**

This application demonstrates the procedure to update the SiWx91x module's firmware by receiving a firmware file from a remote device via BLE. The application has the provision to upgrade both TA and M4 firmwares. 
The update process works as follows:
   - The SiWx91x module is configured as an advertiser, and an OTA GATT database acts as a GATT server.
   - The GUI/EFR Connect App is configured as a BLE central and GATT Client.
   - When the BLE connection is established between the SiWx91x device, you can browse either M4 or TA file from the GUI/EFR Connect App.
   - The GUI/EFR Connect App sends the selected firmware, either M4 firmware or TA firmware file.
   - The OTA application programs the firmware onto the SiWx91x flash memory and reboots it.

The application has the provision to upgrade the firmware with below tools
- EFR Connect Mobile example
- Si917-OTA Firmware Update Python Script
- Python script

## **2. Prerequisites**

The user must configure the following items before launching the application.

### 2.1 Hardware Requirements
- A smart phone with Silicon labs **EFR Connect** app
- Windows PC
#### **2.1.1 SoC** 

   - Silicon Labs SiWx917 PK6031A SoC Kit which includes
      - BRD4002A Wireless Starter Kit Mainboard
      - BRD4338A Radio Board
   
#### **2.1.2 NCP**

   - Silicon Labs BRD4346A radio board 
   - [Silicon Labs SLWSTK6006A EFR32xG21 Wireless Starter Kit](https://www.silabs.com/development-tools/wireless/efr32xg21-wireless-starter-kit) which includes
      - BRD4002A Wireless Starter Kit Mainboard
      - BRD4180A/BRD4180B Radio Board


### 2.2 Software Requirements

- SiWx917_WiSeConnect_SDK [link](https://github.com/SiliconLabs/wiseconnect)
- Simplicity Studio IDE
  - To download and install the Simplicity Studio IDE, refer to the [Simplicity Studio IDE Set up](https://docs.silabs.com/) section in ***Getting started with SiWx91x*** guides.
- EFR Connect application for [Android](https://play.google.com/store/apps/details?id=com.siliconlabs.bledemo&hl=en&gl=US) or [iOS](https://apps.apple.com/us/app/efr-connect-ble-mobile-app/id1030932759)
- Docklight or Tera Term or a similar tool for reading serial prints

- Python version 3.7.9 or above

**Note:**
The application can be evaluated from the 3.1.1 SDK & Firmware. But it is recommended to use the latest SDK & Firmware available on the website.

## **3. Setup Diagram**
- This section describes the setup diagram of the BLE OTA application with EFR Connect mobile App.
   <br><img src="resources/readme/setup.png" alt=""><br>

- Follow the "Hardware connections and Simplicity Studio IDE Set up" section in the respective [Getting Started with SiWx91x](https://docs.silabs.com/wiseconnect/3.0.9/wiseconnect-getting-started/getting-started-with-soc-mode) guides to make the hardware connections and add the Gecko and SiWx91x WiFi SDK to the Simplicity Studio IDE.

- Ensure the SiWx91x module is loaded with the latest firmware following the [SiWx91x Firmware Update](https://docs.silabs.com/rs9116/latest/wiseconnect-getting-started) section in the respective Getting started with SiWx91x*** guides. Please note that the current project was tested on SiWx917_WiSeConnect_SDK.3.1.1(SiWG917-B.2.10.0.0.4.rps).

## **4. Project Creation**

### 4.1 SiWx917 Project

- Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

  - Install Studio and WiSeConnect 3 extension
  - Connect your device to the computer
  - Upgrade your connectivity firmware
  - Create a Studio project

- Download the project **iot-pa-ble-ota_fwup_efr_connect** from the GitHub [link](https://github.com/SiliconLabs/wifi_combo_applications/tree/master/SiWx917)
- Keep the project in the Release folder at location: `SDK/examples/snippets/ble/`
- Open **templates.xml** file located at `SDK/templates.xml` and add the code snippet at the end, before `</model:MDescriptors>`:

```sh
 <descriptors name="ble_ota_fwup_efr_connect_soc" label="BLE - Firmware update with EFR Connect(SoC)" description="Wirelessly updates the Si91x firmware either TA or M4 from a connected remote device">
    <properties key="namespace" value="template.uc"/>
    <properties key="keywords" value="universal\ configurator"/>
    <properties key="projectFilePaths" value="examples/snippets/ble/iot-pa-ble-ota_fwup_efr_connect/ble_ota_fwup_efr_connect_soc.slcp"/>
    <properties key="boardCompatibility" value="com.silabs.board.none brd4325a brd4325b brd4325c brd4325g brd4338a"/>
    <properties key="partCompatibility" value="mcu.arm.efr32.mg21.* mcu.arm.efr32.mg21.* .*si917.* .*si917.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m141xgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.*"/>
    <properties key="ideCompatibility" value="makefile-ide simplicity-ide"/>
    <properties key="toolchainCompatibility" value="gcc"/>
    <properties key="quality" value="alpha"/>
    <properties key="category" value="Example|Wi-Fi"/>
    <properties key="stockConfigCompatibility" value="com.silabs.ss.framework.project.toolchain.core.default"/>
    <properties key="sdkAndProtocolTags" value=""/>
    <properties key="readmeFiles" value="examples/featured/firmware_update/readme.md"/>
	<properties key="filters" value="Device\ Type|SoC Wireless\ Technology|Bluetooth Project\ Difficulty|Beginner"/>
  </descriptors>
  <descriptors name="ble_ota_fwup_efr_connect_ncp" label="BLE - Firmware update with EFR Connect (NCP)" description="Wirelessly updates the Si91x firmware either TA M4 from a connected remote device.">
    <properties key="namespace" value="template.uc"/>
    <properties key="keywords" value="universal\ configurator"/>
    <properties key="projectFilePaths" value="examples/snippets/ble/iot-pa-ble-ota_fwup_efr_connect/ble_ota_fwup_efr_connect_ncp.slcp"/>
    <properties key="boardCompatibility" value="com.silabs.board.none brd4180a brd4180b brd4186c"/>
    <properties key="partCompatibility" value="mcu.arm.efr32.mg21.* mcu.arm.efr32.mg21.*  mcu.arm.efr32.mg24.* .*si917.* .*si917.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m141xgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917y111mgab.* .*siwg917y111mgnb.*"/>
    <properties key="ideCompatibility" value="makefile-ide simplicity-ide visual-studio-code generic-template"/>
    <properties key="toolchainCompatibility" value="gcc"/>
    <properties key="quality" value="production"/>
    <properties key="category" value="Example|Wi-Fi"/>
    <properties key="stockConfigCompatibility" value="com.silabs.ss.framework.project.toolchain.core.default"/>
    <properties key="sdkAndProtocolTags" value=""/>
    <properties key="readmeFiles" value="examples/snippets/ble/ble_ae_peripheral/readme.md"/>
	<properties key="filters" value="Device\ Type|NCP Wireless\ Technology|Bluetooth Project\ Difficulty|Advanced"/>
  </descriptors>
   ```

- Refresh the Gecko SDK.
<br><img src="resources/readme/importproject.png" alt=""><br>

### 4.2 SiWx917 SoC Project

- Connect the SiWx917 SoC kit and create the project.
<br><img src="resources/readme/importsi917.png" alt=""><br>

### 4.3 SiWx917 NCP Project
- Connect the SiWx917 SoC kit and create the project.
<br><img src="resources/readme/importsi917ncp.png" alt=""><br>

## **5. Application Configuration**

The application can be configured to suit you requirements and development environment. Go through the following sections and make any changes if needed.

- In the Project explorer pane of the IDE, expand the **ble\_ota\_fwup\_efr\_connect\_soc** folder and open the **ble\_config.h** file. 
   ```c
   //! RSI_BLE_OTA_FWUP_PROFILE Refer to the SiWx91x module name
   #define RSI_BLE_OTA_FWUP_PROFILE "BLE_OTA_FWUP"

   //FW_UPGRADE_TYPE Refer to the SiWx91x module firmware upgrade type either "TA_FW_UP" or "M4_FW_UP".
   #define FW_UPGRADE_TYPE  TA_FW_UP
   ```
   <br><img src="resources/readme/applicationconfiguration.png" alt=""><br>


## **6. Testing the Application**

This section outlines the application's step-by-step execution process.

### **6.1 Build the Application**

- After creating the project, either right-click on it and choose "Build Project" or click the build icon (a hammer) to begin building the project.
   <br><img src="resources/readme/projectbuild.png" alt=""><br>


## **6.2 Application execution**

1. After the program gets executed, The Silicon Labs device is configured as a peripheral device and the OTA GATT server. 

2. Connect any serial console for prints (e.g, **TeraTerm**).

3. Run the SiWx91x example. The SiWx91x device advertises as the "BLE_OTA_FWUP".
   <br><img src="resources/readme/bleadv.png" alt=""><br>   

## **6.3 Firmware Upgrade format change**
- Rename the firmware format from.rps to.gbl, as shown below.
   <br><img src="resources/readme/firmwareformat.png" alt=""><br>

## **6.4 Firmware Upgrade with EFR Connect Mobile App**

- Launch the EFR Connect App. If BLE is not enabled, enable it.
   <br><img src="resources/readme/EC2.png" alt="" width=250 height=500><br>

- Enable the **Start Scanning**. Click on the **CONNECT** option to initiate the connection.
   <br><img src="resources/readme/mobilebleadv.png" alt="" width=250 height=500><br>   

- When the connection is established between the SiWx91x device and mobile phone, click on the **OTA Firmware** option as shown below.
   <br><img src="resources/readme/bleconnection.png" alt="" width=250 height=500><br>


### **6.4.1 TA Firmware Upgrade**

- Click on the **Select application .gbl file** option to browse the required TA firmware.
   **Note:** When the SiWx91x device is with 1.8MB, choose the 1.8MB firmware(like, SiWG917-B.2.10.0.0.4.rps) only.  Simmilarly, choose the 1.6MB firmware(like, SiWG917-B.2.9.0.0.21.rps) for 1.6MB MBR.
   <br><img src="resources/readme/browsefw.png" alt="" width=250 height=500><br>

- Choosen the **SiWG917-B.2.10.0.0.3.gbl** firmware due to the SiWx91x device is with 1.8MB MBR.
   <br><img src="resources/readme/fwselection.png" alt="" width=250 height=500><br>

- Once the firmware is selected, Click on the **upload** option to start the firmware upgrade process.
   <br><img src="resources/readme/taselectedfw.png" alt="" width=250 height=500><br>

- Firmware upgrade process is started.
   <br><img src="resources/readme/tafwupprocess.png" alt="" width=250 height=500><br>

- When the firmware upgrade is completed, the mobile phone automatically initiates the disconnection.
   <br><img src="resources/readme/tafwupcompleted.png" alt="" width=250 height=500><br>

- Prints can see below in any Console terminal
   <br><img src="resources/readme/tafirmwareupgradesuccess.png" alt=""><br>   

### **6.4.2 M4 Firmware Upgrade**

- Create and build the required M4 application. The **ble_heartrate** example is used as the M4 firmware.

- Once the project is built, the corresponding binary files are generated in the workspace as shown below.
   <br><img src="resources/readme/m4fw.png" alt=""><br>

- Consider the either ble_heart_rate_profile_soc.rps (or) ble_heart_rate_profile_soc_isp.bin.
   <br><img src="resources/readme/m4fwfile.png" alt=""><br>

- Click on the **Select application .gbl file** option to browse the required M4 firmware

   **Note:** Considered the ble_heart_rate_profile_soc.rps file and changed the format to ble_heart_rate_profile_soc.gbl file.
   <br><img src="resources/readme/m4fwfileselection.png" alt="" width=250 height=500><br>

- Click on the **upload** option.
   <br><img src="resources/readme/slectedm4fw.png" alt="" width=250 height=500><br>   

- Firmware upgrade process is started.
   <br><img src="resources/readme/m4fwupprocess.png" alt="" width=250 height=500><br>

- When the firmware upgrade is completed, the mobile phone automatically initiates the disconnection.
   <br><img src="resources/readme/m4fwupcompleted.png" alt="" width=250 height=500><br>

- The SiWx91x device bootup and executes as the heartrate example.
   <br><img src="resources/readme/m4fwupgraded.png" alt="" width=250 height=500><br>

- Prints can see below in any Console terminal
   <br><img src="resources/readme/m4fwupsuccess.png" alt=""><br>    

## **6.5 Firmware Upgrade with Python script**

- Navigate to the "iot-pa-ble-ota_fwup_efr_connect" folder in the release as shown below.
```
$\SimplicityStudio\SDKs\gecko_sdk\extension\wiseconnect\examples\snippets\ble\iot-pa-ble-ota_fwup_efr_connect\tools\Python_script
```
   <br><img src="resources/readme/pythonscriptpath.png" alt=""><br>

   ```sh
   python Si917-OTA Firmware Update Python Script.py
   ```
- Click on the **START** button, to start execution.
   <br><img src="resources/readme/bleotafwuppyscript1.png" alt="" width=250 height=250><br>

- The application will automatically scan for the device named **BLE_OTA_FWUP**, as shown below.     
   <br><img src="resources/readme/bleotafwuppyscanning.png" alt=""><br>

- When the Python script received the **BLE_OTA_FWUP** advertsing report, it initates the connection.      
   <br><img src="resources/readme/bleotafwuppyconnection.png" alt=""><br>

- Once the connection is established successfully, the Python script fetches the OTA service and its corresponding characteristic services. And display the Name, MAC Address, and the current firmware version of the SiWx91X module, as shown below.     
   <br><img src="resources/readme/bleotafwupconnected.png" alt=""><br>

- The application default configured to the TA firmware upgrade.

### **6.5.1 TA firmware Upgrade**

- Click on the  **Update Firmware** button, to browse and select the firmware.
-  Navigate to the TA firmware location, choose the firmware to be updated, and click **Open**.        
   <br><img src="resources/readme/bleotafwuptafwselection.png" alt=""><br>

- The selected firmware file is shown below.
   <br><img src="resources/readme/bleotafwuptafwselectedncp.png" alt=""><br>

- If the chosen firmware is incorrect and you wish to choose another firmware for the firmware upgrade, click **Select Different Firmware** to begin browsing and selecting firmware.

- Click on the **Start Firmware Update**, it will redirect to the below pop-up.    
   <br><img src="resources/readme/bleotafwupta1.png" alt=""><br>

- Again click on the **Start Firmware Update**, to start the firmware upgrade in the above specified pop-up window.

-  The Python script continuously sends the firmware in a chunk-by-chunk manner. 
   <br><img src="resources/readme/bleotafwuptaupdagrading.png" alt=""><br>

- When the Python script sends the entire firmware file, the module begins the safe update process.
   <br><img src="resources/readme/bleotafeuptasafeupgrade.png" alt=""><br>  

- Once the fimrware gets successfully upgrades, the below window appears. 
   <br><img src="resources/readme/bleotafwuptasuccess.png" alt=""><br>

- Prints can see below in any Console terminal
   <br><img src="resources/readme/bleotafwuptasocprints.png" alt=""><br>    

### **6.5.2 M4 firmware Upgrade**

- Create any sample example for M4 firmware upgrade. We used the **ble_heart_rate_profile** as an example.

- Build the project to generate corresponding binary files, the resulting binary files are located in the workspace path as follows:
Path:($\ble_heart_rate_profile_soc\GNU ARM v12.2.1 - Default)

- Click on the  **Update Firmware** button , to browse and select the firmware.     
   <br><img src="resources/readme/bleotafwupconnected.png" alt=""><br> 

-  Navigate to the M4 firmware location, choose the firmware to be updated, and click **Open**.         
   <br><img src="resources/readme/bleotam4fwupslection.png" alt=""><br> 

- The selected firmware file is shown below.      
   <br><img src="resources/readme/bleotaseltectedm4firmware.png" alt=""><br> 

- The choosen firmware isn't correct and you want to choose another firmware for the firmware upgrade then you the  **Select different Firmware** for firmware browsing and selection.

- Click on the **Start Firmware Update**, it will redirect to the below pop-up.    
   <br><img src="resources/readme/bleotafwupm4.png" alt=""><br> 

- Again click on the **Start Firmware Update**, to start the firmware upgrade in the above specified pop-up window.

- The Python script continuously sends the firmware in a chunk-by-chunk manner.

- When the Python script sends the entire firmware file, the module begins the safe update process.
   <br><img src="resources/readme/bleotafeuptasafeupgrade.png" alt=""><br> 

- Once the fimrware gets successfully upgrades, the below window appears.   
   <br><img src="resources/readme/bleotam4fwupsuccess.png" alt=""><br> 

- The SoC prints can see as below in any Console terminal     
   <br><img src="resources/readme/bleotafwupm4socprints.png" alt=""><br> 


## **7 Apendix**
- The SiWx91x NCP supports the TA firmware upgrade only
- The SiWx91x SoC supports both TA and M4 firmware upgrades.
