#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

/* clogger can be in one of the following */
#define CLOGGER_ON 1
#define CLOGGER_OFF 0
#define CLOGGER_ERR (-1)

#define LEN_TIME 30

#define CONF_COMNT_CHAR '#'
#define MAX_PATH_LEN 256
#define MAX_LEN_MSG_LEVEL 8
#define MAX_MSG_LEVELS 4

int indexOf(char *, char);
char *del_leading_spaces(char *);
void del_trailing_spaces(char *);
char *lt_trim(char *);
int open_clogger_log();
int open_app_log(uint8_t, char *);
int write_clogger_log(char *);
int write_application_log(char *, unsigned int, uint8_t, char *);
int close_app_log();
int close_clogger_log();
void current_time(char *);
uint8_t check_loaded_switches (uint8_t, uint8_t, char *);

