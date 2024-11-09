#include <gmssl/sm9.h>
#include <string.h>
#include "SM9.h"


uint32_t SM9_Init()
{
    return 0x00;
}

uint32_t SM9_Release()
{
    return 0x00;
}

#define IF_FAIL_GOTO_END(x) if(x) {goto END;}
#define ERROR_INVALID_PARAMETER 0x80001

extern const sm9_bn_t SM9_ZERO;

static uint32_t Ks2SignMasterKey(SM9_SIGN_MASTER_KEY* signMasterKey, const uint8_t* pu8Ks)
{
    uint32_t  u32Result = -1;

    if(NULL == signMasterKey || NULL == pu8Ks)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    u32Result = sm9_fn_from_bytes(signMasterKey->ks, pu8Ks);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    sm9_twist_point_mul_generator(&(signMasterKey->Ppubs), signMasterKey->ks);
    u32Result = 1;
END:
    return u32Result;
}


static uint32_t Ke2EncMasterKey(SM9_ENC_MASTER_KEY* encMasterKey, const uint8_t* pu8Ke)
{
    uint32_t  u32Result = -1;

    if(NULL == encMasterKey || NULL == pu8Ke)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    sm9_bn_from_bytes(encMasterKey->ke, pu8Ke);
    sm9_point_mul_generator(&(encMasterKey->Ppube), encMasterKey->ke);
    u32Result = 1;
END:
    return u32Result;

}

uint32_t SM9_GenMasterSignKey(INOUT uint8_t* pu8Ks, OUT uint8_t *pu8Ppubs)
{
    uint32_t  u32Result = -1;
    SM9_SIGN_MASTER_KEY signMasterKey;

    if(NULL == pu8Ks || NULL == pu8Ppubs)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    if(0x00 != memcmp(pu8Ks, (void*)SM9_ZERO, 32))
    {
        u32Result = Ks2SignMasterKey(&signMasterKey, pu8Ks);
        IF_FAIL_GOTO_END(0x01 != u32Result);
    }else  //  all zero, generate new key
    {
        u32Result = sm9_sign_master_key_generate(&signMasterKey);
        IF_FAIL_GOTO_END(0x01 != u32Result);
        sm9_fn_to_bytes(signMasterKey.ks, pu8Ks);
    }
    // SM9_SIGN_MASTER_KEY to buf
    u32Result = sm9_twist_point_to_bytes(&signMasterKey.Ppubs, pu8Ppubs);
    IF_FAIL_GOTO_END(0x01 != u32Result);

END:
    return u32Result == 1 ? 0 : u32Result;

}

uint32_t SM9_GenMasterEncKey(INOUT uint8_t* pu8Ke, OUT uint8_t* pu8Ppube)
{
    uint32_t  u32Result = -1;
    SM9_ENC_MASTER_KEY encMasterKey;


    if(0x00 != memcmp(pu8Ke, (void*)SM9_ZERO, 32))
    {
        u32Result = Ke2EncMasterKey(&encMasterKey, pu8Ke);
        IF_FAIL_GOTO_END(0x01 != u32Result);
    }else  //  all zero, generate new key
    {
        u32Result = sm9_enc_master_key_generate(&encMasterKey);
        IF_FAIL_GOTO_END(0x01 != u32Result);
        sm9_fn_to_bytes(encMasterKey.ke, pu8Ke);
    }
    u32Result = sm9_point_to_bytes(&encMasterKey.Ppube, pu8Ppube);
    IF_FAIL_GOTO_END(0x01 != u32Result);
END:
    return u32Result == 1 ? 0 : -1;
}

