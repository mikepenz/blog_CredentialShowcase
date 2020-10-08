#include <jni.h>

JNIEXPORT jstring JNICALL
Java_com_mikepenz_credsshowcase_CustomApplication_getSdkKey(JNIEnv *env, jobject thiz, jobject context) {
    return (*env)->NewStringUTF(env, "amazing-key-dev");
}

JNIEXPORT jstring JNICALL
Java_com_mikepenz_credsshowcase_CustomApplication_getSdkSecret(JNIEnv *env, jobject thiz, jobject context) {
    return (*env)->NewStringUTF(env, "super-secure-secret-dev");
}