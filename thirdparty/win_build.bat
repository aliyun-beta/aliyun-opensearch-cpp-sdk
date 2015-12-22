@echo off

rem other version, change follow vars:
set target="Visual Studio 14 2015"
set varset="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

if not exist apr mklink /d apr apr-1.5.2
if not exist apr-util mklink /d apr-util apr-util-1.5.4
if not exist apr-iconv mklink /d apr-iconv apr-iconv-1.2.1
if not exist googletest mklink /d googletest googletest-release-1.7.0
if not exist pcre mklink /d pcre pcre-8.38

echo set env %varset% ...

if "%1" == "curl" (
	echo build CURL
	cd curl-7.45.0\winbuild
	%varset%
	nmake /f Makefile.vc mode=dll
	cd ..
) else if "%1" == "apr" (
	echo build APR
	cd apr
	%varset%
	nmake /f libapr.mak
	cd ..
) else if "%1" == "apu" (
	echo build APR-UTIL
	cd apr-util
	%varset%
	nmake /f libaprutil.mak
	cd ..
)
