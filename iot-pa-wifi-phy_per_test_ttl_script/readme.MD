# WLAN PER Mode Tera Term Script
This application demonstrates the WLAN PER (Packet Error Rate) Transmit and Receive test, in RS9116 NCP Module using AT commands.
Customers going for regulatory tests ((FCC, CE/RED, TELEC, etc) can utilize this script to setup the module in PHY test mode.
You can select between different bands, channels, data rates, mode (Burst, Continuous, CW), and power.
Both conducted and radiated tests can be performed. 

Below are the data rate and modulation details: <br />
802.11b (DSSS) : 1, 2, 5.5, 11 Mbps <br />
802.11g (OFDM): 6, 9, 12, 18, 24, 36, 48, 54 Mbps <br />
802.11n (OFDM): MCS0 to MCS7 with and without Short GI

## Prerequisites

1. Windows PC
2. RS9116 Evaluation Board (RS9116W-SB-EVK1) / (RS9116W-DB-EVK1) along with a Micro USB cable
3. Tera Term software from [link.](https://tera-term.en.softonic.com/download)
4. Spectrum Analyzer and RF connectors 
5. Signal Generator and RF connectors

## Setup 

## Transmit 
In Transmit test (conducted), make sure the RS9116 NCP EVK is connected to the Spectrum Analyzer through u.Fl.

![Tera Term Macro](./resources/Transmit-Test-Setup.PNG)


##  Receive 
In this case, RS9116 NCP EVK will be in receive mode, and the signal generator transmits the packet to the EVK.

Make sure to connect with u.Fl on the RS9116 NCP EVK to the Signal generator using an RF connector (conducted).

 ![Tera Term Macro](./resources/Receive-Test-Setup.PNG)

## Sequence of Steps for executing PER Script 
Connect the RS9116 NCP EVK to your PC using a USB cable and connect u.Fl on the RS9116 NCP EVK to Spectrum Analyzer/Signal generator as per the above-mentioned setup.
After the setup is done, perform the following:

For setting up Tera Term to use with the RS9116 NCP EVK, refer to this [link](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-getting-started-with-pc/tera-term-setup).

Upon configuring the Tera Term settings, follow the below steps.

**STEP 1.** Reset the RS9116 NCP EVK.

**STEP 2.** In the Tera Term menu go to `Control -> Macro`.

![Tera Term Macro](./resources/tera-term-macro.png)

**STEP 3.** Now, navigate to the downloaded folder `iot-pa-wifi-phy_per_ttl_script` which contains the example ttl script. Select the file `wlan_per_mode.ttl`.


![Tera Term Script to be selected](./resources/tera-term-scripts-wlan_per_mode.PNG)

**STEP 4.** Now the wlan_per_mode script starts executing. A pop-up indication will be shown as below.

![Application Start pop up](./resources/starting-message-popup.PNG)


**STEP 5.** Initially, the ABRD process continues and you will see a pop-up with the message “Firmware qieried successfully“.

![Fimware queried successfully](./resources/Firmware-queried-successfully.PNG)

**STEP 6.** Opermode command `at+rsi_opermode=8` configures the module in PER mode, and you can see the module responding with “OK”.

![Opermode command success](./resources/opermode-successful.PNG)

**STEP 7.** Next, select the band as per your choice. <br />
2.4GHz - 0  <br />
5GHz   - 1

The command `at+rsi_band=0` configures the operating band of the Wi-Fi radio to the 2.4GHz band. 

![Enter the band](./resources/band.png)

**STEP 8.** Now, `at+rsi_feat_frame` command is sent to the module. This command is used to select Internal RF type or External RF type, and Clock Frequency. Default parameters are already selected.

![feat frame success](./resources/Feature-Frame-sent.PNG)

**STEP 9.** `at+rsi_init` command initializes the RF of the module. The module responds with OK<MAC_Address> (MAC Address is in hex format).

![Init command succcess](./resources/Init-successful.PNG)

**STEP 10.** In the next pop-up, Antenna has to be selected. <br />
1 - for u.FL connector <br />
0 - for the inbuilt Antenna

 The command `at+rsi_antenna` is used for antenna selection.

![Antenna selection](./resources/Antenna-selection.PNG)

**STEP 11.** Select the region in the pop-up window. This command `at+rsi_setregion` configures the device to operate according to the regulations of the selected region.

![Region selection](./resources/region.PNG)

**STEP 12.** Now enter the appropriate option for [PER Tx](#per-modes) or [PER Rx](#receive-test) test. Click on 'OK'

![PER Test selection](./resources/PER-Test-selection.PNG)


### PER Modes : 
**0 - Burst Mode:** This mode is used to transmit valid WLAN packets, with a few microseconds gap between the packets. <br />
**1 - Continuous Mode:**  This mode is used to transmit valid WLAN packets, with no gap between the packets. This mode will transmit packets with a 100% duty cycle.  <br />
**2 - Continuous Wave Mode in DC:** This mode generates an unmodulated Sine wave. <br />
**3 - Continuous Wave Mode in single tone mode (center freq - 2.5MHz):** This mode generates an  Unmodulated Sine wave with a frequency offset of minus 2.5 MHz. <br />
**4 - Continuous Wave Mode in a single tone mode (center freq + 5MHz):** This mode generates an Unmodulated Sine wave with a frequency offset of plus 5 MHz.

**STEP 1.** Now, select the type of PER Transmit mode. Here, Burst mode is selected.

![PER modes selection](./resources/PER-mode-selection.PNG)

The command `at+rsi_per` is used to start the transmission of packets.

**STEP 2.** Enter the transmit power value. (127 - Based on the region set, it takes automatically the highest value that passes for EVM and Mask. You can set the power index value from 2-18 to arrive at pass value). 

![power level selection](./resources/powerlevel.PNG)

**Note:** To configure the maximum transmit power under the regulatory considerations and channels for that frequency band, set ***power*** = 127


**STEP 3.** In the next pop-up, enter the data rate.

![Data rate](./resources/Data-Rate.PNG)

**STEP 4.** Now select the channel number in which you would like to transmit.

![pop up to take for channel selection](./resources/Enter-channel.PNG)

**STEP 5.** Next, enter the PER packet length.

![pop up to take for length](./resources/Enter-length.PNG)

**STEP 6.** You can see that the PER Burst mode is executed successfully.

**Note:** The process to input parameters is similar for all PER modes.

![Mode](./resources/Mode-Executed-Successfully.PNG)

**STEP 7.** Stop the PER mode, and repeat the process to run the other modes.

![PER mode stop ](./resources/PER-Mode-stopped.PNG)

## Expected Result
1. The below result is for PER Mode with Channel '6' with 6Mbps data rate and max 127dBm Power, in Burst mode, OFDM modulation technique.

![Result](./resources/Burst-mode-OFDM.PNG)

2. The below result is for PER Mode with Channel '1' with 1Mbps data rate and max 127dBm Power in Burst mode, DSSS modulation technique. 

![Result](./resources/Burst-mode-DSSS.PNG)

3. The below result is for PER Mode with Channel '6' with 5.5Mbps data rate and max 127dBm Power in Continous mode, CCK modulation technique. 

![Result](./resources/Continous-mode-CCK.PNG)

 ## Receive Test 

**STEP 1.** Select the channel number in which the module needs to be receiving the packet and click on 'OK'.

Now start the transmit in the same channel from the signal generator.
(Here you can also use other RS9116 NCP Module for transmitting the signal)

![PER-RX-Test-Enter-Channel](./resources/PER-RX-Test-Enter-Channel.PNG)

**STEP 2.** Below are the PER receive stats in the hex format.

![PER-RX-Stats-Start](./resources/PER-RX-Stats-Start.PNG)

**STEP 3.**  Click on OK to stop the PER RX test.

![Stop-RX-Test](./resources/Stop-RX-Test.PNG)

##  RX test is completed  

STATS RESPONSE 

Each stats response starts with AT+RSI_PER_STATS= 
41 54 2B 52 53 49 5F 50 45 52 5F 53 54 41 54 53 3D - > (AT+RSI_PER_STATS=)

Each stats response ends with 0D 0A.

For more information about PER stats and its fields, please refer to the rsi_per_stats command in the [AT Command PRM](https://docs.silabs.com/rs9116/wiseconnect/rs9116w-wifi-at-command-prm/latest/wlan-commands#rsi-per-stats----query-per-statistics).
 


