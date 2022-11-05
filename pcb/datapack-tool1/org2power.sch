EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 2 3
Title "Psion Organiser 2 Re-Creation "
Date "2022-08-20"
Rev "REV2.0"
Comp ""
Comment1 "Has 21V VPP supply.Wireless using Pico W"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+3V3 #PWR?
U 1 1 5EED9BA4
P 2000 1350
F 0 "#PWR?" H 2000 1200 50  0001 C CNN
F 1 "+3V3" H 2015 1523 50  0000 C CNN
F 2 "" H 2000 1350 50  0001 C CNN
F 3 "" H 2000 1350 50  0001 C CNN
	1    2000 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6133A297
P 2525 1900
F 0 "C?" H 2640 1946 50  0000 L CNN
F 1 "100n" H 2640 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2563 1750 50  0001 C CNN
F 3 "~" H 2525 1900 50  0001 C CNN
	1    2525 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6133A298
P 2975 1900
F 0 "C?" H 3090 1946 50  0000 L CNN
F 1 "100n" H 3090 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3013 1750 50  0001 C CNN
F 3 "~" H 2975 1900 50  0001 C CNN
	1    2975 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6133A299
P 3425 1900
F 0 "C?" H 3540 1946 50  0000 L CNN
F 1 "100n" H 3540 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3463 1750 50  0001 C CNN
F 3 "~" H 3425 1900 50  0001 C CNN
	1    3425 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 1750 3425 1700
Wire Wire Line
	2975 1750 2975 1700
Connection ~ 2975 1700
Wire Wire Line
	2975 1700 3425 1700
Wire Wire Line
	2525 1750 2525 1700
Wire Wire Line
	2525 1700 2975 1700
$Comp
L power:GND #PWR?
U 1 1 6133A29E
P 3425 2200
F 0 "#PWR?" H 3425 1950 50  0001 C CNN
F 1 "GND" H 3430 2027 50  0000 C CNN
F 2 "" H 3425 2200 50  0001 C CNN
F 3 "" H 3425 2200 50  0001 C CNN
	1    3425 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2525 2050 2525 2150
Wire Wire Line
	2975 2050 2975 2150
Wire Wire Line
	2975 2150 2525 2150
Wire Wire Line
	3425 2050 3425 2150
$Comp
L power:GND #PWR?
U 1 1 5F00B2D4
P 2000 2225
F 0 "#PWR?" H 2000 1975 50  0001 C CNN
F 1 "GND" H 2005 2052 50  0000 C CNN
F 2 "" H 2000 2225 50  0001 C CNN
F 3 "" H 2000 2225 50  0001 C CNN
	1    2000 2225
	1    0    0    -1  
$EndComp
Wire Wire Line
	2975 2150 3425 2150
Connection ~ 2975 2150
Connection ~ 3425 2150
Wire Wire Line
	3425 2150 3425 2200
Text GLabel 1350 8450 2    50   Input ~ 0
SD1
Text GLabel 1350 8650 2    50   Input ~ 0
SD3
Text GLabel 1350 8850 2    50   Input ~ 0
SD5
Text GLabel 1350 9050 2    50   Input ~ 0
SD7
Text GLabel 1350 9450 2    50   Input ~ 0
SMR
Text GLabel 1350 9350 2    50   Input ~ 0
SOE
Text GLabel 1350 9250 2    50   Input ~ 0
SCLK
Text GLabel 1350 8350 2    50   Input ~ 0
SD0
Text GLabel 1350 8950 2    50   Input ~ 0
SD6
Text GLabel 1350 8750 2    50   Input ~ 0
SD4
Text GLabel 1350 8550 2    50   Input ~ 0
SD2
$Comp
L power:GND #PWR?
U 1 1 61058D25
P 2075 8250
AR Path="/6104FF77/61058D25" Ref="#PWR?"  Part="1" 
AR Path="/61058D25" Ref="#PWR025"  Part="1" 
AR Path="/6366C142/61058D25" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2075 8000 50  0001 C CNN
F 1 "GND" H 2075 8075 50  0000 C CNN
F 2 "" H 2075 8250 50  0001 C CNN
F 3 "" H 2075 8250 50  0001 C CNN
	1    2075 8250
	1    0    0    -1  
$EndComp
Text GLabel 6425 4225 0    50   Input ~ 0
SD0
Text GLabel 6425 4325 0    50   Input ~ 0
SD1
Text GLabel 6425 4425 0    50   Input ~ 0
SD2
Text GLabel 6425 4525 0    50   Input ~ 0
SD3
Text GLabel 6425 4625 0    50   Input ~ 0
SD4
Text GLabel 6425 4725 0    50   Input ~ 0
SD5
Text GLabel 6425 4825 0    50   Input ~ 0
SD6
Text GLabel 6425 4925 0    50   Input ~ 0
SD7
Text GLabel 8425 1725 0    50   Input ~ 0
SMR
Text GLabel 8425 1825 0    50   Input ~ 0
SCLK
Text GLabel 8425 1625 0    50   Input ~ 0
SOE
Text GLabel 8425 1925 0    50   Input ~ 0
SS1
Text GLabel 8425 1525 0    50   Input ~ 0
SPGM
Text GLabel 8525 4225 2    50   Input ~ 0
P_SD0
Text GLabel 8525 4325 2    50   Input ~ 0
P_SD1
Text GLabel 8525 4425 2    50   Input ~ 0
P_SD2
Text GLabel 8525 4525 2    50   Input ~ 0
P_SD3
Text GLabel 8525 4625 2    50   Input ~ 0
P_SD4
Text GLabel 8525 4725 2    50   Input ~ 0
P_SD5
Text GLabel 8525 4825 2    50   Input ~ 0
P_SD6
Text GLabel 8525 4925 2    50   Input ~ 0
P_SD7
Text GLabel 9425 1725 2    50   Input ~ 0
P_SMR
Text GLabel 9425 1825 2    50   Input ~ 0
P_SCLK
Text GLabel 9425 1625 2    50   Input ~ 0
P_SOE
Text GLabel 9425 1925 2    50   Input ~ 0
P_SS1
Text GLabel 9425 1525 2    50   Input ~ 0
P_SPGM
Wire Wire Line
	6925 3650 6925 3775
