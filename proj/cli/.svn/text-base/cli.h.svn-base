/**
 * @file cli.h
 *
 * @brief Declaracao das funcoes usadas para interpretar comandos da CLI.
 *
 * @author Marco Tulio Martins
 */

#include "../lpc_config.h"
#include "../uart/uart.h"
#include "debug.h"
#include <stdio.h>
#include "api.h"
#include "../mprintf/mprintf.h"

extern cli_str const api_cli[];

int mount_token(char *data);
//int CLI_Builder(char *data);
int CLI_Builder(char *data, char *dest);
//static char* cmdExec(int (*ApiFunc) (str_t* tp));
void cmdExec(int (*ApiFunc) (str_t *tp));
int ExecCommand(const cli_str* acp);
void UARTTakeString(char *str, char channel);
static void help_func(void);
static void unknown_cmd(char *str);
int UARTTakeStringAsync(char *str, char channel);



