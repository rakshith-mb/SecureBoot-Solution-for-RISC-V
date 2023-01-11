/******************************************************************************
 * File Name    - secure_rom_boot.c
 * 
 * Description  - This c file contains the function definitions of all the 
 *                functions used by the secure rom boot code. The secure rom boot 
 *                validates the user application and launches the application 
 *                if the validation is successful.
 ******************************************************************************/
#include <stdlib.h>
#include "secure_rom_boot.h"
#include "sha256.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "user_app.h"

// ToDo: Can't be a variable. Should be a memory location that is preserved on boot
uint8_t flash_boot_curr_version = 0;

uint8_t* flash_boot_start_ptr = (uint8_t*)FLASH_START_ADDR_MEM;
uint8_t* flash_boot_end_ptr = (uint8_t*)FLASH_END_ADDR_MEM;
uint8_t* flash_boot_version_ptr = &flash_boot_curr_version;

void rom_boot_handle_error(void)
{
    // Add error correction code if any
    while(1);
}

boot_result_t rom_boot_invalidate_checks()
{
    printf("\nNOTE: Invalidating flash boot flags\n");

    uint8_t* magic_number_ptr = FLASH_MAGIC_NUMBER_ADDR;
    uint8_t* valid_flag_ptr = FLASH_BOOT_IS_VALID_ADDR;

    *magic_number_ptr = 0x00;
    *(magic_number_ptr + 1) = 0x00;

    *valid_flag_ptr = FLASH_BOOT_NOT_VALID;

    return BOOT_SUCCESS;
}

void rom_boot_valid_success()
{
    printf("\nNOTE: Enabling all flash boot flags and magic number\n");

    uint8_t* magic_number_ptr = FLASH_MAGIC_NUMBER_ADDR;
    uint8_t* valid_flag_ptr = FLASH_BOOT_IS_VALID_ADDR;

    *magic_number_ptr = FLASH_MAGIC_NUMBER;
    *(magic_number_ptr + 1) = FLASH_MAGIC_NUMBER;
    *valid_flag_ptr = FLASH_BOOT_VALID;
}

boot_result_t rom_boot_validate_flash_boot()
{
    uint8_t flash_line_size = 16, is_valid = 0;
    uint8_t hash_buf[HASH_BUF_SIZE] = {0};
    
    printf("\nNOTE: Initiating flash boot validation\n");

    uint8_t* hash_ptr = (uint8_t*)HASH_ADDRESS;

   sha256_easy_hash_hex(flash_boot_start_ptr, 4096, hash_buf);
    for(int i = 0; i < HASH_BUF_SIZE; i++)
    {
        if(hash_buf[i] == (uint8_t)(*(hash_ptr + i)))
        {
            is_valid = 1;
        }
        else
        {
            is_valid = 0;
            break;
        }
    }

    if(is_valid)
    {
        printf("\nNOTE: Flash Boot validation successful\n");
        return BOOT_SUCCESS;
    }
    else
    {
        return BOOT_VALIDATION_FAIL;
    }
}

boot_result_t rom_boot_validate_version()
{
    printf("\nNOTE: Validating the boot version\n");

    uint8_t* version_ptr = (uint8_t*)FLASH_BOOT_VERSION_ADDR;

    if(*version_ptr > *flash_boot_version_ptr)
    {
        *flash_boot_version_ptr = (uint8_t)(*version_ptr);
        return BOOT_SUCCESS;
    }
    else if(*version_ptr == *flash_boot_version_ptr)
    {
        return BOOT_SUCCESS;
    }
    else
    {
        return BOOT_FLASH_VERSION_ERROR;
    }
}

void rom_boot_launch_app()
{
    uint8_t* magic_number_ptr = FLASH_MAGIC_NUMBER_ADDR;
    uint8_t* valid_flag_ptr = FLASH_BOOT_IS_VALID_ADDR;

    if(*magic_number_ptr == FLASH_MAGIC_NUMBER && *valid_flag_ptr == FLASH_BOOT_VALID)
    {
        printf("\nNOTE: Launching user application\n");
        user_app_main();
        // Should not return;
        while(1);
    }
    else
    {
        printf("\nERROR: Flash not valid. Not launching user application\n");
        rom_boot_handle_error();
    }
}

void malicious_code()
{
    printf("\nWARN: Corrupting device code\n");
    *(flash_boot_start_ptr + 4) = 0xFF;
}

void secure_boot_launcher(void)
{
    printf("\n************************************************************\n");
    printf(  "*     ECE 751 - Implemntation of Secure Boot on RISC-V     *\n");
    printf(  "************************************************************\n");

    if(rom_boot_invalidate_checks() != BOOT_SUCCESS)
    {
        printf("\nERROR: Error in invalidating flash checks\n");
        rom_boot_handle_error();
    }

#if INSERT_MALICIOUS_CODE
    malicious_code();
#endif

    if(rom_boot_validate_version() != BOOT_SUCCESS)
    {
        printf("\nERROR: Attempting to program previous version of boot code\n");
        rom_boot_handle_error();
    }

    if(rom_boot_validate_flash_boot() != BOOT_SUCCESS)
    {
        printf("\nERROR: Flash Boot validation failed. Integrity check failed\n");
        rom_boot_handle_error();
    }

    rom_boot_valid_success();

    rom_boot_launch_app();

    // Should not return
    while(1);
}