$Comp
L mirotan-due-cassette-shield-cache:+3.3V #PWR?
U 1 1 611C2F8D
P 6925 3650
AR Path="/6104FF77/611C2F8D" Ref="#PWR?"  Part="1" 
AR Path="/611C2F8D" Ref="#PWR023"  Part="1" 
AR Path="/6366C142/611C2F8D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6925 3500 50  0001 C CNN
F 1 "+3.3V" H 6940 3823 50  0000 C CNN
F 2 "" H 6925 3650 50  0001 C CNN
F 3 "" H 6925 3650 50  0001 C CNN
	1    6925 3650
	1    0    0    -1  
$EndComp
Text GLabel 5375 5125 0    50   Input ~ 0
LS_DIR
$Comp
L Device:R_Small R?
U 1 1 611C2F98
P 5650 4675
AR Path="/60C45767/611C2F98" Ref="R?"  Part="1" 
AR Path="/611C2F98" Ref="R15"  Part="1" 
AR Path="/6104FF77/611C2F98" Ref="R?"  Part="1" 
AR Path="/6366C142/611C2F98" Ref="R?"  Part="1" 
F 0 "R?" H 5709 4721 50  0000 L CNN
F 1 "100k" H 5709 4630 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 5650 4675 50  0001 C CNN
F 3 "~" H 5650 4675 50  0001 C CNN
	1    5650 4675
	1    0    0    -1  
$EndComp
Wire Wire Line
	6925 3775 5650 3775
Wire Wire Line
	5650 3775 5650 4575
Connection ~ 6925 3775
Wire Wire Line
	6925 3775 6925 3925
Wire Wire Line
	5650 4775 5650 5125
Wire Wire Line
	5650 5125 6425 5125
Wire Wire Line
	5375 5125 5650 5125
Connection ~ 5650 5125
$Comp
L 74xx:74LS245 U?
U 1 1 611C2FA6
P 8925 2025
AR Path="/6104FF77/611C2FA6" Ref="U?"  Part="1" 
AR Path="/611C2FA6" Ref="U6"  Part="1" 
AR Path="/6366C142/611C2FA6" Ref="U?"  Part="1" 
F 0 "U?" H 9100 2875 50  0000 C CNN
F 1 "74LS245" H 9200 2775 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 8925 2025 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS245" H 8925 2025 50  0001 C CNN
	1    8925 2025
	1    0    0    -1  
$EndComp
$Comp
L mirotan-due-cassette-shield-cache:+3.3V #PWR?
U 1 1 6133A2B5
P 8925 975
AR Path="/6104FF77/6133A2B5" Ref="#PWR?"  Part="1" 
AR Path="/6133A2B5" Ref="#PWR021"  Part="1" 
AR Path="/6366C142/6133A2B5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8925 825 50  0001 C CNN
F 1 "+3.3V" H 8940 1148 50  0000 C CNN
F 2 "" H 8925 975 50  0001 C CNN
F 3 "" H 8925 975 50  0001 C CNN
	1    8925 975 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8925 975  8925 1100
Wire Wire Line
	8925 1100 7800 1100
Wire Wire Line
	7800 1100 7800 2425
Wire Wire Line
	7800 2425 8425 2425
Connection ~ 8925 1100
Wire Wire Line
	8925 1100 8925 1225
Wire Wire Line
	8925 2825 8925 2850
$Comp
L power:+3V3 #PWR?
U 1 1 61672821
P 2150 10775
F 0 "#PWR?" H 2150 10625 50  0001 C CNN
F 1 "+3V3" H 2165 10948 50  0000 C CNN
F 2 "" H 2150 10775 50  0001 C CNN
F 3 "" H 2150 10775 50  0001 C CNN
	1    2150 10775
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1350 2000 1700
Wire Wire Line
	2525 1700 2000 1700
Connection ~ 2525 1700
Wire Wire Line
	2525 2150 2000 2150
Connection ~ 2525 2150
Wire Wire Line
	2000 2150 2000 2225
$Comp
L Connector:Conn_01x27_Female J?
U 1 1 61BCAB7A
P 1150 9550
F 0 "J?" H 1042 11035 50  0000 C CNN
F 1 "Conn_01x27_Female" H 1042 10944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x27_P2.54mm_Vertical" H 1150 9550 50  0001 C CNN
F 3 "~" H 1150 9550 50  0001 C CNN
	1    1150 9550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1350 8250 2075 8250
Wire Wire Line
	2150 10850 2150 10775
Wire Wire Line
	1350 10850 2150 10850
Text GLabel 1350 10750 2    50   Input ~ 0
VBAT
Text Notes 11900 10275 0    157  ~ 31
Power PCB
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J?
U 1 1 61931D66
P 11350 1825
F 0 "J?" H 11400 2342 50  0000 C CNN
F 1 "Conn_02x08_Odd_Even" H 11400 2251 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Horizontal" H 11350 1825 50  0001 C CNN
F 3 "~" H 11350 1825 50  0001 C CNN
	1    11350 1825
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J?
U 1 1 61932566
P 11400 3925
F 0 "J?" H 11450 4442 50  0000 C CNN
F 1 "Conn_02x08_Odd_Even" H 11450 4351 50  0000 C CNN
F 2 "ajm_kicad:PinHeader_2x08_P2.54mm_Horizontal_psionSide" H 11400 3925 50  0001 C CNN
F 3 "~" H 11400 3925 50  0001 C CNN
	1    11400 3925
	1    0    0    -1  
$EndComp
Text GLabel 11700 3625 2    50   Input ~ 0
P_SD0
Text GLabel 11200 3625 0    50   Input ~ 0
P_SD1
Text GLabel 11700 3725 2    50   Input ~ 0
P_SD2
Text GLabel 11200 3725 0    50   Input ~ 0
P_SD3
Text GLabel 11700 3825 2    50   Input ~ 0
P_SD4
Text GLabel 11200 3825 0    50   Input ~ 0
P_SD5
Text GLabel 11700 3925 2    50   Input ~ 0
P_SD6
Text GLabel 11200 3925 0    50   Input ~ 0
P_SD7
Text GLabel 11200 4025 0    50   Input ~ 0
P_SMR
Text GLabel 11700 4025 2    50   Input ~ 0
P_SCLK
Text GLabel 11200 4125 0    50   Input ~ 0
P_SOE
$Comp
L power:GND #PWR?
U 1 1 6193C2C5
P 8925 2850
F 0 "#PWR?" H 8925 2600 50  0001 C CNN
F 1 "GND" H 8930 2677 50  0000 C CNN
F 2 "" H 8925 2850 50  0001 C CNN
F 3 "" H 8925 2850 50  0001 C CNN
	1    8925 2850
	1    0    0    -1  
