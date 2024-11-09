/*
 *  Copyright 2014-2022 The GmSSL Project. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the License); you may
 *  not use this file except in compliance with the License.
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <stdint.h>
#include <gmssl/mem.h>
#include <gmssl/sm3.h>
#include <gmssl/sm9.h>
#include <gmssl/asn1.h>
#include <gmssl/error.h>


#if defined (_MSC_VER) && _MSC_VER <= 1800
const sm9_bn_t SM9_ZERO = {0,0,0,0,0,0,0,0};
const sm9_bn_t SM9_N = {0xd69ecf25, 0xe56ee19c, 0x18ea8bee, 0x49f2934b, 0xf58ec744, 0xd603ab4f, 0x02a3a6f1, 0xb6400000};

// P1.X 0x93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD
// P1.Y 0x21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616
const SM9_POINT _SM9_P1 = {
	{0x7c66dddd, 0xe8c4e481, 0x09dc3280, 0xe1e40869, 0x487d01d6, 0xf5ed0704, 0x62bf718f, 0x93de051d},
	{0x0a3ea616, 0x0c464cd7, 0xfa602435, 0x1c1c00cb, 0x5c395bbc, 0x63106512, 0x4f21e607, 0x21fe8dda},
	{1,0,0,0,0,0,0,0}
};
const SM9_POINT *SM9_P1 = &_SM9_P1;


/*
	X : [0x3722755292130b08d2aab97fd34ec120ee265948d19c17abf9b7213baf82d65bn,
	     0x85aef3d078640c98597b6027b441a01ff1dd2c190f5e93c454806c11d8806141n],
	Y : [0xa7cf28d519be3da65f3170153d278ff247efba98a71a08116215bba5c999a7c7n,
	     0x17509b092e845c1266ba0d262cbee6ed0736a96fa347c8bd856dc76b84ebeb96n],
	Z : [1n, 0n],
*/
const SM9_TWIST_POINT _SM9_P2 = {
	{{0xAF82D65B, 0xF9B7213B, 0xD19C17AB, 0xEE265948, 0xD34EC120, 0xD2AAB97F, 0x92130B08, 0x37227552},
	 {0xD8806141, 0x54806C11, 0x0F5E93C4, 0xF1DD2C19, 0xB441A01F, 0x597B6027, 0x78640C98, 0x85AEF3D0}},
	{{0xC999A7C7, 0x6215BBA5, 0xA71A0811, 0x47EFBA98, 0x3D278FF2, 0x5F317015, 0x19BE3DA6, 0xA7CF28D5},
	 {0x84EBEB96, 0x856DC76B, 0xA347C8BD, 0x0736A96F, 0x2CBEE6ED, 0x66BA0D26, 0x2E845C12, 0x17509B09}},
	{{1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}},
};
const SM9_TWIST_POINT *SM9_P2 = &_SM9_P2;
#else
extern const sm9_bn_t SM9_ZERO;
extern const sm9_bn_t SM9_N;
extern const SM9_POINT *SM9_P1;
extern const SM9_TWIST_POINT *SM9_P2;
#endif


int sm9_signature_to_der(const SM9_SIGNATURE *sig, uint8_t **out, size_t *outlen)
{
	uint8_t hbuf[32];
	uint8_t Sbuf[65];
	size_t len = 0;

	sm9_fn_to_bytes(sig->h, hbuf);
	sm9_point_to_uncompressed_octets(&sig->S, Sbuf);

	if (asn1_octet_string_to_der(hbuf, sizeof(hbuf), NULL, &len) != 1
		|| asn1_bit_octets_to_der(Sbuf, sizeof(Sbuf), NULL, &len) != 1
		|| asn1_sequence_header_to_der(len, out, outlen) != 1
		|| asn1_octet_string_to_der(hbuf, sizeof(hbuf), out, outlen) != 1
		|| asn1_bit_octets_to_der(Sbuf, sizeof(Sbuf), out, outlen) != 1) {
		error_print();
		return -1;
	}
	return 1;
}

