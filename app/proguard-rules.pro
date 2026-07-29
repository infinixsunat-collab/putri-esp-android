# ============================================================
# PROGUARD RULES — PUTRI ESP MOD
# Obfuscate aggressively while keeping native/JNI methods
# ============================================================

# Keep ALL native methods (they're registered via JNI)
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep classes that use native methods (JNI registration targets)
-keep class com.tencent.qq.LoginActivity { *; }
-keep class com.tencent.qq.MainActivity { *; }
-keep class com.tencent.qq.FloatLogo { *; }
-keep class com.tencent.qq.Overlay { *; }

# Keep Prefs (SharedPreferences wrapper used by login)
-keep class com.tencent.qq.Prefs { *; }

# Keep all public classes, methods, fields in our package
-keep class com.tencent.qq.** { *; }

# Keep serialization / JSON classes
-keep class com.alibaba.fastjson.** { *; }

# Keep support libraries
-keep class android.support.v4.** { *; }
-keep class android.support.v7.** { *; }
-dontwarn android.support.**

# Keep AndroidX
-keep class androidx.** { *; }
-dontwarn androidx.**

# Obfuscate everything else aggressively
-allowaccessmodification
-repackageclasses 'z'
-flattenpackagehierarchy 'z'
-useuniqueclassmembernames
-optimizations !code/simplification/arithmetic,!code/simplification/cast,!field/*,!class/merging/*

# Remove debug logs
-assumenosideeffects class android.util.Log {
    public static boolean isLoggable(java.lang.String, int);
    public static int v(...);
    public static int d(...);
    public static int i(...);
    public static int w(...);
}

# Remove redundant code
-optimizationpasses 5
-mergeinterfacesaggressively
-overloadaggressively