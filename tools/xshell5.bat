:: If Xshell is out of date, using this script for starting
cd /d %~dp0

set a=%date%

date 2018-12-25

start Xshell.exe

@ping 127.0.0.1 -n 10 >nul

date %a%

exit
