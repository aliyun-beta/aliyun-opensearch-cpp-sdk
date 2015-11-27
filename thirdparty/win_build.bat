@echo off

rem other version, change follow path:
set varset="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

echo set env %varset% ...

if "%1" == "curl" (
	echo build CURL
	cd curl-7.45.0\winbuild
	%varset%
	nmake /f Makefile.vc mode=static
	cd ..
) else if "%1" == "apr" (
	echo build APR
	cd apr-1.5.2
	%varset%
	nmake /f apr.mak
	cd ..
) else if "%1" == "apu" (
	echo build APR-UTIL
	cd apr-util-1.5.4
	%varset%
	nmake /f aprutil.mak
	cd ..
)
