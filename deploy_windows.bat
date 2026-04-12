@echo off
REM =====================================================
REM  ReportSystem - Windows Deployment Script
REM  Run this after building in Qt Creator
REM =====================================================

SET APP_NAME=ReportSystem
SET BUILD_DIR=release
SET DEPLOY_DIR=Deploy

echo ====================================
echo  Deploying %APP_NAME%
echo ====================================

IF NOT EXIST "%BUILD_DIR%\%APP_NAME%.exe" (
    echo ERROR: %BUILD_DIR%\%APP_NAME%.exe not found.
    echo Please build the project first in Qt Creator.
    pause
    exit /b 1
)

mkdir "%DEPLOY_DIR%" 2>nul
copy "%BUILD_DIR%\%APP_NAME%.exe" "%DEPLOY_DIR%\"

REM Run windeployqt to copy Qt DLLs
echo Running windeployqt...
windeployqt --release --no-translations ^
    --sql --printsupport ^
    "%DEPLOY_DIR%\%APP_NAME%.exe"

REM Copy SQLite driver if not already there
IF NOT EXIST "%DEPLOY_DIR%\sqldrivers\qsqlite.dll" (
    echo WARNING: SQLite driver not found. Check Qt installation.
)

echo.
echo ====================================
echo  Deployment complete: %DEPLOY_DIR%\
echo ====================================
echo.
echo Default login: username=1  password=1
echo.
pause
