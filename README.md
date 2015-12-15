#opensearch
| [Linux & Mac OS][travis-ci] | [Windows][appveyor-ci] |
|-----------------------------|------------------------|
| ![unix-build-status]        | ![win-build-status]    |

[travis-ci]: https://travis-ci.org/xusiwei/aocs
[unix-build-status]: https://travis-ci.org/xusiwei/aliyun-opensearch-cpp-sdk.svg?branch=master
[appveyor-ci]: https://ci.appveyor.com/project/xusiwei/aliyun-opensearch-cpp-sdk
[win-build-status]: https://ci.appveyor.com/api/projects/status/r27xdn5a69tvk5hi?svg=true

## Build
### Linux/MacOS

#### install cmake

Linux(Unbuntu):
```
sudo apt-get install cmake
```
MacOS:
download from https://cmake.org/download/

#### download dependencies
```
cd <opensearch_source_code>/thirdparty
cmake -P downloadPackages.cmake
```
uncompress:
```
for t in `ls *.tar.gz`; do tar xzvf $t; done
```
if some error reported, likes file corrupted; delete that file and redo download command.

#### configure and build
```
mkdir build
cd build
cmake <absolute_path_of_opensearch_source_code>
make
```
build dependencies script was saved as `thirdparty/unix_build.sh`, by default, it will execute by `cmake <absolute_path_of_opensearch_source_code>`.
you can execute it manually by:`cd thirdparty && ./unix_build.sh [curl|apr|apu]`.
NOTE:
* on Linux cmake 2.8, it maybe some pthread_* error, omit it, redo cmake command.


### Windows

##### install cmake
download from https://cmake.org/download/

#### add cmake bin directory do PATH
    (refers: http://www.computerhope.com/issues/ch000549.htm)
    open new cmd window, use `cmake --version` command to test.

#### download dependencies use cmake
```
cd <opensearch_source_code>/thirdparty
cmake -P downloadPackages.cmake
```
uncompress all of *.zip use right menu "uncompress here".
if some error reported, likes file corrupted; delete that file and redo download command.

#### configure
```
mkdir build
cd build
cmake <absolute_path_of_opensearch_source_code>
```
build dependencies script was saved as `thirdparty/win_build.bat`, by default, it will execute by `cmake <absolute_path_of_opensearch_source_code>`.
you can execute it manually by:`cd thirdparty && ./win_build.bat [curl|apr|apu]`.
NOTE:
* on cmake 2.8, it maybe some pthread_* error, omit it, redo cmake command.
* in win_build.bat, we assume you are using Visual Studio 2015; if not, you need change it manually.

#### build

after configure, there will have a *.sln gernerated in `build`, you can use Visual Studio 2015 to open and build it.

or, use `msbuild *.sln`(use actual name replace *) command follow configure.
