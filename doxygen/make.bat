@echo off
:: ==========================================================================
:: Product: QP-nano script for generating Doxygen documentation
:: Last Updated for Version: 6.0.3
:: Date of the Last Update:  2017-12-12
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) Quantum Leaps, LLC. All rights reserved.
::
:: This program is open source software: you can redistribute it and/or
:: modify it under the terms of the GNU General Public License as published
:: by the Free Software Foundation, either version 3 of the License, or
:: (at your option) any later version.
::
:: Alternatively, this program may be distributed and modified under the
:: terms of Quantum Leaps commercial licenses, which expressly supersede
:: the GNU General Public License and are specifically designed for
:: licensees interested in retaining the proprietary status of their code.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
:: GNU General Public License for more details.
::
:: You should have received a copy of the GNU General Public License
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: https://state-machine.com
:: mailto:info@state-machine.com
:: ==========================================================================
setlocal

echo usage:
echo make
echo make -CHM

set VERSION=6.0.3

:: Generate Resource Standard Metrics for QP-nano ............................ 
set DOXHOME="C:\tools\doxygen\bin"
set RCMHOME="C:\tools\MSquared\M2 RSM"

set RSM_OUTPUT=metrics.dox
set RSM_INPUT=..\include\*.h ..\source\*.h ..\source\*.c

echo /** @page metrics Code Metrics > %RSM_OUTPUT%
echo.>> %RSM_OUTPUT%
echo @code >> %RSM_OUTPUT%
echo                    Standard Code Metrics for QP-nano %VERSION% >> %RSM_OUTPUT%

%RCMHOME%\rsm.exe -fd -xNOCOMMAND -xNOCONFIG -u"File cfg rsm_qpn.cfg" %RSM_INPUT% >> %RSM_OUTPUT%

echo @endcode >> %RSM_OUTPUT%
echo */ >> %RSM_OUTPUT%

:: Generate Doxygen Documentation ........................................... 
if "%1"=="-CHM" (
    echo Generating HTML...
    ::( type Doxyfile & echo GENERATE_HTMLHELP=YES ) | %DOXHOME%\doxygen.exe -
    %DOXHOME%\doxygen.exe Doxyfile-CHM
    
    echo Adding custom images...
    xcopy preview.js tmp\
    xcopy img tmp\img\
    echo img\img.htm >> tmp\index.hhp

    echo Generate CHM...
    "C:\tools\HTML Help Workshop\hhc.exe" tmp\index.hhp
    
    echo Cleanup...
    rmdir /S /Q  tmp
    echo CHM file generated in ..\..\doxygen\qpn\


) else (
    echo Cleanup...
    rmdir /S /Q  ..\..\doxygen\qpn
    
    echo Adding custom images...
    xcopy preview.js ..\..\doxygen\qpn\
    xcopy img ..\..\doxygen\qpn\img\
    copy images\favicon.ico ..\..\doxygen\qpn

    echo Generating HTML...
    %DOXHOME%\doxygen.exe Doxyfile
)

endlocal
