echo off
title Illusion Restarter
color 45
cls
echo   (***** Illusion ******)
:controllerini
Illusion.exe
koza.exe
echo ------------------------------------------------------------
echo Your ot is crashed, the restarter is restarting this OTServ.
echo ------------------------------------------------------------
goto :controllerini