$EndComp
Text GLabel 8425 2025 0    50   Input ~ 0
SS2
Text GLabel 8425 2125 0    50   Input ~ 0
SS3
Text GLabel 9425 2025 2    50   Input ~ 0
P_SS2
Text GLabel 9425 2125 2    50   Input ~ 0
P_SS3
Text GLabel 11700 4125 2    50   Input ~ 0
P_SS1
$Comp
L power:GND #PWR?
U 1 1 6194A617
P 6925 5625
F 0 "#PWR?" H 6925 5375 50  0001 C CNN
F 1 "GND" H 6930 5452 50  0000 C CNN
F 2 "" H 6925 5625 50  0001 C CNN
F 3 "" H 6925 5625 50  0001 C CNN
	1    6925 5625
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6194AB66
P 10975 4550
F 0 "#PWR?" H 10975 4300 50  0001 C CNN
F 1 "GND" H 10980 4377 50  0000 C CNN
F 2 "" H 10975 4550 50  0001 C CNN
F 3 "" H 10975 4550 50  0001 C CNN
	1    10975 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10975 4225 11200 4225
Wire Wire Line
	10975 4225 10975 4550
Text GLabel 10675 4325 0    50   Input ~ 0
VCC_SLOT
Wire Wire Line
	11200 4325 10675 4325
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J?
U 1 1 61950CF0
P 11375 5825
F 0 "J?" H 11425 6342 50  0000 C CNN
F 1 "Conn_02x08_Odd_Even" H 11425 6251 50  0000 C CNN
F 2 "ajm_kicad:PinHeader_2x08_P2.54mm_Horizontal_psionSide" H 11375 5825 50  0001 C CNN
F 3 "~" H 11375 5825 50  0001 C CNN
	1    11375 5825
	1    0    0    -1  
$EndComp
Text GLabel 11675 5525 2    50   Input ~ 0
P_SD0
Text GLabel 11175 5525 0    50   Input ~ 0
P_SD1
Text GLabel 11675 5625 2    50   Input ~ 0
P_SD2
Text GLabel 11175 5625 0    50   Input ~ 0
P_SD3
Text GLabel 11675 5725 2    50   Input ~ 0
P_SD4
Text GLabel 11175 5725 0    50   Input ~ 0
P_SD5
Text GLabel 11675 5825 2    50   Input ~ 0
P_SD6
Text GLabel 11175 5825 0    50   Input ~ 0
P_SD7
Text GLabel 11175 5925 0    50   Input ~ 0
P_SMR
Text GLabel 11675 5925 2    50   Input ~ 0
P_SCLK
Text GLabel 11175 6025 0    50   Input ~ 0
P_SOE
Text GLabel 11675 6025 2    50   Input ~ 0
P_SS2
$Comp
L power:GND #PWR?
U 1 1 61950D03
P 10450 6400
F 0 "#PWR?" H 10450 6150 50  0001 C CNN
F 1 "GND" H 10455 6227 50  0000 C CNN
F 2 "" H 10450 6400 50  0001 C CNN
F 3 "" H 10450 6400 50  0001 C CNN
	1    10450 6400
	1    0    0    -1  
$EndComp
Text GLabel 11200 6225 0    50   Input ~ 0
VCC_SLOT
Wire Wire Line
	11175 6225 11200 6225
Text GLabel 1350 9850 2    50   Input ~ 0
LS_DIR
Text GLabel 1350 10550 2    50   Input ~ 0
5V_ON
Text GLabel 1350 10650 2    50   Input ~ 0
5V
$Comp
L New_Library:Si2302CDS-n-channel TR?
U 1 1 6195EB4C
P 2175 4375
F 0 "TR?" H 2453 4421 50  0000 L CNN
F 1 "Si2302CDS-n-channel" H 2453 4330 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2205 4525 20  0001 C CNN
F 3 "" H 2175 4375 50  0001 C CNN
	1    2175 4375
	1    0    0    -1  
$EndComp
$Comp
L GeekAmmo:P-CHANNEL-MOSFET M?
U 1 1 61960A66
P 2900 3525
F 0 "M?" H 3178 3571 50  0000 L CNN
F 1 "P-CHANNEL-MOSFET" H 3178 3480 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2930 3675 20  0001 C CNN
F 3 "" H 2900 3525 50  0001 C CNN
	1    2900 3525
	1    0    0    -1  
$EndComp
Text GLabel 1800 2900 0    50   Input ~ 0
5V
$Comp
L Device:R_Small R?
U 1 1 61962FE3
P 2275 3125
F 0 "R?" H 2334 3171 50  0000 L CNN
F 1 "10k" H 2334 3080 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 2275 3125 50  0001 C CNN
F 3 "~" H 2275 3125 50  0001 C CNN
	1    2275 3125
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2900 2275 2900
Wire Wire Line
	2275 2900 2275 3025
Wire Wire Line
	2275 3225 2275 3425
Wire Wire Line
	2700 3425 2275 3425
Connection ~ 2275 3425
Wire Wire Line
	2275 3425 2275 4075
Wire Wire Line
	2275 2900 3000 2900
Wire Wire Line
	3000 2900 3000 3225
Connection ~ 2275 2900
$Comp
L power:GND #PWR?
U 1 1 61968917
P 2275 4925
F 0 "#PWR?" H 2275 4675 50  0001 C CNN
F 1 "GND" H 2280 4752 50  0000 C CNN
F 2 "" H 2275 4925 50  0001 C CNN
F 3 "" H 2275 4925 50  0001 C CNN
	1    2275 4925
	1    0    0    -1  
$EndComp
Wire Wire Line
	2275 4925 2275 4675
Text GLabel 3325 4175 2    50   Input ~ 0
VCC_SLOT
Wire Wire Line
	3000 3825 3000 4175
