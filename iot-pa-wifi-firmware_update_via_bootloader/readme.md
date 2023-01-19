# Firmware update via bootloader

## Purpose

Whenever an RS9116W module is initially powered on, bootloader is the first piece of code which gets loaded on to the system.

The RS9116W's firmware can be updated over the air through remote servers or via the bootloader. Firmware update over the air requires the presence of valid firmware in the module. At times, the corrupted firmware might be loaded on to RS9116W or valid firmware might not be present in RS9116W. In such a case, the RS9116W's firmware can be updated via bootloader.

This example demonstrates how the host MCU and the RS9116W's bootloader interact with each other through SPI interface to upgrade the firmware to the latest/desired version.
As per this application, the firmware file can be stored in a microSD card or internal flash of host MCU.

## Prerequisites
																																   
For this lab, you will need the following:

### Hardware Requirements

- EFM32GG11 Starter Kit with Wireless Gecko (SLSTK3701A Base board: BRD2204A)

- RS9116 Evaluation Board (RS9116W-SB-EVK1)/(RS9116W-DB-EVK1)

- MicroSD card

- Mini-USB to USB Type-A cable 

- Micro-USB to USB Type-A cable (included with RS9116X-SB-EVK1/RS9116W-DB-EVK1)

- Interconnect board and SPI ribbon cable (for SPI communication)
 
- Female to Female connectors - 2
																
- Windows PC with 2 USB ports 
																				
### Software Requirements
																																												   
