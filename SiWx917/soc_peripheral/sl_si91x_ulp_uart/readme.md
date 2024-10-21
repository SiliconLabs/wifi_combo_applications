# SL ULP UART

## Table of Contents

- [Purpose/Scope](#purposescope)
- [Overview](#overview)
- [About Example Code](#about-example-code)
- [Prerequisites/Setup Requirements](#prerequisitessetup-requirements)
  - [Hardware Requirements](#hardware-requirements)
  - [Software Requirements](#software-requirements)
  - [Setup Diagram](#setup-diagram)
- [Getting Started](#getting-started)
- [Application Build Environment](#application-build-environment)
- [Test the Application](#test-the-application)

## Purpose/Scope

- This application demonstrates how to configure ULP UART In asynchronous mode, it will send and receive data in loopback mode

## Overview

- ULP UART is used in communication through wired medium in Asynchronous fashion. It enables the device to
  communicate using serial protocols
- This application is configured with following configs
  - Tx and Rx enabled
  - Asynchronous mode
  - 8 Bit data transfer
  - Stop bits 1
  - No Parity
  - No Auto Flow control
  - Baud Rates - 115200

## About Example Code

- \ref ulp_uart_example.c this example code demonstrates how to configure the uart to send and receive data in loopback mode
- In this example, first uart get initialized if it's not initialized already with clock and dma configurations if dma is
  enabled using \ref sl_si91x_uart_init
- After uart initialization ,sets the uart power mode using \ref sl_si91x_uart_set_power_mode() and configures the UART
  with default configurations from UC and uart transmit and receive lines using \ref sl_si91x_uart_set_configuration()
- Then register's user event callback for send ,receive and transfer complete notification using
  \ref sl_si91x_uart_register_event_callback()
- After user event callback registered data send and receive can happen through \ref sl_si91x_uart_send_data() and
  \ref sl_si91x_uart_receive_data() respectively.

## Prerequisites/Setup Requirements

### Hardware Requirements

- Windows PC
- Silicon Labs Si917 Evaluation Kit [WPK(BRD4002) + BRD4338A]

### Software Requirements

- Simplicity Studio
- Serial console Setup
  - The Serial Console setup instructions are provided below:
Refer [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/getting-started-with-soc-mode#perform-console-output-and-input-for-brd4338-a).

### Setup Diagram

![Figure: Introduction](resources/readme/setupdiagram.png)

## Getting Started

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

- Install Studio and WiSeConnect 3 extension
- Connect your device to the computer
- Upgrade your connectivity firmware
- Create a Studio project

## Application Build Environment

- - Enable the ULP_UART mode in UC before running/flashing the code.

   ![Figure: Introduction](resources/readme/image513d.png)

- Remove DEBUG_UART macro from preprocessor window
- Data send and receive from vcom console is for one iteration. To check continuous data transfer modify below macro to ENABLE in ulp_uart_example.h file.

  ```c
  #define USE_SEND    ENABLE
  ```

- Loopback ULP_GPIO_2 to ULP_GPIO_11 to observe continuous toggle, once data sent and received matches.

## Pin Configuration 

|         GPIO pin    |Description|
|   --------------    | --------- |
|ULP_GPIO_11  [F6]    |    TX     |
|ULP_GPIO_9   [F7]    |    RX     |
|ULP_GPIO_10 [P17]    |GPIO_Toggle|

## Test the Application

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

- Build the SL ULP UART example in Studio.
- Flash, run and debug the application

Follow the steps below for successful execution of the application:

1. When the application runs,ULP_UART sends and receives data in full duplex mode
2. When tx and rx data both are matching ULP_GPIO_10 should be toggled ,connect logic analyzer to observe the toggle state. 
3. Here same PINS which are used to send and receive the data where used for data transfer so we cannot able to see prints 
 we can use GPIO toggling instead like shown below.
 - when use send disabled:

   ![output](resources/readme/ulp_uart_gpio_toggle.png)

- when use send enabled:

   ![output](resources/readme/ulp_uart_continuous_toggling.png)


 
> **Note:**
>
>- This application is executed from RAM.
>- In this application while changing the MCU mode from PS4 to PS2, M4 flash will be turned off.
>- The debug feature of Simplicity Studio will not work after M4 flash is turned off.
>- After Flashing ULP examples as M4 flash will be turned off,flash erase does not work.
>- To Erase the chip follow the below procedure
>- Press ISP and RESET button at same time and then release, now perform Chip erase through commander.

## Important Note for using this project with wiseconnect SDK

sl_si91x_ulp_uart application is being migrated to the Power Manager, this application will be available in the wiseconnect SDK from next release.
To use the application now, follow below steps:-
**Step 1:-**
Copy sl_si91x_ulp_uart folder into Gecko SDK Suite i.e. gecko-sdk/extension/wiseconnect3/examples/si91x_soc/peripheral/ folder.
**Step 2:-**
Add the example details in the gecko-sdk/extension/wiseconnect3/wifi_templates.xml like below
```
<descriptors name="sl_si91x_ulp_uart" label="Si91x - SL_ULP_UART" description="Demonstrates how to to use ULP Uart.">
    <properties key="namespace" value="template.uc"/>
    <properties key="keywords" value="universal\ configurator"/>
    <properties key="projectFilePaths" value="examples/si91x_soc/peripheral/sl_si91x_ulp_uart/sl_si91x_ulp_uart.slcp"/>
    <properties key="boardCompatibility" value="com.silabs.board.none brd4325a brd4325b brd4325c brd4325e brd4325f brd4325g brd4338a brd4343a brd4343b brd4339b brd4340b"/>
    <properties key="partCompatibility" value="mcu.arm.efr32.mg21.* mcu.arm.efr32.mg21.*  mcu.arm.efr32.mg24.* .*si917.* .*si917.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m141xgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917y111mgab.* .*siwg917y111mgnb.*"/>
    <properties key="ideCompatibility" value="makefile-ide simplicity-ide visual-studio-code generic-template"/>
    <properties key="toolchainCompatibility" value="gcc"/>
    <properties key="quality" value="production"/>
    <properties key="category" value="Example|Wi-Fi"/>
    <properties key="stockConfigCompatibility" value="com.silabs.ss.framework.project.toolchain.core.default"/>
    <properties key="sdkAndProtocolTags" value=""/>
    <properties key="readmeFiles" value="examples/si91x_soc/peripheral/sl_si91x_ulp_uart/readme.md"/>
	<properties key="filters" value="Device\ Type|SoC"/>
  </descriptors>
```
**Step 3:-**
Update the Gecko-SDK version and wiseconnect SDK version in the sl_si91x_ulp_uart.slcp
In the Simplicity Studio click on prefernces -> search for sdk -> ![Know the SDK version and wiseconnect Version from Simplicity Studio](resources/uc_screen/uart_know_sdk_version_slcp.png)
Update the GSDK and wiseconnect SDK in ![slcp](resources/uc_screen/uart_change_version_slcp.png)
**Step 4:-**
Refresh the SDK in the Simplicity Studio by clicking on the preferences refer ![Simplicity Studio refresh SDK](resources/uc_screen/uart_refresh_sdk.png)
**Step 5:-**
Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) for getting started with example.