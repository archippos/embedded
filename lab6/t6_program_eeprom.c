#include "esos.h"
#include "esos_f14ui.h"
#include "esos_at24c02d.h"

static uint8_t au8_EEPROM_DATA[AT24C02D_NUM_PAGES * AT24C02D_PAGE_SIZE] = {
    0x80, 0x86, 0x8C, 0x92, 0x98, 0x9E, 0xA5, 0xAA, // Page 00 - SINE
    0xB0, 0xB6, 0xBC, 0xC1, 0xC6, 0xCB, 0xD0, 0xD5, // Page 01
    0xDA, 0xDE, 0xE2, 0xE6, 0xEA, 0xED, 0xF0, 0xF3, // Page 02
    0xF5, 0xF8, 0xFA, 0xFB, 0xFD, 0xFE, 0xFE, 0xFF, // Page 03
    0xFF, 0xFF, 0xFE, 0xFE, 0xFD, 0xFB, 0xFA, 0xF8, // Page 04
    0xF5, 0xF3, 0xF0, 0xED, 0xEA, 0xE6, 0xE2, 0xDE, // Page 05
    0xDA, 0xD5, 0xD0, 0xCB, 0xC6, 0xC1, 0xBC, 0xB6, // Page 06
    0xB0, 0xAA, 0xA5, 0x9E, 0x98, 0x92, 0x8C, 0x86, // Page 07
    0x80, 0x79, 0x73, 0x6D, 0x67, 0x61, 0x5A, 0x55, // Page 08
    0x4F, 0x49, 0x43, 0x3E, 0x39, 0x34, 0x2F, 0x2A, // Page 09
    0x25, 0x21, 0x1D, 0x19, 0x15, 0x12, 0x0F, 0x0C, // Page 10
    0x0A, 0x07, 0x05, 0x04, 0x02, 0x01, 0x01, 0x00, // Page 11
    0x00, 0x00, 0x01, 0x01, 0x02, 0x04, 0x05, 0x07, // Page 12
    0x0A, 0x0C, 0x0F, 0x12, 0x15, 0x19, 0x1D, 0x21, // Page 13
    0x25, 0x2A, 0x2F, 0x34, 0x39, 0x3E, 0x43, 0x49, // Page 14
    0x4F, 0x55, 0x5A, 0x61, 0x67, 0x6D, 0x73, 0x79, // Page 15
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // Page 16 - BUTT
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // Page 17
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // Page 18
    0x7A, 0x74, 0x70, 0x6C, 0x68, 0x62, 0x5E, 0x5A, // Page 19
    0x56, 0x54, 0x50, 0x4C, 0x4A, 0x46, 0x44, 0x42, // Page 20
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, // Page 21
    0x42, 0x44, 0x46, 0x4A, 0x4C, 0x50, 0x54, 0x56, // Page 22
    0x5A, 0x5E, 0x62, 0x68, 0x6C, 0x70, 0x74, 0x7A, // Page 23
    0x7A, 0x74, 0x70, 0x6C, 0x68, 0x62, 0x5E, 0x5A, // Page 24
    0x56, 0x54, 0x50, 0x4C, 0x4A, 0x46, 0x44, 0x42, // Page 25
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, // Page 26
    0x42, 0x44, 0x46, 0x4A, 0x4C, 0x50, 0x54, 0x56, // Page 27
    0x5A, 0x5E, 0x62, 0x68, 0x6C, 0x70, 0x74, 0x7A, // Page 28
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // Page 29
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // Page 30
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // Page 31
};

ESOS_USER_TIMER(heartbeat_LED)
{
    esos_uiF14_toggleLED3();
}

ESOS_USER_TASK(program_eeprom)
{
    ESOS_TASK_BEGIN();

    static uint16_t u16_addr = 0;

    // Write au8_EEPROM_DATA to entire memory
    for (u16_addr = 0; u16_addr < AT24C02D_NUM_PAGES * AT24C02D_PAGE_SIZE; u16_addr += AT24C02D_PAGE_SIZE) {
        /*                                             ↓ 1337 h4x (may not work) */
        ESOS_AT24C02D_WRITE_PAGE(u16_addr, (uint8_t *)(au8_EEPROM_DATA + u16_addr), AT24C02D_PAGE_SIZE);
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("\nEEPROM Write Page - Addr: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(*((uint8_t *)(au8_EEPROM_DATA + u16_addr)));
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_TICKS(10);
    }

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("\nVerifying memory state...\n");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    static uint8_t u8_data = 0;
    for (u16_addr = 0; u16_addr < AT24C02D_NUM_PAGES * AT24C02D_PAGE_SIZE; u16_addr++) {
        ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
        ESOS_TASK_WAIT_ON_WRITE1I2C1(AT24C02D_ADDR, u16_addr);
        ESOS_TASK_SIGNAL_AVAILABLE_I2C();

        ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
        ESOS_TASK_WAIT_ON_READ1I2C1(AT24C02D_ADDR, u8_data);
        ESOS_TASK_SIGNAL_AVAILABLE_I2C();

        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("\nADDR: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(u16_addr);
        ESOS_TASK_WAIT_ON_SEND_STRING(" EXP: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(au8_EEPROM_DATA[u16_addr]);
        ESOS_TASK_WAIT_ON_SEND_STRING(" ACT: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(u8_data);
        ESOS_TASK_WAIT_ON_SEND_STRING((u8_data == au8_EEPROM_DATA[u16_addr]) ? " GOOD" : " BAD");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_TICKS(10);
    }

    ESOS_TASK_YIELD();
    ESOS_TASK_END();
}

void user_init()
{
    config_esos_uiF14();
    esos_pic24_configI2C1(400);
    esos_RegisterTimer(heartbeat_LED, 500);
    esos_RegisterTask(program_eeprom);
}
