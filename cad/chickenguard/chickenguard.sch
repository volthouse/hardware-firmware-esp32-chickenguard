EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x18 J1
U 1 1 5C82815B
P 3800 2450
F 0 "J1" H 3719 3375 50  0000 C CNN
F 1 "Conn_01x18" H 3550 3400 50  0001 L CNN
F 2 "" H 3800 2450 50  0001 C CNN
F 3 "~" H 3800 2450 50  0001 C CNN
	1    3800 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x18 J2
U 1 1 5C8281B7
P 5250 2450
F 0 "J2" H 5170 3375 50  0000 C CNN
F 1 "Conn_01x18" H 5170 3376 50  0001 C CNN
F 2 "" H 5250 2450 50  0001 C CNN
F 3 "~" H 5250 2450 50  0001 C CNN
	1    5250 2450
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5C82849C
P 8950 1700
F 0 "#PWR?" H 8950 1550 50  0001 C CNN
F 1 "+5V" H 8965 1873 50  0000 C CNN
F 2 "" H 8950 1700 50  0001 C CNN
F 3 "" H 8950 1700 50  0001 C CNN
	1    8950 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C828502
P 9200 1700
F 0 "#PWR?" H 9200 1450 50  0001 C CNN
F 1 "GND" H 9205 1527 50  0000 C CNN
F 2 "" H 9200 1700 50  0001 C CNN
F 3 "" H 9200 1700 50  0001 C CNN
	1    9200 1700
	-1   0    0    1   
$EndComp
Text Label 5600 1650 0    50   ~ 0
GND
Text Label 5600 1750 0    50   ~ 0
+5V
Wire Wire Line
	1750 1650 1850 1650
Text Label 2850 1650 0    50   ~ 0
GND
Text Label 2850 1750 0    50   ~ 0
+5V
Wire Wire Line
	5450 1850 5900 1850
Text Label 5600 1850 0    50   ~ 0
3.3V
Text Label 2850 1850 0    50   ~ 0
3.3V
$Comp
L Connector:Screw_Terminal_01x12 J4
U 1 1 5C828744
P 7050 2150
F 0 "J4" H 7000 2900 50  0000 L CNN
F 1 "Screw_Terminal_01x12" H 6850 2800 50  0000 L CNN
F 2 "" H 7050 2150 50  0001 C CNN
F 3 "~" H 7050 2150 50  0001 C CNN
	1    7050 2150
	1    0    0    -1  
$EndComp
Text Notes 3900 2050 0    50   ~ 0
GPIO13
Text Notes 3900 2150 0    50   ~ 0
GPIO12
Text Notes 3900 2250 0    50   ~ 0
GPIO14
Text Notes 3900 2350 0    50   ~ 0
GPIO27
Text Notes 3900 2450 0    50   ~ 0
GPIO26
Text Notes 3900 2550 0    50   ~ 0
GPIO25
Text Notes 3900 2650 0    50   ~ 0
XTAL32
Text Notes 3900 2750 0    50   ~ 0
XTAL32
Text Notes 3900 2850 0    50   ~ 0
GPIO35* ADC1_7
Text Notes 3900 2950 0    50   ~ 0
GPIO34* ADC1_6
Text Notes 3900 3050 0    50   ~ 0
GPIO39*
Text Notes 3900 3150 0    50   ~ 0
GPIO38*
Text Notes 3900 3250 0    50   ~ 0
GPIO37*
Text Notes 3900 3350 0    50   ~ 0
GPIO36*
Text Notes 4850 2050 0    50   ~ 0
GPIO1
Text Notes 4850 2150 0    50   ~ 0
GPIO3
Text Notes 4850 2250 0    50   ~ 0
GPIO15
Text Notes 4850 2350 0    50   ~ 0
GPIO2
Text Notes 4850 2450 0    50   ~ 0
GPIO0
Text Notes 4850 2550 0    50   ~ 0
GPIO4
Text Notes 4850 2650 0    50   ~ 0
GPIO16
Text Notes 4850 2750 0    50   ~ 0
GPIO17
Text Notes 4850 2850 0    50   ~ 0
GPIO5
Text Notes 4650 2950 0    50   ~ 0
GPIO18 SCK
Text Notes 4650 3050 0    50   ~ 0
GPIO23 MOSI
Text Notes 4650 3150 0    50   ~ 0
GPIO19 MISO
Text Notes 4650 3250 0    50   ~ 0
GPIO22 SCL
Text Notes 4650 3350 0    50   ~ 0
GPIO21 SDA
Text Label 2850 2450 0    50   ~ 0
MOT_DRV
Text Label 2850 2550 0    50   ~ 0
MOT_DIR
Text Label 2850 2350 0    50   ~ 0
MOT_PWM
Text Label 2850 2050 0    50   ~ 0
DIST_TRIGGER
Text Label 2850 2250 0    50   ~ 0
DIST_ECHO
Text Label 5600 2850 0    50   ~ 0
UP_BUTTON
Text Label 5600 2750 0    50   ~ 0
DOWN_BUTTON
Wire Wire Line
	5450 1950 5900 1950
Text Label 5600 1950 0    50   ~ 0
GND
Wire Wire Line
	5450 2050 5900 2050
Wire Wire Line
	5450 2150 5900 2150
Text Label 5600 2050 0    50   ~ 0
U0_TX
Text Label 5600 2150 0    50   ~ 0
U0_RX
Wire Wire Line
	5450 2250 5900 2250
Wire Wire Line
	5450 2550 5900 2550
Text Label 5600 2250 0    50   ~ 0
OLED_SCL
Text Label 5600 2550 0    50   ~ 0
OLED_SDA
Wire Wire Line
	5450 2650 5900 2650
