# BT SPP Loop Back Test Tera Term Script

## About

The BT loop back test is used to test the application-level data integrity between two RS9116 NCP modules. The performance should not degrade in presence of an interference signal as per the limits set by the test specifications. 
<br>

This example demonstrates the configuration of the two RS9116 NCP modules in BT SPP_secondary and BT SPP_main mode to establish a connection between them. The BT SPP_main device will send data to the BT SPP_secondary device using the SPP Profile, and the BT SPP_secondary device will loop the data back to the BT SPP_main device. 

Ensure that both the RS9116 NCP modules are plugged into your computer/laptop and tera-term is installed and connected as described in [Getting Started with PC using AT Commands](http://docs.silabs.com/rs9116-wiseconnect/2.4/wifibt-wc-getting-started-with-pc/).

## Prerequisites

### Hardware
**1.** Two RS9116 Evaluation Boards (RS9116W-SB-EVK1) / (RS9116W-DB-EVK1).

**2.** Two USB micro cables.

**3.** Windows PC/Laptop.

### Software 
**1.** tera-term Software. [Click to download tera-term](https://osdn.net/projects/ttssh2/releases/) 

## Steps to run the BT SPP Loop back test

**STEP 1.** Reset both the RS9116 NCP modules.

**STEP 2.** In the Tera Term menu, select `Control->Macro`.

![Tera Term Macro](./resources/tera-term-macro.png)

**STEP 3.** Navigate to the folder `iot_bt_spp_loop_back_test/source` and select the file `spp_secondary.ttl`.

![Tera Term Script to be selected](./resources/tera-term-spp-secondary.png)

**STEP 4.**  After the "spp_secondary.ttl" script is selected,  The initial dialogue window will appear as below, and as a response, the user needs to click on the "OK" button.

![Introduction of spp-secondary](./resources/spp-secondary-start.png)

**STEP 5.**  Initially, the ABRD process starts and you will see a pop-up with the message saying firmware is loaded as below.  

![Bootup options execution](./resources/firmware-loding-secondary.png)

**STEP 6.**  Command sequence in this script:

1. Opermode

2. Set connectable mode

3. Set discoverable mode

4. Set profile mode

5. Set local name

**STEP 7.** After these commands are executed, the RS9116 NCP module becomes discoverable in the vicinity.

![RS9116W device in discoverable and connectable mode](./resources/spp-secondary-command-execute.png)

**STEP 8.** Connect the second RS9116 NCP module to the same PC and open another TeraTerm terminal later select and open the appropriate com port.

**STEP 9.**  In the Tera Term menu, select `Control->Macro`.

![Tera Term Macro](./resources/tera-term-macro-main.png)

**STEP 10.** Navigate to the folder `iot_bt_spp_loop_back_test/source` and select the file `spp_main.ttl`.

![Tera Term script to be selected](./resources/tera-term-spp-main.png)

**STEP 11.**  After the "spp_main.ttl" script is selected, The initial dialogue window will appear as below, and as a response, the user needs to click on the "OK" button.

![Introduction of spp-secondary](./resources/spp-main-start-4.png)

**STEP 12.**  Initially, the ABRD process starts and you will see a pop-up with the message saying firmware is loaded as below.

![Bootup options execution](./resources/firmware-loding-main-5.png)

**STEP 13.**  After these commands are executed successfully, the remote secondary device needs to be in a connectable mode. And once it will be in connectable mode, the user needs to give the bond command. For this BD address of the RS9116 NCP spp_secondary device needs to be entered here.

Note :- Address should be in following format "XX-XX-XX-XX-XX-XX".

![remote BD Address input](./resources/spp-main-address-6.png)

**STEP 14.** By entering the RS9116 NCP spp_secondary BD address itself, RS9116 NCP spp_main will send the "bond" command. Later, the respective command menu will pop up on both the "spp_secondary" side and the "spp_main" side.

1) Here the below picture is for the "spp_secondary" one.

![Command menu for secondary](./resources/spp-secondary-linkkey-7.png)

2) Here the below picture is for the "spp_main" one.

