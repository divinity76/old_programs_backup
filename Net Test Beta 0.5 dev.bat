::NOTES:
::Google.com servers (they have TONS! ALL OVER THE WORLD!)
:: ^ always reply with a minumum of 18 bytes and maximum of 64, regardless of how much data you send.. (excluding TCP/IP/Ping protocol overhead etc tho!)
::so ping -l 0 will still get reply of 18 bytes (when using google)
::TODO: a better way to handle "lost 1 times" S problem?
::TODO: statistics packet loss % over time?
@echo off
:sets
set /A LossCounter=0
set StartTime=%time%
set StartDate=%date%
cls
echo by hans henrik ^^ made cause my isp got issues
echo public beta 1,01 MS Batch edition
pause
title started at %StartTime% - Net Test
:FirstCheck
ping -l 0 -n 2 www.google.com
if %errorlevel% == 1 goto NoInternetAtStart
if %errorlevel% == -1073741502 goto ErrorAtFirstCheck
if %errorlevel% == 0 goto clsbegin
:ErrorAtFirstCheck
echo something went wrong during startup - i deny to log anything.
echo DEBUG: Errorlevel is %errorlevel%, failed during :FirstCheck
pause
exit
:clsbegin
cls
:begin
ping -l 0 -n 2 www.google.com
if %errorlevel% == 0 goto sucessfull
if %errorlevel% == -1073741502 goto Shutdown
:NotSucessfull
set /A atStartFix=1
::atStartFix fixes a bug where it would say "internet last lost 0 - 0" or throw an error if there was no internet at script startup..
set /A LossCounter=%LossCounter%+1
set LastTimeInternetLostTime=%time%
set LastTimeInternetLostDate=%date%
title No internet since %LastTimeInternetLostTime%
echo no internet
echo %LastTimeInternetLostTime%
echo creating a log for it called NoInternetLog.log
echo ------------------------------------------------->>NoInternetLog.log
echo                  %LastTimeInternetLostDate%>>NoInternetLog.log
echo no internet found at %LastTimeInternetLostTime% >>NoInternetLog.log
if %errorlevel% == -1073741502 goto NoInternetShutdown
:NoInternetTest
cls
if "%atStartFix%" == "1" (
echo Internet was lost at %LastTimeInternetLostTime% - %LastTimeInternetLostDate%
) else (
echo no internet since started at %StartTime% - %StartDate%
)
echo still no internet...
ping -l 0 -n 2 ww.google.com
if %errorlevel% == 0 goto InternetBack
if %errorlevel% == -1073741502 goto NoInternetShutdown
goto NoInternetTest
:InternetBack
echo Internet returned circa %time% >>NoInternetLog.log
echo ------------------------------------------------->>NoInternetLog.log
goto begin
:NoInternetShutdown
echo PC is shutting down.>>NoInternetLog.log
echo PC was shut down before internet returned, cant log.>>NoInternetLog.log
echo ------------------------------------------------->>NoInternetLog.log 
cls
title Computer is shutting down. standing by
echo The Computer is shutting down. standing by
pause
exit
:Sucessfull
::TODO: Seriously, gotta be a better way to do this "1 times" bugfix.. right?...
if %LossCounter% == 1 (
title internet last tested WORKING at %time% and lost %LossCounter% time since %StartTime% - %StartDate%
) else (
title internet last tested WORKING at %time% and lost %LossCounter% times since %StartTime% - %StartDate%
)
cls
::true story, my mom could not understand the output untill i added the line below.
echo ------------------YOU CURRENTLY HAVE INTERNET CONNECTION-----------------------
echo Internet last tested WORKING at %time% - %date%
if %LossCounter% == 1 (
echo lost %LossCounter% time since %StartTime% - %StartDate%
) else (
echo lost %LossCounter% times since %StartTime% - %StartDate%
)
if %LossCounter% GTR 0 (
echo Internet was last lost at %LastTimeInternetLostTime% - %LastTimeInternetLostDate%
)
goto begin
:Shutdown
cls
title Computer is shutting down. standing by
echo The Computer is shutting down. standing by
pause
exit
:NoInternetAtStart
title No internet since started at %StartTime%
echo no internet
echo %time%
echo creating a log for it called NoInternetLog.log
echo ------------------------------------c------------->>NoInternetLog.log
echo                  %date%>>NoInternetLog.log
echo no internet found at %time% >>NoInternetLog.log
echo But net test was STARTED with no internet so>>NoInternetLog.log
echo cant really say WHEN it was lost >>NoInternetLog.log
goto NoInternetTest