echo off
title DevLand Restarter
color 70
cls
echo   (***** http://www.asteria.boo.pl ******)
:controllerini
DevLand.exe
echo ------------------------------------------------------------
echo Your ot is crashed, the restarter is restarting this OTServ.
echo ------------------------------------------------------------
goto :controllerini
