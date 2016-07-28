#include <stdlib.h>
#include <stdio.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/engine.h>

int main(void)
{
  // initializing OpenSSL library
  OPENSSL_load_builtin_modules();
  ENGINE_load_dynamic();

  // building OpenSSL's configuration file path
  char openssl_cnf_path[] = "./openssl.cnf"; 

  // loading configuration
  if (CONF_modules_load_file(openssl_cnf_path, "openssl_conf", 0) != 1) {
    fprintf(stderr, "OpenSSL failed to load required configuration\n");
    ERR_print_errors_fp(stderr);
    return 1;
  }

  ENGINE* eng = ENGINE_by_id("engineX");
  if(NULL == eng) {
    printf("failed to retrieve engine by id (mppa)\n");
    return 1;
  }

  printf("EngineX has been successfully loaded \n");


  ENGINE_finish(eng);
  ENGINE_free(eng);
  ENGINE_cleanup();

  EVP_cleanup();
  CRYPTO_cleanup_all_ex_data();
  ERR_remove_state(0);
  ERR_free_strings(); 

  return 0;
}
