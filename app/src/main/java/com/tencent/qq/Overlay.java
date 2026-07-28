package com.tencent.qq;

import android.annotation.*;
import android.app.*;
import android.content.*;
import android.graphics.*;
import android.os.*;
import android.view.*;
import java.io.*;

import java.lang.Process;
import android.view.WindowManager.LayoutParams;


public class Overlay extends Service {
    @Override
	public IBinder onBind(Intent intent) {
        return null;
    }
    WindowManager windowManager;
	Process process;
    View mainView;
    ESPView overlayView;
    @SuppressLint("StaticFieldLeak")
    private static Overlay Instance;

	static Context ctx;
    @SuppressLint("InflateParams")
    @Override
	public void onCreate() {
		super.onCreate();
		ctx=this;
		if (MainActivity.gameType == 1) {
			Start(ctx,1,1);
		}
		windowManager = (WindowManager) ctx.getSystemService(Context.WINDOW_SERVICE);
		overlayView = new ESPView(ctx);
		DrawCanvas();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Close();

		if(overlayView != null)
		{
			((WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE)).removeView(overlayView);
			overlayView = null;
		}

		process.destroy();
	}

	public void Start(final Context context, final int gametype, final int bit) {
        if (Instance == null) {         
            Thread t = new Thread(new Runnable() {
					@Override
					public void run() {             
						getReady(gametype);
					}
				});
            t.start();


            Thread t2 = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}

						StartDaemon(context, bit);

					}
				});
            t2.start();
        }
    }

	static native boolean getReady(int nameofgame);

	public void StartDaemon(final Context context,int bit){
		Shell(FloatLogo.socket);

	}


	public static void Stop(Context context) {

		Intent intent = new Intent(context, Overlay.class);
		context.stopService(intent);

		Intent floatLogo = new Intent(context, FloatLogo.class);
		context.stopService(floatLogo);

	}

	private native void Close();
	static boolean getConfig(String key){
		SharedPreferences sp=ctx.getSharedPreferences("espValue",Context.MODE_PRIVATE);
		return  sp.getBoolean(key,false);
		// return !key.equals("");
	}
	@SuppressLint("WrongConstant")
	private void DrawCanvas() {

		final LayoutParams params = new LayoutParams(
			LayoutParams.MATCH_PARENT,
			LayoutParams.MATCH_PARENT,
			getLayoutType(),
			LayoutParams.FLAG_NOT_FOCUSABLE | LayoutParams.FLAG_NOT_TOUCHABLE | LayoutParams.FLAG_NOT_TOUCH_MODAL,

			PixelFormat.TRANSLUCENT);

		if(MainActivity.Hide_Recorder) RecorderFakeUtils.setFakeRecorderWindowLayoutParams(params);
		params.gravity = Gravity.TOP | Gravity.START;
		params.x = 0;
		params.y = 0;
		if (Build.VERSION.SDK_INT >= 28) {
			params.layoutInDisplayCutoutMode = 1;
		}
		windowManager.addView(overlayView, params);
	}
	public static native void DrawOn(ESPView espView, Canvas canvas);

	public int getLayoutType() {
		int LAYOUT_FLAG;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
			LAYOUT_FLAG = LayoutParams.TYPE_APPLICATION_OVERLAY;
		} else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			LAYOUT_FLAG = LayoutParams.TYPE_PHONE;
		} else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			LAYOUT_FLAG = LayoutParams.TYPE_PHONE;
		} else {
			LAYOUT_FLAG = LayoutParams.TYPE_SYSTEM_ALERT;
		}
		return LAYOUT_FLAG;
	}
	public void Shell(String str) {

		DataOutputStream dataOutputStream = null;
		try {
			process = Runtime.getRuntime().exec(str);
		} catch (IOException e) {
			e.printStackTrace();
			process = null;
		}
		if (process != null) {
			dataOutputStream = new DataOutputStream(process.getOutputStream());
		}
		try {
			dataOutputStream.flush();
		} catch (IOException e2) {
			e2.printStackTrace();
		}
		try {
			process.waitFor();
		} catch (InterruptedException e3) {
			e3.printStackTrace();
		}
	}

}


