#include "curl/curl.h"
#include "Tools.h"
#include "json.hpp"
#include "Includes.h"

using json = nlohmann::ordered_json;
bool bValid = false, lolo = false, g_bInitialized = false;
std::string g_Token, g_Auth;

// ============================================================
// XOR-ENCRYPTED STRINGS (decrypted at runtime, key 0xAE)
// ============================================================
#define XOR_KEY 0xAE
#define DEC(carr) Tools::DecryptString(carr, sizeof(carr), XOR_KEY)

// SALT: 'Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E'
static const unsigned char ENC_SALT[] = {0xf8, 0xc3, 0x96, 0xe2, 0xc5, 0x99, 0xfb, 0xc4, 0x9c, 0xe4, 0xc3, 0xdd, 0xc4, 0xed, 0xfe, 0xf8, 0xfe, 0xf8, 0xc4, 0xdc, 0xe2, 0xcf, 0x99, 0xd4, 0xc9, 0xc8, 0xd6, 0x9d, 0xdb, 0xd4, 0x97, 0xeb};
// URL: 'putri-esp-server.vercel.app/api/connect'
static const unsigned char ENC_URL[] = {0xde, 0xdb, 0xda, 0xdc, 0xc7, 0x83, 0xcb, 0xdd, 0xde, 0x83, 0xdd, 0xcb, 0xdc, 0xd8, 0xcb, 0xdc, 0x80, 0xd8, 0xcb, 0xdc, 0xcd, 0xcb, 0xc2, 0x80, 0xcf, 0xde, 0xde, 0x81, 0xcf, 0xde, 0xc7, 0x81, 0xcd, 0xc1, 0xc0, 0xc0, 0xcb, 0xcd, 0xda};
// GAME_PARAM: 'game=ROV&user_key=%s&serial=%s&hmac=%s'
static const unsigned char ENC_GAME_PARAM[] = {0xc9, 0xcf, 0xc3, 0xcb, 0x93, 0xfc, 0xe1, 0xf8, 0x88, 0xdb, 0xdd, 0xcb, 0xdc, 0xf1, 0xc5, 0xcb, 0xd7, 0x93, 0x8b, 0xdd, 0x88, 0xdd, 0xcb, 0xdc, 0xc7, 0xcf, 0xc2, 0x93, 0x8b, 0xdd, 0x88, 0xc6, 0xc3, 0xcf, 0xcd, 0x93, 0x8b, 0xdd};
// ERR_HMAC: 'Invalid server HMAC'
static const unsigned char ENC_ERR_HMAC[] = {0xe7, 0xc0, 0xd8, 0xcf, 0xc2, 0xc7, 0xca, 0x8e, 0xdd, 0xcb, 0xdc, 0xd8, 0xcb, 0xdc, 0x8e, 0xe6, 0xe3, 0xef, 0xed};
// ERR_EXPIRED: 'Token expired'
static const unsigned char ENC_ERR_EXPIRED[] = {0xfa, 0xc1, 0xc5, 0xcb, 0xc0, 0x8e, 0xcb, 0xd6, 0xde, 0xc7, 0xdc, 0xcb, 0xca};
// ERR_NOT_INIT: 'Auth system not initialized'
static const unsigned char ENC_ERR_NOT_INIT[] = {0xef, 0xdb, 0xda, 0xc6, 0x8e, 0xdd, 0xd7, 0xdd, 0xda, 0xcb, 0xc3, 0x8e, 0xc0, 0xc1, 0xda, 0x8e, 0xc7, 0xc0, 0xc7, 0xda, 0xc7, 0xcf, 0xc2, 0xc7, 0xd4, 0xcb, 0xca};
// PUTRI_LIB: 'Putri'
static const unsigned char ENC_PUTRI_LIB[] = {0xfe, 0xdb, 0xda, 0xdc, 0xc7};
// CONTENT_TYPE: 'Content-Type: application/x-www-form-urlencoded'
static const unsigned char ENC_CONTENT_TYPE[] = {0xed, 0xc1, 0xc0, 0xda, 0xcb, 0xc0, 0xda, 0x83, 0xfa, 0xd7, 0xde, 0xcb, 0x94, 0x8e, 0xcf, 0xde, 0xde, 0xc2, 0xc7, 0xcd, 0xcf, 0xda, 0xc7, 0xc1, 0xc0, 0x81, 0xd6, 0x83, 0xd9, 0xd9, 0xd9, 0x83, 0xc8, 0xc1, 0xdc, 0xc3, 0x83, 0xdb, 0xdc, 0xc2, 0xcb, 0xc0, 0xcd, 0xc1, 0xca, 0xcb, 0xca};
// OK_STR: 'OK'
static const unsigned char ENC_OK[] = {0xe1, 0xe5};
// WELCOME: 'Welcome'
static const unsigned char ENC_WELCOME[] = {0xf9, 0xcb, 0xc2, 0xcd, 0xc1, 0xc3, 0xcb};

// ============================================================
// INTEGRITY CHECK (DISABLED — calculated post-build)
// ============================================================
// To enable: build once, extract XOR64 of Register() function, update value below
/*
#define EXPECTED_REGISTER_FP 0x7A3B5C1D9E8F4A2BULL

extern int Register(JNIEnv *env);

static bool VerifyIntegrity(JNIEnv *env) {
    uintptr_t regAddr = (uintptr_t)&Register;
    unsigned char buffer[128] = {0};
    memcpy(buffer, (void*)regAddr, 128);
    uint64_t fp = Tools::CalcXOR64(buffer, 128);
    if (fp != EXPECTED_REGISTER_FP) {
        LOGE("%s", DEC(ENC_ERR_INTEGRITY).c_str());
        return false;
    }
    return true;
}
*/

