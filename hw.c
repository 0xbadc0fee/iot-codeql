/*************************
* _               _
*| |             | |
*| |__   __ _  __| |
*| '_ \ / _` |/ _` |
*| |_) | (_| | (_| |
*|_.__/ \__,_|\__,_|
*      _____  __
*     |  _  |/ _|
*  ___| |/' | |_ ___  ___
* / __|  /| |  _/ _ \/ _ \
*| (__\ |_/ / ||  __/  __/
* \___|\___/|_| \___|\___|
*
************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> /*someimtes required if unistd.h alone doesn't work*/
#include <curl/curl.h>
#include <string.h>
#include <errno.h> /* NOTE: using for debugging, consider removing before deploy*/

/*NOTE: debug & dev comments are denoted 'NOTE', clean them up before deploying*/

/* GLOBAL CONSTANTS */

#define OK        0
#define INIT_ERR  0
#define REQ_ERR   0

//#define URL       "http://192.168.1.1:8000"
// NOTE: may need to use ' --resolve example.com:443:127.0.0.1' method if trouble parsing 'localhost' later


/* GLOBAL VARIABLES */

struct flags
{
  int g;
  int o;
  int p;
  int d;
  int h;
  int v;
  int u;
};

/* GLOBAL FUNCTIONS */
/*CURL modes, mutually exclusive, select one*/

void curl_get(char *url, CURL *curl, CURLcode res) {
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /*GET is default mode, no special options necessary*/
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "[CURL] Could not complete: %s\n", curl_easy_strerror(res));
    }
    if(res == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    }
    curl_easy_cleanup(curl);
  }
}

void curl_post(char *url, char *message, CURL *curl, CURLcode res) {
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /*POST mode options*/
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(message));
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "[CURL] Could not complete: %s\n", curl_easy_strerror(res));
    }
    if(res == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    }
    curl_easy_cleanup(curl);
  }
}

void curl_put(char *url, char *message, CURL *curl, CURLcode res) {
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /*PUT mode options*/
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "[CURL] Could not complete: %s\n", curl_easy_strerror(res));
    }
    if(res == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    }
    curl_easy_cleanup(curl);
  }
}

void curl_delete(char *url, char *message, CURL *curl, CURLcode res) {
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /*DELETE mode options*/
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "[CURL] Could not complete: %s\n", curl_easy_strerror(res));
    }
    if(res == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    }
    curl_easy_cleanup(curl);
  }
}

/* Other GLOBAL functions, non-curl */

void hw_help() {
  printf("HELP MESSAGE HERE\n");
}

void hw_usage() {
  printf("USAGE MESSAGE HERE\n");
}

void hw_version() {
  printf("VERSION MESSAGE HERE\n");
}


/************************************/

int main(int argc, char **argv) {
  int long_index=0;

  /* ARGUMENT FLAGS */

  struct flags flags;

  /*Default flags values*/
  flags.g = 0;
  flags.o = 0;
  flags.p = 0;
  flags.d = 0;
  flags.u = 0;
  flags.h = 0;
  flags.v = 0;

  /*CURL inputs*/

  int c;
  char *message;
  char *url = NULL;

  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();

  //DEFINE ARGUMENTS
  static struct option long_options[] = {
    {"get",      no_argument,             0,    'g'},
    {"post",     no_argument,             0,    'o'},
    {"put",      no_argument,             0,    'p'},
    {"delete",   no_argument,             0,    'd'},
    {"url",      required_argument,       0,    'u'},
    {"help",     no_argument,             0,    'h'},
    {"version",  no_argument,             0,    'v'},
    {0,          0 ,                      0,     0 }
  };

  // COLLECT ARGUMENTS
  //c = getopt_long(argc, argv, "gopdu:hv", long_options, &long_index);
  while ((c = getopt_long(argc, argv, "gopdu:hv", long_options, &long_index)) != -1){	  
    switch (c) {
    case 'g':
      flags.g = 1;
      printf("option -g --get set. Setting flags.g = 1  \n");
      break;
    case 'o':
      flags.o = 1;
      printf("option -o --put set. Setting flags.o = 1  \n");
      break;
    case 'p':
      flags.p = 1;
      printf("option -p --post set. Setting flags.p = 1  \n");
      break;
    case 'd':
      flags.d = 1;
      printf("option -d --delete set. Setting flags.d = 1  \n");
      break;
    case 'u':
      flags.u = 1;
      url = optarg;
      printf("option u with value '%s'\n", url);
      break;
    case 'h':
      printf("option h chosen\n");
      break;
    case 'v':
      printf("option v chosen\n");
      break;
    case '?':
      printf("Unknown option: %c\n", optopt);
    default:
      hw_usage();
      printf("?? getopt.h returned error code 0%o ??\n", c);
      exit(EXIT_FAILURE);
    }
  }

  // URL STRING PARSING VALIDATION...

  // VALIDATE & PROCESS ARGUMENTS - CURL COMMANDS FIRST THEN HELP, USAGE, VERSION
if (flags.g == 1){
  if ((flags.g + flags.o + flags.p + flags.d) > 1){
    printf("WARNING:Too many options chosen!\n");
    hw_usage;
    exit(1);
  }
  if (flags.u != 1){
    printf("WARNING: No URL specified!\n");
  }
  else {
	printf("running curl_get\n");
  printf("URL: %s\n", url);
  curl_get(url, curl, res);
  printf("CURL response code: %d\n", res);
  return 0;  /*exit program, succeede*/
 }
}
if (flags.o == 1){
  if ((flags.g + flags.o + flags.p + flags.d) > 1){
    printf("WARNING:Too many options chosen!\n");
    hw_usage;
    exit(1);
  } else {
  curl_post(url, message, curl, res);
	printf("running curl_post\n");
 }
}
if (flags.p == 1){
  if ((flags.g + flags.o + flags.p + flags.d) > 1){
    printf("WARNING:Too many options chosen!\n");
    hw_usage;
    exit(1);
  } else {
	curl_put(url, message, curl, res);
	printf("running curl_put\n");
  }
}
if (flags.d == 1){
  if ((flags.g + flags.o + flags.p + flags.d) > 1){
    printf("WARNING:Too many options chosen!\n");
    hw_usage;
    exit(1);
  } else {
	curl_delete(url, message, curl, res);
	printf("running curl_delete\n");
  }
}
 if (flags.h == 1){
   hw_help();
 }
 if (flags.v == 1){
   hw_version();
 }
 else {
   printf("WARNING: No modes selected!\n");
 }

 return 0;
}
