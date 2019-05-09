#include "incl/util.h"

static FILE *clogger_log;
static FILE *app_log;

/******************************************************************************
 * This function returns the index (starting at `0`) of the `needle` in the 
 * `haystack`, if found. Else, returns `NULL`
 *****************************************************************************/

        int
indexOf(char *haystack,
                char needle)
{
        char *ptr = strchr(haystack, needle);
        return ptr == NULL ? (-1) : ptr - haystack;
}

/******************************************************************************
 * This function removes leading spaces and returns a pointer to the first
 * non-space character.
 *****************************************************************************/

        char *
del_leading_spaces(char *str)
{
        write_clogger_log("Deleting leading spaces for:");
        write_clogger_log(str);

        char *ptr;
        ptr = str;

        while (*ptr) {
                if (*ptr == ' ') {
                        ptr++;
                        continue;
                } else {
                        return ptr;
                }
        }
        write_clogger_log("Could not remove leading spaces for:");
        write_clogger_log(str);

        return NULL;
}

/******************************************************************************
 * This function removes trailing spaces and puts a NUL just after the last
 * non-space character.
 *****************************************************************************/

        void
del_trailing_spaces(char *str)
{
        write_clogger_log("Deleting trailing spaces for:");
        write_clogger_log(str);

        int idx;
        idx = strlen(str) - 1;

        while (str[idx] == ' ') {
                --idx;
        }

        str[idx + 1] = '\0';
}

/******************************************************************************
 * This function removes leading and trailing space characters and returns a
 * pointer to the first non-space character
 *****************************************************************************/

        char
*lt_trim(char *str)
{
        del_trailing_spaces(str);
        return del_leading_spaces(str);
}

/******************************************************************************
 * This function opens `../log/clogger.log` - clogger's own log file.
 * ***************************************************************************/

        int
open_clogger_log()
{
        clogger_log = fopen("./clogger.log", "w+");
        if (clogger_log == NULL) return -1;

        return 0;
}

/******************************************************************************
 * This function opens the application's log file.
 *****************************************************************************/

        int
open_app_log(uint8_t write_method,
                char *log_file_path)
{
        if (write_method == 0) {
                app_log = fopen(log_file_path, "w+");
        } else if (write_method == 1) {
                app_log = fopen(log_file_path, "a");
        }

        if (app_log == NULL) {
                write_clogger_log("FAILED to open application log.");
                return -1; 
        }

        return 0;
}

/******************************************************************************
 * This function writes to clogger's own log file.
 *****************************************************************************/

        int
write_clogger_log(char *msg)
{
        char now[LEN_TIME];

        if (clogger_log == NULL) {
                open_clogger_log();

                /* If we still do not have a pointer to clogger's log file, do nothing. */
                if (clogger_log == NULL) return -1;
        }

        current_time(now);
        fprintf(clogger_log, "[%-19s]  %s\n", now, msg);

        return 0;
}

/******************************************************************************
 * This function writes to the application log.
 *****************************************************************************/

        int
write_application_log(char *file_name,
                unsigned int line_no,
                uint8_t msg_level,
                char *msg)
{
        char *msg_level_table[MAX_MSG_LEVELS] = {"OFF"
                , "INFO"
                        , "ERR"
                        , "FATAL"};

        char now[LEN_TIME];
        char msg_level_txt[MAX_LEN_MSG_LEVEL];

        if (app_log == NULL) {
                write_clogger_log("Handle to app log is NULL.");
                return -1;
        }

        /* Get current time. */
        current_time(now);

        /* Get msg level text. */
        if (msg_level >= MAX_MSG_LEVELS) {
                strcpy(msg_level_txt, "UNKNOWN");
        } else {
                strcpy(msg_level_txt, msg_level_table[msg_level]);
        }

        fprintf(app_log, "[%-19s]  [%-7s]  %s:%u  %s\n", now, msg_level_txt, file_name, line_no, msg);

        return 0;
}

/******************************************************************************
 * This function closes `../log/clogger.log` - clogger's own log file.
 * ***************************************************************************/

        int
close_clogger_log()
{
        if (clogger_log == NULL) return -1;

        fflush(clogger_log);
        fclose(clogger_log);

        return 0;
}

        int
close_app_log()
{
        if (app_log == NULL) return -1;

        fflush(app_log);
        fclose(app_log);

        return 0;
}

/******************************************************************************
 * This function takes a char array and fills it up with the current time.
 *****************************************************************************/

        void
current_time(char* str)
{
        time_t timer;
        struct tm* tm_info;

        time(&timer);
        tm_info = localtime(&timer);

        strftime(str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

/******************************************************************************
 * This function will check whether the values read for the switches are
 * correct. It returns the number of switches having invalid values.
 *****************************************************************************/

        uint8_t
check_loaded_switches(uint8_t debug_level,
                uint8_t write_method,
                char *log_file_path)
{
        uint8_t counter;
        counter = 0;

        /* Considering 255 to be an invalid number for these cases */
        if (debug_level == 255) ++counter;
        if (write_method == 255) ++counter;
        if (strlen(log_file_path) == 0) ++counter;

        return counter;
}
