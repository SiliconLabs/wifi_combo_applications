# SI91x - SL_I2S_LOWPOWER

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

- This application demonstrates ULP_I2S will be configured in PS2 state and master mode.Here we will connect TX pin to RX pin in loopback mode
- Master transmits data on TX pin using DMA and receives same data on RX pin using DMA
- The data received should match with the transmitted data

## Overview

- The I2S_2CH supports two stereo channels while the ULP_I2S and the NWP/Security subsystem I2S support one stereo channel
- Programmable Audio data resolutions of 12, 16, 20, 24 and 32 bits
- Supported audio sampling rates are 8, 11.025, 16, 22.05, 24, 32, 44.1, 48, 88.2, 96 and 192 kHz
- Support for Master and Slave modes
- Full duplex communication due to the independence of transmitter and receiver
- Programmable FIFO thresholds with maximum FIFO depth of 8 and support for DMA
- Supports generation of interrupts for different events

## About Example Code

- This example fetches current I2S version using sl_si91x_i2s_get_version()
- Initializes I2S peripheral and store driver handle in i2s_driver_handle using sl_si91x_i2s_init()
- Get the transfer status of I2S peripheral using sl_si91x_i2s_get_status()
- Configure ARM power mode to full power using sl_si91x_i2s_configure_power_mode()
- Register user callback using sl_si91x_i2s_register_event_callback()
- Configure transmitter and receiver transfer parameters for i2s using sl_si91x_i2s_transmit_receive_config()
- Configure receive DMA channel and prepare I2S for data receiving
- Configure transmit DMA channel and send data
- When send data is received by receiver channel, it compares the data received with transferred data
- Loopback test will be pass after successful data comparison.

**Note!** Make sure to use non-ROM I2S APIs for this application and SL_I2S driver.

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

- Configure UC from the slcp component.

   ![Figure: Introduction](resources/uc_screen/i2s_lowpower_uc_screen.png)

- Open **sl_si91x_i2s_low_power.slcp** project file select **software component** tab and search for **I2S** in search bar.
- Using configuration wizard one can configure different parameters like:

### General Configuration

- I2S resolution: I2S resolution can be configured through this macro,valid resolution values are 12, 16, 20, 24 and 32 bit.
- I2S sampling rate: I2S sampling rate can be configured through this macro,valid sampling rate values are
    8kHz, 11.025kHz, 16kHz, 22.05kHz, 24kHz, 32kHz, 44.1kHz, 48kHz, 88.2kHz, 96kHz and 192kHz
- Configuration files are generated in **config folder**, if not changed then the code will run on default UC values.

- Configure the following macros in i2s_lowpower_example.c file and update/modify following macros if required.

```C
#define BUFFER_SIZE 1024    ///< I2S transfer size
```

### Pin Configuration

| GPIO pin                | Description             |
| ----------------------- | ----------------------- |
| ULP_GPIO_1 [P16]           | I2S DOUT          |
| ULP_GPIO_6 [EXP_HEADER-16]           | I2S DIN           |

### Pin Description

**Note!** Make sure pin configuration in RTE_Device_917.h file.(path: /$project/config/RTE_Device_917.h)

## Test the Application

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

1. Connect DOUT to DIN for loopback connection.
2. Compile and run the application.
3. When the application runs,It sends and receives data in loopback.
4. Data send/receive success and data comparison success prints can be seen on serial console.
5. After successful program execution the prints in serial console looks as shown below.

   >![output](resources/readme/output_i2s_low_power.png)

## Important Note for using this project with wiseconnect SDK

SL_I2S_LOWPOWER application is being migrated to the Power Manager, this application will be available in the wiseconnect SDK from next release.
To use the application now, follow below steps:-
**Step 1:-**
Copy sl_si91x_i2s_low_power folder into Gecko SDK Suite i.e. gecko-sdk/extension/wiseconnect3/examples/si91x_soc/peripheral/ folder.
**Step 2:-**
Add the example details in the gecko-sdk/extension/wiseconnect3/wifi_templates.xml like below
```
  </descriptors>
  </descriptors>
<descriptors name="sl_si91x_i2s_low_power" label="Si91x - SL_I2S_LOWPOWER" description="Demonstrates how to use the I2S loopback on the Si91x SoC device in low power.">
    <properties key="namespace" value="template.uc"/>
    <properties key="keywords" value="universal\ configurator"/>
    <properties key="projectFilePaths" value="examples/si91x_soc/peripheral/sl_si91x_i2s_low_power/sl_si91x_i2s_low_power.slcp"/>
    <properties key="boardCompatibility" value="com.silabs.board.none brd4325a brd4325b brd4325c brd4325e brd4325f brd4325g brd4338a brd4343a brd4343b brd4339b brd4340b"/>
    <properties key="partCompatibility" value="mcu.arm.efr32.mg21.* mcu.arm.efr32.mg21.*  mcu.arm.efr32.mg24.* .*si917.* .*si917.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m141xgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917m111mgtba.* .*siwg917y111mgab.* .*siwg917y111mgnb.*"/>
    <properties key="ideCompatibility" value="makefile-ide simplicity-ide visual-studio-code generic-template"/>
    <properties key="toolchainCompatibility" value="gcc"/>
    <properties key="quality" value="production"/>
    <properties key="category" value="Example|Wi-Fi"/>
    <properties key="stockConfigCompatibility" value="com.silabs.ss.framework.project.toolchain.core.default"/>
    <properties key="sdkAndProtocolTags" value=""/>
    <properties key="readmeFiles" value="examples/si91x_soc/peripheral/sl_si91x_i2s_low_power/readme.md"/>
	<properties key="filters" value="Device\ Type|SoC"/>
  </descriptors>
```
**Step 3:-**
Update the Gecko-SDK version and wiseconnect SDK version in the sl_si91x_i2s_low_power.slcp
In the Simplicity Studio click on prefernces -> search for sdk -> ![Know the SDK version and wiseconnect Version from Simplicity Studio](resources/uc_screen/i2s_know_sdk_version_slcp.png)
Update the GSDK and wiseconnect SDK in ![slcp](resources/uc_screen/i2s_change_version_slcp.png)
**Step 4:-**
Refresh the SDK in the Simplicity Studio by clicking on the preferences refer ![Simplicity Studio refresh SDK](resources/uc_screen/i2s_refresh_sdk.png)
**Step 5:-**
Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) for getting started with example.
