@echo off
>log.txt (
cl Serial.cpp
cl Parallel.cpp
)

mkdir out\serial
mkdir out\parallel

>log.txt (
for /f %%f in ('dir /b in\') do Serial in\%%f out\serial\%%f 

echo.

for /f %%f in ('dir /b in\') do Parallel in\%%f out\Parallel\%%f )

type log.txt