- The latest Simplicity Studio IDE(v5.1.2.0 or higher)

   a. Download the Simplicity Studio IDE from [Simplicity Studio](https://www.silabs.com/developers/simplicity-studio).

   b. Follow the [Simplicity Studio user guide](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-overview/) to install Simplicity Studio.

- Download and extract the 2.7.0 RS9116 NCP release package from the [link](https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk/tree/2.7.0). 
														 
- **firmware_update_via_bootloader** project folder

- [Python tool with version higher than 3.10.6](https://www.python.org/downloads/)

**NOTE**:
- This application project is designed to work with EFM32GG11 host MCU.
- This application is tested with the 2.7.0 SDK and 2.7.0.0.39 firmware.
- This example application supports Bare metal only.

## Set up
														
This section describes the hardware setup and the connections.

**![Setup diagram](resources/readme/setup.png)**

   1. Connect the Interconnect adapter board provided with the RS9116 NCP Development Kit to the EXP header of EFM32.

   2. Connect the SPI header of Interconnect adapter board to the SDIO/SPI connector of the RS9116 NCP using a 10-pin ribbon cable (SPI cable) provided with RS9116 NCP.

   3. Connect the RST_PS pin on J9 header of RS9116 NCP to the pin labeled 11 on the J4 header of the Interconnect adapter board using a female-to-female connector.

   4. Connect the GND pin on J9 header of RS9116 NCP to the pin labeled GND on the J4 header of the Interconnect board using a female-to-female connector.

   5. Ensure the ISP switch on the RS9116 NCP module is set to OFF position. 

   6. Connect the EFM32 Starter Kit to the PC using a Mini USB cable.
   
   7. Connect the RS9116 NCP to a PC using the USB interface labeled POWER. Note that this connection only provides power. There is no USB communication over this connection.

**NOTE**: 
If you don't have an Interconnect board, make the SPI connections between EFM32 and RS9116 NCP using a SPI cable by following the below references.

**![Figure: connection_setup_with_SPI_cable](resources/readme/setup_no_interconnect.png)**

1. Pin definitions of EFM32 expansion header are as shown below.

   **![Figure: EFM32_pin_numbers](resources/readme/EFM32_pin_numbers.png)**

3. Pin Configurations for EFM32GG11 with RS9116W.

   **![Figure: pin_connections](resources/readme/connections.PNG)**

   NOTE: For soft-reset configuration, connect the pin 11 on expansion header of EFM32 to the RST_PS pin on J9 header of RS9116 NCP.

## Setting up the development environment

The following section describes how to set up the Simplicity Studio IDE in Windows Operating System.

1. Ensure the RS9116 NCP module is pre-loaded with 2.7.0.0.39 firmware (RS9116W.2.7.0.0.39.rps) following the steps mentioned in the [update EVK firmware](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-getting-started-with-pc/update-evk-firmware).

2. Place the **firmware_update_via_bootloader** project folder in the downloaded 2.7.0 SDK at path: **\<SDK_path\> → examples → featured**

   **![Project in to SDK](resources/readme/project_path.PNG)** 

3. Launch the Simplicity Studio IDE on your PC.

4. Select the desired path as workspace directory by clicking on **Browse**. The following directory has been chosen for the workspace as shown in the image below.
												  
5. Click on **Launch**.
							 
   **![Choose workspace](resources/readme/directory.PNG)**
									
6. Ensure the EFM32 and RS9116 NCP setup is connected to your PC. 

7. When EFM32 is connected to the PC, the board detection appears under **Debug Adapters** pane as shown below.

   **![Board detection](resources/readme/efm32_detection.PNG)**

### Import the Project

This section provides the steps for importing the project into Simplicity Studio IDE.

1. In the Simplicity Studio IDE, go to **File** and select **Import**.

   **![Import the project](resources/readme/import_project.PNG)**

2. A pop up window appears. Click on **Browse**.

3. Navigate to the path, **\<SDK_path\> → examples → featured → rsi\_firmware\_update\_via\_bootloader → projects** in the 2.7.0 release SDK downloaded.

4. Select the project **firmware\_update\_via\_bootloader-brd2204a-gg11.slsproj**.

5. Click on **Next** → **Next**.

   **![Select project file](resources/readme/project_select.PNG)** 

6. Now click on **Finish**.

### Application configuration

1. Once the project is successfully imported, the project name appears in the **Project Explorer** pane.

2. Expand the project folder by clicking on the drop down.

   **![Project name](resources/readme/drop_down.PNG)**
   
3. Navigate to the **firmware\_update\_via\_bootloader** folder. Open the **rsi_wlan_config.h** file and ensure that the below macro is set to 1 for fast firmware upgradation.

   ``#define RSI_FAST_FW_UP                    1``

   **![Configuration](resources/readme/wlan_config.PNG)**

#### Case 1: Storing the firmware file on microSD card and burning it to RS9116's flash

In this case, the firmware file is stored on a microSD card. The firmware content is flashed on to RS9116W in chunks with the length of each chunk being 4096 bytes.
For implementing this, you need a microSD card with an available size of more than 2 MB because the RS9116 firmware file (.rps) is closer to 2 MB. In this example, the below mentioned microSD card is used.

Part number: SDCS2/32GB

Reference site - https://www.kingston.com/en/memory-cards/canvas-select-plus-microsd-card

1. Insert the microSD card into the SD card slot of your PC.
2. Format the microSD card to FAT32 file system.
NOTE: FAT32 is a 32-bit version of the FAT file system and is a format typically used for USB sticks and SD cards. This file system arranges the space on your drive so that you can store files and access them on any kind of computer. FAT32 is an extension of file systems in which the data is stored in chunks of 32 bits.

   a. Right click on the microSD card detected on your PC and choose **Format...**.

   **![microSD configuration](resources/readme/format_fat32.PNG)** 

   b. Select the **File system** as **FAT32** and click on **Start**.

   **![FAT32 file system](resources/readme/format_start.PNG)** 

3. Copy one or more firmware files to your microSD card. In this example, only one firmware file is copied.
   **![Copy firmware file to microSD card](resources/readme/firmware_file.PNG)** 

4. Go back to Simplicity Studio IDE and open the **rsi\_firmware\_update\_via\_bootloader.c** and **main.c** files present in **firmware\_update\_via\_bootloader-brd2204a-gg11 → firmware\_update\_via\_bootloader** folder.

   **![Application file](resources/readme/application.PNG)** 

5. Navigate to the following macro definition in the **rsi\_firmware\_update\_via\_bootloader.c** file and define the firmware file name same as in the microSD card.

    ``#define FW_FILE_NAME1        "RS9116W.2.6.0.0.34.rps"``

    **![Include firmware file](resources/readme/include_fw_file.PNG)** 

   **NOTE**: If you want to include more firmware files, 

   - define them with macros FW_FILE_NAME2, FW_FILE_NAME3,....etc., in the **rsi\_firmware\_update\_via\_bootloader.c** file.
   - navigate to **openFile_sd()** function and replace the existing macro (in this example *FW_FILE_NAME1*) with the desired macro (say *FW_FILE_NAME2*).

   **![firmware file name](resources/readme/fw_file_name.PNG)** 

6. In the project that has been imported on to the Simplicity Studio IDE, the **SD_CARD_FW** preprocessor symbol is enabled by default.

**Note**: If you are facing any issues with multiple files on microSD card, please copy only one firmware file (.rps) file to your microSD card.

#### Case 2: Storing the firmware file on to the internal flash of EFM32 and burning it to RS9116's flash

In this case, the firmware file is stored on internal flash of the host MCU (EFM32). The firmware content is flashed on to RS9116W in chunks with the length of each chunk being 4096 bytes.
For implementing this, you need a python utility script **GenerateHexArray.py** present at **\<SDK_path\> → examples → featured → rsi\_firmware\_update\_via\_bootloader → resources** is used for converting the RS9116's firmware file into a character array. The character array is stored on EFM32's internal flash.

1. Copy the desired firmware file (in this case, *RS9116W.2.6.0.0.34.rps*) to the path - **\<SDK_path\> → examples → featured → rsi\_firmware\_update\_via\_bootloader → resources**.

   **![Generate hex array](resources/readme/fw_file_python_script.PNG)** 

2. Give the following command to convert the firmware file to array.
   
   ``py GenerateHexArray.py RS9116W.2.6.0.0.34.rps``

   **![Convert to array](resources/readme/convert_to_array.PNG)** 

   **NOTE**: In some systems/PCs, the **py** command might not work/might not be recognized. In such a case, replace **py** with **python** and give the below command.

   ``python GenerateHexArray.py RS9116W.2.6.0.0.34.rps``

3. The **fw_file.h** file will be generated. Replace the **fw_file.h** present at **\<SDK_path\> → examples → featured → rsi\_firmware\_update\_via\_bootloader → resources → fw_files** folder with the **fw_file.h** file generated file.

   **![Generated firmware file](resources/readme/fw_file.PNG)** 

4. Delete the **SD_CARD_FW** pre-processor symbol and add the **INTERNAL_FLASH_FW** pre-processor symbol.

   a. Right click on the Project → Properties → C/C++ General → Paths and Symbols → Symbols. Select **SD_CARD_FW** and click on **Delete**.

   **![Delete SD_CARD_FW symbol](resources/readme/macro_delete.PNG)**

   b. Under GNU C, click on **Add** to add **INTERNAL_FLASH_FW** symbol as shown in the below image.
   
   c. Click on **Apply and Close**.

   **![Add INTERNAL_FLASH_FW symbol](resources/readme/internal_flash_symbol.PNG)**

## Build the Project

1. Build the project by right clicking on **firmware\_update\_via\_bootloader-brd2204a-gg11** project name and select **Build Project**. It takes a few seconds to build.

   **![Build project](resources/readme/build_project.PNG)**

2. Make sure that the build returns with 0 errors and 0 warnings.

### Code walkthrough

Below are some important APIs used in the application.
1. microSD card driver APIs

   ```
   openFile_sd() - This API is used to open the specified firmware file (in this case, FW_FILE_NAME1) on microSD card.
   ```

   ```
   file_read() - This API is used to read the contents of specific firmware file on microSD card in chunks (512 bytes at a time) into an application buffer (in this case, recv_buffer).
   ```

2. Internal flash APIs

   ```
   file_read_flash() - This API is used to copy the contents of firmware array file (in this case, fw_file.h) into an application buffer (in this case, recv_buffer) in chunks (each chunk size is 4096 bytes).
   ```

3. The following API is used to burn the firmware image to the RS9116W's flash in chunks via bootloader. The **recv_buffer** mentioned above is passed as **fw_image_size** parameter in the below API.

   ```c
   int16_t rsi_bl_upgrade_firmware(uint8_t *firmware_image, uint32_t fw_image_size, uint8_t flags)
   ```

For more info about the API, refer to [bootloader upgrade](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-sapi-reference/common#rsi-bl-upgrade-firmware).

### Set up the Virtual COM port on Simplicity Studio IDE

The Virtual COM (VCOM) port interface is used for printing out debug prints from the application.
1. In the Debug Adapters pane, right-click on the detected device and choose **Launch Console** to launch a console connection.

   **![Launch console](resources/readme/launch_console.PNG)**

2. J-link Silicon Labs console pane opens as shown below. Switch to the Serial 1 tab in the console pane, for viewing the debug prints from the application.

3. Hit Enter on the Serial 1 tab to establish a serial connection between PC and setup.

   **![Serial 1](resources/readme/serial1.PNG)**

### Execution of the project

1. To flash the code, right click on the project name and select **Debug As** → **Silicon Labs ARM Program**.

   **![Debug](resources/readme/debug_project.PNG)**

  - If the EFM32 has an older SE firmware, the following warning may pop up. Click **Yes** to continue.

    **![Warning](resources/readme/warning1.png)**

2. As soon as the debug process is completed, the application control branches to the main().

3. Go to the J-link Silicon Labs console pane to observe the debug prints in the Serial 1 tab.

4. Click the **Resume** icon in the Simplicity Studio IDE toolbar to run the project.

   **![Run](resources/readme/run.png)**

5. For case 1 (using microSD card), the following application prints are observed on the serial console.

   **NOTE**: If you do not see any application prints, re-install Simplicity Studio IDE and follow the steps from the [setting up the development environment](#Setting-up-the-development-environment) section.

   **![microSD Prints](resources/readme/sdcard_prints.PNG)**

   **![microSD Prints](resources/readme/sdcard_prints2.PNG)**

6. For case 2 (using internal Host MCU's flash), the following application prints are observed on the serial console.

   **![Flash Prints](resources/readme/flash_prints.PNG)**

   **![Flash Prints](resources/readme/flash_prints2.PNG)**

**NOTE**: 
1. In this example, the firmware file size is around 1.7 MB. 
2. The time taken for burning the firmware on to RS9116 using microSD card is ~68 seconds.
3. The time taken for burning the firmware on to RS9116 using EFM32 internal flash is ~32 seconds.

  
 

