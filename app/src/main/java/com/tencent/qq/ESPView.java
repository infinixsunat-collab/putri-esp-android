package com.tencent.qq;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.os.SystemClock;
import android.view.View;
import android.graphics.Matrix;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import android.graphics.Rect;
import android.graphics.drawable.GradientDrawable;
import android.util.Log;
import static com.tencent.qq.Overlay.getConfig;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.drawable.GradientDrawable;
import android.util.Log;
import android.util.LruCache;
import android.view.View;

import androidx.annotation.Keep;
import androidx.annotation.Nullable;

import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class ESPView extends View implements Runnable {
	static long sleepTime;
	Thread mThread;
	Rect eRect;
    Bitmap bitmap,out,bitmap2,out2;
	private final LruCache<Integer, Bitmap> bitmapCache;
    GradientDrawable eGradientDrawable;
    Date time;
	int eColor[] = {Color.TRANSPARENT,Color.GREEN,Color.TRANSPARENT};   
    int FPS = 60;
	Paint mStrokePaint2;
    Paint mFilledPaint2;
    Paint mStrokePaint;
    Paint mFilledPaint;
    Paint mTextPaint;
    Paint mTextPaint2;
    Paint mRectPaint;   
    Paint p;
	Paint mHealthPaint;
    Paint mTextStroke;
    SimpleDateFormat formatter;
    SimpleDateFormat formatter2;
    public static void ChangeFps(int fps){
        sleepTime =1000/(20+fps);
    }  
    
    public ESPView(Context context) {
        super(context, null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(Color.TRANSPARENT);
        time = new Date();
        formatter = new SimpleDateFormat("HH:mm:ss", Locale.getDefault());
        formatter2 = new SimpleDateFormat("dd/MM/yyyy", Locale.getDefault());
		final int maxMemory = (int) (Runtime.getRuntime().maxMemory() / 1024);
		final int cacheSize = maxMemory / 8;
        sleepTime = 1000 / FPS;
        mThread = new Thread(this);
        mThread.start();
		bitmapCache = new LruCache<Integer, Bitmap>(cacheSize) {
			@Override
			protected int sizeOf(Integer key, Bitmap bitmap) {
				return bitmap.getByteCount() / 1024;
			}
		};	
    }
    
	
    @Override
    protected void onDraw(Canvas canvas) {
        if (canvas != null && getVisibility() == VISIBLE) {
            ClearCanvas(canvas);
            Overlay.DrawOn(this, canvas);
        }
    }

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
        while (mThread.isAlive() && !mThread.isInterrupted()) {
            try {
                long t1 = System.currentTimeMillis();
                postInvalidate();
                long td = System.currentTimeMillis() - t1;
                Thread.sleep(Math.max(Math.min(0, sleepTime - td), sleepTime));
            } catch (Exception e) {
                Thread.currentThread().interrupt();
                return;
            }
        }
    }
	
    public void InitializePaints() {
        eRect = new Rect(-100,60,100,100);
        eGradientDrawable = new GradientDrawable();
        eGradientDrawable.setOrientation(GradientDrawable.Orientation.LEFT_RIGHT);
        eGradientDrawable.setColors(eColor);
        eGradientDrawable.setShape(GradientDrawable.RECTANGLE);
        eGradientDrawable.setBounds(eRect);
        
		mHealthPaint = new Paint();
        mHealthPaint.setStyle(Paint.Style.STROKE);
        mHealthPaint.setAntiAlias(true);
		
        mStrokePaint = new Paint();
        mStrokePaint.setStyle(Paint.Style.STROKE);
        mStrokePaint.setAntiAlias(true);
        mStrokePaint.setColor(Color.rgb(0, 0, 0));
        
        mStrokePaint2 = new Paint();
        mStrokePaint2.setStyle(Paint.Style.STROKE);
        mStrokePaint2.setAntiAlias(true);
        mStrokePaint2.setColor(Color.rgb(0, 0, 0));

        mFilledPaint2 = new Paint();
        mFilledPaint2.setStyle(Paint.Style.FILL);
        mFilledPaint2.setAntiAlias(true);
        mFilledPaint2.setColor(Color.rgb(0, 0, 0));
        
        mFilledPaint = new Paint();
        mFilledPaint.setStyle(Paint.Style.FILL);
        mFilledPaint.setAntiAlias(true);
        mFilledPaint.setColor(Color.rgb(0, 0, 0));
        String str = "#000000";
        mTextStroke = new Paint();
        mTextStroke.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextStroke.setAntiAlias(true);
        mTextStroke.setColor(Color.parseColor(str));
        mTextStroke.setTextAlign(Paint.Align.CENTER);
        mTextStroke.setStrokeWidth(4.0f);
        mTextStroke.setStrokeMiter(4.0f);
        mTextStroke.setTypeface(Typeface.DEFAULT_BOLD);
        mTextPaint = new Paint();
        mTextPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setColor(Color.rgb(0, 0, 0));
        mTextPaint.setTextAlign(Paint.Align.CENTER);
        mTextPaint.setStrokeWidth(1.1f);
        
        mTextPaint2 = new Paint();
        mTextPaint2.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextPaint2.setAntiAlias(true);
        mTextPaint2.setColor(Color.rgb(0, 0, 0));
        mTextPaint2.setTextAlign(Paint.Align.CENTER);
        mTextPaint2.setStrokeWidth(1.1f);
            
        mRectPaint = new Paint();
        mRectPaint.setStyle(Paint.Style.STROKE);
        mRectPaint.setAntiAlias(true);
        
        p=new Paint();{                  
                    
                    
        }
	}
	public void addBitmapToMemoryCache(Integer key, Bitmap bitmap) {
        synchronized (bitmapCache) {
            if (bitmapCache.get(key) == null) {
                bitmapCache.put(key, bitmap);
            }
        }
    }

    public Bitmap getBitmapFromMemCache(Integer key) {
        synchronized (bitmapCache) {
            return bitmapCache.get(key);
        }
    }

    @Nullable
    public Bitmap loadBitmap(int index, int width, int height){
        final int key = Objects.hash(index, width, height);
        final Bitmap bitmap = getBitmapFromMemCache(key);
        if (bitmap != null) {
            return bitmap;
        } else {
            int id = getResources().getIdentifier("ic_hero_" + index, "drawable", getContext().getPackageName());
            if (id == 0) {
                id = getResources().getIdentifier("ic_hero_0", "drawable", getContext().getPackageName());
            }
            Bitmap decodedBitmap = BitmapFactory.decodeResource(getResources(), id);
            Bitmap out = Bitmap.createScaledBitmap(decodedBitmap, width, height, true);
            addBitmapToMemoryCache(key, out);
            return out;
        }
    }

    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }
	public void DrawEnemyCount(Canvas cvs, int colors, float dx, float dy) {
        if (colors == 1){ // Enemy = 0
            eColor[1] = Color.GREEN;
        }
        if (colors == 2){ // Enemy > 5
            eColor[1] = Color.YELLOW;
        }
        if (colors == 3){ // Enemy > 10
            eColor[1] = Color.parseColor("#FF6D00");
        }
        if (colors == 4){ // Enemy > 15
            eColor[1] = Color.RED;
        }
        cvs.save();
        cvs.translate(dx,dy);
        eGradientDrawable.setGradientType(GradientDrawable.LINEAR_GRADIENT);
        eGradientDrawable.draw(cvs);
        cvs.restore();
    }  
	
    public void DrawText2(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.clearShadowLayer();
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }
    public void DrawText4(Canvas cvs, int a, int r, int g, int b,float stroke, String txt, float posX, float posY, float size) {
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        mTextPaint.setStrokeWidth(stroke);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(2 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);

            cvs.drawText(txt, posX, posY, mTextPaint);
     } 
	public void DrawText3(Canvas cvs, int a, int r, int g, int b,float stroke, String txt, float posX, float posY, float size) {
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        mTextPaint.setStrokeWidth(stroke);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(2 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);

		cvs.drawText(txt, posX, posY, mTextPaint);
	} 
     
     public void DebugText(String s) {
         System.out.println(s);
     }      
     public void DrawLine(Canvas cvs, int a, int r, int g, int b, float lineWidth, float fromX, float fromY, float toX, float toY) {
         mStrokePaint.setColor(Color.rgb(r, g, b));
         mStrokePaint.setAlpha(a);
         mStrokePaint.setStrokeWidth(lineWidth);
         cvs.drawLine(fromX, fromY, toX, toY, mStrokePaint);
     }
	public static String countryCodeToFlag(String input) {
        // Regex pattern to match <CID>{countryCode}<CID> or <CID><CID> (empty)
        Pattern pattern = Pattern.compile("<CID>([a-zA-Z]{2})?<CID>");
        Matcher matcher = pattern.matcher(input);

        // Buffer to store the final result
        StringBuffer result = new StringBuffer();

        // Iterate through the matches
        while (matcher.find()) {
            String countryCode = matcher.group(1);  // Get the country code or null if empty

            // If countryCode is not null (i.e., there is something inside <CID><CID>)
            if (countryCode != null) {
                countryCode = countryCode.toUpperCase(); // Convert to uppercase

                // Convert the country code to flag emoji using Unicode regional indicators
                int firstChar = Character.codePointAt(countryCode, 0) - 0x41 + 0x1F1E6;
                int secondChar = Character.codePointAt(countryCode, 1) - 0x41 + 0x1F1E6;
                String flagEmoji = new String(Character.toChars(firstChar)) + new String(Character.toChars(secondChar));

                // Replace <CID>{countryCode}<CID> with the flag emoji
                matcher.appendReplacement(result, flagEmoji);
            } else {
                // If <CID><CID> is empty, just replace it with an empty string
                matcher.appendReplacement(result, "");
            }
        }

        // Append the remaining part of the input
        matcher.appendTail(result);

        return result.toString();
    }

    public void DrawTextA(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size, int align) {
        mTextPaint.setARGB(a, r, g, b);
        mTextPaint.setTextAlign(Paint.Align.values()[align - 1]);
        mTextPaint.setTextSize(size);

        String[] lines = txt.split("\n");
        float lineHeight = mTextPaint.descent() - mTextPaint.ascent();
        float spacing = lineHeight * 0.2f; // Adjust spacing between lines

        for (int i = 0; i < lines.length; i++) {
            float newY = posY + (lineHeight + spacing) * i;
            cvs.drawText(countryCodeToFlag(lines[i]), posX, newY, mTextPaint);
        }
    }
	 public void DrawRect2(Canvas canvas, int a, int r, int g, int b, float strokeSize, float startX, float startY, float endX, float endY){
         mRectPaint.setColor(Color.rgb(r, g, b));
         mTextPaint.setAlpha(a);
         mRectPaint.setStrokeWidth(strokeSize);
         canvas.drawRect(startX, startY, endX, endY, mRectPaint);
     }
     public void DrawCircle2(Canvas cvs, int a, int r, int g, int b, float cx, float cy, float radius){
         mFilledPaint.setColor(Color.rgb(r, g, b));
         mFilledPaint.setAlpha(a);
         cvs.drawCircle(cx, cy, radius, mFilledPaint);
     }
	public void DrawFilledRectB(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawRect(x, y, x + width, y + height, mFilledPaint);
	}
     public void DrawText(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
         mTextPaint.setColor(Color.rgb(r, g, b));
         mTextPaint.setAlpha(a);
		 if (getRight() > 1920 || getBottom() > 1920)
             mTextPaint.setTextSize(4 + size);
         else if (getRight() == 1920 || getBottom() == 1920)
             mTextPaint.setTextSize(2 + size);
         else
             mTextPaint.setTextSize(size);
             cvs.drawText(txt, posX, posY, mTextPaint);
     }
     public void DrawCircle(Canvas cvs, int a, int r, int g, int b, float stroke, float posX, float posY, float radius) {
         mStrokePaint.setColor(Color.rgb(r, g, b));
         mStrokePaint.setAlpha(a);
         mStrokePaint.setStrokeWidth(stroke);
         cvs.drawCircle(posX, posY, radius, mStrokePaint);
     }
	 public void DrawCircle3(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float radius, float stroke) {
         mStrokePaint.setARGB(a,r,g,b);
         mStrokePaint.setStrokeWidth(stroke);
         cvs.drawCircle(posX, posY, radius, mStrokePaint);
     }
     public void DrawFilledCircle(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float radius) {
         mFilledPaint.setColor(Color.rgb(r, g, b));
         mFilledPaint.setAlpha(a);
         cvs.drawCircle(posX, posY, radius, mFilledPaint);
     }
     public void DrawRect(Canvas cvs, int a, int r, int g, int b, int stroke, float x, float y, float width, float height) {
         mStrokePaint.setStrokeWidth(stroke);
         mStrokePaint.setColor(Color.rgb(r, g, b));
         mStrokePaint.setAlpha(a);
         cvs.drawRect(x, y, x + width, y + height, mStrokePaint);
     }
     public void DrawFilledRect(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
         mFilledPaint.setColor(Color.rgb(r, g, b));
         mFilledPaint.setAlpha(a);
         cvs.drawRect(x, y, x + width, y + height, mFilledPaint);
     }
	
    public void DrawBase64Bitmap(Canvas cvs, int index, int width, int height, float posX, float posY) {
        if (width > 1 && height > 1) {
            Bitmap bitmap = loadBitmap(index, width, height);
            if (bitmap != null) {
                cvs.drawBitmap(bitmap, posX, posY, null);
            }
        }
    }

    public void DrawCircularHealth(Canvas cvs, float currentHealth, float maxHealth, float posX, float posY, float radius, int a, int r, int g, int b) {
        mHealthPaint.setColor(Color.rgb(r, g, b));
        mHealthPaint.setAlpha(a);
        mHealthPaint.setStrokeWidth(5);

        // Calculate the fraction of health remaining
        float healthFraction = currentHealth / maxHealth;

        // Full circle in degrees
        float fullCircle = 360f;

        // Calculate start angle and sweep angle based on health fraction
        // Assuming the health bar starts at 12 o'clock and fills clockwise
        float startAngle = -90; // 12 o'clock start
        float sweepAngle = -(fullCircle * healthFraction);

        // Adjust posX and posY to be the center of the circle
        // The circle is drawn based on its center position
        float left = posX - radius;
        float top = posY - radius;
        float right = posX + radius;
        float bottom = posY + radius;

        // Draw the arc representing health
        cvs.drawArc(left, top, right, bottom, startAngle, sweepAngle, false, mHealthPaint);

    }
}
