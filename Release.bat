C:\Factory\Tools\RDMD.exe /RC out

SET RAWKEY=5d47556eb3051f23420ffc9069d3ea72
rem $_git:secretBegin
///////////////////////////////////////////
rem $_git:secretEnd

C:\Factory\SubTools\makeAESCluster.exe Picture.txt     out\Data1.dat %RAWKEY% 100000000
C:\Factory\SubTools\makeAESCluster.exe Music.txt       out\Data2.dat %RAWKEY% 200000000
C:\Factory\SubTools\makeAESCluster.exe SoundEffect.txt out\Data3.dat %RAWKEY% 300000000

COPY /B Actor83\Release\Actor83.exe out
COPY /B AUTHORS out
C:\Factory\Tools\xcp.exe doc out
ren out\Manual.txt ƒ}ƒjƒ…ƒAƒ‹.txt

out\Actor83.exe /L
IF ERRORLEVEL 1 START ?_LOG_ENABLED

C:\Factory\SubTools\zip.exe /G out Actor83
C:\Factory\Tools\summd5.exe /M out

IF NOT "%1" == "/-P" PAUSE
