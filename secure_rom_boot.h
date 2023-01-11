/******************************************************************************
 * File Name    - secure_rom_boot.h
 * 
 * Description  - This is the header file for the secure rom boot source code.
 ******************************************************************************/

#ifndef SOURCE_SECURE_ROM_BOOT_H_
#define SOURCE_SECURE_ROM_BOOT_H_



/*******************************************************************************
* Global constants
*******************************************************************************/
#define INSERT_MALICIOUS_CODE       0

#define FLASH_START_ADDR_MEM        0x1C0094C4
#define FLASH_END_ADDR_MEM          0x1C07FD74

#define HASH_ADDRESS                0x1C07FEE0
#define HASH_BUF_SIZE               32

#define FLASH_MAGIC_NUMBER_ADDR     0x1C07FFFC
#define FLASH_BOOT_IS_VALID_ADDR    0x1C07FFFE
#define FLASH_BOOT_VERSION_ADDR     0x1C07FFE0

#define FLASH_MAGIC_NUMBER          0x5A
#define FLASH_BOOT_VALID            0xF1
#define FLASH_BOOT_NOT_VALID        0xF0

/*******************************************************************************
* Structures and enumerations
*******************************************************************************/
typedef enum {
    BOOT_SUCCESS, 
    BOOT_VALIDATION_FAIL,
    BOOT_CRYPT_ERROR,
    BOOT_FLASH_VERSION_ERROR, 
    BOOT_FLASH_BOOT_CORRUPT
} boot_result_t;

/*******************************************************************************
* Function prototypes
*******************************************************************************/
boot_result_t rom_boot_validate_version();
boot_result_t rom_boot_validate_flash_boot();
void secure_boot_launcher(void);

#endif /* SOURCE_SECURE_ROM_BOOT_H_ */

/* [] END OF FILE */