Text Label 5600 2650 0    50   ~ 0
OLED_RST
Wire Wire Line
	3600 1950 2850 1950
Text Label 2850 1950 0    50   ~ 0
RESET
Text Label 5600 2450 0    50   ~ 0
STOP_SWITCH
$Comp
L Connector:Jack-DC J5
U 1 1 5C82D45C
P 9700 1950
F 0 "J5" H 9470 2000 50  0000 R CNN
F 1 "Jack-DC" H 9470 1909 50  0000 R CNN
F 2 "" H 9750 1910 50  0001 C CNN
F 3 "~" H 9750 1910 50  0001 C CNN
	1    9700 1950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9400 1850 9200 1850
Wire Wire Line
	9200 1850 9200 1700
Wire Wire Line
	9400 2050 8950 2050
Wire Wire Line
	8950 2050 8950 1700
Text Label 2850 2850 0    50   ~ 0
LUMINANCE
Wire Wire Line
	2700 3150 1750 3150
$Comp
L Connector:Screw_Terminal_01x16 J3
U 1 1 5C8286D7
P 1550 2350
F 0 "J3" H 1550 3300 50  0000 C CNN
F 1 "Screw_Terminal_01x16" H 1550 3200 50  0000 C CNN
F 2 "" H 1550 2350 50  0001 C CNN
F 3 "~" H 1550 2350 50  0001 C CNN
	1    1550 2350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3600 2550 2600 2550
Wire Wire Line
	2600 2550 2600 3050
Wire Wire Line
	2600 3050 1750 3050
Wire Wire Line
	2500 2450 2500 2950
Wire Wire Line
	2500 2950 1750 2950
Wire Wire Line
	2500 2450 3600 2450
Wire Wire Line
	2400 2350 2400 2850
Wire Wire Line
	2400 2850 1750 2850
Wire Wire Line
	2400 2350 3600 2350
Wire Wire Line
	2300 2250 2300 2750
Wire Wire Line
	2300 2750 1750 2750
Wire Wire Line
	2300 2250 3600 2250
Wire Wire Line
	2200 2050 2200 2650
Wire Wire Line
	2200 2650 1750 2650
Wire Wire Line
	2200 2050 3600 2050
Wire Wire Line
	1750 1950 1850 1950
Wire Wire Line
	1850 1950 1850 1850
Connection ~ 1850 1650
Wire Wire Line
	1850 1650 3600 1650
Wire Wire Line
	1750 1750 1850 1750
Connection ~ 1850 1750
Wire Wire Line
	1850 1750 1850 1650
Wire Wire Line
	1750 1850 1850 1850
Connection ~ 1850 1850
Wire Wire Line
	1850 1850 1850 1750
Wire Wire Line
	1950 1750 1950 2350
Wire Wire Line
	1950 2350 1850 2350
Wire Wire Line
	1950 1750 3600 1750
Wire Wire Line
	1750 2050 1850 2050
Wire Wire Line
	1850 2050 1850 2150
Connection ~ 1850 2350
Wire Wire Line
	1850 2350 1750 2350
Wire Wire Line
	1750 2150 1850 2150
Connection ~ 1850 2150
Wire Wire Line
	1850 2150 1850 2250
Wire Wire Line
	1750 2250 1850 2250
Connection ~ 1850 2250
Wire Wire Line
	1850 2250 1850 2350
Wire Wire Line
	2700 2850 3600 2850
Wire Wire Line
	2700 2850 2700 3150
Wire Wire Line
	2050 1850 2050 2450
Wire Wire Line
	2050 2450 1750 2450
Wire Wire Line
	2050 1850 3600 1850
Wire Wire Line
	5450 1650 6750 1650
Wire Wire Line
	6750 2850 6750 2750
Wire Wire Line
	6750 2750 6850 2750
Wire Wire Line
	5450 2850 6750 2850
Wire Wire Line
	6650 2750 6650 2650
Wire Wire Line
	6650 2650 6850 2650
Wire Wire Line
	5450 2750 6650 2750
Wire Wire Line
	6650 2450 6650 2550
Wire Wire Line
	6650 2550 6850 2550
Wire Wire Line
	5450 2450 6650 2450
Wire Wire Line
	6750 1650 6750 1750
Wire Wire Line
	6750 1850 6850 1850
Connection ~ 6750 1650
Wire Wire Line
	6750 1650 6850 1650
Wire Wire Line
	6850 1750 6750 1750
Connection ~ 6750 1750
Wire Wire Line
	6750 1750 6750 1850
Wire Wire Line
	6650 1750 6650 1950
Wire Wire Line
	6650 1950 6750 1950
Wire Wire Line
	5450 1750 6650 1750
Wire Wire Line
	6750 1950 6750 2050
Wire Wire Line
	6750 2150 6850 2150
Connection ~ 6750 1950
Wire Wire Line
	6750 1950 6850 1950
Wire Wire Line
	6850 2050 6750 2050
Connection ~ 6750 2050
Wire Wire Line
	6750 2050 6750 2150
Wire Notes Line
	5500 3500 5500 1350
Wire Notes Line
	5500 1350 3550 1350
Wire Notes Line
	3550 1350 3550 3500
Wire Notes Line
	3550 3500 5500 3500
Text Notes 3550 1300 0    50   ~ 0
HELTEC WIFI KIT 32\n
Text Notes 3700 3700 0    50   ~ 0
*GPIO input only
Text Notes 3700 3900 0    50   ~ 0
GPIOs are 3.3V tolerant only
$EndSCHEMATC
