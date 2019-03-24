# clogger
A logging API for C programs.

The `clogger/src/main.c` file contains a program which demonstrates how clogger
works. A log file with the name of `clogger.log` is created (provided there is
write access) in the directory from where the `clogger` executable is invoked.

`clogger.log` contains clogger's own logs.

There are just three functions that a client program needs to call:
  1. `int clogger_init(void)` - Initialize clogger.

  2. `int clogger_log(char *, uint8_t)` - Takes a c-string and it's priority level
        and prints the message to the application's log file.

  3. `int clogger_terminate(void)` - Closes files, resources and shuts down
        clogger.

The `conf/runtime.conf` file contains some configuration parameters or
switches. These are read and required by clogger to configure itself at
runtime. Comments in this file explain more about each switch.

The `makefile` is very simple and produces an executable in `clogger/bin`. In
case `make` fails, please create the `clogger/bin` directory manually.

clogger will not stop the client application in any case. If the application
does not generate a log file, please check `clogger.log` for any problems that
clogger might have faced.

The following is a sample log file:
```
[2019-03-24 13:44:44]  [INFO   ]  src/main.c:17  some info message
[2019-03-24 13:44:44]  [ERR    ]  src/main.c:18  some err message
[2019-03-24 13:44:44]  [UNKNOWN]  src/main.c:19  some unk message
[2019-03-24 13:44:44]  [FATAL  ]  src/main.c:20  some fatal message
[2019-03-24 13:44:44]  [INFO   ]  src/main.c:21  another info message
```
