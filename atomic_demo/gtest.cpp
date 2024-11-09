#include <curl/curl.h>
#include <gtest/gtest.h>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <uv.h>

typedef unsigned int u32;
typedef unsigned char u8;

u32 Hex2Asc(const void *pvHex, u32 u32HexLen, char *pszAsc) {
  u32 u32Idx;
  u8 *pu8Hex = (u8 *)pvHex;
  for (u32Idx = 0; u32Idx < u32HexLen; u32Idx++) {
    sprintf(pszAsc + u32Idx * 2, "%02X", pu8Hex[u32Idx]);
  }
  return u32Idx * 2;
}

TEST(HelloTest, HelloWorld) {
  // printf("Hello World\n");
  std::string pszRandom = "1380796347342848";
  char szRandom[0x21] = {0};
Hex2Asc(pszRandom.c_str(), pszRandom.length(), szRandom);
  printf("szRandom: %s\n", szRandom);
  printf("Hello World\n");
  ASSERT_EQ(1, 1);
  ASSERT_STREQ("1380796347342848", pszRandom.c_str());
  ASSERT_TRUE(true);
  ASSERT_FALSE(false);
  }

TEST(HelloTest, HelloWorld2) {
  // printf("Hello World\n");
  std::string pszRandom = "1380796347342848";
  expect_eq(pszRandom, "1380796347342848");
  printf("Hello World\n");
  ASSERT_EQ(1, 1);      
}

TEST(HelloTest, BasicAssertions) {
  std::string pszRandom = "1380796347342848";
  char szRandom[0x21] = {0};

  Hex2Asc(pszRandom.c_str(), pszRandom.length(), szRandom);
  printf("szRandom: %s\n", szRandom);

  EXPECT_STRNE("hello", szRandom);

  EXPECT_STREQ("31333830373936333437333432383438", szRandom);
}
int64_t counter = 0;

void curl_get(uv_idle_t *handle) {
  CURL *curl;
  CURLcode res;
  std::string url = "https://www.baidu.com";
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // 证书验证
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    // 在windows上验证百度证书的路径
    // openssl s_client -showcerts -servername server -connect www.baidu.com:443
    // > cacert.pem
    curl_easy_setopt(curl, CURLOPT_CAINFO,
                     "C:/Users/guoya/scoop/apps/curl/8.0.1_8/bin/curl-ca-bundle.crt");

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);

    std::cout<< std::endl;
    std::cout << "curl_easy_perform() success" << std::endl;
    if(counter++ > 10)
      uv_idle_stop(handle);
  }
}

void on_open(uv_fs_t *req); 
void on_read(uv_fs_t *req); 
void on_write(uv_fs_t *req); 

// open_req
uv_fs_t open_req;
// read_req
uv_fs_t read_req;
// write_req
uv_fs_t write_req;
char buffer[1024];
uv_buf_t iov;

int main(int argc, char *argv[]) {
  uv_fs_open(uv_default_loop(), &open_req, "d:/workspace/cprojects/atomic_demo/CMakeLists.txt", O_RDONLY, 0, on_open);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  uv_fs_req_cleanup(&open_req);
  uv_fs_req_cleanup(&read_req);
  uv_fs_req_cleanup(&write_req);
  
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}



void on_open(uv_fs_t *req) {
    // The request passed to the callback is the same as the one the call setup
    // function was passed.
    assert(req == &open_req);
    if (req->result >= 0) {
        iov = uv_buf_init(buffer, sizeof(buffer));
        uv_fs_read(uv_default_loop(), &read_req, req->result,
                   &iov, 1, -1, on_read);
    }
    else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
}

void on_read(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
    }
    else if (req->result == 0) {
        uv_fs_t close_req;
        // synchronous
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    }
    else if (req->result > 0) {
        iov.len = req->result;
        uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
    }
}

void on_write(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror((int)req->result));
    }
    else {
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}
