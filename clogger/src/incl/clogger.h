#include <stdint.h>

#define OFF 0
#define INFO 1
#define ERR 2
#define FATAL 3

#define clogger_log(str, msg_level) _clogger_log(__FILE__, __LINE__, str, msg_level)

#define FAILURE (-1)
#define SUCCESS 1

int clogger_init(void);
int _clogger_log(char *, unsigned int, char *, uint8_t);
int clogger_terminate(void);
