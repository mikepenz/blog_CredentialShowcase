#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <jni.h>

const char *getSignature(JNIEnv *env, jobject context) {
    // Build.VERSION.SDK_INT
    jclass versionClass = (*env)->FindClass(env, "android/os/Build$VERSION");
    jfieldID sdkIntFieldID = (*env)->GetStaticFieldID(env, versionClass, "SDK_INT", "I");
    int sdkInt = (*env)->GetStaticIntField(env, versionClass, sdkIntFieldID);
    // Context
    jclass contextClass = (*env)->FindClass(env, "android/content/Context");
    // Context#getPackageManager()
    jmethodID pmMethod = (*env)->GetMethodID(env, contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject pm = (*env)->CallObjectMethod(env, context, pmMethod);
    jclass pmClass = (*env)->GetObjectClass(env, pm);
    // PackageManager#getPackageInfo()
    jmethodID piMethod = (*env)->GetMethodID(env, pmClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    // Context#getPackageName()
    jmethodID pnMethod = (*env)->GetMethodID(env, contextClass, "getPackageName", "()Ljava/lang/String;");
    jstring packageName = (jstring) ((*env)->CallObjectMethod(env, context, pnMethod));
    int flags;
    if (sdkInt >= 28) {
        flags = 0x08000000; // PackageManager.GET_SIGNING_CERTIFICATES
    } else {
        flags = 0x00000040; // PackageManager.GET_SIGNATURES
    }
    jobject packageInfo = (*env)->CallObjectMethod(env, pm, piMethod, packageName, flags);
    jclass piClass = (*env)->GetObjectClass(env, packageInfo);
    // PackageInfo#signingInfo.apkContentsSigners | PackageInfo#signatures
    jobjectArray signatures;
    if (sdkInt >= 28) {
        // PackageInfo#signingInfo
        jfieldID signingInfoField = (*env)->GetFieldID(env, piClass, "signingInfo", "Landroid/content/pm/SigningInfo;");
        jobject signingInfoObject = (*env)->GetObjectField(env, packageInfo, signingInfoField);
        jclass signingInfoClass = (*env)->GetObjectClass(env, signingInfoObject);
        // SigningInfo#apkContentsSigners
        jmethodID signaturesMethod = (*env)->GetMethodID(env, signingInfoClass, "getApkContentsSigners", "()[Landroid/content/pm/Signature;");
        jobject signaturesObject = (*env)->CallObjectMethod(env, signingInfoObject, signaturesMethod);
        signatures = (jobjectArray) (signaturesObject);
    } else {
        // PackageInfo#signatures
        jfieldID signaturesField = (*env)->GetFieldID(env, piClass, "signatures", "[Landroid/content/pm/Signature;");
        jobject signaturesObject = (*env)->GetObjectField(env, packageInfo, signaturesField);
        if ((*env)->IsSameObject(env, signaturesObject, NULL)) {
            return ""; // in case signatures is null
        }
        signatures = (jobjectArray) (signaturesObject);
    }
    // Signature[0]
    jobject firstSignature = (*env)->GetObjectArrayElement(env, signatures, 0);
    jclass signatureClass = (*env)->GetObjectClass(env, firstSignature);
    // PackageInfo#signatures[0].toCharString()
    jmethodID signatureByteMethod = (*env)->GetMethodID(env, signatureClass, "toByteArray", "()[B");
    jobject signatureByteArray = (jobject) (*env)->CallObjectMethod(env, firstSignature, signatureByteMethod);
    // MessageDigest.getInstance("MD5")
    jclass mdClass = (*env)->FindClass(env, "java/security/MessageDigest");
    jmethodID mdMethod = (*env)->GetStaticMethodID(env, mdClass, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jobject md5Object = (*env)->CallStaticObjectMethod(env, mdClass, mdMethod, (*env)->NewStringUTF(env, "MD5"));
    // MessageDigest#update
    jmethodID mdUpdateMethod = (*env)->GetMethodID(env, mdClass, "update", "([B)V");// The return value of this function is void, write V
    (*env)->CallVoidMethod(env, md5Object, mdUpdateMethod, signatureByteArray);
    // MessageDigest#digest
    jmethodID mdDigestMethod = (*env)->GetMethodID(env, mdClass, "digest", "()[B");
    jobject fingerPrintByteArray = (*env)->CallObjectMethod(env, md5Object, mdDigestMethod);
    // iterate over the bytes and convert to md5 array
    jsize byteArrayLength = (*env)->GetArrayLength(env, fingerPrintByteArray);
    jbyte *fingerPrintByteArrayElements = (*env)->GetByteArrayElements(env, fingerPrintByteArray, JNI_FALSE);
    char *charArray = (char *) fingerPrintByteArrayElements;
    char *md5 = (char *) calloc(2 * byteArrayLength + 1, sizeof(char));
    int k;
    for (k = 0; k < byteArrayLength; k++) {
        sprintf(&md5[2 * k], "%02X", charArray[k]); // Not sure if the cast is needed
    }
    return md5;
}

JNIEXPORT jstring JNICALL
Java_com_mikepenz_credsshowcase_CustomApplication_getSdkKey(JNIEnv *env, jobject thiz, jobject context) {
    return (*env)->NewStringUTF(env, "amazing-key");
}

JNIEXPORT jstring JNICALL
Java_com_mikepenz_credsshowcase_CustomApplication_getSdkSecret(JNIEnv *env, jobject thiz, jobject context) {
    // modify fingerprint to your certificates fingerprint here
    if (strcmp("66C60BDAE163ECDB2A8871C0B53FFF00", getSignature(env, context)) == 0) {
        char str[20];
        const char *Z = "secret";
        const char *X = "super-";
        const char *Y = "secure-";
        strcpy(str, X);
        strcat(str, Y);
        strcat(str, Z);
        return (*env)->NewStringUTF(env, str);
    } else {
        return (*env)->NewStringUTF(env, "not-verified");
    }
}