uint32_t SM9_GenerateSignKey(IN const uint8_t* pu8Ks, IN const uint8_t* pu8ID, IN uint32_t u32IDLen, OUT uint8_t* pu8dsA)
{
    uint32_t  u32Result = -1;
    SM9_SIGN_MASTER_KEY signMasterKey;
    SM9_SIGN_KEY userSignKey;

    if(NULL == pu8Ks || NULL == pu8ID || NULL == pu8dsA)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    u32Result = Ks2SignMasterKey(&signMasterKey, pu8Ks);
    IF_FAIL_GOTO_END(0x01 != u32Result);

    u32Result = sm9_sign_master_key_extract_key(&signMasterKey, (char*)pu8ID, u32IDLen, &userSignKey);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    // userSignKey to buf
    u32Result = sm9_point_to_bytes(&userSignKey.ds, pu8dsA);
    IF_FAIL_GOTO_END(0x01 != u32Result);   
END:
    return u32Result == 1 ? 0 : -1;
}
uint32_t SM9_GenerateEncryptKey(IN const uint8_t* pu8Ke, IN const uint8_t* pu8ID, IN uint32_t u32IDLen , OUT uint8_t* pu8deB)
{
    uint32_t  u32Result = -1;
    SM9_ENC_MASTER_KEY encMasterKey;
    SM9_ENC_KEY userEncKey;

    if(NULL == pu8Ke || NULL == pu8ID || NULL == pu8deB)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    u32Result  = Ke2EncMasterKey(&encMasterKey, pu8Ke);
    IF_FAIL_GOTO_END(0x01 != u32Result);

    u32Result = sm9_enc_master_key_extract_key(&encMasterKey, (char*)pu8ID, u32IDLen, &userEncKey);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    // userEncKey to buf
    u32Result = sm9_twist_point_to_bytes(&userEncKey.de, pu8deB);
    IF_FAIL_GOTO_END(0x01 != u32Result);   
END:
    return u32Result == 1 ? 0 : -1;
}

static uint32_t ds2SignKey(SM9_SIGN_KEY* signKey, const uint8_t* pu8dsA, const uint8_t* pu8Ppubs)
{
    uint32_t  u32Result = -1;

    if(NULL == signKey || NULL == pu8dsA || NULL == pu8Ppubs)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    u32Result = sm9_point_from_bytes(&(signKey->ds), pu8dsA);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_twist_point_from_bytes(&(signKey->Ppubs), pu8Ppubs);
    IF_FAIL_GOTO_END(0x01 != u32Result);

END:
    return u32Result;
}

uint32_t SM9_Sign(IN const uint8_t* pu8Ppubs, IN const uint8_t* pu8dsA, 
		IN const uint8_t* pu8Msg, IN uint32_t u32MsgLen, 
		OUT uint8_t* pu8H, OUT uint8_t* pu8S)
{
    uint32_t  u32Result = -1;
	SM9_SIGN_CTX ctx;
	SM9_SIGN_KEY key;

    u32Result = ds2SignKey(&key, pu8dsA, pu8Ppubs);
    IF_FAIL_GOTO_END(0x01 != u32Result);

	u32Result = sm9_sign_init(&ctx);
    IF_FAIL_GOTO_END(0x01 != u32Result);
	u32Result = sm9_sign_update(&ctx, pu8Msg, u32MsgLen);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_sign_finish_no_der(&ctx, &key, pu8H, pu8S);
    IF_FAIL_GOTO_END(0x01 != u32Result);
END:
    return u32Result == 1 ? 0 : -1;
}

uint32_t SM9_Verify(IN const uint8_t* pu8Ppubs, IN const uint8_t* pu8IDA, IN uint32_t u32IDALen,
		IN const uint8_t* pu8Msg, IN uint32_t u32MsgLen, 
		IN const uint8_t* pu8H, IN const uint8_t* pu8S)
{
    uint32_t  u32Result = -1;
	SM9_SIGN_CTX ctx;
	SM9_SIGN_MASTER_KEY signMasterKey;

    u32Result = sm9_twist_point_from_bytes(&(signMasterKey.Ppubs), pu8Ppubs);
    IF_FAIL_GOTO_END(0x01 != u32Result);

	u32Result = sm9_verify_init(&ctx);
    IF_FAIL_GOTO_END(0x01 != u32Result);
	u32Result = sm9_verify_update(&ctx, pu8Msg, u32MsgLen);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_verify_finish_no_der(&ctx, pu8H, pu8S, &signMasterKey, (char *)pu8IDA, u32IDALen);
    IF_FAIL_GOTO_END(0x01 != u32Result);
END:
    return u32Result == 1 ? 0 : -1;
}

