#include <openssl/engine.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

/** Engine name and id */
static const char* engine_id   = "engineX";
static const char* engine_name = "Engine example"; 

/** SHA1 implementation */
static int sha1_init(EVP_MD_CTX *ctx)
{
  SHA1_Init(ctx->md_data);
  return 1;
}

static int sha1_update(EVP_MD_CTX* ctx, const void* data, size_t count)
{
  SHA1_Update(ctx->md_data, data, count);
  return 1;
}

static int sha1_final(EVP_MD_CTX* ctx, unsigned char* md)
{
  SHA1_Final(md, ctx->md_data);
  return 1;
}

static const EVP_MD digest_sha1 = {
  NID_sha1,   /* Name id for SHA1 digest */
  0,          /* pkey_type, NID with private key */
  20,         /* message digest size */
  0,          /* Flags */
  sha1_init, 
  sha1_update,
  sha1_final,
  NULL,
  NULL, 
  EVP_PKEY_NULL_method, 
  64, /* internal block size */
  sizeof(SHA_CTX),
  NULL             /* control function */
};



static digest_nids[] = {NID_sha1, 0};

int digest_selector(ENGINE* e, const EVP_MD** digest, const int** nids, int nid)
{
  int ok = 1;
  if (!digest) {
    /* expected to return the list of supported NIDs */
    *nids = digest_nids;
    return (sizeof(digest_nids) - 1) / sizeof(digest_nids[0]);
  }

  /** Request for a specific digest */
  switch (nid) {
  case NID_sha1:
    *digest = &digest_sha1;
    break;
  default:
    ok = 0;
    *digest = NULL;
    break;
  }
  return ok;
}

#if 0
static int init_key (EVP_CIPHER_CTX *ctx, const unsigned char* key, 
              const unsigned char *iv, int enc)
{
  EVP_CipherInit_ex
  return 1;
}

static int do_cipher(EVP_CIPHER_CTX *ctx, unsigned char* out,
                     const unsigned char* iv, size_t inl)
{
  return 1;
}

static const EVP_CIPHER engine_X_cipher_aes_gcm = {
  NID_aes_256_gcm,
  16, /* block size */
  32, /* default key len */
  0, /* flags */
  init_key, /* init key */
  do_cipher, /* do_cipher */
  EVP_CIPHER_CTX_cleanup, /* context cleanup */
  sizeof(EVP_CIPHER_CTX), /* ctx_size */
  EVP_CIPHER_set_asn1_iv, /* set_asn1_parameters */
  EVP_CIPHER_get_asn1_iv, /* get_asn1_parameters */
  EVP_CIPHER_CTX_ctrl,
  NULL /** app_data */
};
#endif 

static int bind(ENGINE* e, const char* id) 
{
  int ret = 0;
  if (!ENGINE_set_id(e, engine_id)) {
    fprintf(stderr, "ENGINE_set_id failed\n");
    goto end;
  }
  if (!ENGINE_set_name(e, engine_name)) {
    printf("ENGINE_set_name failed\n");
    goto end;
  }
  if (!ENGINE_set_digests(e, digest_selector)) {
    printf("ENGINE_set_digest failed\n");
    goto end;
  }

  ret = 1;
end:
  return ret;
}

IMPLEMENT_DYNAMIC_BIND_FN(bind)
IMPLEMENT_DYNAMIC_CHECK_FN()
