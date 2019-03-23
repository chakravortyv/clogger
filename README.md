# clogger
A logging API for C programs.

The `clogger/src/main.c` file contains a program which demonstrates how clogger
works. A log file with the name of `clogger.log` is created (provided there is
write access) in the directory from where the `clogger` executable is invoked.

`clogger.log` contains clogger's own logs.

The `makefile` is very simple and produces an executable in `clogger/bin`. In
case `make` fails, please create the `clogger/bin` directory manually.

clogger will not stop the client application in any case. If the application
does not generate a log file, please check `clogger.log` for any problems that
clogger might have faced.

