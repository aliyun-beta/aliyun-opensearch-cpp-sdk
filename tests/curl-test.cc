// Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
//
// for valgrind test
#include <curl/curl.h>

int main() {
  curl_global_init(CURL_GLOBAL_ALL);
  curl_global_cleanup();
  return 0;
}
