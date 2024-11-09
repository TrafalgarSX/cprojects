#include <jni.h>
#include <string.h>
#include <iconv.h>

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_example_convertutf8togbk_MainActivity_convertUtf8ToGbk(
        JNIEnv *env,
        jobject /* this */,
        jbyteArray utf8Array) {

    // 获取 UTF-8 编码的字符数组长度
    jsize utf8Length = env->GetArrayLength(utf8Array);

    // 将 Java 层传入的 jbyteArray 类型转换为本地 char 类型指针
    jbyte *utf8Data = env->GetByteArrayElements(utf8Array, NULL);

    // 创建 GBK 编码的字符数组
    size_t gbkLength = utf8Length * 2;  // 预估 GBK 编码数组的长度
    char *gbkData = new char[gbkLength];
    memset(gbkData, 0, gbkLength);  // 初始化 GBK 编码数组

    // 将 UTF-8 编码的字符数组转换为 GBK 编码的字符数组
    iconv_t cd = iconv_open("GBK", "UTF-8");  // 打开 iconv 转换句柄
    char *inbuf = (char *) utf8Data;
    char *outbuf = gbkData;
    size_t inbytesleft = utf8Length;
    size_t outbytesleft = gbkLength;
    iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);  // 转换
    iconv_close(cd);  // 关闭 iconv 转换句柄

    // 将本地 char 类型指针转换为 jbyteArray 类型
    jbyteArray gbkArray = env->NewByteArray(gbkLength);
    env->SetByteArrayRegion(gbkArray, 0, gbkLength, (jbyte *) gbkData);

    // 释放本地字符数组内存
    delete[] gbkData;
    env->ReleaseByteArrayElements(utf8Array, utf8Data, 0);

    // 返回 GBK 编码的字符数组
    return gbkArray;
}
