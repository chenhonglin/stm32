#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>


/* SDIO Commands  Index */
#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /* SD_CMD_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SD_CMD_SEN_OP_COND                  ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /* SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /* SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /* SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /* reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /* To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /* To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /* To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /* To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /* SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /* SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_READ_OCR                            ((uint8_t)58) /* Read OCR register */
#define SD_CMD_NO_CMD                              ((uint8_t)64)

/* Following commands are SD Card Specific commands.
   SD_CMD_APP_CMD should be sent before sending these
   commands. */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /* For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /* For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /* For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /* For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /* For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /* For SD Card only */
#define SD_CMD_SD_CMD_RW_DIRECT                    ((uint8_t)52) /* For SD I/O Card only */
#define SD_CMD_SD_CMD_RW_EXTENDED                  ((uint8_t)53) /* For SD I/O Card only */

/* Following commands are SD Card Specific security commands.
   SD_CMD_APP_CMD should be sent before sending these commands. */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /* For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /* For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /* For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /* For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /* For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /* For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /* For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /* For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /* For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /* For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /* For SD Card only */

/* Masks for R6 Response */
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)


void SD_Init(void);
uint8_t SD_SendRecv(uint8_t data);
uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg);
uint8_t SD_CardInit(void);
uint8_t SD_GetVersion(void);