static bool VerifyIntegrity(JNIEnv *env) {
    // Integrity check placeholder — will be enabled after first build
    return true;
}

// ============================================================
// JNI HELPER FUNCTIONS
// ============================================================
const char *GetAndroidID(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID getContentResolverMethod = env->GetMethodID(contextClass,"getContentResolver","()Landroid/content/ContentResolver;");
    jclass settingSecureClass = env->FindClass("android/provider/Settings$Secure");
    jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass,"getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");

    auto obj = env->CallObjectMethod(context, getContentResolverMethod);
    auto str = (jstring) env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj,env->NewStringUTF("android_id"));
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceModel(JNIEnv *env) {
    jclass buildClass = env->FindClass("android/os/Build");
    jfieldID modelId = env->GetStaticFieldID(buildClass, "MODEL","Ljava/lang/String;");
    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceBrand(JNIEnv *env) {
    jclass buildClass = env->FindClass("android/os/Build");
    jfieldID modelId = env->GetStaticFieldID(buildClass, "BRAND","Ljava/lang/String;");
    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid) {
    jclass uuidClass = env->FindClass("java/util/UUID");
    auto len = strlen(uuid);
    jbyteArray myJByteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *) uuid);
    jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass,"nameUUIDFromBytes","([B)Ljava/util/UUID;");
    jmethodID toStringMethod = env->GetMethodID(uuidClass, "toString","()Ljava/lang/String;");
    auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
    auto str = (jstring) env->CallObjectMethod(obj, toStringMethod);
    return env->GetStringUTFChars(str, 0);
}

// ============================================================
// HTTP CALLBACK
// ============================================================
struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;
    mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if (!mem->memory) return 0;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

// ============================================================
// NATIVE CHECK — Main login function
// ============================================================
jstring native_Check(JNIEnv *env, jclass clazz, jobject mContext, jstring mUserKey) {
    bValid = false;
    auto userKey = env->GetStringUTFChars(mUserKey, 0);

    std::string hwid = userKey;
    hwid += GetAndroidID(env, mContext);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);
    std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());
    std::string errMsg;
    std::string SALT = DEC(ENC_SALT);

    struct MemoryStruct chunk{};
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        std::string api_key = "https://" + DEC(ENC_URL);
        curl_easy_setopt(curl, CURLOPT_URL, api_key.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, DEC(ENC_CONTENT_TYPE).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // HMAC two-way auth
        std::string clientHmacData = std::string(userKey) + UUID;
        std::string clientHmac = Tools::CalcHMAC(SALT, clientHmacData);

        char data[4096];
        sprintf(data, DEC(ENC_GAME_PARAM).c_str(), userKey, UUID.c_str(), clientHmac.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            try {
                json result = json::parse(chunk.memory);
                auto STATUS = std::string{"status"};
                if (result[STATUS] == true) {
                    auto DATA = std::string{"data"};
                    auto TOKEN = std::string{"token"};
                    auto RNG = std::string{"rng"};
                    auto HMAC = std::string{"hmac"};
                    std::string token = result[DATA][TOKEN].get<std::string>();
                    time_t rng = result[DATA][RNG].get<time_t>();
                    std::string serverHmac = result[DATA][HMAC].get<std::string>();

                    if (rng + 30 > time(0)) {
                        std::string expectedServerHmac = Tools::CalcHMAC(SALT, token + std::to_string(rng));
                        if (serverHmac == expectedServerHmac) {
                            std::string auth = "ROV";
                            auth += std::string("-");
                            auth += userKey;
                            auth += std::string("-");
                            auth += UUID;
                            auth += std::string("-");
                            auth += SALT;
                            std::string outputAuth = Tools::CalcMD5(auth);
                            g_Token = token;
                            g_Auth = outputAuth;
                            bValid = g_Token == g_Auth;
                            g_bInitialized = true;
                            lolo = true;
                            if (bValid && lolo) {
                                LOGI("putri");
                            }
                        } else {
                            errMsg = DEC(ENC_ERR_HMAC);
                        }
                    } else {
                        errMsg = DEC(ENC_ERR_EXPIRED);
                    }
                } else {
                    auto REASON = std::string{"reason"};
                    errMsg = result[REASON].get<std::string>();
                }
            } catch (json::exception &e) {
                errMsg = e.what();
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    std::string okStr = DEC(ENC_OK);
    return bValid ? env->NewStringUTF(okStr.c_str()) : env->NewStringUTF(errMsg.c_str());
}

// ============================================================
// NATIVE INIT — Show welcome toast
// ============================================================
void native_Init(JNIEnv *env, jclass clazz, jobject mContext) {
    std::string welcome = DEC(ENC_WELCOME);
    jstring pMsg = env->NewStringUTF(welcome.c_str());
    jclass toastClass = env->FindClass("android/widget/Toast");
    jmethodID makeTextMethod = env->GetStaticMethodID(toastClass, "makeText","(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    jobject toastObj = env->CallStaticObjectMethod(toastClass, makeTextMethod, mContext, pMsg, 0);
    jmethodID methodShow = env->GetMethodID(toastClass, "show","()V");
    env->CallVoidMethod(toastObj, methodShow);
}

// ============================================================
// NATIVE IS_VERIFIED — Second-layer check (called from MainActivity)
// This prevents DEX-only bypasses
// ============================================================
jboolean native_IsVerified(JNIEnv *env, jclass clazz) {
    // Must have passed Check() first
    if (!g_bInitialized || !bValid) {
        return JNI_FALSE;
    }
    // Re-verify token vs auth hash
    if (g_Token.empty() || g_Auth.empty() || g_Token != g_Auth) {
        bValid = false;
        return JNI_FALSE;
    }
    return JNI_TRUE;
}