static uint32_t de2EncKey(SM9_ENC_KEY* encKey, const uint8_t* pu8deB, const uint8_t* pu8Ppube)
{
    uint32_t  u32Result = -1;

    if(NULL == encKey || NULL == pu8deB)
    {
        u32Result = ERROR_INVALID_PARAMETER;
        goto END;
    }

    if(NULL != pu8Ppube)
    {
        u32Result = sm9_point_from_bytes(&(encKey->Ppube), pu8Ppube);
        IF_FAIL_GOTO_END(0x01 != u32Result);
    }
    u32Result = sm9_twist_point_from_bytes(&(encKey->de), pu8deB);
    IF_FAIL_GOTO_END(0x01 != u32Result);
END:
    return u32Result;
}

uint32_t SM9_Encrypt(IN const uint8_t* pu8Ppube, 
		IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen,
		IN const uint8_t* pu8Msg, IN uint32_t u32MsgLen, 
		OUT uint8_t* pu8C, OUT uint32_t *pu32CLen)
{
    uint32_t  u32Result = -1;
	SM9_ENC_MASTER_KEY encMasterKey;

    u32Result = sm9_point_from_bytes(&(encMasterKey.Ppube), pu8Ppube);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_encrypt_no_der(&encMasterKey, (char *)pu8IDB, u32IDBLen, pu8Msg, u32MsgLen, pu8C, pu32CLen);
    IF_FAIL_GOTO_END(0x01 != u32Result);

END:
    return u32Result == 1 ? 0 : -1;
}

uint32_t SM9_Decrypt(IN const uint8_t* pu8deB, IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen,
		IN const uint8_t* pu8C, IN uint32_t u32CLen, 
		OUT uint8_t* pu8M, OUT uint32_t *pu32MLen)
{
    uint32_t  u32Result = -1;
	SM9_ENC_KEY key;

    u32Result = de2EncKey(&key, pu8deB, NULL);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_decrypt_no_der(&key, (char *)pu8IDB, u32IDBLen, pu8C, u32CLen, pu8M, pu32MLen);

END:
    return u32Result == 1 ? 0 : -1;
}


uint32_t SM9_Key_encap(IN const uint8_t* pu8Ppube, 
		IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen, IN uint32_t u32KLen, 
		OUT uint8_t* pu8C, OUT uint8_t* pu8K)
{
    uint32_t  u32Result = -1;
    SM9_ENC_MASTER_KEY encMasterKey;
    SM9_POINT C;

    u32Result = sm9_point_from_bytes(&(encMasterKey.Ppube), pu8Ppube);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_kem_encrypt(&encMasterKey, (char *)pu8IDB, u32IDBLen, u32KLen, pu8K, &C);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_point_to_bytes(&C, pu8C);
END:
    return u32Result == 1 ? 0 : -1;
}

uint32_t SM9_Key_decap(IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen, IN const uint8_t* pu8deB, IN const uint8_t* pu8C, IN uint32_t u32KLen, OUT uint8_t* pu8K)
{
    uint32_t  u32Result = -1;
    SM9_ENC_KEY  sm9EncKey;
    SM9_POINT C;

    u32Result = sm9_point_from_bytes(&C, pu8C);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = de2EncKey(&sm9EncKey, pu8deB, NULL);
    IF_FAIL_GOTO_END(0x01 != u32Result);
    u32Result = sm9_kem_decrypt(&sm9EncKey, (char *)pu8IDB, u32IDBLen, &C, u32KLen, pu8K);
    IF_FAIL_GOTO_END(0x01 != u32Result);
END:
    return u32Result == 1 ? 0 : -1;
}



uint32_t test(char* tarray){
    for (size_t i = 0; i < 3; i++)
    {
        printf("tarray[%d] = %d", i, tarray[i]);
    }
    tarray[0] = 'a';
    return 0x00;
}