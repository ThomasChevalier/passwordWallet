/*
 * File generated by pindef_parser.
 * Copyright - Thomas Chevalier - 2017.
 * thomasom.chevalier@gmail.com
 * File created on 18-09-2019 13-51-36
*/

#ifndef PINDEFINITION_HEADER_THOMAS_CHEVALIER
#define PINDEFINITION_HEADER_THOMAS_CHEVALIER

//BUTTON
#define BUTTON_1_PIN_NUM 0
#define BUTTON_1_DDR DDRB
#define BUTTON_1_PORT PORTB
#define BUTTON_1_PIN PINB
#define BUTTON_1_PIN_CHANGE_NUM PCINT0

#define BUTTON_2_PIN_NUM 4
#define BUTTON_2_DDR DDRB
#define BUTTON_2_PORT PORTB
#define BUTTON_2_PIN PINB
#define BUTTON_2_PIN_CHANGE_NUM PCINT4

#define BUTTON_3_PIN_NUM 6
#define BUTTON_3_DDR DDRB
#define BUTTON_3_PORT PORTB
#define BUTTON_3_PIN PINB
#define BUTTON_3_PIN_CHANGE_NUM PCINT6

#define BUTTON_4_PIN_NUM 5
#define BUTTON_4_DDR DDRB
#define BUTTON_4_PORT PORTB
#define BUTTON_4_PIN PINB
#define BUTTON_4_PIN_CHANGE_NUM PCINT5


//FRAM
#define FRAM_CS_PIN_NUM 5
#define FRAM_CS_DDR DDRD
#define FRAM_CS_PORT PORTD
#define FRAM_CS_PIN PIND


//LED
#define LED_PIN_NUM 7
#define LED_DDR DDRC
#define LED_PORT PORTC
#define LED_PIN PINC


//OLED
#define OLED_CS_PIN_NUM 7
#define OLED_CS_DDR DDRB
#define OLED_CS_PORT PORTB
#define OLED_CS_PIN PINB

#define OLED_DC_PIN_NUM 2
#define OLED_DC_DDR DDRD
#define OLED_DC_PORT PORTD
#define OLED_DC_PIN PIND

#define OLED_RES_PIN_NUM 3
#define OLED_RES_DDR DDRD
#define OLED_RES_PORT PORTD
#define OLED_RES_PIN PIND


//RFID
#define RFID_CS_PIN_NUM 4
#define RFID_CS_DDR DDRD
#define RFID_CS_PORT PORTD
#define RFID_CS_PIN PIND

#define RFID_RES_PIN_NUM 6
#define RFID_RES_DDR DDRC
#define RFID_RES_PORT PORTC
#define RFID_RES_PIN PINC


//SPI
#define SPI_SS_PIN_NUM 0
#define SPI_SS_DDR DDRB
#define SPI_SS_PORT PORTB
#define SPI_SS_PIN PINB

#define SPI_SCK_PIN_NUM 1
#define SPI_SCK_DDR DDRB
#define SPI_SCK_PORT PORTB
#define SPI_SCK_PIN PINB

#define SPI_MOSI_PIN_NUM 2
#define SPI_MOSI_DDR DDRB
#define SPI_MOSI_PORT PORTB
#define SPI_MOSI_PIN PINB

#define SPI_MISO_PIN_NUM 3
#define SPI_MISO_DDR DDRB
#define SPI_MISO_PORT PORTB
#define SPI_MISO_PIN PINB


// INIT VALUE
#define DDRB_INIT_VALUE ((1 << OLED_CS_PIN_NUM) | (1 << SPI_SS_PIN_NUM) | (1 << SPI_SCK_PIN_NUM) | (1 << SPI_MOSI_PIN_NUM) | 0)
#define PORTB_INIT_VALUE ((1 << BUTTON_1_PIN_NUM) | (1 << BUTTON_2_PIN_NUM) | (1 << BUTTON_3_PIN_NUM) | (1 << BUTTON_4_PIN_NUM) | (1 << OLED_CS_PIN_NUM) | 0)

#define DDRC_INIT_VALUE ((1 << LED_PIN_NUM) | (1 << RFID_RES_PIN_NUM) | 0)
#define PORTC_INIT_VALUE ((1 << RFID_RES_PIN_NUM) | 0)

#define DDRD_INIT_VALUE ((1 << FRAM_CS_PIN_NUM) | (1 << OLED_DC_PIN_NUM) | (1 << OLED_RES_PIN_NUM) | (1 << RFID_CS_PIN_NUM) | 0)
#define PORTD_INIT_VALUE ((1 << FRAM_CS_PIN_NUM) | (1 << RFID_CS_PIN_NUM) | 0)

#define INIT_ALL_DDR DDRB = DDRB_INIT_VALUE; \
DDRC = DDRC_INIT_VALUE; \
DDRD = DDRD_INIT_VALUE; \

#define INIT_ALL_PORT PORTB = PORTB_INIT_VALUE; \
PORTC = PORTC_INIT_VALUE; \
PORTD = PORTD_INIT_VALUE; \


#endif // PINDEFINITION_HEADER_THOMAS_CHEVALIER