Wire Wire Line
	3000 4175 3325 4175
Text GLabel 1675 4475 0    50   Input ~ 0
5V_ON
Wire Wire Line
	1675 4475 1975 4475
Text GLabel 1575 5675 0    50   Input ~ 0
VBAT_SWITCHED2
$Comp
L Memory_EEPROM:CAT24C256 U?
U 1 1 619D1109
P 6175 9950
F 0 "U?" H 5125 10400 50  0000 C CNN
F 1 "CAT24C256" H 5300 10300 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 6175 9950 50  0001 C CNN
F 3 "https://www.onsemi.cn/PowerSolutions/document/CAT24C256-D.PDF" H 6175 9950 50  0001 C CNN
	1    6175 9950
	1    0    0    -1  
$EndComp
$Comp
L Memory_EEPROM:CAT24C256 U?
U 1 1 619D20D1
P 4400 9950
F 0 "U?" H 3425 10375 50  0000 C CNN
F 1 "CAT24C256" H 3600 10275 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4400 9950 50  0001 C CNN
F 3 "https://www.onsemi.cn/PowerSolutions/document/CAT24C256-D.PDF" H 4400 9950 50  0001 C CNN
	1    4400 9950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 619D2D74
P 6175 9525
F 0 "#PWR?" H 6175 9375 50  0001 C CNN
F 1 "+3V3" H 6190 9698 50  0000 C CNN
F 2 "" H 6175 9525 50  0001 C CNN
F 3 "" H 6175 9525 50  0001 C CNN
	1    6175 9525
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 619D3861
P 4400 9575
F 0 "#PWR?" H 4400 9425 50  0001 C CNN
F 1 "+3V3" H 4415 9748 50  0000 C CNN
F 2 "" H 4400 9575 50  0001 C CNN
F 3 "" H 4400 9575 50  0001 C CNN
	1    4400 9575
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 9575 4400 9650
Wire Wire Line
	6175 9525 6175 9575
$Comp
L power:GND #PWR?
U 1 1 619DD7BA
P 4400 10400
F 0 "#PWR?" H 4400 10150 50  0001 C CNN
F 1 "GND" H 4405 10227 50  0000 C CNN
F 2 "" H 4400 10400 50  0001 C CNN
F 3 "" H 4400 10400 50  0001 C CNN
	1    4400 10400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 619DDD0C
P 6175 10425
F 0 "#PWR?" H 6175 10175 50  0001 C CNN
F 1 "GND" H 6180 10252 50  0000 C CNN
F 2 "" H 6175 10425 50  0001 C CNN
F 3 "" H 6175 10425 50  0001 C CNN
	1    6175 10425
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 10250 4400 10325
Wire Wire Line
	6175 10250 6175 10325
$Comp
L power:GND #PWR?
U 1 1 619ED0E1
P 3325 8250
AR Path="/6104FF77/619ED0E1" Ref="#PWR?"  Part="1" 
AR Path="/619ED0E1" Ref="#PWR0114"  Part="1" 
AR Path="/6366C142/619ED0E1" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3325 8000 50  0001 C CNN
F 1 "GND" H 3325 8075 50  0000 C CNN
F 2 "" H 3325 8250 50  0001 C CNN
F 3 "" H 3325 8250 50  0001 C CNN
	1    3325 8250
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 619ED0E7
P 3400 9175
F 0 "#PWR?" H 3400 9025 50  0001 C CNN
F 1 "+3V3" H 3415 9348 50  0000 C CNN
F 2 "" H 3400 9175 50  0001 C CNN
F 3 "" H 3400 9175 50  0001 C CNN
	1    3400 9175
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 8250 3325 8250
Wire Wire Line
	3400 9250 3400 9175
Wire Wire Line
	2600 9250 3400 9250
Text GLabel 2600 8350 2    50   Input ~ 0
PICO_TX
Text GLabel 2600 8450 2    50   Input ~ 0
PICO_RX
Text GLabel 2600 8550 2    50   Input ~ 0
OLED_SCL
Text GLabel 2600 8650 2    50   Input ~ 0
OLED_SDA
Text GLabel 1350 10450 2    50   Input ~ 0
VBAT_SWITCHED2
Text GLabel 1350 10350 2    50   Input ~ 0
EX_ON_INV
Text GLabel 1350 10250 2    50   Input ~ 0
ESP_ON
Wire Wire Line
	6925 5525 6925 5575
Text GLabel 1350 9950 2    50   Input ~ 0
SD_OE
Text GLabel 1350 10050 2    50   Input ~ 0
SC_OE
Text GLabel 8425 2525 0    50   Input ~ 0
SD_OE
Text GLabel 6425 5225 0    50   Input ~ 0
SC_OE
Text GLabel 2600 8750 2    50   Input ~ 0
SCLKOUT
Text GLabel 2600 8850 2    50   Input ~ 0
SDAOUT
Text GLabel 2600 8950 2    50   Input ~ 0
QH_O
Text GLabel 2600 9050 2    50   Input ~ 0
LATCHOUT1
Text GLabel 2600 9150 2    50   Input ~ 0
LATCHOUT2
$Comp
L 74xx:74HC595 U?
U 1 1 61A457E4
P 8400 10150
F 0 "U?" H 8600 10875 50  0000 C CNN
F 1 "74HC595" H 8725 10800 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 8400 10150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 8400 10150 50  0001 C CNN
	1    8400 10150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 61A48EB9
P 8400 9425
F 0 "#PWR?" H 8400 9275 50  0001 C CNN
F 1 "+3V3" H 8415 9598 50  0000 C CNN
F 2 "" H 8400 9425 50  0001 C CNN
F 3 "" H 8400 9425 50  0001 C CNN
	1    8400 9425
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 9425 8400 9475
$Comp
L power:GND #PWR?
U 1 1 61A4E362
P 8400 10950
F 0 "#PWR?" H 8400 10700 50  0001 C CNN
F 1 "GND" H 8405 10777 50  0000 C CNN
F 2 "" H 8400 10950 50  0001 C CNN
F 3 "" H 8400 10950 50  0001 C CNN
	1    8400 10950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 10950 8400 10900
Wire Wire Line
	8000 10350 7900 10350
