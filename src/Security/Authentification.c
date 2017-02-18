#include "Authentification.h"

#include <string.h>

#include <avr/eeprom.h>
#include <util/delay.h>

#include "../Globals.h"

#include "Aes.h"
#include "Random.h"
#include "Passwords.h"

#include "../Hardware/Rfid.h"
#include "../Hardware/Buttons.h"
#include "../Hardware/Oled.h"

#include "../Graphics/Ascii85.h"
#include "../Graphics/ProgressBar.h"
#include "../Graphics/String.h"

#include "../FSM/Events.h"


static void waitRfidTag(void)
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
static uint8_t authenticate_on_card(void)
{
    // sak == 0x08 <=> MIFARE 1K
    if(rfid_uid.sak != 0x08)
    {
        str_to_buffer(str_error_card_index);
        draw_text(0, 0, str_buffer, 0);
        draw_update();
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
            draw_text(0, 0, str_buffer, 0);
            draw_update();
        }
        else
        {
            return 1; // SUCCESS !
        }
    }
    return 0;
}

void wait_for_valid_card(void)
{
    uint8_t noCard = 1;
    oled_dim(1); // Save 4 mA
    do
    {   
        // Waiting for the user to present his card
        waitRfidTag();
        draw_clear();
        draw_update();

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
                draw_text(0, 0, str_buffer, 0);
                draw_update();
            }
            else
            {
                // .. Success
                memcpy(KEY, buffer, 16);

                if(check_key()) // If the key of the rfid is the good one.
                {
                    noCard = 0;
                }
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

    events_happen(EVENT_PASSWORD_ENTERED);
}

// Check if the key is valid against memory buffer
uint8_t check_key(void)
{
    uint8_t randSeq[16];
    uint8_t* eeprom_addr = 0;
    for(uint8_t i = 0; i < 16; ++i)
    {
        eeprom_busy_wait();
        randSeq[i] = eeprom_read_byte(eeprom_addr);
        ++eeprom_addr;
    }

    uint8_t output[16];
    const uint8_t zeroIv[16]  =
    {0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
    AES128_CBC_encrypt_buffer(output, randSeq, 16, KEY, zeroIv);

    eeprom_addr = 0;
    for(uint8_t verifCounter = 0; verifCounter < 16; ++verifCounter)
    {
        eeprom_busy_wait();
        if(output[verifCounter] != eeprom_read_byte(eeprom_addr+16))
        {
            draw_clear();
            str_to_buffer(str_error_pwd_index);
            draw_text(19, 20, str_buffer, 0);
            draw_update();
            return 0;
        }
        ++eeprom_addr;

    }
    return 1;
}

void change_master_key(void)
{
    // rfid may be power down
    rfid_init();

    // Ask for card
    draw_clear();
    str_to_buffer(str_misc_approachCard_index);
    draw_text(18, 30, str_buffer, 0);
    draw_update();

    // Waiting for the user to present his card
    waitRfidTag();

    // Generate new key
    uint8_t newKey[16];
    draw_clear();
    str_to_buffer(str_misc_generateKey_index);
    draw_text(15, 40, str_buffer, 0);
    progress_begin(16);
    for(uint8_t i = 0; i < 16; ++i)
    {
        // No whitespace because it can't be read reliably on the screen
        do
        {
            newKey[i] = random_request_byte();
        }while(newKey[i] == ' ');
        
        progress_add(1);
    }
    progress_end();

    // Write it to the rfid tag ...
    if(authenticate_on_card())
    {
        // Trying to write on tag
        if(rfid_MIFARE_write(4, newKey, 16) != STATUS_OK)
        {
            // .. Failure
            draw_clear();
            str_to_buffer(str_error_read_index);
            draw_text(0, 0, str_buffer, 0);
            draw_update();
            goto EXIT;
        }
    }
    else
    {
        // If we cannot authenticate, abort operation
        draw_clear();
        str_to_buffer(str_error_auth_index);
        draw_text(0, 0, str_buffer, 0);
        draw_update();
        goto EXIT;
    }

    // Update encryption key
    update_encryption_with(newKey);
    memcpy(KEY, newKey, 16);

    // Update encryption validation

    // Generate random sequence
    draw_clear();
    progress_begin(20);
    str_to_buffer(str_misc_updateEncryptVerif_index);
    draw_text(20, 40, str_buffer, 0);
    uint8_t randomSequence[16];
    uint8_t* eeprom_addr = 0;
    for(uint8_t i = 0; i < 16; ++i)
    {
        randomSequence[i] = random_request_byte();
        eeprom_busy_wait();
        eeprom_update_byte(eeprom_addr, randomSequence[i]);
        ++eeprom_addr;
        progress_add(1);
    }

    // Encrypt that sequence
    uint8_t encryptionValidation[16];
    const uint8_t zeroIv[16]  = {0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
    AES128_CBC_encrypt_buffer(encryptionValidation, randomSequence, 16, KEY, zeroIv);
    progress_add(2);

    // Write it to the eeprom
    eeprom_addr = 0;
    for(uint8_t i = 0; i < 16; ++i)
    {
        eeprom_busy_wait();
        eeprom_update_byte(eeprom_addr+16, encryptionValidation[i]);
        ++eeprom_addr;
    }
    progress_add(2);
    progress_end();


    // Display new key
    draw_clear();
    str_to_buffer(str_misc_hereIsNewKey_index);
    draw_text(12, 10, str_buffer, 0);
    char outputText[20];
    encode_16B(KEY, outputText);
    draw_text(15, 30, outputText, 20);
    draw_update();


    // Wait for the user to press a button
    EXIT:
    rfid_power_down();
    while(!buttons_pressed());
}
