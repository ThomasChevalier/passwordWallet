EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:password_wallet-cache
EELAYER 25 0
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
L ATmega32U4 U?
U 1 1 596E064D
P 5650 3200
F 0 "U?" H 4750 4900 50  0000 C CNN
F 1 "ATmega32U4" H 5000 1650 50  0000 C CNN
F 2 "" H 6850 4300 50  0000 C CNN
F 3 "" H 6850 4300 50  0000 C CNN
	1    5650 3200
	1    0    0    -1  
$EndComp
$Comp
L USB_A P?
U 1 1 596E358F
P 950 1050
F 0 "P?" H 1150 850 50  0000 C CNN
F 1 "USB_A" H 900 1250 50  0000 C CNN
F 2 "" V 900 950 50  0000 C CNN
F 3 "" V 900 950 50  0000 C CNN
	1    950  1050
	0    -1   1    0   
$EndComp
Wire Wire Line
	4400 4250 4400 4550
Connection ~ 4400 4350
Connection ~ 4400 4450
$Comp
L +5V #PWR01
U 1 1 596E3A09
P 1450 750
F 0 "#PWR01" H 1450 600 50  0001 C CNN
F 1 "+5V" H 1450 890 50  0000 C CNN
F 2 "" H 1450 750 50  0000 C CNN
F 3 "" H 1450 750 50  0000 C CNN
	1    1450 750 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 596E3A21
P 1450 1250
F 0 "#PWR02" H 1450 1000 50  0001 C CNN
F 1 "GND" H 1450 1100 50  0000 C CNN
F 2 "" H 1450 1250 50  0000 C CNN
F 3 "" H 1450 1250 50  0000 C CNN
	1    1450 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 1150 1450 1150
Wire Wire Line
	1450 1150 1450 1250
Wire Wire Line
	1250 850  1450 850 
Wire Wire Line
	1450 850  1450 750 
Wire Wire Line
	1250 950  1800 950 
Wire Wire Line
	1250 1050 1800 1050
Text Label 1500 950  0    60   ~ 0
USB_D-
Text Label 1500 1050 0    60   ~ 0
USB_D+
$Comp
L R R2
U 1 1 596E3B57
P 4050 2450
F 0 "R2" V 4050 2200 50  0000 C CNN
F 1 "22" V 4050 2450 50  0000 C CNN
F 2 "" V 3980 2450 50  0000 C CNN
F 3 "" H 4050 2450 50  0000 C CNN
	1    4050 2450
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 596E3B78
P 4050 2550
F 0 "R1" V 4050 2300 50  0000 C CNN
F 1 "22" V 4050 2550 50  0000 C CNN
F 2 "" V 3980 2550 50  0000 C CNN
F 3 "" H 4050 2550 50  0000 C CNN
	1    4050 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 2450 4200 2450
Wire Wire Line
	4400 2550 4200 2550
Wire Wire Line
	3900 2450 3550 2450
Text Label 3550 2450 0    60   ~ 0
USB_D+
Wire Wire Line
	3900 2550 3550 2550
Text Label 3550 2550 0    60   ~ 0
USB_D-
$EndSCHEMATC