Wire Wire Line
	7900 10350 7900 10900
Wire Wire Line
	7900 10900 8400 10900
Connection ~ 8400 10900
Wire Wire Line
	8400 10900 8400 10850
Text GLabel 8000 9750 0    50   Input ~ 0
QH_O
Text GLabel 8000 9950 0    50   Input ~ 0
SCLKOUT
Wire Wire Line
	8400 9475 7475 9475
Wire Wire Line
	7475 9475 7475 10050
Wire Wire Line
	7475 10050 8000 10050
Connection ~ 8400 9475
Wire Wire Line
	8400 9475 8400 9550
Text GLabel 8000 10250 0    50   Input ~ 0
LATCHOUT2
Text GLabel 4800 9950 2    50   Input ~ 0
OLED_SCL
Text GLabel 4800 9850 2    50   Input ~ 0
OLED_SDA
Text GLabel 6575 9950 2    50   Input ~ 0
OLED_SCL
Text GLabel 6575 9850 2    50   Input ~ 0
OLED_SDA
Wire Wire Line
	4800 10050 4950 10050
Wire Wire Line
	4950 10050 4950 10325
Wire Wire Line
	4950 10325 4400 10325
Connection ~ 4400 10325
Wire Wire Line
	4400 10325 4400 10400
Wire Wire Line
	6575 10050 6575 10325
Wire Wire Line
	6575 10325 6175 10325
Connection ~ 6175 10325
Wire Wire Line
	6175 10325 6175 10425
Text Notes 4725 9200 0    50   ~ 0
Device address\n1 0 1 0 0 A1 A0 (RW)\n
Wire Wire Line
	4000 10050 3925 10050
Wire Wire Line
	3925 10050 3925 10325
Wire Wire Line
	3925 10325 4400 10325
Wire Wire Line
	4000 9850 3925 9850
Wire Wire Line
	3925 9850 3925 9950
Connection ~ 3925 10050
Wire Wire Line
	4000 9950 3925 9950
Connection ~ 3925 9950
Wire Wire Line
	3925 9950 3925 10050
Wire Wire Line
	5775 9950 5675 9950
Wire Wire Line
	5675 9950 5675 10050
Wire Wire Line
	5675 10050 5775 10050
Wire Wire Line
	5675 10050 5675 10325
Wire Wire Line
	5675 10325 6175 10325
Connection ~ 5675 10050
Wire Wire Line
	5775 9850 5775 9575
Wire Wire Line
	5775 9575 6175 9575
Connection ~ 6175 9575
Wire Wire Line
	6175 9575 6175 9650
$Comp
L Connector:Conn_01x11_Female J?
U 1 1 61B733DC
P 2400 8750
F 0 "J?" H 2292 9435 50  0000 C CNN
F 1 "Conn_01x11_Female" H 2292 9344 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x11_P2.54mm_Vertical" H 2400 8750 50  0001 C CNN
F 3 "~" H 2400 8750 50  0001 C CNN
	1    2400 8750
	-1   0    0    -1  
$EndComp
Text GLabel 11650 1625 2    50   Input ~ 0
P_SD1
Text GLabel 11650 1825 2    50   Input ~ 0
P_SD3
Text GLabel 11150 1725 0    50   Input ~ 0
P_SD5
Text GLabel 11150 1525 0    50   Input ~ 0
P_SD7
Text GLabel 11650 2225 2    50   Input ~ 0
P_SMR
Text GLabel 11150 2225 0    50   Input ~ 0
P_SOE
Text GLabel 11650 1525 2    50   Input ~ 0
P_SD0
Text GLabel 11650 1725 2    50   Input ~ 0
P_SD2
Text GLabel 11150 1825 0    50   Input ~ 0
P_SD4
Text GLabel 11150 1625 0    50   Input ~ 0
P_SD6
Text GLabel 11650 1925 2    50   Input ~ 0
P_SCLK
Text GLabel 11650 2025 2    50   Input ~ 0
P_SS3
Text GLabel 11150 2125 0    50   Input ~ 0
VCC_SLOT
$Comp
L power:GND #PWR?
U 1 1 61D620F7
P 10375 2375
F 0 "#PWR?" H 10375 2125 50  0001 C CNN
F 1 "GND" H 10380 2202 50  0000 C CNN
F 2 "" H 10375 2375 50  0001 C CNN
F 3 "" H 10375 2375 50  0001 C CNN
	1    10375 2375
	1    0    0    -1  
$EndComp
Wire Wire Line
	10375 1925 11150 1925
Wire Wire Line
	10375 1925 10375 2375
Text GLabel 11650 2125 2    50   Input ~ 0
EX_ON
Text GLabel 11150 2025 0    50   Input ~ 0
EXT_PWR_IN
Text GLabel 1350 10150 2    50   Input ~ 0
EXT_PWR_IN
$Comp
L Connector:Conn_01x02_Female J?
U 1 1 61D9012E
P 4800 8600
F 0 "J?" H 4828 8576 50  0000 L CNN
F 1 "Conn_01x02_Female" H 4828 8485 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical_SMD_Pin1Left" H 4800 8600 50  0001 C CNN
F 3 "~" H 4800 8600 50  0001 C CNN
	1    4800 8600
	1    0    0    -1  
$EndComp
Text GLabel 4600 8600 0    50   Input ~ 0
VBAT
$Comp
L power:GND #PWR?
U 1 1 61D9AA18
P 4450 8800
AR Path="/6104FF77/61D9AA18" Ref="#PWR?"  Part="1" 
AR Path="/61D9AA18" Ref="#PWR0123"  Part="1" 
AR Path="/6366C142/61D9AA18" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4450 8550 50  0001 C CNN
F 1 "GND" H 4450 8625 50  0000 C CNN
F 2 "" H 4450 8800 50  0001 C CNN
F 3 "" H 4450 8800 50  0001 C CNN
	1    4450 8800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 8800 4450 8700
Wire Wire Line
	4450 8700 4600 8700