int sm9_signature_from_der(SM9_SIGNATURE *sig, const uint8_t **in, size_t *inlen)
{
	int ret;
	const uint8_t *d;
	size_t dlen;
	const uint8_t *h;
	size_t hlen;
	const uint8_t *S;
	size_t Slen;

	if ((ret = asn1_sequence_from_der(&d, &dlen, in, inlen)) != 1) {
		if (ret < 0) error_print();
		return ret;
	}
	if (asn1_octet_string_from_der(&h, &hlen, &d, &dlen) != 1
		|| asn1_bit_octets_from_der(&S, &Slen, &d, &dlen) != 1
		|| asn1_check(hlen == 32) != 1
		|| asn1_check(Slen == 65) != 1
		|| asn1_length_is_zero(dlen) != 1) {
		error_print();
		return -1;
	}
	if (sm9_fn_from_bytes(sig->h, h) != 1
		|| sm9_point_from_uncompressed_octets(&sig->S, S) != 1) {
		error_print();
		return -1;
	}
	return 1;
}

int sm9_sign_init(SM9_SIGN_CTX *ctx)
{
	const uint8_t prefix[1] = { SM9_HASH2_PREFIX };
	sm3_init(&ctx->sm3_ctx);
	sm3_update(&ctx->sm3_ctx, prefix, sizeof(prefix));
	return 1;
}

int sm9_sign_update(SM9_SIGN_CTX *ctx, const uint8_t *data, size_t datalen)
{
	sm3_update(&ctx->sm3_ctx, data, datalen);
	return 1;
}

int sm9_sign_finish(SM9_SIGN_CTX *ctx, const SM9_SIGN_KEY *key, uint8_t *sig, size_t *siglen)
{
	SM9_SIGNATURE signature;

	if (sm9_do_sign(key, &ctx->sm3_ctx, &signature) != 1) {
		error_print();
		return -1;
	}
	*siglen = 0;
	if (sm9_signature_to_der(&signature, &sig, siglen) != 1) {
		error_print();
		return -1;
	}
	return 1;
}

int sm9_sign_finish_no_der(SM9_SIGN_CTX *ctx, const SM9_SIGN_KEY *key, uint8_t *pu8h, uint8_t *pu8S)
{
	SM9_SIGNATURE signature;

	if (sm9_do_sign(key, &ctx->sm3_ctx, &signature) != 1) {
		error_print();
		return -1;
	}
	sm9_fn_to_bytes(signature.h, pu8h);
	sm9_point_to_bytes(&signature.S, pu8S);
	return 1;
}

int sm9_do_sign(const SM9_SIGN_KEY *key, const SM3_CTX *sm3_ctx, SM9_SIGNATURE *sig)
{
	sm9_fn_t r;
	sm9_fp12_t g;
	uint8_t wbuf[32 * 12];
	SM3_CTX ctx = *sm3_ctx;
	SM3_CTX tmp_ctx;
	uint8_t ct1[4] = {0,0,0,1};
	uint8_t ct2[4] = {0,0,0,2};
	uint8_t Ha[64];

	// A1: g = e(P1, Ppubs)
	sm9_pairing(g, &key->Ppubs, SM9_P1);

	do {
		// A2: rand r in [1, N-1]
		if (sm9_fn_rand(r) != 1) {
			error_print();
			return -1;
		}
		//sm9_fn_from_hex(r, "00033C8616B06704813203DFD00965022ED15975C662337AED648835DC4B1CBE"); // for testing

		// A3: w = g^r
		sm9_fp12_pow(g, g, r);
		sm9_fp12_to_bytes(g, wbuf);

		// A4: h = H2(M || w, N)
		sm3_update(&ctx, wbuf, sizeof(wbuf));
		tmp_ctx = ctx;
		sm3_update(&ctx, ct1, sizeof(ct1));
		sm3_finish(&ctx, Ha);
		sm3_update(&tmp_ctx, ct2, sizeof(ct2));
		sm3_finish(&tmp_ctx, Ha + 32);
		sm9_fn_from_hash(sig->h, Ha);

		// A5: l = (r - h) mod N, if l = 0, goto A2
		sm9_fn_sub(r, r, sig->h);

	} while (sm9_fn_is_zero(r));

	// A6: S = l * dsA
	sm9_point_mul(&sig->S, r, &key->ds);

	gmssl_secure_clear(&r, sizeof(r));
	gmssl_secure_clear(&g, sizeof(g));
	gmssl_secure_clear(wbuf, sizeof(wbuf));
	gmssl_secure_clear(&tmp_ctx, sizeof(tmp_ctx));
	gmssl_secure_clear(Ha, sizeof(Ha));

	return 1;
}

