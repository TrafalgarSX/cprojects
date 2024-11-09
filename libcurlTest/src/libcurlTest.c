#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

size_t write_string(void *ptr, size_t size, size_t nmemb, char **str) {
  size_t realsize = size * nmemb;
  *str = realloc(*str, realsize + 1);
  memcpy_s(*str, realsize, ptr, realsize);
  (*str)[realsize] = '\0';
  return realsize;
}

void curl_get(char *url, char **str) {
#if 1
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }
#endif
}

int main(void) {
  char *str = NULL;
  curl_get("www.baidu.com", &str);
  printf("%s\n", str);
  free(str);
  return 0;
}