Text GLabel 8800 10050 2    50   Input ~ 0
SS1
Text GLabel 8800 10150 2    50   Input ~ 0
SS2
Text GLabel 8800 10250 2    50   Input ~ 0
SS3
Text GLabel 1350 9150 2    50   Input ~ 0
SPGM
Text GLabel 11700 4225 2    50   Input ~ 0
P_SPGM
Text GLabel 11675 6125 2    50   Input ~ 0
P_SPGM
$Comp
L power:+3V3 #PWR?
U 1 1 61BE794A
P 6575 7950
F 0 "#PWR?" H 6575 7800 50  0001 C CNN
F 1 "+3V3" H 6590 8123 50  0000 C CNN
F 2 "" H 6575 7950 50  0001 C CNN
F 3 "" H 6575 7950 50  0001 C CNN
	1    6575 7950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61BE7950
P 7100 8500
F 0 "C?" H 7215 8546 50  0000 L CNN
F 1 "100n" H 7215 8455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7138 8350 50  0001 C CNN
F 3 "~" H 7100 8500 50  0001 C CNN
	1    7100 8500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61BE7956
P 7550 8500
F 0 "C?" H 7665 8546 50  0000 L CNN
F 1 "100n" H 7665 8455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7588 8350 50  0001 C CNN
F 3 "~" H 7550 8500 50  0001 C CNN
	1    7550 8500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61BE795C
P 8000 8500
F 0 "C?" H 8115 8546 50  0000 L CNN
F 1 "100n" H 8115 8455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 8038 8350 50  0001 C CNN
F 3 "~" H 8000 8500 50  0001 C CNN
	1    8000 8500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 8350 8000 8300
Wire Wire Line
	7550 8350 7550 8300
Connection ~ 7550 8300
Wire Wire Line
	7550 8300 8000 8300
Wire Wire Line
	7100 8350 7100 8300
Wire Wire Line
	7100 8300 7550 8300
$Comp
L power:GND #PWR?
U 1 1 61BE7968
P 8000 8800
F 0 "#PWR?" H 8000 8550 50  0001 C CNN
F 1 "GND" H 8005 8627 50  0000 C CNN
F 2 "" H 8000 8800 50  0001 C CNN
F 3 "" H 8000 8800 50  0001 C CNN
	1    8000 8800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 8650 7100 8750
Wire Wire Line
	7550 8650 7550 8750
Wire Wire Line
	7550 8750 7100 8750
Wire Wire Line
	8000 8650 8000 8750
$Comp
L power:GND #PWR?
U 1 1 61BE7972
P 6575 8825
F 0 "#PWR?" H 6575 8575 50  0001 C CNN
F 1 "GND" H 6580 8652 50  0000 C CNN
F 2 "" H 6575 8825 50  0001 C CNN
F 3 "" H 6575 8825 50  0001 C CNN
	1    6575 8825
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 8750 8000 8750
Connection ~ 7550 8750
Connection ~ 8000 8750
Wire Wire Line
	8000 8750 8000 8800
Wire Wire Line
	6575 7950 6575 8300
Wire Wire Line
	7100 8300 6575 8300
Connection ~ 7100 8300
Wire Wire Line
	7100 8750 6575 8750
Connection ~ 7100 8750
Wire Wire Line
	6575 8750 6575 8825
Text GLabel 1350 9550 2    50   Input ~ 0
BUZZER
$Comp
L pak_breakout:AMS1117-5.0 U?
U 1 1 61C9C9EE
P 3025 5675
F 0 "U?" H 3025 5917 50  0000 C CNN
F 1 "AMS1117-5.0" H 3025 5826 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 3025 5875 50  0001 C CNN
F 3 "" H 3125 5425 50  0001 C CNN
	1    3025 5675
	1    0    0    -1  
$EndComp
Text Notes 750  11000 0    50   ~ 0
*5V generated on this PCB*
$Comp
L Device:C_Small C?
U 1 1 61CB733E
P 2050 6000
F 0 "C?" H 2142 6046 50  0000 L CNN
F 1 "100nF" H 2142 5955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2050 6000 50  0001 C CNN
F 3 "~" H 2050 6000 50  0001 C CNN
	1    2050 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61CB7344
P 3775 6000
F 0 "C?" H 3867 6046 50  0000 L CNN
F 1 "100nF" H 3867 5955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3775 6000 50  0001 C CNN
F 3 "~" H 3775 6000 50  0001 C CNN
	1    3775 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61CB734A
P 2050 6175
F 0 "#PWR?" H 2050 5925 50  0001 C CNN
F 1 "GND" H 2055 6002 50  0000 C CNN
F 2 "" H 2050 6175 50  0001 C CNN
F 3 "" H 2050 6175 50  0001 C CNN
	1    2050 6175
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61CB7350
P 3025 6175
F 0 "#PWR?" H 3025 5925 50  0001 C CNN
F 1 "GND" H 3030 6002 50  0000 C CNN
F 2 "" H 3025 6175 50  0001 C CNN
F 3 "" H 3025 6175 50  0001 C CNN
	1    3025 6175
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61CB7356
P 3775 6150
F 0 "#PWR?" H 3775 5900 50  0001 C CNN
F 1 "GND" H 3780 5977 50  0000 C CNN
F 2 "" H 3775 6150 50  0001 C CNN
F 3 "" H 3775 6150 50  0001 C CNN
	1    3775 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2725 5675 2050 5675
Wire Wire Line
	2050 5900 2050 5675
Connection ~ 2050 5675
Wire Wire Line
	2050 5675 1575 5675
Wire Wire Line
	2050 6100 2050 6175
Wire Wire Line
	3025 5975 3025 6175
Wire Wire Line
	3775 6100 3775 6150
Wire Wire Line
	3775 5900 3775 5675
Wire Wire Line
	3775 5675 3325 5675
Wire Wire Line
	4100 5675 3775 5675
Connection ~ 3775 5675
Text GLabel 1350 9650 2    50   Input ~ 0
VBAT_SWITCHED1
Text GLabel 4100 5675 2    50   Input ~ 0
5V
$Comp
L power:GND #PWR?
U 1 1 61D265E4
P 15750 1550
F 0 "#PWR?" H 15750 1300 50  0001 C CNN
F 1 "GND" H 15755 1377 50  0000 C CNN
F 2 "" H 15750 1550 50  0001 C CNN
F 3 "" H 15750 1550 50  0001 C CNN
	1    15750 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	15750 1550 15750 1500
