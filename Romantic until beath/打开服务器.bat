@echo off
cls
echo Romantic-Server
title Roman tic服务端防崩溃
:srcds
echo (%time%) Romantic服务器已开启。
start /wait samp-server.exe Y/N
echo (%time%) Romantic服务器异常退出，已经重启。
goto srcds