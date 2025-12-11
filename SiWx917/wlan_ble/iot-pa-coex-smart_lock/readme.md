# SoC - Smart Lock

## Table of Contents
  - [Overview](#overview)
  - [Prerequisites / Setup Requirements](#prerequisites--setup-requirements)
    - [Hardware Requirements](#hardware-requirements)
    - [Software Requirements](#software-requirements)
    - [Setup Diagram](#setup-diagram)
  - [Getting Started](#getting-started)
    - [Software Components Installed](#software-components-installed)
  - [Key Features](#key-features)
  - [Wakeup Sources](#wakeup-sources)
  - [Application Configuration Parameters](#application-configuration-parameters)
  - [Test the Application](#test-the-application)
    - [Application Output](#application-output)
    - [MQTT Connection](#mqtt-connection)
  - [Additional Information](#additional-information)
    - [Current Measurement using Simplicity Studio Energy Profiler](#current-measurement-using-simplicity-studio-energy-profiler)
    - [Setting up Security Certificates](#setting-up-security-certificates)
    - [Create an AWS Thing](#create-an-aws-thing)
  
## Overview

This multiprotocol smart-lock application leverages the SiWG917 operating simultaneously in Wi-Fi Station and Bluetooth Low Energy (BLE) modes. The device is initially provisioned over BLE using the SiConnect mobile application, through which the network credentials are securely transferred. Once configured, the SiWG917 connects to the cloud over Wi-Fi using the MQTT protocol.

After establishing the MQTT connection, the device subscribes to a designated topic for receiving lock and unlock requests. To optimize power consumption, the SiWG917’s Network Wireless Processor (NWP) transitions into Associated Power Save mode, while the MCU enters sleep mode. Even in this low-power state, the system remains responsive to MQTT commands issued from the cloud.

When lock or unlock commands are received, the device wakes as needed to actuate the smart-lock mechanism. Following each operation, the updated lock status is published back to the cloud, ensuring real-time synchronization with the user’s application or management platform.

This reference application demonstrates a real‑time smart‑lock system and integrates the key software components, drivers, and event handling required for a complete end‑to‑end solution. It serves as a starting point for prototyping and developing production smart‑lock applications by illustrating typical workflows for connectivity, power management, persistent storage, and user interaction.


## Prerequisites / Setup Requirements

### Hardware Requirements

- A Windows PC 
- [SiWG917 Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pro-kit?tab=overview) (Or) [SiWG917-DK2605A Dev Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit?tab=overview)

- USB-C cable for flashing the firmware image
- Wireless Access point (with internet access)
- Mini Simplicity Connector
- Android Phone or iPhone with [Simplicity Connect](https://www.silabs.com/software-and-tools/simplicity-connect-mobile-app?tab=downloads) App, which is available in Play Store and App Store.

### Software Requirements

- [Simplicity Studio](https://www.silabs.com/software-and-tools/simplicity-studio)
- [Si Connect mobile Application](https://www.silabs.com/software-and-tools/simplicity-connect-mobile-app)
- [Serial Console](https://docs.silabs.com/wiseconnect/latest/wiseconnect-developers-guide-developing-for-silabs-hosts/using-the-simplicity-studio-ide#console-input-and-output), Teraterm or Docklight
- [OpenSSL](https://openssl-library.org/source/old/3.5/) version 3.5.2 to generate aws certificate.
- Smart Lock application is created using Wiseconnect SDK 3.5.1

### Setup Diagram
  ![Setup diagram of WLAN Station BLE Provisioning with AWS cloud ](resources/readme/Smartlock_functionality.png)

## Getting Started

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-starte/) to:

- Install Simplicity Studio and WiSeConnect 3 extension
- Connect your device to the computer
- Upgrade your connectivity firmware
- .sls file is in **/project** folder, import the .sls file into the Simplicity Studio.

To know how to import a .sls file refer [here](https://docs.silabs.com/simplicity-studio-5-users-guide/1.0/about-the-simplicity-ide/import-and-export/)

## Key Features

- BLE provisioning via Simplicity Connect App
- Wi-Fi station mode with MQTT cloud connectivity (AWS IoT Core or Adafruit)
- Remote lock/unlock/erase via cloud commands
- Local control via BLE and physical button (BTN0)
- Voice control through Google Assistant (Adafruit/IFTTT)
- Si70xx temperature sensor integration (I2C)
- Ultra-low-power operation
- Wakeup via SysRTC, Wireless signal, or GPIO interrupt

## Wakeup Sources

- **SysRTC (System Real-Time Clock)**: Device enters sleep and wakes when SysRTC matches the idle time set by the scheduler. Timeout can be configured in 'wakeup_time_ms' in **wifi_app.c**.
- **Wireless Wakeup**: Device wakes in response to a wireless signal before the scheduled idle time.
- **Button Wakeup**: Device wakes on a button interrupt (BTN0).

After Wake up, the application processes incoming MQTT commands ("lock", "unlock", "erase") and executes the requested action. If no command is received, for every wakeup(as configured) SiWG917 sends ping message to the MQTT broker to maintain connectivity.

**NOTE:** Macro definitions and configuration details are in [Application Configuration Parameters](#application-configuration-parameters).

### Software Components Installed

| Component                   | Purpose                                    |
| --------------------------- | ------------------------------------------ |
| **NVM3**                    | Non-volatile storage for Wi-Fi credentials |
| **PM UULP Wireless Wakeup** | Enables GPIO and wireless wake-up sources  |
| **I2C (instance: i2c2)**    | Si70xx temperature sensor         |
| **PM ULP / PM UULP GPIO**   | Used for wake-up and control               |
| **MQTT Client**             | Adafruit Cloud connectivity                |
| **LED (instance: led0)**    | Status indication for lock state           |
| **PM ULP SSI**              | Enable  SSI in the low-power mode for LCD display |
| **GLIB Graphics Library**   | GLIB Graphics Library |
| **GLIB driver for Memory LCD** | GLIB dot matrix display driver that support memory LCD displays |
| **Memory LCD SPI driver**      | Driver implementation to drive the LCD device |
| **Monochrome sharp memory lcd** | Driver for Sharp Memory LCD LS013B7DH03 |
| **ULP timer (instance: timer0)** | Timer used to re-initialize MEMLCD interface after sleep wakeup |

### Application Configuration Parameters

The application can be configured to suit your requirements and development environment. Read through the following sections and make any changes needed.

1. In `ble_app.c` file `RSI_BLE_APP_DEVICE_NAME` refers to the name of the Silicon Labs device to appear during scanning by remote devices. Use the same name as mentioned.

    ```c
    #define  RSI_BLE_APP_DEVICE_NAME               "BLE_CONFIGURATOR"
    ```

2. Open `app.h` file and update/modify following macros :
    ```c
    // Enable this macro only if you have a reference PCB with Motor 
    #define SILABS_SMARTLOCK_HARDWARE   0   

    //Enable this macro only if BRD4002A Mainboard and BRD4338A radio board is used
    #define DISPLAY           1             
    
    //User can choose to connect to either AWS server or Adafruit Server
    #define AWS                       1
    #define ADAFRUIT                  0
    ```
    
3. Open `wifi_app.h` file and update/modify following macros:

    ```c
    #define ENABLE_POWER_SAVE          1                 //! Enable this macro to run application with power save mode.
    #define I2C_SENSOR_PERI_ENABLE     0                 //! Enable this macro to read temperature data from Si70xx sensor and publish it periodically to the cloud
    #define PUBLISH_PERIODICITY       (30000)            // Configure this macro to publish temperature data(if present) every 30 seconds if **ENABLE_POWER_SAVE** is disabled
    ```

  If **AWS** is enabled, follow the below steps:

  - SiWG917 is subscribed to `SUBSCRIBE_TO_TOPIC` and publishing to `PUBLISH_ON_TOPIC`. Modify the MQTT topics and give different names as per your choice.

  - QoS 0 and Qos 1 are supported.

  - MQTT web application is subscribed to topic `PUBLISH_ON_TOPIC` and publishing on `SUBSCRIBE_TO_TOPIC`.

    ```c
    #define SUBSCRIBE_TO_TOPIC   "aws_requests"   //! Topic to receive the message from cloud
    #define PUBLISH_ON_TOPIC     "si91x_status"   //! Topic to send the status from application to cloud
    #define LWT_TOPIC            "si91x_LWT"      //! Topic to send the device disconnected state to other clients
    #define SUBSCRIBE_QOS             QOS0        //! Quality of Service for subscribed topic "SUBSCRIBE_TO_TOPIC"
    #define PUBLISH_QOS               QOS0        //! Quality of Service for publish topic "PUBLISH_ON_TOPIC"
    ```
  If **ADAFRUIT** is enabled, please refer to the [Controlling_Smart_Lock_with_Voice_Commands.md](Controlling_Smart_Lock_with_Voice_Commands.md) file to create an Adafruit account and for IFTTT integration to send lock/unlock voice commands over Google Assistant service.

4. Open `aws_iot_config.h` file and update/modify following macros:

 - Before configuring the parameters, register the SiWG917 device in the AWS IoT registry by following the steps mentioned in [Create an AWS Thing](#create-an-aws-thing) section.

 - Configure `AWS_IOT_MQTT_HOST` macro with the device data endpoint to connect to AWS. For getting the device data endpoint, in the AWS IoT Console navigate to Settings and copy the Endpoint.

  ```c
    //AWS Host name 
    #define AWS_IOT_MQTT_HOST          "xxxxxxxxxxxxxx-xxx.iot.us-east-2.amazonaws.com"  

    //default port for MQTT
    #define AWS_IOT_MQTT_PORT          "8883"
    
    #define AWS_IOT_MQTT_CLIENT_ID     "silicon_labs_thing"
    
    // Thing Name of the Shadow this device is associated with 
    #define AWS_IOT_MY_THING_NAME      "silicon_labs_thing"    
  ```
 - To authenticate and securely connect with AWS, the SiWG917 device requires a unique x.509 security certificate and private key, as well as a CA certificate. At this point, you must be having device certificate, private key and CA certificate which are downloaded during the creation/registration of AWS Thing.
 
 - By default the device certificate and private key that are downloaded from the AWS are in [.pem format](https://en.wikipedia.org/wiki/Privacy-Enhanced_Mail). To load the device certificate and private key to the SiWG917, the device certificate and private key should be converted into a C-array. For converting the certificates and private key into C-array refer to [Setting up Security Certificates](#setting-up-security-certificates).

 - By default the WiSeConnect 3 SDK contains the Starfield Root CA Certificate in C-array format. 

## Test the Application

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-developers-guide-developing-for-silabs-hosts/) to:

- Build the application.
- Flash, run and debug the application

Follow the steps below for successful execution of the application:

1. Configure the Access point in OPEN/WPA-PSK/WPA2-PSK/WPA3 mode to connect the SiWG917 in STA mode.

2. Connect any serial console for prints.

3. On power up, SiWG917 enters BLE advertising mode. User can click on **BTN0** to perform lock/unlock operations to control the Smart Lock device. During this, the SiWG917 sends notifications to Si Connect App everytime the state of the **Smart Lock** changes. 

4. Launch the **Si Connect** App. Click on Demo and select **Smart Lock** to continue with the BLE provisioning process.

    ![Smart Lock UI](resources/readme/Smartlock_UI.png)

5. It will scan for the module, and it appears as `BLE_CONFIGURATOR` on the UI, select as shown below.

   ![](resources/readme/dut_scan_result1.png)

6. You can observe Bluetooth icon on the Mainboard LCD, which indicates BLE connection. Now, SiWG917 module will start as a station and scan for the access points (AP) nearby.

    ![](resources/readme/only_ble.png)

7. Once the list of AP scan results is displayed on the screen, you can select the SSID of the AP to which you want to connect.

   ![](resources/readme/remote_screen2.png)

8. Click on the SSID of the AP, enter password if the AP is in security mode. Click on connect to associate with the access point.

   ![](resources/readme/remote_screen3.png)

9. Once Silicon labs module is connected to the access point, you can observe Wi-Fi icon along with Bluetooth icon on the Mainboard LCD.

    ![](resources/readme/Both.png)

10. At this point, the Access Point details are stored in NVM3 flash memory. This completes the BLE provisioning, next step is the [MQTT Connection](#mqtt-connection).

11. Next time on power up, SiWG917 module uses the credentials stored in NVM3 flash to connect to AP. BLE will be advertising even after commissioning to control **Smart Lock**.

- If `BRD2605A` devkit is used, tri color led is configured in the application to visualise different status:

  | Event           | Source             | LED Color |
  | --------------- | ------------------ | --------- |
  | Lock            | AWS / BLE / Button | Blue      |
  | Unlock          | AWS / BLE / Button | Red       |
  | Connected to AP | Wi-Fi association  | Green     |

- The LED1 on the mainboard blinks every time a lock/unlock event is received and after association with AP.

### Application Output

  ![](resources/readme/output1.png)

  ![](resources/readme/output2.png)

  ![](resources/readme/output3.png)

  ![](resources/readme/output4.png)
  
### MQTT Connection

- After successful Wi-Fi connection, application connects to AWS IoT Core and subscribes to a topic `SUBSCRIBE_TO_TOPIC`. Continue further in the mobile app to send commands over both BLE and AWS.

1. Here `CONTROL VIA BLE` is selected, Click on `Lock` or `Unlock` buttons to send commands over BLE to the device.

    ![](resources/readme/Smartlock_UI1.png)

2. Whenever command is sent over BLE, on the Mainboard LCD you can see the lock state changing and an arrow will be pointing from BLE.

    ![](resources/readme/BLE.png)

3. Click on `CONTROL VIA AWS` button and click on `Settings` icon on the top right corner to set up AWS.

    ![](resources/readme/Smartlock_UI2.png)

4. Software app expects the certificates to be uploaded in .p12 format to setup AWS. Install OpenSSL in your PC, open command prompt where your client certificate and private key are present and run the below command to generate the certificate.p12 file.
    ```
    OpenSSL pkcs12 -export -legacy -out <Certificate_File_Name.p12> -in aws-certificate.pem.crt -in key aws-private.pem.key
    ```
    ![](resources/readme/aws_cert.png)

    `Note:` Certificate files should be sent to the mobile phone as a ZIP file and then unzipped on the phone.


5. Provide publish, subscribe topics, End point url, certificate.p12 file along with the certificate passwords and click on `Connect`. 

    ![](resources/readme/AWS_setup.png)

4. After AWS setup, you can click on **Lock** or **Unlock** buttons to send commands over AWS to the device. You can also send any custom message in the below text box and click on `Send` to send to the module.

    ![](resources/readme/Smartlock_UI3.png)

5. Whenever command is sent over AWS, on the Mainboard LCD you can see the lock state changing and an arrow will be pointing from Wi-Fi.

    ![](resources/readme/Wi-Fi.png)

6. You can use **BTN0** to lock and unlock the **Smart Lock**. Every time the button is pressed you can see the state change on the LCD.

    ![](resources/readme/Both.png)

You can also use any MQTT client or the one within AWS IOT console and connect to the AWS cloud, and subscribe to receive data from SiWG917 device and to publish "lock/unlock/erase" message to SiWG917 device.

To do that, 
1. Go to the  [AWS IoT console](https://console.aws.amazon.com/iot/home), in the navigation pane, under Manage, choose All devices, and then choose Things.

2. Click on the thing you have created. Go to activity at below. Click on MQTT test client as shown below.

   ![](resources/readme/aws_screen.png)


3. Then subscribe to a topic that is configured in the application, give the name of the topic and click on subscribe as shown below. You can see the published data from the device.

   ![](resources/readme/aws_screen1.png)
   
   ![](resources/readme/aws_screen2.png)

4. To send lock/unlock command to Smartlock from AWS. Give the name of the topic configured in the application and write down the data("lock" to lock the smartlock and "unlock" to unlock the smartlock) at Message payload as shown below. Then click on publish.

    **Note:** User need to use **lock** and **unlock** strings as application compares and displays accordingly on the screen.

   ![](resources/readme/aws_screen3.png)

5. To send erase command to Smartlock from AWS. Give "erase" at Message payload and then click on Publish. This is to erase AP credentials stored in flash. SiWG917 will then enter BLE Advertising mode, follow steps from previous section to configure SiWG917 as a STA.

## Additional Information

### Current Measurement using Simplicity Studio Energy Profiler

Using Simplicity Studio Energy Profiler for current measurement:
  
- After flashing the application code to the module. Energy profiler can be used for current consumption measurements.

  ![Figure: Smartlock and Mini connector](resources/readme/smartlock_with_mini_connector.png)

- Connect the BRD2605A and BRD4002A WPK via the Mini Simplicity Connector.

- Make sure you don't connect the BRD4338A on WPK while connected to BRD2605A. And remove the USB cable from BRD2605A.

- Connect the BRD4002A WPK to the PC via the USB-C type cable. Check in the Simplicity Commander if the Debug Mode is selected as 'MINI'.

  ![Figure: Mini Debug Mode](resources/readme/Debug_Mode_MINI.png)

- From tools, choose Energy Profiler and click "OK"

    ![Figure: Energy Profiler Step 6](resources/readme/energy_profiler_step_6.png)

- From Quick Access, choose Start Energy Capture option

    ![Figure: Energy Profiler Step 7](resources/readme/energy_profiler_step_7.png)

  **NOTE** : The measured current may vary if the scenario is performed in open environment. AP to AP variation is also observed.
  **NOTE** : To achieve the lowest power numbers in connected sleep, in SoC mode, one should configure `RAM_LEVEL` to `SL_SI91X_RAM_LEVEL_NWP_BASIC_MCU_ADV` and M4 to without RAM retention i.e. `sl_si91x_configure_ram_retention` should not be done.

- Current consumption measured in power-meter

    ![output_prints](resources/readme/power_meter_current_consumption.png)

### Setting up Security Certificates

- The WiSeConnect 3 SDK provides a conversion script (written in Python 3) to make the conversion straightforward. The script is provided in the SDK `<SDK>/resources/scripts` directory and is called [certificate_to_array.py](https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk/tree/master/resources/certificates/).

- Copy the downloaded device certificate, private key from AWS and also the certificate_to_array.py to the `<SDK>/resources/certificates`.

- To convert the device certificate and private key to C arrays, open a system command prompt in the same path and give the following commands.

  ```sh
  $> python3 certificate_to_array.py <input filename> <output arrayname>

  For example:
  $> python3 certificate_to_array.py d8f3a44d3f.pem.crt aws_client_certificate.pem
  $> python3 certificate_to_array.py d8f3a44d3f.pem.key aws_client_private_key.pem
  ```

- After running the above commands, two new files shall be created as below:

  ```sh
  aws_client_certificate.pem.crt.h
  aws_client_private_key.pem.key.h
  ```
- After converting the device certificate and private key to C - array, it is essential to include the device certificate: aws_client_certificate.pem.crt.h and private key: aws_client_private_key.pem.key.h in the wifi_app.c.

- Add the certificates into the "simplicity_sdk\extension\wiseconnect\resources\certificates" folder.

- Ensure to load the device certificate and private key to SiWG917 using [sl_net_set_credential()](https://docs.silabs.com/wiseconnect/latest/wiseconnect-api-reference-guide-nwk-mgmt/net-credential-functions#sl-net-set-credential) API.

   ```c
   status = sl_net_set_credential(SL_NET_TLS_SERVER_CREDENTIAL_ID(0), SL_NET_CERTIFICATE, aws_client_certificate, (sizeof(aws_client_certificate) - 1));
  
   status = sl_net_set_credential(SL_NET_TLS_SERVER_CREDENTIAL_ID(0), SL_NET_PRIVATE_KEY, aws_client_private_key, (sizeof(aws_client_private_key) - 1));
   ```

- Ensure to update the certificate names in the **IoT_Client_Init_Params** structure before calling the **aws_iot_mqtt_init()** API.

- The Starfield Root CA certificate used by your Wi-Fi device to verify the AWS server is already included in the WiSeConnect 3 SDK at `<SDK>/resources/certificates`; no additional setup is required.
 
  > **NOTE :**
  > Amazon uses [Starfield Technologies](https://www.starfieldtech.com/) to secure the AWS website, the WiSeConnect SDK includes the [Starfield CA Certificate](https://github.com/SiliconLabs/wiseconnect-wifi-bt-sdk/tree/master/resources/certificates/aws_starfield_ca.pem.h).
  >
  > For AWS connectivity, StarField Root CA Class 2 certificate has the highest authority being at the top of the signing hierarchy.
  >
  > The StarField Root CA Class 2 certificate is an expected/required certificate which usually comes pre-installed in the operating systems and plays a key part in certificate chain verification when a device is performing TLS authentication with the IoT endpoint.
  >
  > On SiWx91x device, we do not maintain the root CA trust repository due to memory constraints, so it is mandatory to load Starfield Root CA Class 2 certificate for successful mutual authentication to the AWS server.
  >
  > The certificate chain sent by AWS server is as below:
  > id-at-commonName=Amazon,id-at-organizationalUnitName=Server CA 1B,id-at-organizationName=Amazon,id-at-countryName=US
  > id-at-commonName=Amazon Root CA 1,id-at-organizationName=Amazon,id-at-countryName=US
  > id-at-commonName=Starfield Services Root Certificate Authority ,id-at-organizationName=Starfield Technologies, Inc.,id-at-localityName=Scottsdale,id-at- stateOrProvinceName=Arizona,id-at-countryName=US)
  >
  > On SiWx91x to authenticate the AWS server, firstly Root CA is validated (validate the Root CA received with the Root CA loaded on the device). Once the Root CA validation is successful, other certificates sent from the AWS server are validated.
  > SiWx91x doesn't authenticate to AWS server if intermediate CA certificates are loaded instead of Starfield Root CA Class 2 certificate and would result in a Handshake error.
  > StarField Root CA Class 2 certificate is at <https://certs.secureserver.net/repository/sf-class2-root.crt>
  >
  > Reference links :
  > <https://aws.amazon.com/blogs/security/how-to-prepare-for-aws-move-to-its-own-certificate-authority/>

### Create an AWS Thing

Create a thing in the AWS IoT registry to represent your IoT device.

- In the [AWS IoT console](https://console.aws.amazon.com/iot/home), in the navigation pane, under Manage, choose All devices, and then choose Things.

  ![AWS console](resources/readme/aws_create_thing_step1.png)

- If **No things** message is displayed, click on **Create things**.

  ![AWS thing](resources/readme/aws_create_thing_step2.png)

- On the **Create things** page, choose **Create single thing** and click next.

  ![AWS thing creation](resources/readme/aws_create_thing_step3.png)

- On the **Specify thing properties** page, enter a name for your IoT thing (for example, **Test_IoT**), and choose **Unnamed shadow (classic)** in the Device Shadow section, then choose **Next**. You can't change the name of a thing after you create it. To change a thing's name, you must create a new thing, give it a new name, and then delete the old thing.

  ![Add Device 1](resources/readme/aws_create_thing_step4.png)

- During **Configure device certificate** step, choose **Auto-generate a new certificate (recommended)** option and click next.

  ![Add Device 2](resources/readme/aws_create_thing_step5.png)

- Attach the policy to the thing created
  -  If you have any existing policy, attach it and click on create thing

     ![Attach policy](resources/readme/aws_choosing_policy.png)

  -  If policy is not yet created, follow the below steps.
  
     -   Choose **Create policy** and fill the fields as per your requirements.
     
         ![Create policy](resources/readme/aws_create_thing_attach_policy.png)

     - Give the **Name** to your Policy, Fill **Action** and **Resource ARN** as shown in below image, Click on **Allow** under **Effect** and click **Create**.
    
       ![Filling fields for policy](resources/readme/aws_create_thing_policy_create.png)

     - Choose the created policy and click on **Create thing**.

 - Choose the **Download** links to download the device certificate and private key. Note that Root CA certificate is already present in SDK (aws_starfield_ca.pem.h), and can be directly used.
  
    >**Warning:** This is the only instance you can download your device certificate and private key. Make sure to save them safely.

    ![Downloading certificates](resources/readme/aws_thing_certificates_download.png)

  - Click **Done**.

  - The created thing should now be visible on the AWS console (Manage > All devices > Things).

    