Wire Wire Line
	8525 4925 7500 4925
Wire Wire Line
	7425 4825 7625 4825
Wire Wire Line
	8525 4725 7750 4725
Wire Wire Line
	7425 4625 7875 4625
Wire Wire Line
	8525 4525 8000 4525
Wire Wire Line
	8525 4325 8250 4325
Wire Wire Line
	7425 4225 8375 4225
$Comp
L Device:R_Small R?
U 1 1 61DD98D5
P 7500 5225
F 0 "R?" V 7500 4500 50  0000 L CNN
F 1 "100k" V 7500 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7500 5225 50  0001 C CNN
F 3 "~" H 7500 5225 50  0001 C CNN
	1    7500 5225
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS245 U?
U 1 1 611C2F85
P 6925 4725
AR Path="/6104FF77/611C2F85" Ref="U?"  Part="1" 
AR Path="/611C2F85" Ref="U7"  Part="1" 
AR Path="/6366C142/611C2F85" Ref="U?"  Part="1" 
F 0 "U?" H 7075 5725 50  0000 C CNN
F 1 "74LS245" H 7175 5625 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 6925 4725 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS245" H 6925 4725 50  0001 C CNN
	1    6925 4725
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E00E84
P 7625 5225
F 0 "R?" V 7625 4500 50  0000 L CNN
F 1 "100k" V 7625 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7625 5225 50  0001 C CNN
F 3 "~" H 7625 5225 50  0001 C CNN
	1    7625 5225
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E08C5A
P 7750 5225
F 0 "R?" V 7750 4500 50  0000 L CNN
F 1 "100k" V 7750 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7750 5225 50  0001 C CNN
F 3 "~" H 7750 5225 50  0001 C CNN
	1    7750 5225
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E10900
P 7875 5225
F 0 "R?" V 7875 4500 50  0000 L CNN
F 1 "100k" V 7875 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7875 5225 50  0001 C CNN
F 3 "~" H 7875 5225 50  0001 C CNN
	1    7875 5225
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E1A197
P 8000 5225
F 0 "R?" V 8000 4500 50  0000 L CNN
F 1 "100k" V 8000 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 8000 5225 50  0001 C CNN
F 3 "~" H 8000 5225 50  0001 C CNN
	1    8000 5225
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E1A19D
P 8125 5225
F 0 "R?" V 8125 4500 50  0000 L CNN
F 1 "100k" V 8125 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 8125 5225 50  0001 C CNN
F 3 "~" H 8125 5225 50  0001 C CNN
	1    8125 5225
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E1A1A3
P 8250 5225
F 0 "R?" V 8250 4450 50  0000 L CNN
F 1 "100k" V 8250 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 8250 5225 50  0001 C CNN
F 3 "~" H 8250 5225 50  0001 C CNN
	1    8250 5225
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61E1A1A9
P 8375 5225
F 0 "R?" V 8375 4450 50  0000 L CNN
F 1 "100k" V 8375 4625 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 8375 5225 50  0001 C CNN
F 3 "~" H 8375 5225 50  0001 C CNN
	1    8375 5225
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5125 7500 4925
Connection ~ 7500 4925
Wire Wire Line
	7500 4925 7425 4925
Connection ~ 7625 4825
Wire Wire Line
	7625 4825 8525 4825
Wire Wire Line
	7625 4825 7625 5125
Wire Wire Line
	7750 5125 7750 4725
Connection ~ 7750 4725
Wire Wire Line
	7750 4725 7425 4725
Wire Wire Line
	7875 5125 7875 4625
Connection ~ 7875 4625
Wire Wire Line
	7875 4625 8525 4625
Wire Wire Line
	8000 5125 8000 4525
Connection ~ 8000 4525
Wire Wire Line
	8000 4525 7425 4525
Wire Wire Line
	8125 5125 8125 4425
Wire Wire Line
	7425 4425 8125 4425
Connection ~ 8125 4425
Wire Wire Line
	8125 4425 8525 4425
Wire Wire Line
	8250 5125 8250 4325
Connection ~ 8250 4325
Wire Wire Line
	8250 4325 7425 4325
Wire Wire Line
	8375 5125 8375 4225
Connection ~ 8375 4225
Wire Wire Line
	8375 4225 8525 4225
Wire Wire Line
	6925 5575 7500 5575
Wire Wire Line
	8375 5575 8375 5325
Connection ~ 6925 5575
Wire Wire Line
	6925 5575 6925 5625
Wire Wire Line
	8250 5325 8250 5575
Connection ~ 8250 5575
Wire Wire Line
	8250 5575 8375 5575
Wire Wire Line
	8125 5325 8125 5575
Connection ~ 8125 5575
Wire Wire Line
	8125 5575 8250 5575
Connection ~ 8000 5575
Wire Wire Line
	8000 5575 8125 5575
Wire Wire Line
	7875 5325 7875 5575
Connection ~ 7875 5575
Wire Wire Line
	7875 5575 8000 5575
Wire Wire Line
	8000 5325 8000 5575
Wire Wire Line
	7750 5325 7750 5575
Connection ~ 7750 5575
Wire Wire Line
	7750 5575 7875 5575
Connection ~ 7625 5575
Wire Wire Line
	7625 5575 7750 5575
Wire Wire Line
	7500 5325 7500 5575
Connection ~ 7500 5575
Wire Wire Line
	7500 5575 7625 5575
Wire Wire Line
	7625 5325 7625 5575
Text GLabel 11125 6975 2    50   Input ~ 0
EX_ON_INV
Text GLabel 8850 8050 0    50   Input ~ 0
EX_ON
$Comp
L Device:R_Small R?
U 1 1 61EF7E98
P 9050 7375
F 0 "R?" H 9109 7421 50  0000 L CNN
F 1 "100k" H 9109 7330 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 9050 7375 50  0001 C CNN
F 3 "~" H 9050 7375 50  0001 C CNN
	1    9050 7375
	1    0    0    -1  
$EndComp
Text GLabel 9875 6525 0    50   Input ~ 0
VBAT
Wire Wire Line
	8850 8050 9050 8050