![Command menu for main](./resources/spp-main-pincode-8.png)

**STEP 15.** While opening the command pop-ups on both the "spp_main" & "spp_secondary" sides, Give the "Userlinkkey Response" from the "spp_main" side for the "userlinkkey event" on spp_main and likewise same for "spp_secondary" side for the "userlinkkey event" on spp_secondary.

1) Below picture is for spp_main.

![Userlinkkey response main menu](./resources/linkkey-response-main.png)

![Userlinkkey response main command](./resources/linkkey-response-command-main.png)

2) Below picture is for spp_secondary.

![Userlinkkey response from secondary](./resources/linkkey-response-selection-secondary.png)

![Userlinkey response from seconday command](./resources/linkkey_response-command-secondary.png)

**STEP 16.** After "userlinkey response" from the "spp_secondary" side, "spp_main" will get the "userpincode request" command, for this user should give the "Userpincode Response" command
from the "spp_main" side. Likewise, parallelly user should give the "userpincode Response" on the "spp_secondary" side too.

1) Below picture is for spp_main.

![Userpincode response from main](./resources/spp-main-pincode-8.png)

![Userpincode response command from main](./resources/userpincode-response-command-main.png)

2) Below picture is for spp_secondary.

![Userlinkkey response from spp secondary](./resources/spp-secondary-linkkey-7.png)

![Userpincode response from secondary](./resources/spp-secondary-picode-9.png)

**STEP 17.** After successful command exchanges between the RS9116 NCP spp_main & RS9116 NCP spp_secondary, the user should give the "spp_connect" command from the "spp_main" side, then the connection should be successful.

1) Below picture is for spp_main.

![spp_connect command from spp_main](./resources/spp-main-connect-10.png) 

![spp_connect command from main](./resources/spp-main-spp-connected-12.png)

2) Below picture is for spp_secondary.

![spp_secondary connected](./resources/spp-secondary-spp-connected-11.png)

**STEP 18.** Both the spp_main and spp_secondary are connected. Now, the user needs to send the data from the "spp_main" side to "spp_secondary" by giving the "send_data" from "spp_main".

![Send_data_from_main](./resources/spp-main-send-data-13.png)

1) By clicking the send_data, spp_main will ask the input how many times you want to send the data. 

![Data_send_count](./resources/spp-main-number-send-data-15.png)

2) spp_main start the data sending.

![Data transfer start from spp_main](./resources/spp-main-send-data-start-16.png)

3) spp_secondary starts receiving the data.

![Data Receive start at spp_secondary](./resources/spp-secondary-data-receive-17.png)

**STEP 19.** Once the data sent reaches the mentioned number of times, spp_main will ask the input to stop (or) start sending the data again. If "1" is given as input spp_main will ask the 
input how many times you want to send the data.

![Data send ask again](./resources/spp-main-data-ask-again-18.png)

![Data send count again](./resources/spp-main-data-send-again-19.png)

1) Below picture is for spp_main.

![Data send by spp_main](./resources/spp-main-data-send-again-20.png)

2) Below picture is for spp_secondary.

![Data received by spp secondary](./resources/spp-secondary-received-data-21.png)

**STEP 20.** If the input is "0" then the spp_main script will end and spp_secondary is in the listening state.

1)Below are pictures for spp_main.

![spp main stop script input](./resources/spp-main-stop-data.png)

![spp main script end](./resources/spp-main-loop-back-test-completed-22.png)

2)Below is the picture for spp_secondary.

![spp_secondary listining state](./resources/spp-secondary-received-data-21.png)

3)If the input is other than "1" or "0" then the spp_main will ask for the proper input.

![spp_main wrong input](./resources/data-send-ask-wrong-input.png)

![spp_main wrong input pop-up](./resources/continuos-wrong-input.png)