int sm9_verify_init(SM9_SIGN_CTX *ctx)
{
	const uint8_t prefix[1] = { SM9_HASH2_PREFIX };
	sm3_init(&ctx->sm3_ctx);
	sm3_update(&ctx->sm3_ctx, prefix, sizeof(prefix));
	return 1;
}

int sm9_verify_update(SM9_SIGN_CTX *ctx, const uint8_t *data, size_t datalen)
{
	sm3_update(&ctx->sm3_ctx, data, datalen);
	return 1;
}

int sm9_verify_finish(SM9_SIGN_CTX *ctx, const uint8_t *sig, size_t siglen,
	const SM9_SIGN_MASTER_KEY *mpk, const char *id, size_t idlen)
{
	int ret;
	SM9_SIGNATURE signature;

	if (sm9_signature_from_der(&signature, &sig, &siglen) != 1
		|| asn1_length_is_zero(siglen) != 1) {
		error_print();
		return -1;
	}

	if ((ret = sm9_do_verify(mpk, id, idlen, &ctx->sm3_ctx, &signature)) < 0) {
		error_print();
		return -1;
	}
	return ret;
}

int sm9_verify_finish_no_der(SM9_SIGN_CTX *ctx, const uint8_t *pu8h, const uint8_t *pu8S,
	const SM9_SIGN_MASTER_KEY *mpk, const char *id, size_t idlen)
{
	int ret;
	SM9_SIGNATURE signature;

	if (sm9_fn_from_bytes(signature.h, pu8h) != 1
		|| sm9_point_from_bytes(&signature.S, pu8S) != 1) {
		error_print();
		return -1;
	}

	if ((ret = sm9_do_verify(mpk, id, idlen, &ctx->sm3_ctx, &signature)) < 0) {
		error_print();
		return -1;
	}
	return ret;
}

int sm9_do_verify(const SM9_SIGN_MASTER_KEY *mpk, const char *id, size_t idlen,
	const SM3_CTX *sm3_ctx, const SM9_SIGNATURE *sig)
{
	sm9_fn_t h1;
	sm9_fn_t h2;
	sm9_fp12_t g;
	sm9_fp12_t t;
	sm9_fp12_t u;
	sm9_fp12_t w;
	SM9_TWIST_POINT P;
	uint8_t wbuf[32 * 12];
	SM3_CTX ctx = *sm3_ctx;
	SM3_CTX tmp_ctx;
	uint8_t ct1[4] = {0,0,0,1};
	uint8_t ct2[4] = {0,0,0,2};
	uint8_t Ha[64];

	// B1: check h in [1, N-1]

	// B2: check S in G1

	// B3: g = e(P1, Ppubs)
	sm9_pairing(g, &mpk->Ppubs, SM9_P1);

	// B4: t = g^h
	sm9_fp12_pow(t, g, sig->h);

	// B5: h1 = H1(ID || hid, N)
	sm9_hash1(h1, id, idlen, SM9_HID_SIGN);

	// B6: P = h1 * P2 + Ppubs
	sm9_twist_point_mul_generator(&P, h1);
	sm9_twist_point_add_full(&P, &P, &mpk->Ppubs);

	// B7: u = e(S, P)
	sm9_pairing(u, &P, &sig->S);

	// B8: w = u * t
	sm9_fp12_mul(w, u, t);
	sm9_fp12_to_bytes(w, wbuf);

	// B9: h2 = H2(M || w, N), check h2 == h
	sm3_update(&ctx, wbuf, sizeof(wbuf));
	tmp_ctx = ctx;
	sm3_update(&ctx, ct1, sizeof(ct1));
	sm3_finish(&ctx, Ha);
	sm3_update(&tmp_ctx, ct2, sizeof(ct2));
	sm3_finish(&tmp_ctx, Ha + 32);
	sm9_fn_from_hash(h2, Ha);
	if (sm9_fn_equ(h2, sig->h) != 1) {
		return 0;
	}

	return 1;
}

