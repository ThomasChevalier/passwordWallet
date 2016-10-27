#include "Authentification.h"

#include <avr/eeprom.h>
#include <util/delay.h>

#include "Oled.h"
#include "Rfid.h"
#include "String.h"
#include "Random.h"

#include "Globals.h"

#include "Aes.h"
#include "Ascii85.h"

#include "Buttons.h"
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

// Return 1 if success, 0 otherwise
static uint8_t authenticate_on_card()
{
    // sak == 0x08 <=> MIFARE 1K
    if(rfid_uid.sak != 0x08)
    {
        str_to_buffer(str_error_card_index);
        oled_draw_text(0, 0, str_buffer, 0);
        oled_display();
    }
    else
    {
        MIFARE_Key key;
        StatusCode status;
        for(uint8_t i = 0; i < 6; ++i)
        {
            key.keyByte[i] = 0xFF;
        }

        status = rfid_pcd_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, &key, &rfid_uid);
        if(status != STATUS_OK)
        {
            str_to_buffer(str_error_auth_index);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
        }
        else
        {
            return 1; // SUCCESS !
        }
    }
    return 0;
}

void wait_for_valid_card()
{
    uint8_t noCard = 1;
    oled_dim(1); // Save 4 mA
    do
    {   
        // Waiting for the user to present his card
        waitRfidTag();
        oled_clear_display();

        // Trying to authenticate
        if(authenticate_on_card())
        {
            // Authenticate ... success

            uint8_t buffer[18];
            uint8_t size = 18;

            // Trying to read master key ...
            if(rfid_MIFARE_read(4, buffer, &size) != STATUS_OK && size != 16)
            {
                // .. Failure
                str_to_buffer(str_error_read_index);
                oled_draw_text(0, 0, str_buffer, 0);
                oled_display();
            }
            else
            {
                // .. Success
                uint8_t i = 0;
                for(i = 0; i < 16; ++i)
                {
                    KEY[i] = buffer[i];
                }

                noCard = 0;
            }
        }
        else
        {
            // If the authentication fail, wait for an other card
        }
       
        // Necessary to procede to other communications
        rfid_PICC_haltA();
        rfid_pcd_stopCrypto1();

    }
    while(noCard);

    // Normal contrast
    oled_dim(0);

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
            str_to_buffer(str_error_pwd_index);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
            return 0;
        }

    }
    return 1;
}

void change_master_key()
{
    // rfid may be power down
    rfid_init();

    oled_clear_display();
    str_to_buffer(str_misc_approachCard_index);
    oled_draw_text(0, 0, str_buffer, 0);
    oled_display();

    // Waiting for the user to present his card
    waitRfidTag();

    // Generate new key
    random_fill(KEY, 16);

    // Write it to the rfid tag ...
    /*if(authenticate_on_card())
    {
        // Trying to write on tag
        if(rfid_MIFARE_write(4, KEY, 16) != STATUS_OK)
        {
            // .. Failure
            str_to_buffer(STRING_ERROR_READ);
            oled_draw_text(0, 0, str_buffer, 0);
            oled_display();
        }
        else
        {
            // .. Success
        //     str_to_buffer(STRING_MISC_SUCCESS);
        //     oled_draw_text(0, 0, str_buffer, 0);
        //     oled_display();
         }
    }
    else
    {
        // If we cannot authenticate, abort operation
        return;
    }
    rfid_power_down();

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
    }*/

    // Display new key
    char outputText[20];
    encode_16B(KEY, outputText);
    oled_clear_display();
    oled_draw_text(0, 0, outputText, 20);
    oled_display();

    // Wait for the user to press a button
    while(1)
    {
        buttons_update_event();
        uint8_t event = getEvents();
        if((event & EVENT_BUTTON_1) || (event & EVENT_BUTTON_2) || (event & EVENT_BUTTON_3) || (event & EVENT_BUTTON_4))
            return;
    }
}
