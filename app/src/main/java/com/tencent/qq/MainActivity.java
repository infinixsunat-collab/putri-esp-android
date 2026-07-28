package com.tencent.qq;
import java.io.File;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

public class MainActivity extends Activity { 

    private static native boolean IsVerified();

    static {
        System.loadLibrary("Putri");
    }

	public static int REQUEST_OVERLAY_PERMISSION = 5469;
	static int gameType=1;
    static boolean vercheck=true;
    private ProgressBar progressBar;
    private TextView modeexc;
    private ImageView logo;
	public String daemonPath;
	public static String socket;
    public static boolean Hide_Recorder = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // LAYER 2 VERIFICATION: Must pass native auth check
        if (!IsVerified()) {
            // DEX bypass detected or session invalid
            new AlertDialog.Builder(this, 5)
                .setTitle("Security Error")
                .setMessage("Authentication verification failed.\nPlease re-login.")
                .setCancelable(false)
                .setPositiveButton("Exit", (d, w) -> finishAffinity())
                .show();
            return;
        }
        
        setContentView(R.layout.main);
        loadMain();
		permissionWindows();
		modeexc = findViewById(R.id.modeexc);        
        TextView dev = findViewById(R.id.dev);
        dev.setText("—͟͞͞  Putri —͟͞͞");
        TextView txtstart=findViewById(R.id.txtstart);
        TextView txtstop=findViewById(R.id.txtstop);
        txtstart.setTypeface(Typeface.createFromAsset(getAssets(), "fonts/font1.ttf"));
        txtstop.setTypeface(Typeface.createFromAsset(getAssets(), "fonts/font1.ttf"));


        //Navigation
		final LinearLayout hackmenu = findViewById(R.id.hackmenu);
    
		hackmenu.setVisibility(View.VISIBLE);

        //ToolsMenu
        final LinearLayout listgl = findViewById(R.id.listgl);

        final ImageView icrec=findViewById(R.id.icrec);
        icrec.setBackgroundResource(R.drawable.ic_rec);

        final Switch hiderec = findViewById(R.id.hiderec);
        hiderec.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener(){
                @Override
                public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                    if (isChecked) {
                        Hide_Recorder =  true;
                        icrec.setBackgroundResource(R.drawable.ic_rec_on);
                    } else {
                        Hide_Recorder= false;
                        icrec.setBackgroundResource(R.drawable.ic_rec);
                    }
                }
            });

        listgl.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    listgl.setVisibility(View.VISIBLE);                            
				}
			});

		if (isRootGiven()) {
            modeexc.setText("Root Mode");
		} else {
            modeexc.setText("Virtual Mode");
		}

        final TextView vertext = findViewById(R.id.vertext);
        final TextView verandro = findViewById(R.id.verandro);

        vertext.setText(Build.MODEL);
        verandro.setText("Android " + Build.VERSION.RELEASE);
        LinearLayout telegram = findViewById(R.id.telegram);

        telegram.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    GoToTelegram();
                }
			});

		LinearLayout startButton = findViewById(R.id.startButton);
		startButton.setOnClickListener(new OnClickListener(){
				@Override
				public void onClick(View v) {
                    if (vercheck) {
                        if (isRootGiven()) {
                            if (gameType == 1) {
                                final ProgressDialog progressDialog = new ProgressDialog(MainActivity.this); 
                                final Handler handles = new Handler() { 
                                    @Override 
                                    public void handleMessage(Message msg) {
                                        super.handleMessage(msg); 
                                        progressDialog.incrementProgressBy(1); 
                                    } 
                                };                 
                                progressDialog.setMessage("Please Wait..."); 
                                progressDialog.show(); 
                                new Thread(new Runnable() { 
                                        @Override public void run() { 
                                            try { 
                                                while (progressDialog.getProgress() <= progressDialog.getMax()) { 
                                                    Thread.sleep(200); 

                                                    progressDialog.dismiss(); 

                                                } 
                                            } catch (Exception e) {
                                                e.printStackTrace(); 
                                            } 
                                        } 
                                    }).start();                          

                            } 
                        }
						Intent launchIntent = getPackageManager().getLaunchIntentForPackage("");
						if (launchIntent != null) {
							startActivity(launchIntent);}
						startFloating();     
                    } else {
                        Toast.makeText(MainActivity.this, "Please Select Version First !!", Toast.LENGTH_LONG).show();
                    }
				}
			});

		LinearLayout stopButton = findViewById(R.id.stopButton);
		stopButton.setOnClickListener(new OnClickListener(){
				@Override
				public void onClick(View p1) {         
					stopFloating();
				}
			});

    }

    private void GoToTelegram() {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://t.me/PModsIndonesia"));
        startActivity(browserIntent);
    }

	private void startFloating() {
		startService(new Intent(this, FloatLogo.class));
	}

	private void stopFloating() {
        stopService(new Intent(this, Overlay.class));
		stopService(new Intent(this, FloatLogo.class));
	}

	private void loadMain() {
        MoveAssets(getFilesDir() + "/", "CRASH");  
		//Move Your CPP From Assets To Injector Dir
	}

	private void permissionWindows() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(this)) {
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
				builder.setMessage("This application requires window overlays access permission, please allow first.");
				builder.setPositiveButton("OK", new DialogInterface.OnClickListener(){
						@Override
						public void onClick(DialogInterface p1, int p2) {
							Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
													   Uri.parse("package:" + getPackageName()));
                            startActivityForResult(intent, REQUEST_OVERLAY_PERMISSION);
						}
                    });
				builder.setCancelable(false);
				builder.show();
            }
        }
	}

    public static boolean isRootAvailable() {
        for (String pathDir : System.getenv("PATH").split(":")) {
            if (new File(pathDir, "su").exists()) {
                return true;
            }
        }
        return false;
    }

    public static boolean isRootGiven() {
        if (isRootAvailable()) {
            Process process = null;
            try {
                process = Runtime.getRuntime().exec(new String[]{"su", "-c", "id"});
                BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String output = in.readLine();
                if (output != null && output.toLowerCase().contains("uid=0"))
                    return true;
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (process != null)
                    process.destroy();
            }
        }

        return false;
    }

    public void Execute(String path) {
        try {
            ExecuteElf("chmod 777 " + getFilesDir() + path);//VIRTUAL
            ExecuteElf(getFilesDir() + path);
            ExecuteElf("su -c chmod 777 " + getFilesDir() + path);//ROOT
            ExecuteElf("su -c " + getFilesDir() + path);
        } catch (Exception e) {
        }
    }

    private void ExecuteElf(String shell) {
        try {
            Runtime.getRuntime().exec(shell, null, null);
        } catch (Exception e) {
            e.printStackTrace();
        }
	}

    private boolean MoveAssets(String outPath, String fileName) {
        File file = new File(outPath);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                Log.e("--Method--", "copyAssetsSingleFile: cannot create directory.");
                return false;
            }
        }
        try {
            InputStream inputStream = getAssets().open(fileName);
            File outFile = new File(file, fileName);
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = inputStream.read(buffer))) {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            inputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		finishAffinity();
    }
} 