$Comp
L power:GND #PWR?
U 1 1 61F34648
P 9625 8775
F 0 "#PWR?" H 9625 8525 50  0001 C CNN
F 1 "GND" H 9630 8602 50  0000 C CNN
F 2 "" H 9625 8775 50  0001 C CNN
F 3 "" H 9625 8775 50  0001 C CNN
	1    9625 8775
	1    0    0    -1  
$EndComp
Wire Wire Line
	9625 8775 9625 8250
$Comp
L Device:R_Small R?
U 1 1 61F64F12
P 10075 6775
F 0 "R?" H 10134 6821 50  0000 L CNN
F 1 "10k" H 10134 6730 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 10075 6775 50  0001 C CNN
F 3 "~" H 10075 6775 50  0001 C CNN
	1    10075 6775
	1    0    0    -1  
$EndComp
Wire Wire Line
	10075 6525 10075 6675
Wire Wire Line
	9875 6525 10075 6525
$Comp
L Device:R_Small R?
U 1 1 61F8A895
P 10075 7500
F 0 "R?" H 10134 7546 50  0000 L CNN
F 1 "10k" H 10134 7455 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 10075 7500 50  0001 C CNN
F 3 "~" H 10075 7500 50  0001 C CNN
	1    10075 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10075 6875 10075 6975
$Comp
L Device:D_Zener D?
U 1 1 61FAF4DB
P 10800 7950
F 0 "D?" V 10754 8029 50  0000 L CNN
F 1 "3V0" V 10845 8029 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" H 10800 7950 50  0001 C CNN
F 3 "~" H 10800 7950 50  0001 C CNN
	1    10800 7950
	0    1    1    0   
$EndComp
Wire Wire Line
	10075 6975 10800 6975
Wire Wire Line
	10800 6975 10800 7800
$Comp
L power:GND #PWR?
U 1 1 61FBD07E
P 10800 8775
F 0 "#PWR?" H 10800 8525 50  0001 C CNN
F 1 "GND" H 10805 8602 50  0000 C CNN
F 2 "" H 10800 8775 50  0001 C CNN
F 3 "" H 10800 8775 50  0001 C CNN
	1    10800 8775
	1    0    0    -1  
$EndComp
Wire Wire Line
	10800 8100 10800 8775
Connection ~ 9050 8050
Wire Wire Line
	10450 6125 10450 6400
Wire Wire Line
	10450 6125 11175 6125
Wire Wire Line
	10800 6975 11125 6975
Connection ~ 10800 6975
Wire Wire Line
	10075 6975 10075 7400
Connection ~ 10075 6975
Wire Wire Line
	9625 7650 9625 6975
Wire Wire Line
	9625 6975 10075 6975
$Comp
L power:GND #PWR?
U 1 1 6206B324
P 10075 8775
F 0 "#PWR?" H 10075 8525 50  0001 C CNN
F 1 "GND" H 10080 8602 50  0000 C CNN
F 2 "" H 10075 8775 50  0001 C CNN
F 3 "" H 10075 8775 50  0001 C CNN
	1    10075 8775
	1    0    0    -1  
$EndComp
Wire Wire Line
	10075 7600 10075 8775
Wire Wire Line
	9050 7275 9050 6975
Wire Wire Line
	9050 6975 9625 6975
Connection ~ 9625 6975
Wire Wire Line
	9050 7475 9050 8050
Wire Wire Line
	9050 8050 9325 8050
$Comp
L New_Library:Si2302CDS-n-channel TR?
U 1 1 620C915B
P 9525 7950
F 0 "TR?" H 9175 7675 50  0000 L CNN
F 1 "Si2302CDS-n-ch" H 8850 7575 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 9555 8100 20  0001 C CNN
F 3 "" H 9525 7950 50  0001 C CNN
	1    9525 7950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 620CB578
P 7450 2550
F 0 "R?" H 7509 2596 50  0000 L CNN
F 1 "100k" H 7509 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7450 2550 50  0001 C CNN
F 3 "~" H 7450 2550 50  0001 C CNN
	1    7450 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8425 2225 7450 2225
Wire Wire Line
	7450 2225 7450 2450
$Comp
L power:GND #PWR?
U 1 1 620D96A2
P 7450 2825
F 0 "#PWR?" H 7450 2575 50  0001 C CNN
F 1 "GND" H 7455 2652 50  0000 C CNN
F 2 "" H 7450 2825 50  0001 C CNN
F 3 "" H 7450 2825 50  0001 C CNN
	1    7450 2825
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2650 7450 2825
$Comp
L Connector:Conn_01x01_Female J?
U 1 1 622FDD65
P 5600 6975
F 0 "J?" H 5628 7001 50  0000 L CNN
F 1 "Conn_01x01_Female" H 5628 6910 50  0000 L CNN
F 2 "ajm_kicad:psion_beeper_pad" H 5600 6975 50  0001 C CNN
F 3 "~" H 5600 6975 50  0001 C CNN
	1    5600 6975
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Female J?
U 1 1 622FE035
P 5600 7275
F 0 "J?" H 5628 7301 50  0000 L CNN
F 1 "Conn_01x01_Female" H 5628 7210 50  0000 L CNN
F 2 "ajm_kicad:psion_beeper_pad" H 5600 7275 50  0001 C CNN
F 3 "~" H 5600 7275 50  0001 C CNN
	1    5600 7275
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 622FEC3B
P 5250 7325
F 0 "#PWR?" H 5250 7075 50  0001 C CNN
F 1 "GND" H 5255 7152 50  0000 C CNN
F 2 "" H 5250 7325 50  0001 C CNN
F 3 "" H 5250 7325 50  0001 C CNN
	1    5250 7325
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 7325 5250 7275
Wire Wire Line
	5250 7275 5400 7275
Text GLabel 5400 6975 0    50   Input ~ 0
BUZZER
Text GLabel 8800 9750 2    50   Input ~ 0
VPP_ON
Text GLabel 8800 9850 2    50   Input ~ 0
VPP_VOLT_SELECT
$Sheet
S 13250 6950 1500 825 
U 63087D5B
F0 "VPP" 50
F1 "file63087D5A.sch" 50
$EndSheet
Text GLabel 11700 4325 2    50   Input ~ 0
VPP_DRIVE
Text GLabel 11675 6225 2    50   Input ~ 0
VPP_DRIVE
$EndSCHEMATC