int sm9_kem_encrypt(const SM9_ENC_MASTER_KEY *mpk, const char *id, size_t idlen,
	size_t klen, uint8_t *kbuf, SM9_POINT *C)
{
	sm9_fn_t r;
	sm9_fp12_t w;
	uint8_t wbuf[32 * 12];
	uint8_t cbuf[65];
	SM3_KDF_CTX kdf_ctx;

	// A1: Q = H1(ID||hid,N) * P1 + Ppube
	sm9_hash1(r, id, idlen, SM9_HID_ENC);
	sm9_point_mul(C, r, SM9_P1);
	sm9_point_add(C, C, &mpk->Ppube);

	do {
		// A2: rand r in [1, N-1]
		if (sm9_fn_rand(r) != 1) {
			error_print();
			return -1;
		}

		// A3: C1 = r * Q
		sm9_point_mul(C, r, C);
		sm9_point_to_uncompressed_octets(C, cbuf);

		// A4: g = e(Ppube, P2)
		sm9_pairing(w, SM9_P2, &mpk->Ppube);

		// A5: w = g^r
		sm9_fp12_pow(w, w, r);
		sm9_fp12_to_bytes(w, wbuf);

		// A6: K = KDF(C || w || ID_B, klen), if K == 0, goto A2
		sm3_kdf_init(&kdf_ctx, klen);
		sm3_kdf_update(&kdf_ctx, cbuf + 1, 64);
		sm3_kdf_update(&kdf_ctx, wbuf, sizeof(wbuf));
		sm3_kdf_update(&kdf_ctx, (uint8_t *)id, idlen);
		sm3_kdf_finish(&kdf_ctx, kbuf);

	} while (mem_is_zero(kbuf, klen) == 1);

	gmssl_secure_clear(&r, sizeof(r));
	gmssl_secure_clear(&w, sizeof(w));
	gmssl_secure_clear(wbuf, sizeof(wbuf));
	gmssl_secure_clear(&kdf_ctx, sizeof(kdf_ctx));

	// A7: output (K, C)
	return 1;
}

int sm9_kem_decrypt(const SM9_ENC_KEY *key, const char *id, size_t idlen, const SM9_POINT *C,
	size_t klen, uint8_t *kbuf)
{
	sm9_fp12_t w;
	uint8_t wbuf[32 * 12];
	uint8_t cbuf[65];
	SM3_KDF_CTX kdf_ctx;

	// B1: check C in G1
	sm9_point_to_uncompressed_octets(C, cbuf);

	// B2: w = e(C, de);
	sm9_pairing(w, &key->de, C);
	sm9_fp12_to_bytes(w, wbuf);

	// B3: K = KDF(C || w || ID, klen)
	sm3_kdf_init(&kdf_ctx, klen);
	sm3_kdf_update(&kdf_ctx, cbuf + 1, 64);
	sm3_kdf_update(&kdf_ctx, wbuf, sizeof(wbuf));
	sm3_kdf_update(&kdf_ctx, (uint8_t *)id, idlen);
	sm3_kdf_finish(&kdf_ctx, kbuf);

	if (mem_is_zero(kbuf, klen)) {
		error_print();
		return -1;
	}

	gmssl_secure_clear(&w, sizeof(w));
	gmssl_secure_clear(wbuf, sizeof(wbuf));
	gmssl_secure_clear(&kdf_ctx, sizeof(kdf_ctx));

	// B4: output K
	return 1;
}

int sm9_do_encrypt(const SM9_ENC_MASTER_KEY *mpk, const char *id, size_t idlen,
	const uint8_t *in, size_t inlen,
	SM9_POINT *C1, uint8_t *c2, uint8_t c3[SM3_HMAC_SIZE])
{
	SM3_HMAC_CTX hmac_ctx;
	uint8_t K[SM9_MAX_PLAINTEXT_SIZE + 32];

	if (sm9_kem_encrypt(mpk, id, idlen, sizeof(K), K, C1) != 1) {
		error_print();
		return -1;
	}
	gmssl_memxor(c2, K, in, inlen);

	//sm3_hmac(K + inlen, 32, c2, inlen, c3);
#ifdef GMSSL_SM3_HMAC
	sm3_hmac_init(&hmac_ctx, K + inlen, SM3_HMAC_SIZE);
	sm3_hmac_update(&hmac_ctx, c2, inlen);
	sm3_hmac_finish(&hmac_ctx, c3);
#else
    {
        uint8_t* buffer = (uint8_t*)malloc(inlen + SM3_HMAC_SIZE);
        memcpy(buffer, c2, inlen);
        memcpy(buffer + inlen, K + inlen, SM3_HMAC_SIZE);
        sm3_digest(buffer, inlen + SM3_HMAC_SIZE, c3);
        free(buffer);
    }
    #endif

	gmssl_secure_clear(&hmac_ctx, sizeof(hmac_ctx));
	return 1;
}

