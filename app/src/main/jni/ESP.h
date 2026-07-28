#ifndef ESP_H
#define ESP_H

#include <jni.h>
#include "Color.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"

enum Align {
    LEFT = 1,
    CENTER = 2,
    RIGHT = 3
};

class ESP {
private:
    JNIEnv *_env;
    jobject _cvsView;
    jobject _cvs;
public:
    ESP() {
        _env = nullptr;
        _cvsView = nullptr;
        _cvs = nullptr;
    }

    ESP(JNIEnv *env, jobject cvsView, jobject cvs) {
        this->_env = env;
        this->_cvsView = cvsView;
        this->_cvs = cvs;
    }

    JNIEnv *getEnviroument() const {
        return _env;
    }

    jobject getEspView() const {
        return _cvsView;
    }

    jobject getCanavas() const {
        return _cvs;
    }

    bool isValid() const {
        return (_env != nullptr && _cvsView != nullptr && _cvs != nullptr);
    }

    int getWidth() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID width = _env->GetMethodID(canvas, "getWidth", "()I");
            return _env->CallIntMethod(_cvs, width);
        }
        return 0;
    }

    int getHeight() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID width = _env->GetMethodID(canvas, "getHeight", "()I");
            return _env->CallIntMethod(_cvs, width);
        }
        return 0;
    }

    void invalidate() {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID inavlidate = _env->GetMethodID(canvasView, "invalidate", "()V");
            _env->CallVoidMethod(_cvsView, inavlidate);
        }
    }

    void postInvalidate() {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID inavlidate = _env->GetMethodID(canvasView, "postInvalidate", "()V");
            _env->CallVoidMethod(_cvsView, inavlidate);
        }
    }

    void ClearCanvas() {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID clearCanvas = _env->GetMethodID(canvasView, "ClearCanvas",
                                                      "(Landroid/graphics/Canvas;)V");
            _env->CallVoidMethod(_cvsView, clearCanvas, _cvs);
        }
    }

    void DrawLine(Color color, float thickness, Vector2 start, Vector2 end) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawline = _env->GetMethodID(canvasView, "DrawLine",
                                                   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
            _env->CallVoidMethod(_cvsView, drawline, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 thickness,
                                 start.x, start.y, end.x, end.y);
        }
    }
    void DrawTextA(Color color, const char *txt, Vector2 pos, float size, Align align) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawTextA",
                                                   "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFFI)V");
            jstring s=_env->NewStringUTF(txt);
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 s, pos.x, pos.y, size, align);
            _env->DeleteLocalRef(s);
        }
    }

    void DrawText(Color color, const char *txt, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawText",
                                                   "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 _env->NewStringUTF(txt), pos.x, pos.y, size);
        }
    }

    void DrawPlayerText(Color color, const wchar_t *txt, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawText",
                                                   "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 wcstojstr(_env, txt), pos.x, pos.y, size);
        }
    }

    static jstring wcstojstr(JNIEnv *env, const wchar_t *input) {
        jobject bb = env->NewDirectByteBuffer((void *)input, wcslen(input) * sizeof(wchar_t));
        jstring UTF32LE = env->NewStringUTF("UTF-32LE");

        jclass charsetClass = env->FindClass("java/nio/charset/Charset");
        jmethodID forNameMethod = env->GetStaticMethodID(charsetClass, "forName", "(Ljava/lang/String;)Ljava/nio/charset/Charset;");
        jobject charset = env->CallStaticObjectMethod(charsetClass, forNameMethod, UTF32LE);

        jmethodID decodeMethod = env->GetMethodID(charsetClass, "decode", "(Ljava/nio/ByteBuffer;)Ljava/nio/CharBuffer;");
        jobject cb = env->CallObjectMethod(charset, decodeMethod, bb);

        jclass charBufferClass = env->FindClass("java/nio/CharBuffer");
        jmethodID toStringMethod = env->GetMethodID(charBufferClass, "toString", "()Ljava/lang/String;");
        jstring ret = (jstring)env->CallObjectMethod(cb, toStringMethod);

        env->DeleteLocalRef(bb);
        env->DeleteLocalRef(UTF32LE);
        env->DeleteLocalRef(charsetClass);
        env->DeleteLocalRef(charset);
        env->DeleteLocalRef(cb);
        env->DeleteLocalRef(charBufferClass);

        return ret;
    }

    void DrawCircle(Color color, float thickness, Vector2 pos, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawcircle = _env->GetMethodID(canvasView, "DrawCircle",
                                                     "(Landroid/graphics/Canvas;IIIIFFFF)V");
            _env->CallVoidMethod(_cvsView, drawcircle, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 thickness,
                                 pos.x, pos.y, radius);
        }
    }
	 void DrawCircle3(Color color, Vector2 pos, float radius,float thickness) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawcircle = _env->GetMethodID(canvasView, "DrawCircle3",
                                                     "(Landroid/graphics/Canvas;IIIIFFFF)V");
            _env->CallVoidMethod(_cvsView, drawcircle, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, pos.x, pos.y, radius,thickness);
        }
    }
    void DrawCircle2(Color color, Vector2 c, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawCircle = _env->GetMethodID(canvasView, "DrawCircle2",
                                                     "(Landroid/graphics/Canvas;IIIIFFF)V");
            _env->CallVoidMethod(_cvsView, drawCircle, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, c.x, c.y, radius);
        }
    }
    void DrawCircularHealth(Color color, float currentHealth, float maxHealth, Vector2 pos, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawcircle = _env->GetMethodID(canvasView, "DrawCircularHealth", "(Landroid/graphics/Canvas;FFFFFIIII)V");
            _env->CallVoidMethod(_cvsView, drawcircle, _cvs, currentHealth, maxHealth, pos.x, pos.y, radius, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b);
        }
    }

    void DrawBase64Bitmap(int index, int width, int height, Vector2 pos) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawbitmap = _env->GetMethodID(canvasView, "DrawBase64Bitmap","(Landroid/graphics/Canvas;IIIFF)V");
            _env->CallVoidMethod(_cvsView, drawbitmap, _cvs, index, width, height, pos.x, pos.y);
        }
    }
    void DrawFilledCircle(Color color, Vector2 pos, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawfilledcircle = _env->GetMethodID(canvasView, "DrawFilledCircle",
                                                           "(Landroid/graphics/Canvas;IIIIFFF)V");
            _env->CallVoidMethod(_cvsView, drawfilledcircle, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, pos.x, pos.y, radius);
        }
    }

    void DrawRect(Color color, int thickness, Rect rect) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawrect = _env->GetMethodID(canvasView, "DrawRect",
                                                   "(Landroid/graphics/Canvas;IIIIIFFFF)V");
            _env->CallVoidMethod(_cvsView, drawrect, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, thickness,
                                 rect.x, rect.y, rect.width, rect.height);
        }
    }

    void DrawRect2(Color color, float strokeSize, Vector2 start, Vector2 end) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawRect = _env->GetMethodID(canvasView, "DrawRect2",
                                                   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
            _env->CallVoidMethod(_cvsView, drawRect, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, strokeSize, start.x, start.y, end.x, end.y);
        }
    }
	void DrawText2(Color color, const char *nick, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawText2 = _env->GetMethodID(canvasView, "DrawText2",
                                                    "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_cvsView, drawText2, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 _env->NewStringUTF(nick), pos.x, pos.y, size);
        }
    }
     void DrawEnemyCount(int colors, Vector2 d) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawEnemyCount",
                                                   "(Landroid/graphics/Canvas;IFF)V");
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, colors, d.x, d.y);
        }
    }
    void DrawFilledRect(Color color, Rect rect) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawfilledrect = _env->GetMethodID(canvasView, "DrawFilledRect",
                                                         "(Landroid/graphics/Canvas;IIIIFFFF)V");
            _env->CallVoidMethod(_cvsView, drawfilledrect, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 rect.x, rect.y, rect.width, rect.height);
        }
    }
   void DrawFilledRectB(Color color, Vector2 pos, float width, float height) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawfilledrect = _env->GetMethodID(canvasView, "DrawFilledRectB",
                                                           "(Landroid/graphics/Canvas;IIIIFFFF)V");
            _env->CallVoidMethod(_cvsView, drawfilledrect, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, pos.x, pos.y, width, height);
        }
    }
	void DrawBox(Color color, float stroke, Rect rect) {
        Vector2 v1 = Vector2(rect.x, rect.y);
        Vector2 v2 = Vector2(rect.x + rect.width, rect.y);
        Vector2 v3 = Vector2(rect.x + rect.width, rect.y + rect.height);
        Vector2 v4 = Vector2(rect.x, rect.y + rect.height);

        DrawLine(color, stroke, v1, v2);
        DrawLine(color, stroke, v2, v3);
        DrawLine(color, stroke, v3, v4);
        DrawLine(color, stroke, v4, v1);
    }
    void DrawLineBorder(Color color, float stroke, Rect rect) {
        // top left
        Vector2 v1 = Vector2(rect.x, rect.y);
        // top right
        Vector2 v2 = Vector2(rect.x + rect.width, rect.y);
        // bottom right
        Vector2 v3 = Vector2(rect.x + rect.width, rect.y + rect.height);
        // bottom left
        Vector2 v4 = Vector2(rect.x, rect.y + rect.height);

        // top left to top right
        DrawLine(color, stroke, v1, v2);
        // top right to bottom right
        DrawLine(color, stroke, v2, v3);
        // bottom right to bottom left
        DrawLine(color, stroke, v3, v4);
        // bottom left to top left
        DrawLine(color, stroke, v4, v1);
    }

    void DrawHorizontalHealthBar(Vector2 screenPos, float width, float maxHealth, float currentHealth) {
        screenPos -= Vector2(0.0f, 8.0f);
        DrawLineBorder(Color(0, 0, 0, 255), 3, Rect(screenPos.x, screenPos.y, width + 2, 5.0f));
        screenPos += Vector2(1.0f, 1.0f);
        Color clr = Color(0, 255, 0, 255);
        float hpWidth = (currentHealth * width) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = Color(255, 255, 0, 255);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = Color(255, 0, 0, 255);
        }
        DrawLineBorder(clr, 3, Rect(screenPos.x, screenPos.y, hpWidth, 3.0f));
    }

    void DrawVerticalHealthBar(Vector2 screenPos, float height, float maxHealth, float currentHealth) {
        screenPos += Vector2(8.0f, 0.0f);
        DrawLineBorder(Color(0, 0, 0, 255), 3, Rect(screenPos.x, screenPos.y, 5.0f, height + 2));
        screenPos += Vector2(1.0f, 1.0f);
        Color clr = Color(0, 255, 0, 255);
        float barHeight = (currentHealth * height) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = Color(255, 255, 0, 255);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = Color(255, 0, 0, 255);
        }
        DrawLineBorder(clr, 3, Rect(screenPos.x, screenPos.y, 3.0f, barHeight));
    }

    void DrawSSHealthBar(Vector2 screenPos, float width, float maxHealth, float currentHealth){
        screenPos -= Vector2(0.0f, 8.0f);

        DrawLineBorder(Color::Black(), 1, Rect(screenPos.x, screenPos.y, width + 2, 30.0f));
        screenPos += Vector2(1.0f, 1.0f);
        Color clr = Color(133, 255, 139, 135);
        float hpWidth = (currentHealth * width) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = Color(255, 134, 136, 135);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = Color(255, 0, 0, 135);
        }
        DrawLineBorder(clr, 3, Rect(screenPos.x, screenPos.y, hpWidth, 30.0f));
        DrawFilledRect(clr,Rect(screenPos.x, screenPos.y, hpWidth, 30.0f));
    }

    void DrawCrosshair(Color clr, Vector2 center, float size = 20) {
        float x = center.x - (size / 2.0f);
        float y = center.y - (size / 2.0f);
        DrawLine(clr, 3, Vector2(x, center.y), Vector2(x + size, center.y));
        DrawLine(clr, 3, Vector2(center.x, y), Vector2(center.x, y + size));
    }

    void DrawSSNearEnemyBar(int screenWhidth, int count){


        Color clr3 = Color(255,253,112,100);
        Color clr4 = Color(255,253,112,175);

        Color clr5 = Color(11,255,17,100);
        Color clr6 = Color(11,255,17,175);



        if(count>0){
            DrawFilledRect(clr4,Rect(screenWhidth/2-75,77,135,50));
            DrawFilledRect(clr3,Rect(screenWhidth/2+40+20,77,20,50));
            DrawFilledRect(clr3,Rect(screenWhidth/2-75-20,77,20,50));
         //   DrawLine(Color::Yellow(),1,Vector2(screenWhidth/2-75-20,77),Vector2(screenWhidth/2+75+10,77));
           // DrawLine(Color::Yellow(),1,Vector2(screenWhidth/2-75-20,77+50),Vector2(screenWhidth/2+75+10,77+50));

        } else {
          //  DrawLine(Color::Green(),1,Vector2(screenWhidth/2-75-20,77),Vector2(screenWhidth/2+75+10,77));
           // DrawLine(Color::Green(),1,Vector2(screenWhidth/2-75-20,77+50),Vector2(screenWhidth/2+75+10,77+50));
            DrawFilledRect(clr6,Rect(screenWhidth/2-75,77,135,50));
            DrawFilledRect(clr5,Rect(screenWhidth/2+40+20,77,20,50));
            DrawFilledRect(clr5,Rect(screenWhidth/2-75-20,77,20,50));

        }

    }
};

#endif
