# Out of Box Demo AWS - SiWG917

This application demonstrates the WLAN, BLE, MCU peripheral features and NWP (network processor) powersave capabilities of SiWG917 with a ready to go, minimal software installation experience.

## Table of Contents

- [Purpose/Scope](#purposescope)
- [Prerequisites/Setup Requirements](#prerequisitessetup-requirements)
  - [Hardware Requirements](#hardware-requirements)
  - [Software Requirements](#software-requirements)
  - [Setup Diagram](#setup-diagram)
- [Application Build Environment](#application-build-environment)
- [Getting Started](#getting-started)
  - [Application Configuration Parameters](#application-configuration-paraters)
  - [Run the Application](#run-the-application)
  - [Measuring the current consumption using Simplicity Studio Energy Profiler](#measuring-the-current-consumption-using-simplicity-studio-energy-profiler)

## Purpose/Scope

SiWG917 establishes WLAN connectivity via BLE provisioning. SiWG917 then proceeds to ping [www.silabs.com](https://www.silabs.com) for 5 times, after which MQTT connectivity with AWS broker is established.
Then Network Processor of SiWG917 enters connected sleep state.
Si917 interacts with the on-board Si7021 sensor to fetch real-time temperature values via I2C interface, and publishes to the cloud on the topic Si917_APP_STATUS until the device is disconnected from the access point. If a message is published onto the topic from a AWS MQTT client (here AWS Console GUI) to which the module is subscribed (Si917_MQTT_RECEIVE), the NWP wakes up and displays the received data and goes back to connected sleep state. The status of application is updated on the TFT-LCD display on the WPK baseboard, the same can be observed on the serial terminal prints as well.

**Overview of AWS SDK**

AWS IoT Core is a cloud platform which connects devices across AWS cloud services. AWS IoT provides a interface which allows the devices to communicate securely and reliably in bi-directional ways to the AWS touch-points, even when the devices are offline.

The AWS IoT Device SDK allow applications to securely connect to the AWS IoT platform.

## Prerequisites/Setup Requirements

- Before running the application, the user will need the following things to setup.

### Hardware Requirements

- Windows PC
- A Wireless Access point with internet connectivity
- **SoC Mode**:
  - Standalone
    - BRD4002A Wireless pro kit mainboard [SI-MB4002A]
    - Radio Boards
      - BRD4338A [SiWx917-RB4338A]
      - BRD4339B [SiWx917-RB4339B]
  - Kits
    - SiWx917 Pro Kit [Si917-PK6031A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pro-kit?tab=overview)
- Android Phone or iPhone with EFR Connect App, which is available in Play Store and App Store (or) Windows PC with windows Silicon labs connect application.

### Software Requirements

- [Simplicity Studio](https://www.silabs.com/developers/simplicity-studio)
- Silicon Labs [EFR Connect App](https://www.silabs.com/developers/efr-connect-mobile-app?tab=downloads), the app can be downloaded from Google Play store/Apple App store.
- AWS Console GUI provided by Silicon Labs (Path: $GSDK\extension\WSDK\examples\snippets\wlan_ble\out_of_box_aws\resources)

### Setup Diagram

![Figure: Setup Diagram for OOB Demo](resources/readme/oobdemo_soc.png)

## Application build environment

It is highly recommended to set the CLIENT_ID parameter in the wifi_app.c file to a unique string. As the test MQTT server limits multiple devices with same CLIENT ID from connecting at the same time.
In this version of the out of box demo example, the CLIENT_ID parameter is automatically set to a unique value.

## Getting Started

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-developers-guide-developing-for-silabs-hosts/) to:

- Install Studio and WiSeConnect 3 extension
- Connect your device to the computer
- Upgrade your connectivity firmware
- Create a Studio project
- Build the application.
- Flash, run and debug the application

### Application Configuration Parameters

The application can be configured to suit your requirements and development environment. Read through the following sections and make any changes needed.

  Open `wifi_app.c` file and update/modify following macros

- `SUBSCRIBE_TO_TOPIC` refers to the topic to which the device subscribes.
- `PUBLISH_ON_TOPIC` refers to the topic to which the device publishes.

```c
#define SUBSCRIBE_TO_TOPIC         "Si917_MQTT_RECEIVE"
#define PUBLISH_ON_TOPIC           "Si917_APP_STATUS"
```

  Open `aws_iot_config.h` file under config folder in project explorer pane and configure the below mentioned macros if needed. Further information on these are given in the Additional information section.

 ```c
   //AWS Host name 
   #define AWS_IOT_MQTT_HOST          "a25jwtlmds8eip-ats.iot.us-east-2.amazonaws.com"  

   //default port for MQTT
   #define AWS_IOT_MQTT_PORT          "8883"
   
   #define AWS_IOT_MQTT_CLIENT_ID     "silicon_labs_thing"
   
   // Thing Name of the Shadow this device is associated with 
   #define AWS_IOT_MY_THING_NAME      "silicon_labs_thing"    
```

**To Load Certificate**

Place the certificate files in `<SDK>/resources/certificates/` path and include the certificate files in wifi_app.c

   ```c
   Replace the default Device certificate and Private key certificate include in the application with the converted pem file name.

   // Certificate includes
   #include "aws_client_certificate.pem.crt.h"
   #include "aws_client_private_key.pem.key.h"

   Replace the default Device certificate and Private key certificate given in `sl_net_set_credential()` API in the application with the converted pem array.

   // Load Security Certificates
   status = sl_net_set_credential(SL_NET_TLS_SERVER_CREDENTIAL_ID(0), SL_NET_CERTIFICATE, aws_client_certificate, (sizeof(aws_client_certificate) - 1));
  
   status = sl_net_set_credential(SL_NET_TLS_SERVER_CREDENTIAL_ID(0), SL_NET_PRIVATE_KEY, aws_client_private_key, (sizeof(aws_client_private_key) - 1));
   ```

### Run the application

- Once the application is flashed, the Wireless interface is initialized.
- After the Wireless initialization, the module starts BLE advertising and advertises itself as "BLE_CONFIGURATOR"
- The status of the application can be observed on the TFT-LCD display on the WPK baseboard.
- Following is the image of LCD display indicating the application status.

![Here is the image of LCD with the status prints](resources/readme/status_lcd2.png)

- Open the EFR connect app on your mobile phone.

![EFR Connect App](resources/readme/EFRConnect_app.png)

- Go to 'Demo' tab.

![Demo tab](resources/readme/EFRConnect_demo.png)

- Choose the Wi-Fi commissioning option.

![WiFi Commisioning option](resources/readme/EFRConnect_wificommisioning.png)

- Click on the device name "BLE_CONFIGURATOR".

![BLE configurator device list](resources/readme/EFRConnect_bleconfigurator.png)

**NOTE:** For the Wi-Fi commissioning demo, EFR connect mobile app displays the devices which advertise as "BLE_CONFIGURATOR" only. This is to filter out the other Bluetooth devices in the vicinity and if the device name is modified, it won't be detected in the EFR connect mobile app for Wi-Fi commissioning demo.

- A list of the available Access points is displayed in the mobile app.

![AP list](resources/readme/EFRConnect_APlist.png)

- Choose the desired access point and enter the password.

![Enter PSK](resources/readme/EFRConnect_enterpsk.png)

- The module is now commissioned into the Wi-Fi network.

![Provisioning done](resources/readme/EFRConnect_done.png)

- The module starts to ping www.silabs.com.
- After pinging, the module now establishes connectivity with AWS MQTT broker.
- Here is the image of LCD with the status prints.

![Here is the image of LCD with the status prints](resources/readme/status_lcd1.png)

- NWP (network processor of SiWG917) enters connected sleep.

- Now subscribe to the topic 'Si917_APP_STATUS' in the AWS Console GUI to receive the messages published on that topic. The messages can be viewed under Subscribed Messages Window.

- Additionally, messages published from AWS Console GUI will be received by SiWG917 & viewed on the prints console & on LCD.


**AWS Console GUI**

**Note:** Ensure the PC is not connected to office network.

1. Click on the downloaded executable file to run the GUI.
2. Click on connect to establish AWS conectivity.
3. Enter the topic name you want to publish to and enter the message to publish in the Publish Window -> click on Publish.
4. Enter the topic name you want to subscribe to in the Subscribe Window to receive the messages published on that topic from any client -> click on Subscribe.

Here is the snapshot of the console,
![AWS Console GUI](resources/readme/aws_console_gui.png)

- Enter the data to be sent to the SiWG917 (can be any random text) in the publish section and click on the publish button, the online client then publishes data on the "Si917_MQTT_RECEIVE" topic which can be observed in the application prints.

- The current consumption of SiWG917 can observed using the energy profiler tool integrated within the Simplicity Studio. Refer the [Measuring the current consumption using Simplicity Studio Energy Profiler](#measuring-the-current-consumption-using-simplicity-studio-energy-profiler) section for using the energy profiler ti measure the current consumption of the module.

The energy consumption plot would be as follows:

![AEM plot M4 sleep disabled](resources/readme/AEM_graph2.png)

Please note that

- Current consumption would be in the range of 8mA-9mA since M4 is in active state.
- The above energy consumption plots are for reference and the power consumption number would vary based on the on air traffic, environment and the access point used.

Refer the following ouput image:

- To view the prints on the console, follow the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-developers-guide-developing-for-silabs-hosts/#console-input-and-output).

The application prints would be as follows:

![Application prints](resources/readme/output_1.png)
![Application prints](resources/readme/output_2.png)

### Measuring the current consumption using Simplicity Studio Energy Profiler

To open the energy profiler:

- Open the "Tools" section from the toolbar.

  ![Studio tools section](resources/readme/oob_tools_section.png)

- Choose the "Energy Profiler" from the tools dialog box.

  ![Studio tools section](resources/readme/oob_tools_dialog_box.png)

- Click on the "Quick Access" option and choose "Start Energy Capture" option.

  ![Studio tools section](resources/readme/oob_energy_profiler_quick_access.png)

- Click on the device name and click on OK.

  ![Studio tools section](resources/readme/oob_energy_profiler_start.png)

- The energy profiler session begins and the current consumption plot can be observed.
