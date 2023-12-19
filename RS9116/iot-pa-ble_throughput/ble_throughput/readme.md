# BLE Throughput Application

## Description

This application demonstrates how to measure BLE throughputs using notifications or indications when the RS9116 NCP module is configured in a BLE peripheral role. It also has the provision to measure the BLE throughputs by varying the PHY data rates and connection parameters. This application can be validated with on host MCU platforms like EFR32 and STM32.

## Introduction

The RS9116 NCP module is configured as a BLE peripheral, GATT server & client device. The "Throughput demo" of the EFR Connect app is configured as a BLE central, GATT server & client device. When the connection is established between the RS9116 NCP module and the mobile phone, both devices fetch the throughput service and their characteristic services from each other and also enables the notifications and indications accordingly.  

A push button is configured as an interrupt to control the data transfer, i.e., to start or stop the data transfer. The push button "PB0/BTN0" on the EFR32 and the Push button "B1” on the STM32 are configured as the interrupts for controlling the data transfer, respectively. If the button is pressed, the RS9116 NCP module sends the data via notifications or indications, until it is pressed again. 

When the "start" button is pressed on the "Throughput" demo, data is sent from the mobile phone until the "stop" button is pressed. The throughput is determined and submitted to the "Throughput Result" characteristic service based on the number of packets sent and the interval between pressing and repressing the button. The throughput is then printed on the host MCU serial interface.


## Prerequisites 

For this lab, the user will need the following:

### Hardware Requirements

- The Application project can been executed with any one of the following Host MCUs.

  - EFR32xG21 Starter Kit with Wireless Gecko (SLSWSTK6006A Baseboard: BRD4001A, Radio board: BRD4180a or BRD4180b)

  - STM32 - STM32F411RE 

- RS9116 Evaluation Board (RS9116W-SB-EVK1) / (RS9116W-DB-EVK1)

- Mini-USB to USB Type-A cable 

- Micro-USB to USB Type-A cable (included with RS9116X-SB-EVK1)

- Interconnect board and SPI ribbon cable (for SPI communication)

- One Female to Female connector(for the "RST-PS" pin connection)

- Windows PC with 2 USB ports

- BLE-supported smartphone

### Software Requirements

- RS9116 NCP's latest Software release is available in the below link.

  - Link: https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk

- Development Environment
  - For EFR32, use Simplicity studio
  - For STM32, use licensed Keil IDE 

