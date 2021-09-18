#include "incl/rt.h"
#include "incl/util.h"
#include "incl/clogger.h"

static FILE *clogger_config;

/* clogger status - running (ON) else OFF */
static int8_t clogger_stat = CLOGGER_OFF; 

/* Variables which are to be set based on `runtime.conf` */
static uint8_t debug_level = 255;
static uint8_t write_method = 255;
static char log_file_path[MAX_PATH_LEN] = {'\0'};

int clogger_init(void)
{
        char config_line[MAX_LEN_CONFIG_LINE] = {0};

        char *trimmed_config_line = NULL;

        /* `eq_op_pos` contains the position of the `=` operator */
        int eq_op_pos = 0;

        if (clogger_stat == CLOGGER_ERR) {
                write_clogger_log("Clogger is in error state. Aborting reinitialization.");
                return SUCCESS;
        }

        if (clogger_stat == CLOGGER_ON) {
                write_clogger_log("Clogger already initialized. Skipping reinitialization.");
                return SUCCESS;
        }

        write_clogger_log("Initializing clogger...");

        clogger_stat = CLOGGER_ON;
        clogger_config = fopen(RUNTIME_CONFIG_FILE, "r");

        if (clogger_config == NULL) {
                write_clogger_log("../conf/runtime.conf could not be read");
                clogger_stat = CLOGGER_ERR;
                return FAILURE;
        }    

        while (fgets(config_line, MAX_LEN_CONFIG_LINE, clogger_config) != NULL) {
                if (config_line[0] == CONF_COMNT_CHAR ||
                                config_line[0] == '\n') {
                        memset(config_line, 0, MAX_LEN_CONFIG_LINE);
                        continue;
                }

                /* Remove the last newline character. */
                config_line[indexOf(config_line, '\n')] = '\0';

                /* Remove leading and trailing spaces. */
                trimmed_config_line = lt_trim(config_line);

                eq_op_pos = indexOf(trimmed_config_line, '=');

                /* Associate and assign the switch name with the respective variable. */
                if (strncmp("DEBUG_LEVEL", trimmed_config_line, eq_op_pos) == 0)
                        debug_level = (uint8_t) atoi(trimmed_config_line + eq_op_pos + 1);
                else if (strncmp("WRITE_METHOD", trimmed_config_line, eq_op_pos) == 0)
                        write_method = (uint8_t) atoi(trimmed_config_line + eq_op_pos + 1);
                else if (strncmp("LOG_FILE_PATH", trimmed_config_line, eq_op_pos) == 0)
                        strcpy(log_file_path, trimmed_config_line + eq_op_pos + 1);

                memset(config_line, 0, MAX_LEN_CONFIG_LINE);
        }

        /* Close clogger config. */
        fflush(clogger_config);
        fflush(clogger_config);

        /* Check if the switches were loaded */
        if (check_loaded_switches(debug_level, write_method, log_file_path) != 0) {
                clogger_stat = CLOGGER_ERR;
                write_clogger_log("FAILED to read atleast one switch.");
                write_clogger_log("FAILED to initialize clogger.");
                clogger_terminate();

                return FAILURE;
        }

        if (debug_level == OFF) {
                write_clogger_log("Logging has been switched off.");
                clogger_stat = CLOGGER_OFF;

                return SUCCESS;
        }

        /* Open application log based on `WRITE_METHOD`. */
        if (open_app_log(write_method, log_file_path) != 0) {
                clogger_stat = CLOGGER_ERR;
                write_clogger_log("FAILED to initialize clogger.");
                clogger_terminate();

                return FAILURE;
        }

        write_clogger_log("Clogger initialized.");

        return SUCCESS;
}

int _clogger_log(char *file_name,
                unsigned int line_no,
                char *str,
                uint8_t msg_level)
{
        /* This if block returns SUCCESS even if clogger is in error or off status
         * because this function did not fail in such a case. Something else has
         * already failed before execution came here. `clogger.log` should be able
         * to say what failed exactly. */

        if (debug_level == OFF
                        || clogger_stat == CLOGGER_ERR
                        || clogger_stat == CLOGGER_OFF
                        || msg_level == OFF) return SUCCESS;

        if (msg_level & debug_level) {
                if (write_application_log(file_name, line_no, msg_level, str) != 0) {
                        write_clogger_log("Failed while writing to application log:");
                        write_clogger_log(str);
                }
        }

        if (msg_level == FATAL) {
                write_clogger_log("Code FATAL received from application with message:");
                write_clogger_log(str);
        }

        return SUCCESS;
}

int clogger_terminate(void)
{
        if (clogger_stat == CLOGGER_OFF) return SUCCESS;

        write_clogger_log("Terminating clogger...");

        close_app_log();

        write_clogger_log("Clogger terminated.");
        close_clogger_log();

        /* When in err, don't turn off; because then client might reinit. Resulting
         *  in a failure again. */
        if (clogger_stat != CLOGGER_ERR)
                clogger_stat = CLOGGER_OFF;

        return SUCCESS;
}