int sm9_do_decrypt(const SM9_ENC_KEY *key, const char *id, size_t idlen,
	const SM9_POINT *C1, const uint8_t *c2, size_t c2len, const uint8_t c3[SM3_HMAC_SIZE],
	uint8_t *out)
{
	SM3_HMAC_CTX hmac_ctx;
	uint8_t k[SM9_MAX_PLAINTEXT_SIZE + SM3_HMAC_SIZE];
	uint8_t mac[SM3_HMAC_SIZE];

	if (c2len > SM9_MAX_PLAINTEXT_SIZE) {
		error_print();
		return -1;
	}

	if (sm9_kem_decrypt(key, id, idlen, C1, sizeof(k), k) != 1) {
		error_print();
		return -1;
	}
	//sm3_hmac(k + c2len, SM3_HMAC_SIZE, c2, c2len, mac);
#ifdef GMSSL_SM3_HMAC
	sm3_hmac_init(&hmac_ctx, k + c2len, SM3_HMAC_SIZE);
	sm3_hmac_update(&hmac_ctx, c2, c2len);
	sm3_hmac_finish(&hmac_ctx, mac);
	gmssl_secure_clear(&hmac_ctx, sizeof(hmac_ctx));
#else
    {
        uint8_t* buffer = (uint8_t*)malloc(c2len + SM3_HMAC_SIZE);
        memcpy(buffer, c2, c2len);
        memcpy(buffer + c2len, k + c2len, SM3_HMAC_SIZE);
        sm3_digest(buffer, c2len + SM3_HMAC_SIZE, mac);
        free(buffer);       
    }
#endif

	if (gmssl_secure_memcmp(c3, mac, sizeof(mac)) != 0) {
		error_print();
		return -1;
	}
	gmssl_memxor(out, k, c2, c2len);
	return 1;
}

#define SM9_ENC_TYPE_XOR	0
#define SM9_ENC_TYPE_ECB	1
#define SM9_ENC_TYPE_CBC	2
#define SM9_ENC_TYPE_OFB	4
#define SM9_ENC_TYPE_CFB	8

/*
SM9Cipher ::= SEQUENCE {
	EnType		INTEGER, -- 0 for XOR
	C1		BIT STRING, -- uncompressed octets of ECPoint
	C3		OCTET STRING, -- 32 bytes HMAC-SM3 tag
	CipherText	OCTET STRING,
}
*/
int sm9_ciphertext_to_der(const SM9_POINT *C1, const uint8_t *c2, size_t c2len,
	const uint8_t c3[SM3_HMAC_SIZE], uint8_t **out, size_t *outlen)
{
	int en_type = SM9_ENC_TYPE_XOR;
	uint8_t c1[65];
	size_t len = 0;

	if (sm9_point_to_uncompressed_octets(C1, c1) != 1) {
		error_print();
		return -1;
	}
	if (asn1_int_to_der(en_type, NULL, &len) != 1
		|| asn1_bit_octets_to_der(c1, sizeof(c1), NULL, &len) != 1
		|| asn1_octet_string_to_der(c3, SM3_HMAC_SIZE, NULL, &len) != 1
		|| asn1_octet_string_to_der(c2, c2len, NULL, &len) != 1
		|| asn1_sequence_header_to_der(len, out, outlen) != 1
		|| asn1_int_to_der(en_type, out, outlen) != 1
		|| asn1_bit_octets_to_der(c1, sizeof(c1), out, outlen) != 1
		|| asn1_octet_string_to_der(c3, SM3_HMAC_SIZE, out, outlen) != 1
		|| asn1_octet_string_to_der(c2, c2len, out, outlen) != 1) {
		error_print();
		return -1;
	}
	return 1;
}