- Simplicity Studio v5 (v5.1.2.0 or higher)

   - Download the simplicity studio from [Simplicity Studio](https://www.silabs.com/developers/simplicity-studio).

   - Follow the [Simplicity Studio user guide](https://docs.silabs.com/simplicity-studio-5-users-guide/1.1.0/ss-5-users-guide-getting-started/install-ss-5-and-software#install-ssv5) to install Simplicity Studio. 

- Keil 

   - Refer to the [Keil](https://www.keil.com/) for downloading and installation procedure.

- [EFR Connect App](https://www.silabs.com/developers/efr-connect-mobile-app): Download and install this mobile application either on an Android or an IoS smartphone for evaluating this BLE application.

- [TeraTerm](https://ttssh2.osdn.jp/) serial terminal software.


**NOTE:** 
- Refer to [Update EVK Firmware ](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-getting-started-with-pc/update-evk-firmware) to load the firmware into the RS9116 NCP module. The firmware binary is located in \<Release\_Package\>/firmware/

- The application can be evaluated from the 2.6.1.0.7 SAPIS & Firmware. But it is recommended to use the latest SAPIS & Firmware available on the website.

- This example application supports Bare metal as well as FreeRTOS.

## Block Diagram

This section describes the block diagram of the BLE Throughput application.
<br>
<img src="resources/readme/block_diagram.PNG" width=700 alt=""><br>


The RS9116 NCP module and the host MCU are connected over the SPI interface and both are connected to the Windows PC with USB cables.

## Setup

This section describes the hardware setup and the connections for EFR32 and STM32 Host MCUs.

### **EFR32**
This section describes the hardware setup and the connections for EFR32.
<br>
<img src="resources/readme/efr32_setup.png" width=700 alt=""><br>

   1. Connect the Interconnect adapter board provided with the RS9116 NCP Development Kit to the expansion header of EFR32.

   2. Connect the SPI header of Interconnect adapter board to the SPI connector of the RS9116 NCP module using the 10-pin ribbon cable (SPI cable) provided with the RS9116 NCP Development kit.

   3. Connect the "RST_PS" pin on the J9 header of the RS9116 NCP module to the pin labeled 11 on the J4 header of the Interconnect adapter board using a female-to-female connector.
   
   4. Ensure the ISP switch on the RS9116 NCP module is set to the OFF position. 
   
   5. Connect the EFR32 Starter Kit to a windows PC using a Mini USB cable.

   6. Connect the RS9116 NCP module to a windows PC using the USB cable labeled POWER. Note that this connection only provides power to the RS9116 NCP module, there is no USB communication over this connection.

      
If the interconnect board is not available, make the connections between the RS9116 NCP module and EFR32 Host MCU with the SPI cable as described below.\

<br><img src="resources/readme/efr32_setup2.png" width=700 height=300 alt=""><br>
<br><img src="resources/readme/efr32_pin_connections.PNG" alt=""><br>



### **STM32**

This section describes the hardware setup and the connections for STM32.
<br>
<img src="resources/readme/stm32_setup.PNG" width=700 alt=""><br>
   
   1. Connect the RS9116 NCP module reset pin "RST_PS" to the STM32F11RE development board CN10 pin 6, to enable the STM32 to reset the RS9116 NCP module.

   2. As shown in the below image, connect the STM32F411RE board to the SPI connector of the RS9116 NCP module using the 10-pin ribbon cable provided with the RS9116 NCP module.
   <br><img src="resources/readme/stm32_pin.png" width=500 alt=""><br>
<br><img src="resources/readme/stm32_pin_connections.PNG" width=400 alt=""><br>
  
  3. Ensure the ISP switch on the RS9116 NCP module is set to the OFF position. 

  4. Connect the STM32 MCU to a windows PC using a Mini USB cable.

  5. Connect the RS9116 NCP module to a windows PC using the micro USB interface labeled POWER. Note that this connection only provides power. There is no USB communication over this connection.


## Application flow 

This flow chart describes the application flow.
<br>
<img src="resources/readme/flow_chart.PNG" alt=""><br>


## Project Configurations

1. Download the BLE throughput application. Place the **ble_throughput** project folder in the  **wiseconnect-wifi-bt-sdk\examples\snippets\ble** SDK path, else clone or download the desired SDK to your drive at any location (ensure this does not have a long folder path) and that location will be your SDK path <SDK_path>.

     ```
   git clone https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk.git --branch=<tag name> 
   ```
   In this case, the tag used is **2.6.0**. Hence the command given is as follows:

   ```
   git clone https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk.git --branch=2.6.0 
   ```
   <br><img src="resources/readme/clone_2_6_0.PNG" alt=""><br>

      

2.  Now, place the **ble_throughput** project folder in the above cloned SDK at path: **\<SDK_path\> → examples → snippets → ble**
   <br><img src="resources/readme/project_folder_path.png" alt=""><br>
   	

3. Ensure the RS9116 NCP module is pre-loaded with the latest firmware, Refer to [Update EVK Firmware ](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-getting-started-with-pc/update-evk-firmware) to load the firmware.

## Import the project in Keil

The following section describes how to set up Keil IDE in Windows Operating System.

1. Navigate to the path, **\<SDK_path\> → examples → snippets → ble → ble\_throughput → projects** in the downloaded release SDK.

2. Click on the "ble_throughput-nucleo-f411re" project. 
   <br><img src="resources/readme/project_path.PNG" alt=""><br>

3. The project will be open in the Keil IDE. Open the "rsi_ble_throughput.c" application file.
   <br><img src="resources/readme/project.PNG" alt=""><br>

## Setting up the development environment in simplicity studio

The following section describes how to set up Simplicity IDE in Windows Operating System.

1. Launch the Simplicity Studio IDE on your Windows PC.

2. Select the desired path as the workspace directory. The following directory has been chosen for the workspace as shown in the image below.

3. Click on **Launch**.
   <br><img src="resources/readme/directory.png" alt=""><br>

4. Check the default repo to which the Simplicity Studio IDE is linked to.

   a. In the Simplicity Studio IDE, go to 
    **Preferences > Simplicity Studio > External Repos**. 
   <br><img src="resources/readme/preferences.PNG" alt=""><br>

   b. By default, the latest **WiseConnect Wi-Fi/BT SDK** repo is linked to external repos. Click on **update** as shown below so that the latest SDK at https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk gets cloned at path: 
   **<Simplicity_Studio_Installed_Path>SimplicityStudio\v5\developer\repos\wiseconnect-wifi-bt-sdk**.
   <br><img src="resources/readme/default_repo_path.PNG" alt=""><br>

   c. After updating the repo, the SDK gets updated locally as shown below.
   <br><img src="resources/readme/repo_updated.PNG" alt=""><br>
   <br><img src="resources/readme/SDK_updated_locally.PNG" alt=""><br>

5. Ensure the EFR32 device and RS9116 NCP module are connected to your windows PC.

6. When EFR32 is connected to the windows PC, Simplicity studio will detect it as shown below. 
   <br><img src="resources/readme/radio_board.PNG" alt=""><br>

**Note:** While importing the project, select the project based on the radio board version:

- For **BRD4180A**, select the ble_throughput-brd4180a-mg21 project.
- For **BRD4180B**, select the ble_throughput-brd4180b-mg21 project.


### Import the Project in simplicity studio

This section provides the steps for importing the project into Simplicity Studio IDE.

1. In the Simplicity Studio IDE, go to **File** and select **Import**.
   <br><img src="resources/readme/import.png" alt=""><br>

2. A pop up window appears.Click on **Browse**.

3. Navigate to the path, **\<SDK_path\> → examples → snippets → ble → ble\_throughput → projects** in the downloaded release SDK.

4. Select the appropriate project as per the detected radio board variant i.e., **ble\_throughput\-brd4180a-mg21** → **BRD4180A**.
**ble\_throughput\-brd4180b-mg21** → **BRD4180B**.

5. Click on **Next** → **Next**.
   <br><img src="resources/readme/import_project.PNG" alt=""><br>

6. Now click on **Finish**.


### Application Configurations

1. Once the project is successfully imported, the project name appears in the Project Explorer tab.

2. Expand the project folder by clicking on drop-down.

3. Open the **rsi\_ble\_throughput.c** file present in **ble\_throughput-brd4180b-mg21 → ble\_throughput** folder.
   <br><img src="resources/readme/application.png" alt=""><br>

**Note**:
The required features are already set with the default values. Following are the non-configurable macros in the application except the THROUGHPUT_TYPE and the PHY data rates:

- “RSI_REMOTE_DEVICE_NAME” refers to the name of the remote device to which the RS9116 NCP module has to connect.
   ```c

   #define RSI_BLE_DEVICE_NAME                        "Throughput Test"
   ```

4. Open the rsi_ble_config.h file which is present under “ble_throughput” folder and update/modify the following macros,

  - The application has the provision to measure the throughput for both notifications and indications. **“THROUGHPUT\_TYPE”** MACRO refers to the type of throughput either “Notifications” or “Indications”.

   When the “THROUGHPUT_TYPE”  is configured as NOTIFICATIONS, The RS9116 NCP module is configured for “Notifications”.
   ```c
   #define  THROUGHPUT_TYPE                      NOTIFICATIONS
   ```  

   When the “THROUGHPUT_TYPE”  is configured as INDICATIONS, The RS9116 NCP module is configured for “Indications”.

   ```c
   #define  THROUGHPUT_TYPE                       INDICATIONS
   ```    
  - Depending on the TX_PHY_RATE and RX_PHY_RATE MACROs the below configurations are valid.
      - For 2Mbps the TX_PHY_RATE & RX_PHY_RATE values is 0x02

      - For 1Mbps the TX_PHY_RATE & RX_PHY_RATE values is 0x01

      - For Coded-500K the TX_PHY_RATE & RX_PHY_RATE values is 0x04

   ```c
   #define TX_PHY_RATE         0x02
   #define RX_PHY_RATE         0x02
   ```
 - The "Coded-PHY"  rates to be chosen as below
      - For Coded-500K the CODED_PHY_RATE value is 0x01
      - For Coded-125K the CODED_PHY_RATE value is 0x02
      - For Uncoded -PHY (1Mbps & 2 Mbps) the CODED_PHY_RATE value should be 0x00

```c
#define CODED_PHY_RATE          0x00
```
- Connection update params

```c

#define CONN_INTERVAL_MIN        0x08

#define CONN_INTERVAL_MAX        0x08

#define CONN_LATENCY             0

#define SUPERVISION_TIMEOUT      800

```
- “RSI_BLE_CHAR_SERV_UUID” refers to the attribute type of the characteristics to be added to a service.
   ```c
   #define RSI_BLE_CHAR_SERV_UUID                        0x2803
   ```

- “RSI_BLE_CLIENT_CHAR_UUID” refers to the attribute type of the client characteristics descriptor to be added to a service.
   ```c
   #define RSI_BLE_CLIENT_CHAR_UUID                      0x2902
   ```

- “RSI_BLE_CLIENT_CHAR_DESP” refers to the attribute type of the client characteristics a user descriptor to be added to a service.

    ```c
    #define RSI_BLE_CLIENT_CHAR_DESP                      0x2901
    ```

- “RSI_BLE_ATT_PROPERTY_READ” is used to set the ”READ” property to an attribute value.
   ```c
    #define RSI_BLE_ATT_PROPERTY_READ                    0x02
    ```

- “RSI_BLE_ATT_PROPERTY_WRITE” is used to set the “WRITE” property to an attribute value.
   ```c
    #define RSI_BLE_ATT_PROPERTY_WRITE                   0x08
    ```

- “RSI_BLE_ATT_PROPERTY_NOTIFY” is used to set the “NOTIFY” property to an attribute value.

    ```c
    #define RSI_BLE_ATT_PROPERTY_NOTIFY                   0x10
    ```

- “RSI_BLE_ATT_PROPERTY_INDICATE” is used to set the “INDICATE” property to an attribute value.

    ```c
    #define RSI_BLE_ATT_PROPERTY_INDICATE                 0x20
    ```

- “RSI_BLE_ATT_PROPERTY_WRITE_NO_RESPONSE” is used to set the “WRITE WITH NO RESPONSE” property to an attribute value.

    ```c
    #define RSI_BLE_ATT_PROPERTY_INDICATE                 0x04
    ```

- “BT_GLOBAL_BUFF_LEN” refers Number of bytes required by the application and the driver.

   ```c
    #define BT_GLOBAL_BUFF_LEN                           15000
    ```
Note: The relevant configurations are already configured in the rsi ble config.h file in the appropriate example folder. With the exception of "THROUGHPUT TYPE," users might not be necessary to update them for this example. The "THROUGHPUT TYPE" is set by default to "NOTIFICATIONS." The "THROUGHPUT TYPE" option needs to be modified if the "INDICATIONS" are to be validated.

## Execution of Project

### Set up the Serial COM port on Simplicity Studio IDE

The Serial COM port interface is used for printing out debug prints from the application.
1.  When the EFR32 / STM32 is connected to the windows PC, the EFR32/STM32 serial port is automatically detected as shown below.
   <br><img src="resources/readme/efr32_port_detection.PNG" alt=""><br>
   <br><img src="resources/readme/stm32_device_detection.PNG" alt=""><br>

2. When the EFR32/STM32 is connected, Open the Teraterm tool and choose the detected EFR32/STM32 serial port and click on "OK" as shown below.
   <br><img src="resources/readme/port_selection.PNG" alt=""><br>
   <br><img src="resources/readme/stm32_port.PNG" alt=""><br>

3. Once the EFR32/STM32 is detected, Navigate to the **setup** → **Terminal** and update the below-specified setup information as shown in the below image, and click on "OK".
   <br><img src="resources/readme/teraterm_setup.PNG" alt=""><br>
   <br><img src="resources/readme/teriminal_settings.PNG" alt=""><br>

4. Navigate to the **setup** → **Serial port** and update the baud rate to "115200" and click on "OK".
   <br><img src="resources/readme/serial_port_setup.PNG" alt=""><br>
   <br><img src="resources/readme/serial_port.PNG" alt=""><br>

5. The EFR32/STM32 serial port is connected and the teraterm terminal displays the debug prints in the application.

### **Build the project**

- For STM32: Click on the build project option as shown in the figure to build the project.
   <br><img src="resources/readme/stm32_build_project.PNG" alt=""><br>

- For EFR32: Click on the build project option as shown in the figure to build the project.
   <br><img src="resources/readme/efr32_build_project.PNG" alt=""><br>

- When the build process is completed, the project corresponding binary files are generated.

### **Debug the project**

- **EFR32:**
   - To flash the code, right-click on the project name and select **Debug As** → **Silicon Labs ARM Program**.
      - If the EFR32xG21 has an older SE firmware, the following warning may pop up. Click ‘Yes’ to continue
      <br><img src="resources/readme/warning1.png" alt=""><br>

      - If the connected EFR32 board has any other radio board other than 20dbm, Simplicity Studio will not be able to detect the device and gives the below pop-up window for Device Selection. Select the device displayed and click OK.
      <br><img src="resources/readme/warning2.png" alt=""><br>   

      - The following warning may pop-up as shown below, click Yes and continue. 
      <br><img src="resources/readme/warning3.png" alt=""><br>

- **STM32:**
   - To debug the code, click on the debug option as shown in the below image.
   <br><img src="resources/readme/stm32_debug.PNG" alt=""><br>
- As soon as the debug process is completed, the application control branches to the main() function.

### **Execute the Project**

This section demonstrates the execution flow of the application based on the configuration set by the user.

- Once the program gets executed, as per the above configuration the RS9116 NCP module acts as a **"GATT** **SERVER”** and also a BLE peripheral device where the RS9116 NCP module advertises as “Throughput Test”.

- In the EFR Connect mobile application, navigate to the Demos section then select the “Throughput” demo. Under the “Throughput” demo project you can observe the RS9116 NCP module advertising as a “Throughput Test” as demonstrated in the below image.
<br><img src="resources/readme/device_advertising.png" alt="" width=500><br>


- Initiate the connection from the mobile application. Once the connection is successfully established, the PHY, connection interval, slave latency, supervision timeout, PDU & MTU sizes are updated as demonstrated in the above image.
<br><img src="resources/readme/device_pairing.png" alt="" height=500  width=250><br>


- The application has the provision to verify the throughput for the below properties.

  - Notifications

  - Indications

### **Notifications**

- If the RS9116 NCP module is configured for the “Notifications”, RS9116 NCP module sends the data in the form of notifications to the remote device when the button is pressed.

- Once the connection is successfully established, the PHY, connection interval, slave latency, supervision timeout, PDU & MTU sizes are updated as demonstrated in the below image.
<br><img src="resources/readme/device_is_successfully_connected.jpeg"  alt="" width=250 height=500><br>

#### **TX Throughput Measurement**

- For the EFR32 host MCU platform, a user needs to click on the Push button "PB0/BTN0”. Once the button is pressed, the interrupt is triggered and the RS9116 NCP module sends the data to the remote device.
<br><img src="resources/readme/efr32_setup_diagram.jpeg"  alt="" width=300 height=400><br>
   

- For the STM32 host MCU platform, a user needs to click on the Push button "B1” (which indicates in blue color). Once the button is pressed, the interrupt gets triggered in the application and the RS9116 NCP module sends the data to the remote device.
<br><img src="resources/readme/stm32_setup_diagram.jpeg"  alt="" width =300 height=400><br>


- The EFR Connect application continuously receives the data and displays the throughput for every connection interval. As the throughput varies per connection interval, the meter deflection is also updated accordingly as demonstrated in the image.
<br><img src="resources/readme/notification_throughput_readings.PNG"  alt="" width =500 height=500><br>


- To stop the data transfer the a user needs to click on the Push button "PB0/BTN0" again on the EFR32 platform and similarly the Push button "B1” for the STM32 platform. Automatically the interrupt gets triggered and the notifications are disabled. The final TX throughput is displayed on the host MCU (either EFR32 or STM32) serial terminal.
<br><img src="resources/readme/notifications_tx_throughput_serial_print.PNG" alt=""><br>

#### **RX Throughput Measurement**
- When the "Notifications" are selected in the mobile phone, If the "start" button on the "Throughput" demo is pressed, the mobile phone continuously sends the notifications to the RS9116 NCP module until the "stop" button is pressed and as demonstrated in the below image, the EFR Connect application meter deflection shows the throughput variation according to the transmitted data for every connection interval.
<br><img src="resources/readme/notifications_rx_throughput_meter_deflection.PNG"  alt="" width =500 height=500><br>

- The final RX throughput is displayed on the host MCU (either EFR32 or STM32) serial terminal as shown in the below image. 
<br><img src="resources/readme/notifications_rx_throughput_serial_print.PNG" alt=""><br>

- Similarly, When the "Indications" are selected in the mobile phone, the mobile phone continuously sends the indications to the RS9116 NCP module and as demonstrated in the below image, the EFR Connect application meter deflection shows the throughput varies according to the transmitted data for every connection interval.
<br><img src="resources/readme/indications_rx_throughput_meter_deflection.PNG" alt="" width =500 height=500><br>

- The final RX throughput is displayed on the host MCU (either EFR32 or STM32) serial terminal as shown in the below image. 
<br><img src="resources/readme/indications_rx_throughput_serial_print.PNG" alt=""><br>


### **Indications**

- If the RS9116 NCP module is configured for the “Indications”, RS9116 NCP module sends the data in the form of indications to the remote device after the button is pressed.

- Once the connection is successfully established, the PHY, connection interval, slave latency, supervision timeout, PDU & MTU sizes are updated as demonstrated in the below image.
<br><img src="resources/readme/device_is_successfully_connected_for_indications.jpeg"  alt="" width =250 height=500> <br>

#### **TX Throughput Measurement**
- For the EFR32 host MCU platform, a user needs to click on the Push button "PB0/BTN0”. Once the button is pressed, the interrupt gets triggered and the RS9116 NCP module sends the data to the remote device.
<br><img src="resources/readme/efr32_setup_diagram.jpeg"  alt="" width =300 height=400><br>
- For the STM32 host MCU platform, a user needs to click on the “Push button B1” (which indicates in blue color). Once the button is pressed, the interrupt gets triggered and the RS9116 NCP module sends the data to the remote device.
<br><img src="resources/readme/stm32_setup_diagram.jpeg" alt="" width =300 height=400><br>

- The EFR Connect application continuously receives the data and displays the RSSI value of the corresponding packet. As the RSSI value changes then the meter deflection is also updated accordingly as demonstrated in the image.
<br><img src="resources/readme/indications_throughput_readings.PNG"   alt="" width =500 height=500>


- To stop the data transfer the a user needs to click on the Push button "PB0/BTN0" again on the EFR32 platform and similarly the Push button "B1” for the STM32 platform. Automatically the interrupt gets triggered and the notifications are disabled. The final TX throughput is displayed on the host MCU (either EFR32 or STM32) serial terminal.
<br><img src="resources/readme/indications_serial_prints.png" alt=""><br>

#### **RX Throughput Measurement**
- When the "Indications" are selected in the mobile phone, If the "start" button on the "Throughput" demo is pressed, the mobile phone continuously sends the notifications to the RS9116 NCP module until the "stop" button is pressed and as demonstrated in the below image, the EFR Connect application meter deflection shows the throughput variation according to the transmitted data for every connection interval.
<br><img src="resources/readme/indications_rx_throughput_meter_deflection.PNG"  alt="" width =500 height=500><br>


- The final RX throughput is displayed on the host MCU (either EFR32 or STM32) serial terminal as shown in the below image. 
<br><img src="resources/readme/indications_rx_throughput_serial_print_1.PNG" alt=""><br>

- Simillary, When the "Notifications" are selected in the mobile phone, the mobile phone continuously sends the notifications to the RS9116 NCP module and as demonstrated in the below image, the EFR Connect application meter deflection shows the throughput variation according to the transmitted data for every connection interval.
<br><img src="resources/readme/notifications_rx_throughput_meter_deflection.PNG" alt="" width =500 height=500><br>


- The final RX throughput is displayed on the host MCU (either EFR32 or STM32) serial terminal as shown in the below image. 
<br><img src="resources/readme/notifications_rx_throughput_serial_print_1.PNG" alt=""><br>


## Observations

Below are the observations taken in the ideal environment (RF chamber) at different data rates for EFR32 and STM32 host MCU platforms with 2.5.0.24 SAPIS & firmware.

| S.No | Throughput type | Time duration | HOST MCU | Data rate |             |             |             |
|------|-----------------|---------------|----------|-----------|-------------|-------------|-------------|
|       |   ||| **2Mbps** | |**1Mbps** |             |             |             |             |             |             |
||||| **TX Throughput** | **RX Throughput** |**TX Throughput** | **RX Throughput** |             |             |             |             |
| 1 | Notifications | 1min | EFR32 | 830.9kbps | 665.1kbps | 486.4kbps | 163.7kbps |
| 2 | |3min | |826.1kbps | 685.0kbps | 485.1kbps | 156.3kbps |             |             |
| 3 | |1min | STM32 | 845.1kbps | 843.1kbps | 473.5kbps | 473.3kbps |             |
| 4 | |3min | |856.1kbps | 842.6kbps | 476.4kbps | 472.4kbps |             |             |
| 5 | Indications | 1min | EFR32 | 81.0kbps | 79.6kbps | 81.4kbps | 79.8kbps |
| 6 | |3min | |80.35kbps | 80.3kbps | 81.2kbps | 80.44kbps |             |             |
| 7 | |1min | STM32 | 79.7kbps | 79.9kbps | 79.42kbps | 80.1kbps |             |
| 8 | |3min | |79.3kbps | 79.3kbps | 79.3kbps | 79.0kbps |             |             |

## Optional features

1. By default, the application runs on FreeRTOS. Remove the `RSI_WITH_OS` preprocessor symbol to run the application on Bare metal.

## Baremetal Configurations

### **Simplicity studio**
1. Once the project is imported, Right-click on the **ble\_throughput-brd4180b-mg21** / **ble\_throughput-brd4180a-mg21** project and navigate to the properties and click on it.
<br><img src="resources/readme/with_out_os_macro_1.png" alt=""><br>

2. The Properties window appears, navigate **c/c++ General → paths and symbols**.
   

3. Navigate to the **Symbols**, Click on the **GNU C** where the project default preprocessor macros appear.

4. To remove the **`RSI_WITH_OS`** macro choose the **`RSI_WITH_OS`** and click on the **delete** option.

5. Click on **Apply and close**.
   <br>
<img src="resources/readme/with_out_os_macro.PNG" alt=""><br>

6. Clean and build the project.

7. Right click on the **ble\_throughput-brd4180b-mg21** / **ble\_throughput-brd4180b-mg21** project and click on **Clean Project** option.
   <br><img src="resources/readme/clean_project.PNG" alt=""><br>

8. Right click on the **ble\_throughput-brd4180b-mg21** / **ble\_throughput-brd4180a-mg21** project and click on **Build Project** option.
   <br><img src="resources/readme/build_project.PNG" alt=""><br>

9. Once the project is built, the application binary files are generated. 


### **Keil**

1.  Once the project is successfully imported, click on the **Options for Target**.

2. Navigate to the **C/C++** tab and remove the **`RSI_WITH_OS`** macro from the define and click on **OK** as shown below.
   <br><img src="resources/readme/Keil_with_out_os.png" alt=""><br>

3. To clean the project, Navigate to the **Project** tab and click on the **Clean Targets**.
   <br><img src="resources/readme/keil_clean_project.PNG" alt=""><br>

4. To build the project, Navigate to the **Project** tab and click on the **Build Targets**.
<br><img src="resources/readme/keil_build_project.PNG" alt=""><br>

5. Once the project is built, the application binary files are generated. 