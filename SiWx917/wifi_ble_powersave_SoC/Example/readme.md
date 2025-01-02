# BLE Wi-Fi Provisioning with AWS IoT MQTT

## Table of Contents
  - [Purpose / Scope](#purpose--scope)
  - [Prerequisites / Setup Requirements](#prerequisites--setup-requirements)
    - [Hardware Requirements](#hardware-requirements)
    - [Software Requirements](#software-requirements)
  - [Getting Started](#getting-started)
    - [Application Configuration Parameters](#application-configuration-parameters)
    - [Configure the below parameters in `aws_iot_config.h` file present at `<project>/config`](#configure-the-below-parameters-in-aws_iot_configh-file-present-at-projectconfig)
  - [Test the Application](#test-the-application)
    - [Application Output](#application-output)
  
## Purpose / Scope

In this application, the SiWG917 is configured to work in power save mode, acts as a Wi-Fi station and connects to the AWS cloud via MQTT. After the connection is established, it subscribes to MQTT_TOPIC1. The application then publishes a message to the cloud on MQTT_TOPIC2, and thereafter the SiWx917 is put into Associated Power Save mode.Then after the BLE is enabled.

## Soc Mode:

Si917 connected to LM75 Temperature Sensor via I2C interface collects real time temperature data publishes to the cloud until the device is disconnected from the access point. After publish, the NWP processor is set into associated power save. Next, the application works differently in NCP and SoC modes as defined below.  

If macro **SL_SI91X_TICKLESS_MODE** enabled, then the M4 processor is set in sleep mode. The M4 processor can be woken in several ways as mentioned below:

### Tickless Mode

In Tickless Mode, the device enters sleep based on the idle time set by the scheduler. The device can be awakened by these methods: SysRTC, a wireless signal, Button press-based (GPIO), and Alarm-based wakeup.

- **SysRTC (System Real-Time Clock)**: By default, the device uses SysRTC as the wakeup source. The device will enter sleep mode and then wake up when the SysRTC matches the idle time set by the scheduler.

- **Wireless Wakeup**: The device can also be awakened by a wireless signal. If this signal is triggered before the idle time set by the scheduler, the device will wake up in response to it.

- **Button-based Wakeup**:The device can also be awakened by a button signal.

- **Alarm-based Wakeup**:The device can also be awakened by setting the timeout to the appropriate duration in the osSemaphoreAcquire function.

After M4 processor wakes up via any of the above processes, the application publishes the **MQTT_publish_QOS0_PAYLOAD** message on the **MQTT_TOPIC2** topic.

If macro **SL_SI91X_TICKLESS_MODE** is disabled, then the M4 processor does not go to sleep. A timer is run with a periodicity of **PUBLISH_PERIODICITY** milliseconds. The application publishes the **MQTT_publish_QOS0_PAYLOAD** message on the **MQTT_TOPIC2** topic in the following cases:

- Once in every **PUBLISH_PERIODICITY** time period.
- When an incoming publish is received by the application.



**NOTE:** The bold texts are the macros defined in the application. You can find more details about them in the [Application Configuration Parameters](#application-configuration-parameters)

## Prerequisites / Setup Requirements

### Hardware Requirements

- A Windows PC 
- USB-C cable
- A Wireless Access Point (which has an active internet access)
- LM75 Temperature Sensor (in-built sensor available on WSDK/WPK board)
- **SoC Mode**:
  - Standalone
    - BRD4002A Wireless pro kit mainboard [SI-MB4002A]
    - Radio Boards 
     - BRD4338A [SiWx917-RB4338A]
     - BRD4339B [SiWx917-RB4339B]
     - BRD4340A [SiWx917-RB4340A]
  	 - BRD4343A [SiWx917-RB4343A]
  - Kits
   - SiWx917 Pro Kit [Si917-PK6031A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pro-kit?tab=overview)
   - SiWx917 Pro Kit [Si917-PK6032A]
   

### Software Requirements

- Simplicity Studio 
- Serial terminal for viewing the print [Tera term](https://tera-term.en.softonic.com/)


## Getting Started

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

- Install Simplicity Studio and WiSeConnect 3 extension
- Connect your device to the computer
- Upgrade your connectivity firmware
- Create a Studio project

For details on the project folder structure, see the [WiSeConnect Examples](https://docs.silabs.com/wiseconnect/latest/wiseconnect-examples/#example-folder-structure) page.

### Application Configuration Parameters

The application can be configured to suit your requirements and development environment. Read through the following sections and make any changes needed.


 Open `wifi_app.c` file and update/modify the following macros:

   Modify the MQTT topics and give different names for both topics the SiWx917 is subscribed to: MQTT_TOPIC1 and publishing to MQTT_TOPIC2. 
   MQTT web application is subscribed to `MQTT_TOPIC2` and publishing on `MQTT_TOPIC1`.

```c
#define MQTT_TOPIC1               "aws_status"   //! Subscribe topic to receive the message from cloud
#define MQTT_TOPIC2               "si91x_status" //! Publish topic to send the status from application to cloud
#define COEX_SSID                  "Silabs_AP" //! Configure SSID of the third party AP.
#define COEX_PASSWORD              "12345678"   //! Configure password
#define COEX_SECURITY_TYPE    		"2"			//! 0-Open, 1-WPA, 2-WPA2
```

   **NOTE:** You can change the topic names, which are `aws_status` and `si91x_status`.

  

```c
#define ENABLE_POWER_SAVE         1                 //! Set this macro to 1 for enabling NWP power save.Wi-Fi

#define PUBLISH_PERIODICITY       osWaitForever          // Configure this macro to publish data every 30 seconds (this works only in NCP with and without POWERSAVE and in SOC without POWERSAVE).
```
Open `ble_app.c` file and update/modify following macros:



   **Configuring the BLE Application**

   `RSI_BLE_CHAR_SERV_UUID` refers to the attribute type of the characteristics to be added in a service.

```c

#define ENABLE_POWER_SAVE         1                 //! Set this macro to 1 for enabling NWP power save. BLE

#define RSI_REMOTE_DEVICE_NAME 		"Remote_device"        //! Remote Device Name to connect

#define RSI_BLE_APP_DEVICE_NAME 		"917_ble"			//! BLE Device Name 

```

### Configure the following parameters in `aws_iot_config.h` file present at `<project>/config`

> - Before configuring the parameters in `aws_iot_config.h`, register the SiWx917 device in the AWS IoT registry by following the steps mentioned in [Create an AWS Thing](#create-an-aws-thing) section.
>
> - Configure AWS_IOT_MQTT_HOST macro with the device data endpoint to connect to AWS. To get the device data endpoint in the AWS IoT Console, navigate to Settings, copy the Endpoint, and define the AWS_IOT_MQTT_HOST macro with this value.



 ```c
   //AWS Host name 
   #define AWS_IOT_MQTT_HOST          "a2m21kovu9tcsh-ats.iot.us-east-2.amazonaws.com"  

   //default port for MQTT
   #define AWS_IOT_MQTT_PORT          "8883"
   
   #define AWS_IOT_MQTT_CLIENT_ID     "silicon_labs_thing"
   
   // Thing Name of the Shadow this device is associated with 
   #define AWS_IOT_MY_THING_NAME      "silicon_labs_thing"    
```
> - To authenticate and securely connect with AWS, the SiWx917 device requires a unique x.509 security certificate and private key, as well as a CA certificate. At this point, you must have a device certificate, private key, and CA certificate, which are downloaded during the creation/registration of AWS Thing.
> 
> - By default, the device certificate and private key that are downloaded from the AWS are in [.pem format](https://en.wikipedia.org/wiki/Privacy-Enhanced_Mail). To load the device certificate and private key to the SiWx917, the device certificate and private key should be converted into a C-array. For converting the certificates and private key into a C-array, refer to [Setting up Security Certificates](#setting-up-security-certificates).
>
> - By default, the WiSeConnect 3 SDK contains the Starfield Root CA Certificate in C-array format. 

> **Note** :
 The included Cloud connectivity certificates are for reference only. If using default certificates in the release, the cloud connection will not work. You must replace the default certificates with valid certificates while connecting to the appropriate Cloud/OpenSSL Server.

## Test the Application

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

- Build the application.
- Flash, run, and debug the application.

Follow the steps below for successful execution of the application:

1. Configure the access point in OPEN/WPA-PSK/WPA2-PSK/WPA3 mode to connect the SiWx917 in STA mode.
2. Connect any serial console for prints.
3. Initializes the device and loads the required certificates.
4. Enters Power-save mode.
5. User has to Hit BTN0, to wake up the device.
6. The Wi-Fi scanning is done.
7. Enters Power-save mode.
8. User has to Hit BTN0, to wake up the device.
9. 917 STA connects to AP and fetches the IP address.
10. Enters Power-save mode.
11. User has to Hit BTN0, to wake up the device.
12. Connects to the AWS Mqtt server, subscribes, and publishes.
13. Enters Power-save.
14. User has to Hit BTN0, to wake up the device.
15. MQTT disconnection is done followed by Wi-Fi AP disconnection.
16. Enters Power save mode.
17. User has to Hit BTN0, to wake up the device.
18. BLE is enabled and starts advertising.
19. Scan from a mobile phone or some ble device to connect with 917 device. The 917 is advertised with "RSI_BLE_APP_DEVICE_NAME" in ble_app.c.
20. Connect the Mobile phone to 917, after 5 seconds, the 917 will automatically disconnect the mobile phone.
21. The 917 device will be scanning for the mobile/BLE device. Make sure to configure "RSI_REMOTE_DEVICE_NAME" in ble_app.c file. RSI_REMOTE_DEVICE_NAME means the name of the BLE peripheral that the 917 wants to connect to.
22. After 5 seconds, the 917 disconnects the peripheral device.
23. If the peripheral is turned off, Steps 19 to 22 will be repeated.
24. If the peripheral device is still in the ON state, steps 20 to 22 will be repeated.


**Note:**
- To learn more about aws mqtt apis error codes, refer to the wiseconnect3\third_party\aws_sdk\include\aws_iot_error.h file.
- If the user is calling select and experiencing long wait times, and if no data is received, it is the user's responsibility to manage sending the keepalive packets to maintain the connection.
  

  
