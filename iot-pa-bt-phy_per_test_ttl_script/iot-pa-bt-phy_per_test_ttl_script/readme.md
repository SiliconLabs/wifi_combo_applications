
# BLuetooth PER mode TTL Scripts
<!-- TOC -->

- [Introduction](#introduction)
- [Terminology](#terminology)
- [Prerequisites](#prerequisites)
    - [Hardware](#hardware)
    - [Software](#software)
    - [Setup Diagram](#setup-diagram)
- [Bluetooth PER Mode Execution](#bluetooth-per-mode-execution)
    - [BLE](#ble)
    - [BT Classic](#bt-classic)
- [Summary](#summary)
- [Troubleshoot](#troubleshoot)
- [References](#references)

<!-- /TOC -->

# Introduction

This document details the requirements, steps and commands needed for the execution of BT Classic/BLE PER mode via RS9116 NCP module using AT commands. Customers going for regulatory tests (FCC, CE/RED, TELEC, etc) can utilize this script to setup the module in PHY test mode.


# Terminology

1. EVK - Evaluation Kit
2. DUT - Device Under Test
3. BT Classic - Bluetooth Classic
4. BLE - Bluetooth Low Energy
5. PER - Packet Error Rate
6. CW - Continuous Wave


# Prerequisites

## Hardware

1. Windows PC
2. Two RS9116 NCP EVK along with USB-Micro cable



## Software
1. Tera-term application. Download [link](https://ttssh2.osdn.jp/index.html.en)

### PER Commands

1. For BT Classic PER commands, refer [RS9116 NCP BT PER AT COMMANDS](https://docs.silabs.com/rs9116/wiseconnect/rs9116w-bt-classic-at-command-prm/2.6/08-bt-classic-commands#rsibt-perstats----per-stats) 
2. For BLE PER commands, refer [RS9116 NCP PER AT COMMANDS](https://docs.silabs.com/rs9116/wiseconnect/rs9116w-ble-at-command-prm/2.6/08-ble-commands#rsibt-bletransmit----ble-per-transmit)<br><br>


## Setup Diagram
<img src="resources/setup.png" alt="" width=1000><br>

> **Note:** The test can also be performed on a single PC/Laptop with multiple instances of Teraterm running simultaneously.

# Bluetooth PER Mode Execution

Refer [Getting Started with PC using AT Commands](http://docs.silabs.com/rs9116-wiseconnect/2.4/wifibt-wc-getting-started-with-pc/) for getting hands on experience with TeraTerm. In this example, two RS9116 NCP modules are used to perform Receive and Transmit simultaneously. However, its possible to perform only Tx and only Rx activity.

### <p id ="teraterm1">***Teraterm Settings***</p>
1. Configure the serial port settings to 115200 baud / 8-bit data / No parity / 1 stop bit.<br>
<img src="resources/teraterm1.png" alt="" width=1000><br>
2. Enable **local echo**. Set receive and transmit new-line characters to **CR+LF**.<br>
<img src="resources/teraterm2.png" alt="" width=1000><br>

> **Note:** If any issue addressed while running TeraTerm/the ttl scripts, refer the <a href = "#troubleshooting">Troubleshoot</a> section.<br>

## BLE

### <p id ="exe1">***Execution Procedure***</p>
This section list the steps to be followed for getting BLE Tx and Rx PER statistics: 
1. <p id ="dut1">Connect RS9116 NCP (DUT 1) to the PC and reset it.</p>  
2. Open Teraterm, configure it as mentioned <a href = "#teraterm1">here</a>.
3. Click on Control -> Macro. Run the TTL script, `ble_per_mode.ttl`.<br>
<img src="resources/teraterm.png" alt="" width=500><br>
<img src="resources/blescript.png" alt="" width=700><br>
4. Click on **OK** for the succcessful event pop-ups.<br>
<img src="resources/bletx1.png" alt="" width=500><br>
<img src="resources/bletx2.png" alt="" width=300><br>
<img src="resources/bletx3.png" alt="" width=300><br>

5. Enable Receive mode in <a href = "#dut1">DUT 1</a>.<br>
<img src="resources/blerx4.png" alt="" width=500><br>

    >  **Note:** Make sure to always run Receive mode in <a href = "#dut1">DUT 1</a> followed by Transmit mode in <a href = "#dut2">DUT 2</a>.
<br>

6. Enable PER mode in <a href = "#dut1">DUT 1</a>.<br>
<img src="resources/blerx.png" alt="" width=500><br>

7. Enter any 4 bytes Device Access Address with which packets are transmitted.<br>
<img src="resources/bletx6.png" alt="" width=500><br>

8. Select BLE PHY rate at which packets are transmitted.<br>
<img src="resources/blerx8.png" alt="" width=500><br>

9. Select BLE Channel type.<br>
<img src="resources/bletx8.png" alt="" width=500><br>

10. Select the Frequency Hopping type to be used.<br>
<img src="resources/bletx9.png" alt="" width=500><br>

11.  Enter Receive mode RX channel number. (Valid if selected *No Hopping* or *Fixed Hopping*)<br>
<img src="resources/blerx12.png" alt="" width=500><br>

12. Enter Receive mode TX channel number. (Valid if selected *Fixed Hopping*)<br>
<img src="resources/blerx11.png" alt="" width=500><br>

13. Select antenna to be used. If the test is performed using a Spectrum Analyzer or Signal Generator, select *EXT_ANT_SEL* but if the test is performed between two RS9116 NCP modules select *ONBOARD_ANT_SEL*.<br>
<img src="resources/bletx13.png" alt="" width=500><br>

14. Select BLE RF Chain (High Power/Low Power).<br>
<img src="resources/bletx14.png" alt="" width=500><br>

15. Enable/Disable the extended data length feature.<br>
<img src="resources/bletx15.png" alt="" width=500><br>

16. Enable/Disable the loopback feature.<br>
<img src="resources/bletx16.png" alt="" width=500><br>

17. Click on **OK**.<br>
<img src="resources/blerx18.png" alt="" width=500><br>

18. <p id ="dut2"> To run BLE PER Transmit mode, connect second RS9116 NCP module (DUT 2) to the PC.</p>

    > **Note:**  Parameters: *Device Access Address, BLE PHY rate, BLE Channel Type* and *Frequency Hopping* should have same values in both Transmit and Receive mode.

19. To run the BLE PER Transmit mode in <a href = "#dut2">DUT 2</a>, follow the steps as mentioned below:<br> 
    i. Open a new Teraterm Terminal, configure the Teraterm settings and follow steps 1 to 4 as mentioned <a href = "#exe1">here</a>.<br> 

    > **Note:** Make sure to select correct COM port while setting up <a href = "#dut2">DUT 2</a>.
    
    ii. In step 5, enable Transmit mode.<br>
    <img src="resources/bletx4.png" alt="" width=500><br>

    iii. Follow steps 6 to 10. In step 11, enter the Transmit Tx Channel number, same as Receive Rx Channel number. (Valid if selected *No Hopping* or *Fixed Hopping*)<br>
    <img src="resources/bletx11.png" alt="" width=500><br>

    iv. In step 12, enter the Transmit Rx Channel number, same as Receive Tx Channel number. (Valid if selected *Fixed Hopping*)<br>
    <img src="resources/bletx12.png" alt="" width=500><br>

    v. Select antenna to be used.<br>
    <img src="resources/bletx13.png" alt="" width=500><br>

    vi. Select BLE RF Chain (High Power/Low Power).<br>
    <img src="resources/bletx14.png" alt="" width=500><br>

20. Enter length of the packet to be transmitted. <br>
<img src="resources/bletxpkt.png" alt="" width=500><br>

21. Enter type of payload data sequence.<br>
<img src="resources/bletxpayload.png" alt="" width=500><br>

22. Enter Transmit power index based on selected BLE RF Chain.<br>
<img src="resources/bletx17.png" alt="" width=500><br>

<p id ="per1"></p>

23. Select the PER mode to be executed.<br>
<img src="resources/bletx18.png" alt="" width=500><br>

24. Click on **OK**.<br>
<img src="resources/bletx19.png" alt="" width=500><br>

25. The Teraterm screen should be populated with PER stats:<br>
    i. Transmit mode stats:<br>
    <img src="resources/bletx20.png"><br>

    ii. Receive mode stats:<br>
    <img src="resources/blerx20.png"><br>

26. To stop the execution, click on End.<br>
    <img src="resources/bletx21.png" width=500><br>

> **Note:**  To run Continuous PER Mode and Continuous Wave PER mode, follow the same procedure with change of PER mode value (<a href = "#per1">here</a>).<br>



## BT Classic

### <p id ="exe2">***Execution Procedure***</p>
This section list the steps to be followed for getting BT Classic Tx and Rx PER statistics: 
1. <p id ="dut11">Connect RS9116 NCP (DUT 1) to the PC and reset it.</p>  
2. Open Teraterm, configure it as mentioned <a href = "#teraterm1">here</a>.
3. Click on Control -> Macro. Run the TTL script, `btc_per_mode.ttl`<br>
<img src="resources/teraterm.png" alt="" width=500><br>
<img src="resources/btcscript.png" alt="" width=700><br>

4. Click on **OK** for the succcessful event pop-ups.<br>
<img src="resources/btrx1.png" alt="" width=500><br>
<img src="resources/bletx2.png" alt="" width=300><br>
<img src="resources/bletx3.png" alt="" width=300><br>

5. Enable Receive mode in <a href = "#dut11>DUT 1</a>.<br>
<img src="resources/btrx4.png" alt="" width=500><br>
   > **Note:** Make sure to always run Receive mode in <a href = "#dut11">DUT 1</a> followed by Transmit mode in <a href = "#dut22">DUT 2</a>. <br>

6. Enable PER mode in <a href = "#dut22">DUT 2</a>. <br>
<img src="resources/btper.png" alt="" width=500><br>

7. Enter any 48-bit device address in hexadecimal format.<br>
<img src="resources/btrx5.png" alt="" width=500><br>

8. Enter <a href ="https://docs.silabs.com/rs9116/wiseconnect/rs9116w-bt-classic-at-command-prm/latest/08-bt-classic-commands#bt-packet-summary">packet type</a> to be received, as per the Bluetooth standard.<br>
<img src="resources/btrx6.png" alt="" width=500><br>

9. Select the desired data rate.<br>
<img src="resources/btrx7.png" alt="" width=500><br>
                            
10. Select type of channel to be used.<br>
<img src="resources/btrx8.png" alt="" width=500><br>

11. Select the Frequency Hopping type to be used.<br>
<img src="resources/btrx9.png" alt="" width=500><br>

12. Enter Receive mode RX channel number. (Valid if selected *No Hopping* or *Fixed Hopping*)<br>
<img src="resources/btrx11.png" alt="" width=700><br>

13. Enter Receive mode TX channel number. (Valid if selected *Fixed Hopping*)<br>
<img src="resources/btrx10.png" alt="" width=700><br>

14. Select antenna to be used. If the test is performed using a Spectrum Analyzer or Signal Generator, select *EXT_ANT_SEL* but if the test is performed between two RS9116 NCP modules select *ONBOARD_ANT_SEL*.<br>
<img src="resources/btrx12.png" alt="" width=500><br>

15. Enable/Disable the loopback feature.<br>
<img src="resources/btrx13.png" alt="" width=500><br>

16. Click on **OK**.<br>
<img src="resources/btrx14.png" alt="" width=500><br>

17. To run BT PER Transmit mode, connect another RS9116 NCP module to the PC.<br>
    > **Note:**  Parameters: *Device Address, Packet Type, BT Classic Mode, Link Type* and *Frequency Hopping* should have same values in both Transmit and Receive mode.
<br>

18. To run the BT Classic PER Transmit mode in <a href = "#dut22">DUT 2</a>, follow the steps as mentioned below:<br> 
    i. Open a new Teraterm Terminal, configure the Teraterm settings and follow steps 1 to 4 as mentioned <a href = "#exe2">here</a>.<br> 

    > **Note:** Make sure to select correct COM port while setting up <a href = "#dut22">DUT 2</a>.
    
    ii. In step 5, enable Transmit mode.<br>
    <img src="resources/bttx4.png" alt="" width=500><br>

    iii. Follow steps 6 to 11. In step 12, enter the Transmit Rx Channel number, same as Receive Tx Channel number. (Valid if selected *No Hopping* or *Fixed Hopping*)<br>
    <img src="resources/bttx11.png" alt="" width=500><br>

    iv. In step 13, enter the Transmit Rx Channel number, same as Receive Tx Channel number. (Valid if selected *Fixed Hopping*)<br>
    <img src="resources/bttx12.png" alt="" width=500> <br>

    v. Select antenna to be used.<br>
    <img src="resources/bletx13.png" alt="" width=500><br>

19. Enter the packet length based on selected data rate.<br>
<img src="resources/bttx14.png" alt="" width=500><br>

20. Enter type of payload data sequence.<br>
<img src="resources/bttx15.png" alt="" width=500><br>

21. Enter Transmit power index.<br>
<img src="resources/bttx16.png" alt="" width=500><br>

<p id ="per2"></p><br> 

22. Select the PER mode to be executed.<br>
<img src="resources/bletx18.png" alt="" width=500><br>


23. Click on **OK**.<br>
<img src="resources/bttx17.png" alt="" width=500><br>

24. The Teraterm screen should be populated with PER stats:<br>
    i. Transmit mode stats:<br>
    <img src="resources/bttx20.png"><br>

    ii. Receive mode stats:<br>
    <img src="resources/btrx20.png"><br>

25. To stop the execution, click on End.<br>
    <img src="resources/bletx21.png" width=500><br>

    > **Note:**  To run Continuous PER Mode and Continuous Wave PER mode, follow the same procedure with change of PER mode value (<a href = "#per2">here</a>).

# Summary

This document helps the user to test BLE/BT Classic PER mode using RS9116 NCP module. The procedure is followed with minimal traffic and using on-board antenna of the NCP EVK. 

> **Note:** If Tx and Rx characteristics is to be observed separately, we can use a Spectrum Analyzerr and Signal Generator respectively as shown below:<br>
- *Tx Block Diagram:*<br>
<img src="resources/image2.png" alt="" width=600><br><br>
     - The test has been carried out in Conducted Mode between Spectrum Analyzer and DUT to avoid losses. Expected Spectrum Analyzer result is mentioned below:<br>
        - *Burst Mode* (BLE)<br>
        <img src="resources/bletxburstspectrum.png" alt="" width=600><br><br>
        - *CW Mode* (BLE)<br>
        <img src="resources/bletxcwavespectrum.png" alt="" width=600><br><br>
        - *Burst Mode* (BT Classic)<br>
        <img src="resources/bt_burst_0dbm.png" alt="" width=600><br><br>
        - *CW Mode* (BT Classic)<br>
        <img src="resources/btcwspectrum.png" alt="" width=600><br><br>
        
- *Rx Block Diagram:*<br>
<img src="resources/image1.png" alt="" width=600><br><br>

# Troubleshoot
<p id ="troubleshooting"></p>

1. If UART port is undetected by the PC, reset the module and try again. There's a reset button available on the module. If the port is still undetected, disconnect the module and re-connect the module to the PC.

2. Use a thick USB cable for avoiding losses and getting accurate results.

3. After every iteration of measurement with Spectrum Analyzer, ensure the module has been properly reset. 

4. If the Teraterm application stops midway, end the application, close the Teraterm window and re-run the application.<br><br>

# References

1. [RS9116 NCP BLE AT Command Programming Reference Manual](https://docs.silabs.com/rs9116/wiseconnect/rs9116w-ble-at-command-prm/latest/)
2. [RS9116 NCP BT Classic AT Command Programming Reference Manual](https://docs.silabs.com/rs9116/wiseconnect/rs9116w-bt-classic-at-command-prm/latest/)<br><br><br><br>

<img src="resources/image28.png" alt="" width=250>



