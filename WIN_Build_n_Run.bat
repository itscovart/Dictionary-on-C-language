@echo off
echo Compilando TADListaDL.c...
gcc TADListaDL.c -c
if errorlevel 1 goto error

echo Compilando Diccionario.c...
gcc Diccionario.c -c
if errorlevel 1 goto error

echo Enlazando...
gcc main.c TADListaDL.o Diccionario.o -o main.exe
if errorlevel 1 goto error

echo Ejecutando programa...
main.exe
goto end

:error
echo.
echo Error durante la compilacion.
pause

:end
pause
