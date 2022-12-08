/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

#ifndef __SYS_SYS_ISW_CLI_H__
#define __SYS_SYS_ISW_CLI_H__

#include "cam_os_wrapper.h"

#define CLI_VERSION "0.0"
#define CLI_MANUFACTURER "SS-RTOS"
#define CLI_PROMPT "#"
#define CLI_BANNER "SigmaStar RTOS CLI\n"

#define cliPrintf(f_, ...) UartSendTrace((KERN_EMERG f_), ##__VA_ARGS__)

#define CLI_MAX_TOKEN   20
#define CLI_MAX_INBUF   80
#define CLI_MAX_HISTORY 8

enum
{
    eCLI_PARSE_ERROR = -1,
    eCLI_PARSE_OK = 0,
    eCLI_PARSE_NO_TBL = 1,
    eCLI_PARSE_NO_VALUE = 2,
    eCLI_PARSE_TOO_FEW = 3,
    eCLI_PARSE_TOO_MANY = 4,
    eCLI_PARSE_UNKNOWN = 5,
    eCLI_PARSE_INPUT_ERROR = 6,
    eCLI_PARSE_INVALID_PARAMETER = 7,
    eCLI_PARSE_HELP_NOT_FOUND = 8,
};

typedef struct cli_s
{
    char *token[CLI_MAX_TOKEN];
    char *pToken;
    int tokenIdx;
    int tokenLvl;
    int token_count;
    char ibuf[CLI_MAX_INBUF];
    void *cmd_list_start;
    void *cmd_list_end;
    int HisBufInited;
    char _szHistory[CLI_MAX_HISTORY][CLI_MAX_INBUF];
    int _nHisCount;
    int _nHisIdx;
} CLI_t;

typedef struct cli_parse_token_s
{
    char *pCmd;
    char *pHelpText;
    char *pUsageText;
    int (*fHandler)(struct cli_s *, char *);
} CliParseToken_t;

#define SS_RTOS_CLI_CMD(name, help, usage, function)    \
        CliParseToken_t _cli_cmd_list_##name __attribute__((used, section(".cli_cmd_list_"#name))) = {#name, help, usage, function}

int CliTokenCount(CLI_t *pCli);
char *CliTokenPop(CLI_t *pCli);
int CliTokenIsNumOrNot(char *p, unsigned int *pV, int base);
int CliTokenPopNum(CLI_t *pCli, unsigned int *pV, int base);
int CliTokenPopNumU64(CLI_t *pCli, unsigned long long *pV, int base);
int ClihelpCmdScan(CLI_t *pCli, char *pTxt, char *pFilter, int verbose);
int CliReassembleToken(void *pCli, char * pDst, unsigned int size);
int CliRunCommand(CLI_t *pCli, const char *cmd);
void isw_cli_main(void);
int run_command(const char *cmd);

#endif // __SYS_SYS_ISW_CLI_H__
