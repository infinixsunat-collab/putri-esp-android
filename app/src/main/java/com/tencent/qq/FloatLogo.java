package com.tencent.qq;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.os.IBinder;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import static java.lang.System.exit;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.FileOutputStream;
import android.annotation.SuppressLint;

public class FloatLogo extends Service implements View.OnClickListener {

    private View mainView;
	private WindowManager mWindowManager;
	private static boolean esp=false;
	private static boolean crash=false;
	public static String socket;
	public String daemonPath;
    private static FloatLogo Instance;
    
    static {
        System.loadLibrary("Putri");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    View espView, logoView;


    @Override
    public void onCreate() {
        super.onCreate();
        Instance=this;
		loadAssets(); 
        createOver();
        logoView = mainView.findViewById(R.id.relativeLayoutParent);
        espView = mainView.findViewById(R.id.espView);
        Init();
    }

    void createOver(){
        //getting the widget layout from xml using layout inflater
        mainView = LayoutInflater.from(this).inflate(R.layout.float_logo, null);
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
        }
        //setting the layout parameters
        final WindowManager.LayoutParams params = new WindowManager.LayoutParams(
			WindowManager.LayoutParams.WRAP_CONTENT,
			WindowManager.LayoutParams.WRAP_CONTENT,
			LAYOUT_FLAG,
			WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
			PixelFormat.RGBA_8888);
        //getting windows services and adding the floating view to it
        mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
		if (MainActivity.Hide_Recorder) {
			RecorderFakeUtils.setFakeRecorderWindowLayoutParams(params);
		}
        mWindowManager.addView(mainView, params);

        final GestureDetector gestureDetector = new GestureDetector(this, new SingleTapConfirm());
        //window funclion
        TextView closeBtn=mainView.findViewById(R.id.closeBtn);
        closeBtn.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					espView.setVisibility(View.GONE);
					logoView.setVisibility(View.VISIBLE);
				}
			});

		final GradientDrawable shape =  new GradientDrawable();
		shape.setCornerRadii(new float[] { 18, 18, 18, 18, 0, 0, 0, 0});
		shape.setColor(Color.parseColor("#802196F3"));
		
		final GradientDrawable shape1 =  new GradientDrawable();
		shape1.setCornerRadii(new float[] { 18, 18, 18, 18, 0, 0, 0, 0});
		shape1.setColor(Color.parseColor("#FFD1D1D1"));
		
		final GradientDrawable shape2 =  new GradientDrawable();
		shape2.setCornerRadii(new float[] { 18, 18, 18, 18, 0, 0, 0, 0});
		shape2.setColor(Color.parseColor("#FFD1D1D1"));
		
		GradientDrawable c1 =  new GradientDrawable();	
		c1.setCornerRadii(new float[] { 18, 18, 18, 18, 0, 0, 0, 0 });
		c1.setColor(Color.parseColor("#802196F3"));
		mainView.findViewById(R.id.menu_1).setBackground(c1);
	
		GradientDrawable c3 =  new GradientDrawable();	
		c3.setCornerRadii(new float[] { 18, 18, 18, 18, 0, 0, 0, 0 });
		c3.setColor(Color.parseColor("#FFD1D1D1"));
		mainView.findViewById(R.id.menu_3).setBackground(c3);
		
        final LinearLayout player = mainView.findViewById(R.id.players);
        final LinearLayout settings = mainView.findViewById(R.id.settings);

		final TextView menu_1 = mainView.findViewById(R.id.menu_1);
		
		final TextView menu_3 = mainView.findViewById(R.id.menu_3);

        menu_1.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					menu_1.setBackground(shape);
			
					menu_3.setBackground(shape2);

					player.setVisibility(View.VISIBLE);
					
					settings.setVisibility(View.GONE);

				}
			});

        

        menu_3.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					menu_1.setBackground(shape1);
				
					menu_3.setBackground(shape);

					player.setVisibility(View.GONE);
					
					settings.setVisibility(View.VISIBLE);

				}
			});


        //floating window setting
        mainView.findViewById(R.id.relativeLayoutParent).setOnTouchListener(new View.OnTouchListener() {
				private int initialX;
				private int initialY;
				private float initialTouchX;
				private float initialTouchY;

				public boolean onTouch(View v, MotionEvent event) {
					if (gestureDetector.onTouchEvent(event)) {
						espView.setVisibility(View.VISIBLE);
						logoView.setVisibility(View.GONE);
						return true;
					} else {
						switch (event.getAction()) {
							case MotionEvent.ACTION_DOWN:
								initialX = params.x;
								initialY = params.y;
								initialTouchX = event.getRawX();
								initialTouchY = event.getRawY();
								return true;
							case MotionEvent.ACTION_MOVE:
								//this code is helping the widget to move around the screen with fingers
								params.x = initialX + (int) (event.getRawX() - initialTouchX);
								params.y = initialY + (int) (event.getRawY() - initialTouchY);
								//	RecorderFakeUtils.setFakeRecorderWindowLayoutParams(params);
								mWindowManager.updateViewLayout(mainView, params);
								return true;
						}
						return false;
					}
				}
			});
    }

    public void onDestroy() {
        super.onDestroy();
        if (mainView != null) mWindowManager.removeView(mainView);
    }

 
    private void  setValue(String key,boolean b) {
        SharedPreferences sp=this.getSharedPreferences("espValue",Context.MODE_PRIVATE);
        SharedPreferences.Editor ed= sp.edit();
        ed.putBoolean(key,b);
        ed.apply();
    }

    boolean getConfig(String key){
        SharedPreferences sp=this.getSharedPreferences("espValue",Context.MODE_PRIVATE);
        return  sp.getBoolean(key,false);
        // return !key.equals("");
    }

    void setFps(int fps){
        SharedPreferences sp=this.getSharedPreferences("espValue",Context.MODE_PRIVATE);
        SharedPreferences.Editor ed= sp.edit();
        ed.putInt("fps",fps);
        ed.apply();
    }
 
    int getFps(){
        SharedPreferences sp=this.getSharedPreferences("espValue",Context.MODE_PRIVATE);
        return sp.getInt("fps",1);
    }

    public void onClick(View v) {
    }
	
    public static void HideFloat() {
        if (Instance != null)
        {
            Instance.Hide();
        }
    }
	
    public void Hide(){
        stopSelf();
        exit(-1);
    }
	
	private void testes(){
		if (MainActivity.isRootGiven()) {
            socket = "su -c " + daemonPath;                               
        } else {
            socket = daemonPath;  
        }
		startService(new Intent(this, Overlay.class));
	}

	private void testes1(){
		stopService(new Intent(this, Overlay.class));
	}
	private void ExecuteElf(String shell) {
        String s = shell;
        try {
            Runtime.getRuntime().exec(s, null, null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
	public void RunCPP(String path) {
        try {
            ExecuteElf("chmod 777 " + getFilesDir() + path);
            ExecuteElf(getFilesDir() + path);
            ExecuteElf("su -c chmod 777 " + getFilesDir() + path);
            ExecuteElf("su -c " + getFilesDir() + path);
        } catch (Exception e) {
        }
    }
	public void loadAssets() {
        String pathf = getFilesDir().toString() + "/sock";
        try {
            OutputStream myOutput = new FileOutputStream(pathf);
            byte[] buffer = new byte[1024];
            int length;
            InputStream myInput = getAssets().open("sock");
            while ((length = myInput.read(buffer)) > 0) {
                myOutput.write(buffer, 0, length);
            }
            myInput.close();
            myOutput.flush();
            myOutput.close();
        } catch (IOException e) {
        }
        daemonPath = getFilesDir().toString() + "/sock";
        try {
            Runtime.getRuntime().exec("chmod 777 " + daemonPath);
        } catch (IOException e) {
        }
	}
    void Init() {
		final SeekBar hide = mainView.findViewById(R.id.hide);
        hide.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				logoView.setAlpha((progress /255.0f)+1);
			}
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
		});
        final Switch isBox = mainView.findViewById(R.id.isBox);
        isBox.setChecked(getConfig((String) isBox.getText()));
        SettingValue(6, getConfig((String) isBox.getText()));
        isBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(isBox.getText()), isBox.isChecked());
                SettingValue(6, isBox.isChecked());
            }
        });
        final Switch isLine = mainView.findViewById(R.id.isLine);
        isLine.setChecked(getConfig((String) isLine.getText()));
        SettingValue(5, getConfig((String) isLine.getText()));
        isLine.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(isLine.getText()), isLine.isChecked());
                SettingValue(5, isLine.isChecked());
            }
        });
        final Switch isBack = mainView.findViewById(R.id.isBack);
        isBack.setChecked(getConfig((String) isBack.getText()));
        SettingValue(7, getConfig((String) isBack.getText()));
        isBack.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(isBack.getText()), isBack.isChecked());
                SettingValue(7, isBack.isChecked());
            }
        });

        final Switch isHealth = mainView.findViewById(R.id.isHealth);
        isHealth.setChecked(getConfig((String) isHealth.getText()));
        SettingValue(2, getConfig((String) isHealth.getText()));
        isHealth.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(isHealth.getText()), isHealth.isChecked());
                SettingValue(2, isHealth.isChecked());
            }
        });

        final Switch isName = mainView.findViewById(R.id.isName);
        isName.setChecked(getConfig((String) isName.getText()));
        SettingValue(1, getConfig((String) isName.getText()));
        isName.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(isName.getText()), isName.isChecked());
                SettingValue(1, isName.isChecked());
            }
        });
        final Switch isDist = mainView.findViewById(R.id.isDist);
        isDist.setChecked(getConfig((String) isDist.getText()));
        SettingValue(3, getConfig((String) isDist.getText()));
        isDist.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(isDist.getText()), isDist.isChecked());
                SettingValue(3, isDist.isChecked());
            }
        });

        final Switch skeleton = mainView.findViewById(R.id.skeleton);
        skeleton.setChecked(getConfig((String) skeleton.getText()));
        SettingValue(14, getConfig((String) skeleton.getText()));
        skeleton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(skeleton.getText()), skeleton.isChecked());
                SettingValue(14, skeleton.isChecked());
            }
        });
        
        final Switch Count = mainView.findViewById(R.id.Count);
        Count.setChecked(getConfig((String) Count.getText()));
        SettingValue(8, getConfig((String) Count.getText()));
        Count.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(Count.getText()), Count.isChecked());
                SettingValue(8, Count.isChecked());
            }
        });
		
        final Button startesp = mainView.findViewById(R.id.Count1); 
        startesp.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick(View v) {
                    if (!esp){
						testes();
                        esp=true;
						startesp.setText("TAP TO DISABLE ESP");
						startesp.setTextColor(Color.parseColor("#e3323b"));
                    } else {
						testes1();
                        stopService(new Intent(FloatLogo.this, Overlay.class));
						esp=false;
						startesp.setText("TAP TO ENABLE ESP");
						startesp.setTextColor(Color.parseColor("#32a852"));
                    }

                }

            });
		final Button switchcrash = mainView.findViewById(R.id.switchcrash);
		switchcrash.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick(View v) {
					if (!crash){
						RunCPP("/CRASH 20193");
						crash = true;
						switchcrash.setTextColor(Color.parseColor("#e3323b"));
					} else {
						RunCPP("/CRASH 28031");
						crash = false;
						switchcrash.setTextColor(Color.parseColor("#32a852"));
					}
				}
			});	
        final Switch Team = mainView.findViewById(R.id.Team);
        Team.setChecked(getConfig((String) Team.getText()));
        SettingValue(4, getConfig((String) Team.getText()));
        Team.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                setValue(String.valueOf(Team.getText()), Team.isChecked());
                SettingValue(4, Team.isChecked());
            }
        });
		
        final Switch TeamE = mainView.findViewById(R.id.TeamE);
        TeamE.setChecked(getConfig((String) TeamE.getText()));
        SettingValue(9, getConfig((String) TeamE.getText()));
        TeamE.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
				@Override
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					setValue(String.valueOf(Team.getText()), TeamE.isChecked());
					SettingValue(9, TeamE.isChecked());
				}
			});
		final Switch Drone = mainView.findViewById(R.id.Drone);
        Drone.setChecked(getConfig((String) Drone.getText()));
        SettingValue(15, getConfig((String) Drone.getText()));
        Drone.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
				@Override
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					setValue(String.valueOf(Drone.getText()), Drone.isChecked());
					SettingValue(15, Drone.isChecked());
				}
			});
		final Switch Radar = mainView.findViewById(R.id.Radar);
        Radar.setChecked(getConfig((String) Radar.getText()));
        SettingValue(16, getConfig((String) Radar.getText()));
        Radar.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
				@Override
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					setValue(String.valueOf(Radar.getText()), Radar.isChecked());
					SettingValue(16, Radar.isChecked());
				}
			});
			
		final SeekBar droneH = mainView.findViewById(R.id.droneH);
        droneH.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onProgressChanged(SeekBar seekBar, int i, boolean b) {				
					Drones(droneH.getProgress());
					
				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {				
				}

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {

				}
			});
			
		final SeekBar Minipos = mainView.findViewById(R.id.MiniPos);
        Minipos.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onProgressChanged(SeekBar seekBar, int i, boolean b) {				
					MiniP(Minipos.getProgress());

				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {				
				}

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {

				}
			});
			
			
		final SeekBar MiniSize = mainView.findViewById(R.id.MiniSize);
        MiniSize.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onProgressChanged(SeekBar seekBar, int i, boolean b) {				
					MiniS(MiniSize.getProgress());

				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {				
				}

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {

				}
			});
		
		final SeekBar IconSize = mainView.findViewById(R.id.IconSize);
        IconSize.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onProgressChanged(SeekBar seekBar, int i, boolean b) {				
					IconS(IconSize.getProgress());

				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {				
				}

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {

				}
			});
		final RadioGroup rgFps=mainView.findViewById(R.id.rgFps);
		int chkdIdfps=rgFps.getCheckedRadioButtonId();
		RadioButton btnfps=mainView.findViewById(chkdIdfps);
		ESPView.ChangeFps(Integer.parseInt(btnfps.getTag().toString()));
        rgFps.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
				@Override
				public void onCheckedChanged(RadioGroup radioGroup, int i) {
					int chkdIdfps=rgFps.getCheckedRadioButtonId();
					RadioButton btnfps=mainView.findViewById(chkdIdfps);
					ESPView.ChangeFps(Integer.parseInt(btnfps.getTag().toString()));
				}
			});	
    }
	public static native void Drones(int dronee);
	public static native void MiniP(int minip);
	public static native void MiniS(int minis);
	public static native void IconS(int icns);
    public native void SettingValue(int setting_code, boolean value);
}

class SingleTapConfirm extends GestureDetector.SimpleOnGestureListener {

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        return true;
    }
}