int sm9_ciphertext_from_der(SM9_POINT *C1, const uint8_t **c2, size_t *c2len,
	const uint8_t **c3, const uint8_t **in, size_t *inlen)
{
	int ret;
	const uint8_t *d;
	size_t dlen;
	int en_type;
	const uint8_t *c1;
	size_t c1len;
	size_t c3len;

	if ((ret = asn1_sequence_from_der(&d, &dlen, in, inlen)) != 1) {
		if (ret < 0) error_print();
		return ret;
	}
	if (asn1_int_from_der(&en_type, &d, &dlen) != 1
		|| asn1_bit_octets_from_der(&c1, &c1len, &d, &dlen) != 1
		|| asn1_octet_string_from_der(c3, &c3len, &d, &dlen) != 1
		|| asn1_octet_string_from_der(c2, c2len, &d, &dlen) != 1
		|| asn1_length_is_zero(dlen) != 1) {
		error_print();
		return -1;
	}
	if (en_type != SM9_ENC_TYPE_XOR) {
		error_print();
		return -1;
	}
	if (c1len != 65) {
		error_print();
		return -1;
	}
	if (c3len != SM3_HMAC_SIZE) {
		error_print();
		return -1;
	}
	if (sm9_point_from_uncompressed_octets(C1, c1) != 1) {
		error_print();
		return -1;
	}
	return 1;
}

int sm9_encrypt(const SM9_ENC_MASTER_KEY *mpk, const char *id, size_t idlen,
	const uint8_t *in, size_t inlen, uint8_t *out, size_t *outlen)
{
	SM9_POINT C1;
	uint8_t c2[SM9_MAX_PLAINTEXT_SIZE];
	uint8_t c3[SM3_HMAC_SIZE];

	if (inlen > SM9_MAX_PLAINTEXT_SIZE) {
		error_print();
		return -1;
	}

	if (sm9_do_encrypt(mpk, id, idlen, in, inlen, &C1, c2, c3) != 1) {
		error_print();
		return -1;
	}
	*outlen = 0;
	if (sm9_ciphertext_to_der(&C1, c2, inlen, c3, &out, outlen) != 1) { // FIXME: when out == NULL	
		error_print();
		return -1;
	}
	return 1;
}

int sm9_encrypt_no_der(const SM9_ENC_MASTER_KEY *mpk, const char *id, size_t idlen,
	const uint8_t *in, size_t inlen, uint8_t *out, uint32_t *outlen)
{
	SM9_POINT C1;
	uint8_t c2[SM9_MAX_PLAINTEXT_SIZE];
	uint8_t c3[SM3_HMAC_SIZE];
	uint8_t c1[64];

	if (inlen > SM9_MAX_PLAINTEXT_SIZE) {
		error_print();
		return -1;
	}

	if (sm9_do_encrypt(mpk, id, idlen, in, inlen, &C1, c2, c3) != 1) {
		error_print();
		return -1;
	}
	if (sm9_point_to_bytes(&C1, c1) != 1) {
		error_print();
		return -1;
	}
	*outlen = (uint32_t)(64 + SM3_HMAC_SIZE + inlen);
    memcpy(out, c1, sizeof(c1));
    memcpy(out + 64, c3, SM3_HMAC_SIZE);
    memcpy(out + 64 + SM3_HMAC_SIZE, c2, inlen);
	return 1;
}

int sm9_decrypt(const SM9_ENC_KEY *key, const char *id, size_t idlen,
	const uint8_t *in, size_t inlen, uint8_t *out, size_t *outlen)
{
	SM9_POINT C1;
	const uint8_t *c2;
	size_t c2len;
	const uint8_t *c3;

	if (sm9_ciphertext_from_der(&C1, &c2, &c2len, &c3, &in, &inlen) != 1
		|| asn1_length_is_zero(inlen) != 1) {
		error_print();
		return -1;
	}
	*outlen = c2len;
	if (!out) {
		return -1;
	}
	if (sm9_do_decrypt(key, id, idlen, &C1, c2, c2len, c3, out) != 1) {
		error_print();
		return -1;
	}
	return 1;
}

int sm9_decrypt_no_der(const SM9_ENC_KEY *key, const char *id, size_t idlen,
	const uint8_t *in, size_t inlen, uint8_t *out, uint32_t *outlen)
{
	SM9_POINT C1;
	const uint8_t *c1 = in;
	const uint8_t *c2 = in + 64 + 32;
	size_t c2len = inlen - 64 - 32;
	const uint8_t *c3 = in + 64;

	if (sm9_point_from_bytes(&C1, c1) != 1) {
		error_print();
		return -1;
	}
	*outlen = (uint32_t)c2len;
	if (!out) {
		return -1;
	}
	if (sm9_do_decrypt(key, id, idlen, &C1, c2, c2len, c3, out) != 1) {
		error_print();
		return -1;
	}
	return 1;
}
