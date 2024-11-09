#include <curl/curl.h>
#include <errno.h>
#include <pthread.h>
#include <sqlcipher/sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex;

void lock_init() {
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  // 可重入锁
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  int result = pthread_mutex_init(&mutex, &attr);
  pthread_mutexattr_destroy(&attr);
}

typedef struct ResponseTag {
  char *data;
  size_t size;
} Response;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream) {
  size_t realsize = size * nmemb;
  Response *resp = (Response *)stream;

  resp->data = (char *)realloc(resp->data, resp->size + realsize + 1);
  if (resp->data == NULL) {
    printf("realloc failed\n");
    return 0;
  }

  memcpy(&(resp->data[resp->size]), buffer, realsize);
  resp->size += realsize;
  resp->data[resp->size] = 0;

  return realsize;
}

int curl_get(const char *url) {
  int result = 0;
  // pthread_mutex_lock(&mutex);
  result = pthread_mutex_trylock(&mutex);
  if (result == EBUSY) {
    printf("%s: mutex is locked by other thread\n", url);
    return 0;
  } else if (result == 0) {
    printf("%s: mutex is locked by current thread\n", url);
  } else {
    printf("mutex lock failed\n");
    return 0;
  }

  CURL *curl;
  CURLcode res;
  Response *resp;

  resp = (Response *)calloc(1, sizeof(Response));
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    printf("----------------%s\n---------------------", url);
    printf("%s\n", resp->data);
    curl_easy_cleanup(curl);
  }

  pthread_mutex_unlock(&mutex);
  return 0;
}

int curl_post(const char *url, const char *data) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  return 0;
}


int main(void) {
  lock_init();
  // create_db("test.db");
  // create_table("test.db", "COMPANY");
  // printf("-------------------------------\n");
  // insert_line("test.db", "COMPANY", 1, "Paul", 32, "California", 20000.00);
  // printf("-------------------------------\n");

  pthread_t thread1, thread2;
  int ret1, ret2;

  ret1 =
      pthread_create(&thread1, NULL, (void *)curl_get, "https://www.baidu.com");
  ret2 = pthread_create(&thread2, NULL, (void *)curl_get,
                        "https://www.google.com");
  if (ret1 != 0 || ret2 != 0) {
    printf("Create pthread error!\n");
    exit(1);
  }
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // curl_get("https://www.baidu.com");
  // curl_get("https://www.google.com");
  return 0;
}



int create_db(const char *db_name) {
  printf("------------start create db-------------------\n");
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  rc = sqlite3_open(db_name, &db);
  rc = sqlite3_key(db, "guoyawen",
                   8); // apply encryption to previously opened database
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }
  sqlite3_close(db);
  printf("------------end create db-------------------\n");
  return 0;
}

int create_table(const char *db_name, const char *table_name) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[500] = {0};

  rc = sqlite3_open(db_name, &db);
  rc = sqlite3_key(db, "guoyawen",
                   8); // apply encryption to previously opened database
  if (rc) {
    fprintf(stderr, "11 Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  memcpy(sql, "CREATE TABLE IF NOT EXISTS ", 30);
  strcat(sql, table_name);

  strcat(sql, "(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE INT NOT "
              "NULL, ADDRESS CHAR(50), SALARY REAL);");
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }
  sqlite3_close(db);
  return 0;
}

int insert_line(const char *db_name, const char *table_name, int id,
                const char *name, int age, const char *address, float salary) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[500] = {0};
  char tmp[100] = {0};

  rc = sqlite3_open(db_name, &db);
  rc = sqlite3_key(db, "guoyawen",
                   8); // apply encryption to previously opened database
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  memcpy(sql, "INSERT INTO ", 30);
  strcat(sql, table_name);
  strcat(sql, " (ID,NAME,AGE,ADDRESS,SALARY) VALUES (");
  sprintf(tmp, "%d", id);
  strcat(sql, tmp);
  strcat(sql, ", ");
  strcat(sql, name);
  strcat(sql, ", ");
  sprintf(tmp, "%d", age);
  strcat(sql, tmp);
  strcat(sql, ", ");
  strcat(sql, address);
  strcat(sql, ", ");
  sprintf(tmp, "%f", salary);
  strcat(sql, tmp);
  strcat(sql, ");");
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Records created successfully\n");
  }
  sqlite3_close(db);
  return 0;
}
