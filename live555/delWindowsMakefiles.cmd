@Echo OFF
SETLOCAL
for %%I in (%0) do %%~dI
for %%I in (%0) do cd "%%~pI"

del /Q liveMedia\liveMedia.mak
del /Q groupsock\groupsock.mak
del /Q UsageEnvironment\UsageEnvironment.mak
del /Q BasicUsageEnvironment\BasicUsageEnvironment.mak
del /Q BackChannel\BackChannel.mak
del /Q testProgs\testProgs.mak
del /Q mediaServer\mediaServer.mak
del /Q proxyServer\proxyServer.mak

ENDLOCAL
