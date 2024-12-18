# Battery Life Estimator for SiWG917


## About

Battery Life Estimator tool helps in calculating the Average Current when the SiWG917 is configured in different scenarios. This document explains on how to use the tool.


## User Interface Components

The Battery Life Estimator tool has the following fields:

| Field | Description |
| ---   | ---         |
| Battery Capacity | Battery Capacity in mAH |
| Memory Configuration | SiWG917 has a total RAM memory of 672k bytes that can be shared between TA and M4 processors in SoC (System-on-Chip) mode. The following configurations are available: <br /> 352-320KB: TA with 352KB and M4 with 320KB of memory <br >416-256KB: TA with 416KB and M4 with 256KB of memory <br >480-192: TA with 480KB and M4 with 192KB of memory 
| Memory Retention | In SiWG917, both the M4 and the NWP can be configured to sleep with or without SRAM retention. Wherein, the amount of SRAM ot retain in M4 can be confiured via the application |
| Application Processor Activity | To include the M4 alone wakeups (In scenarios of collecting data from sensors/peripherals and validating/storing before on-air data transfer) <br />Periodicity: Frequency of M4 alone wakeup <br /> Duration: Duration of M4 alone active state (Duration of sesing/peripheral operation) |
| NWP Sleep Configuration | NWP can be configured into sleep either in the Unassociated state (without WLAN connection), or in Associated state. If the connected AP supports 11ax TwT, Target Wake Time based sleep can be configured | 
| NWP Listen Interval | Listen Interval in multiples of AP beacon intervals. Configured in [sl_wifi_performance_profile_t](https://docs.silabs.com/wiseconnect/latest/wiseconnect-api-reference-guide-si91x-driver/sl-wifi-performance-profile-t#listen-interval) |
| NWP WLAN Keep Alive Interval | Frequency of WLAN Keep Alive transmission, which is used to maintain the WLAN connection with the AP |
| TwT Wake Interval | Wake Interval or Sleep Duration of NWP during Associated TwT sleep <br /> This parameter effects the Rx Latency |
| TwT Wake Duration | Wake Duration of NWP during Associated TwT sleep <br /> This parameter effects the average throughput |
| Wakeup Periodicity | NWP wakeup periodicity in case of unassociated sleep |
| NWP Data Transfer Needed | Yes: Enable to calculate the battery life with on-air datatransfer |
| Socket Keep Alive 
| Data Transfer Type | None: To maintain socket connectivity without any data transfer <br /> Tx: To perform data transfer at configured periodicity and data size <br />Rx: To perform data receive at configured periodicity and data size <br />Both: To perform both Tx and Rx |
| Calculate | Button to calculate the average current and the battery life |

