# this is a cmake script file.
# cmake -P ${current_file_name}.cmake

function (checkAndDownload url name)
	if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name})
		message("downloading ${url} to ${name} ...")
		file (DOWNLOAD ${url} ${CMAKE_CURRENT_SOURCE_DIR}/${name} SHOW_PROGRESS)
	endif ()
endfunction ()

if (UNIX)
checkAndDownload(https://github.com/google/googletest/archive/release-1.7.0.tar.gz release-1.7.0.tar.gz)
checkAndDownload(https://github.com/bagder/curl/releases/download/curl-7_45_0/curl-7.45.0.tar.gz curl-7.45.0.tar.gz)
checkAndDownload(https://github.com/apache/apr/archive/1.5.2.tar.gz apr-1.5.2.tar.gz)
checkAndDownload(https://github.com/apache/apr-util/archive/1.5.4.tar.gz apr-util-1.5.4.tar.gz)
else ()
checkAndDownload(https://github.com/google/googletest/archive/release-1.7.0.zip release-1.7.0.zip)
checkAndDownload(https://github.com/bagder/curl/releases/download/curl-7_45_0/curl-7.45.0.zip curl-7.45.0.zip)
checkAndDownload(https://github.com/apache/apr/archive/1.5.2.zip apr-1.5.2-win32-src.zip)
checkAndDownload(https://github.com/apache/apr-util/archive/1.5.4.zip apr-util-1.5.4-win32-src.zip)
checkAndDownload(http://www.us.apache.org/dist/apr/apr-iconv-1.2.1-win32-src-r2.zip apr-iconv-1.2.1-win32-src-r2.zip)
endif ()
