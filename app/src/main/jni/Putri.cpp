#include <jni.h>
#include <string>

#include "ESP.h"
#include "Hacks.h"

ESP espOverlay;
int type=1,utype=2;
extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_Overlay_DrawOn(JNIEnv *env, jclass , jobject espView, jobject canvas) {
    espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()){
        DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());
    }
}
extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_Overlay_Close(JNIEnv *,  jobject ) {
   Close();
}
extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_FloatLogo_SettingValue(JNIEnv *,  jobject ,jint code,jboolean jboolean1) {
    switch((int)code){
        case 0:
            isESP = jboolean1;
            break;
        case 1:
            isPlayerName = jboolean1;
            break;
        case 2:
            isPlayerHealth = jboolean1;
            break;
        case 3:
            isPlayerDist = jboolean1;
            break;
        case 4:
            isShowTeamRoom = jboolean1;
            break;
		case 9:
            isShowEnemyRoom = jboolean1;
            break;	
        case 5:
            isPlayerLine = jboolean1;
            break;
        case 6:
            isPlayerBox = jboolean1;
            break;
        case 7:
            isPlayer360 = jboolean1;
            break;
        case 8:
            isNearEnemy = jboolean1;
            break;
        case 14:
            isDrawCoolDown = jboolean1;
            break;
		case 15:
            Drone = jboolean1;
            break;	
		case 16:
            isDrawRadar = jboolean1;
            break;
		case 17:
            ismDrawAvatar = jboolean1;
            break;	
		case 18:
            ismDrawHealth = jboolean1;
            break;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_FloatLogo_Drones(JNIEnv *, jclass clazz,jint dronee) {
    droneH=dronee;
}

extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_FloatLogo_IconS(JNIEnv *, jclass clazz,jint icns) {
    iconSizee=icns;
}

extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_FloatLogo_MiniP(JNIEnv *, jclass clazz,jint minip) {
    minimapPoss=minip;
}

extern "C" JNIEXPORT void JNICALL
Java_com_tencent_qq_FloatLogo_MiniS(JNIEnv *, jclass clazz,jint minis) {
    minimapSizee=minis;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_tencent_qq_Overlay_getReady(JNIEnv *, jclass ,int typeofgame) {
    int sockCheck=1;
    if (!Create()) {
        LOGI("Creation failed");
        return false;
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&sockCheck, sizeof(int));
    if (!Bind()) {
        LOGI("Bind failed");
        return false;
    }
    if (!Listen()) {
        LOGI("Listen failed");
        return false;
    }
    if (Accept()) {
        LOGI("Connected With The Server");
		SetValue sv{};
        sv.mode=typeofgame;
        sv.type=utype;
        send((void*)&sv,sizeof(sv));
        return true;
    }
}

// ============================================================
// JNI REGISTRATION: Init, Check (LoginActivity) + IsVerified (MainActivity)
// ============================================================
int Register(JNIEnv *env) {
    // Register LoginActivity methods
    JNINativeMethod loginMethods[] = {
        {"Init","(Landroid/content/Context;)V",(void *) native_Init},
        {"Check","(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;", (void *) native_Check}
    };
    jclass loginClazz = env->FindClass("com/tencent/qq/LoginActivity");
    if (!loginClazz) return -1;
    if (env->RegisterNatives(loginClazz, loginMethods, 2) != 0) return -1;

    // Register MainActivity verification method
    JNINativeMethod mainMethods[] = {
        {"IsVerified","()Z",(void *) native_IsVerified}
    };
    jclass mainClazz = env->FindClass("com/tencent/qq/MainActivity");
    if (!mainClazz) return -1;
    if (env->RegisterNatives(mainClazz, mainMethods, 1) != 0) return -1;

    return 0;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    // Integrity check: verify Register() function hasn't been patched
    if (!VerifyIntegrity(env)) {
        LOGE("[SECURITY] Integrity check failed!");
        return -1;  // Refuse to load if patched
    }

    if (Register(env) != 0)
        return -1;

    return JNI_VERSION_1_6;
}

