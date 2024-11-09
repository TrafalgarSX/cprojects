%module SM9 

%{
#define SWIG_FILE_WITH_INIT
#include <string>
#include <vector>
#include "../src/SM9.h"
%}

// %include "../src/SM9.h"

%include "numpy.i"

%init %{
    import_array();
%}

/*  typemaps for the two arrays, the second will be modified in-place */
%apply (uint8_t* INPLACE_ARRAY1, int DIM1) {(uint8_t *pu8Ks, uint32_t pu8KsSize)}
%apply (uint8_t* INPLACE_ARRAY1, int DIM1) {(uint8_t *pu8Ppubs, uint32_t pu8PpubsSize)}
%apply (uint8_t* INPLACE_ARRAY1, int DIM1) {(uint8_t *pu8Ke, uint32_t pu8KeSize)}
%apply (uint8_t* INPLACE_ARRAY1, int DIM1) {(uint8_t *pu8Ppube, uint32_t pu8PpubeSize)}

%apply (uint8_t* IN_ARRAY1, int DIM1) {(uint8_t *pu8ID, uint32_t u32IDLen)}

%apply (uint8_t INPLACE_ARRAY1[ANY]){(uint8_t pu8dsA[64])}
%apply (uint8_t INPLACE_ARRAY1[ANY]){(uint8_t pu8deB[128])}

%apply (uint8_t* INPLACE_ARRAY1, int DIM1) {(uint8_t *pu8Msg, uint32_t u32MsgLen)}

%apply (uint8_t INPLACE_ARRAY1[ANY]){(uint8_t pu8H[32])}
%apply (uint8_t INPLACE_ARRAY1[ANY]){(uint8_t pu8S[64])}

%apply (uint8_t* IN_ARRAY1, int DIM1) {(uint8_t *pu8C, uint32_t u32CLen)}
%apply (uint8_t* IN_ARRAY1, int DIM1) {(uint8_t *pu8M, uint32_t u32MLen)}


%apply (uint8_t* IN_ARRAY1, int DIM1) {(uint8_t *pu8K, uint32_t u32KLen)}

%inline %{
    int SM9_GenMasterSignKeyWrap(uint8_t* pu8Ks, uint32_t pu8KsSize, uint8_t *pu8Ppubs, uint32_t pu8PpubsSize)
    {
         return SM9_GenMasterSignKey(pu8Ks, pu8Ppubs);
    }
    int SM9_GenMasterEncKeyWrap(uint8_t* pu8Ke, uint32_t pu8KeSize, uint8_t *pu8Ppube, uint32_t pu8PpubeSize)
    {
         return SM9_GenMasterEncKey(pu8Ke, pu8Ppube);
    }

    int SM9_GenerateSignKeyWrap(uint8_t* pu8Ks, uint32_t pu8KsSize, uint8_t* pu8ID, uint32_t u32IDLen, uint8_t pu8dsA[64])
    {
         return SM9_GenerateSignKey(pu8Ks, pu8ID, u32IDLen, pu8dsA);
    }

    int SM9_GenerateEncKeyWrap(uint8_t* pu8Ke, uint32_t pu8KeSize, uint8_t* pu8ID, uint32_t u32IDLen, uint8_t pu8deB[128])
    {
         return SM9_GenerateEncryptKey(pu8Ke, pu8ID, u32IDLen, pu8deB);
    }

    int SM9_SignWrap(uint8_t* pu8Ppubs, uint32_t pu8PpubsSize, uint8_t pu8dsA[64], uint8_t* pu8Msg, uint32_t u32MsgLen, uint8_t pu8H[32], uint8_t pu8S[64])
    {
         return SM9_Sign(pu8Ppubs, pu8dsA, pu8Msg, u32MsgLen, pu8H, pu8S);
    }

    int SM9_VerifyWrap(uint8_t* pu8Ppubs, uint32_t pu8PpubsSize, uint8_t* pu8ID, uint32_t u32IDLen, uint8_t* pu8Msg, uint32_t u32MsgLen, uint8_t pu8H[32], uint8_t pu8S[64])
    {
         return SM9_Verify(pu8Ppubs, pu8ID, u32IDLen, pu8Msg, u32MsgLen, pu8H, pu8S);
    }

    int SM9_EncryptWrap(uint8_t* pu8Ppube, uint32_t pu8PpubeSize, uint8_t* pu8ID, uint32_t u32IDLen, uint8_t* pu8M, uint32_t u32MLen, uint8_t* pu8C, uint32_t u32CLen)
    {
         return SM9_Encrypt(pu8Ppube, pu8ID, u32IDLen, pu8M, u32MLen, pu8C, &u32CLen);
    }

    int SM9_DecryptWrap(uint8_t pu8deB[128], uint8_t* pu8ID, uint32_t u32IDLen, uint8_t* pu8C, uint32_t u32CLen, uint8_t* pu8M, uint32_t u32MLen)
    {
         return SM9_Decrypt(pu8deB, pu8ID, u32IDLen, pu8C, u32CLen, pu8M, &u32MLen);
    }

    int SM9_Key_encapWrap(uint8_t* pu8Ppube, uint32_t pu8PpubeSize, uint8_t* pu8ID, uint32_t u32IDLen, uint8_t* pu8C, uint32_t u32CLen, uint8_t* pu8K, uint32_t u32KLen)
    {
         return SM9_Key_encap(pu8Ppube, pu8ID, u32IDLen, u32KLen, pu8C, pu8K);
    }

    int SM9_Key_decapWrap(uint8_t* pu8ID, uint32_t u32IDLen, uint8_t pu8deB[128], uint8_t* pu8C, uint32_t u32CLen, uint8_t* pu8K, uint32_t u32KLen)
    {
         return SM9_Key_decap(pu8ID, u32IDLen, pu8deB, pu8C, u32KLen, pu8K);
    }
#if 0
#endif
%}

%include "std_string.i"
%include "std_vector.i"
%apply std::string &INOUT {std::string &}

namespace std{
    %template(IntVector) vector<int>;
    %template(StringVector) vector<std::string>;
}

%inline %{
    std::string testStringRef(std::string &s) {
        s[0] = 'C'; // s is a reference to the original string passed in
        return "return string";
    }

    int testVectorRef(std::vector<int> &v) {
        int i = 98;
        for(auto& e : v) {
            e = ++i;
        }
        return 100;
    }

%}