#include "Authentification.h"

#include <avr/eeprom.h>

#include "Oled.h"
#include "Rfid.h"
#include "String.h"

#include "Globals.h"

#include "Aes.h"
#include "Ascii85.h"

#include "Events.h"

static void waitRfidTag()
{
    while(1)
        {
            if(!rfid_PICC_IsNewCardPresent())
            {
                continue;
            }

            if(rfid_PICC_ReadCardSerial())
                break;
        }
}

void wait_for_valid_card()
{
    uint8_t noCard = 1;
    do
    {
        waitRfidTag();
        oled_clear_display();

        // sak == 0x08 <=> MIFARE 1K
        if(rfid_uid.sak != 0x08)
        {
            str_to_buffer(str_error_card);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
            while(1);
        }

        MIFARE_Key key;
        StatusCode status;
        for(uint8_t i = 0; i < 6; ++i)
        {
            key.keyByte[i] = 0xFF;
        }

        status = rfid_pcd_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, &key, &rfid_uid);
        if(status != STATUS_OK)
        {
            str_to_buffer(str_error_auth);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
            while(1);
        }

        uint8_t buffer[18];
        uint8_t size = 18;

        status = rfid_MIFARE_read(4, buffer, &size);
        if(status != STATUS_OK)
        {
            str_to_buffer(str_error_read);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
            while(1);
        }

        oled_draw_hex(0, 0, buffer, 16);
        oled_display();

        uint8_t i = 0;
        for(i = 0; i < 16; ++i)
        {
            KEY[i] = buffer[i];
        }

        noCard = 0;
        // Lit la clé
        // Une vérification de plus sera obligatoire pour savoir si la clé est bonne (par rapport a un hash interne) : check_key
    }
    while(noCard);
    eventHappen(EVENT_PASSWORD_ENTERED);
}

void encrypt_plain_buffer(uint8_t *output)
{
     uint8_t plainBuffer[16] =
    {0x86, 0xd8 , 0x13 , 0xd3 , 0x2a , 0x25 , 0xc8 , 0x38 , 0x83 , 0xcd , 0xf9 , 0x02 , 0x5e , 0xd8 , 0x0e , 0x43 };
     const uint8_t zeroIv[16]  =
    {0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };

    AES128_CBC_encrypt_buffer(output, plainBuffer, 16, KEY, zeroIv);
}

// Check if the key is valid against memory buffer
uint8_t check_key()
{
    uint8_t output[16];
    encrypt_plain_buffer(output);
    uint8_t verifCounter = 0;
    uint8_t* eeprom_addr = 0;
    for(; verifCounter < 16; ++verifCounter)
    {
        eeprom_busy_wait();
        ++eeprom_addr;
        if(output[verifCounter] != eeprom_read_byte(eeprom_addr))
        {
            oled_clear_display();
            str_to_buffer(str_error_pwd);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
            return 0;
        }

    }
    return 1;
}

void changeMainPassword()
{
    oled_clear_display();
    str_to_buffer(str_misc_changePwd);
    oled_draw_text(0, 0, str_buffer, 0);
    oled_display();

    waitRfidTag();

    // Generate new key
    /*
    uint8_t fillCounter = 0;
    for(; fillCounter < 16; ++fillCounter)
    {
        KEY[i] = random_byte();
    }
    */

    // Write it to the rfid tag



    // Update encryption validation
    uint8_t output[16];
    encrypt_plain_buffer(output);

    uint8_t writeCounter = 0;
    uint8_t* eeprom_addr = 0;
    for(; writeCounter < 16; ++writeCounter)
    {
        eeprom_busy_wait();
        ++eeprom_addr;
        eeprom_update_byte(eeprom_addr, output[writeCounter]);
    }

    // Display new key
    char outputText[20];
    encode_16B(KEY, outputText);
    oled_clear_display();
    oled_draw_text(0, 0, outputText, 20);
}
