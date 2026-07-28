package com.tencent.qq;

import android.app.*;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.*;
import android.net.Uri;
import android.os.*;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.LinearLayout;

public class LoginActivity extends Activity {

    static {
        System.loadLibrary("Putri");
    }

    private static native void Init(Context mContext);

    private static native String Check(Context mContext, String userKey);

    private static Prefs prefs;

    private void setLightStatusBar(Activity activity) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            activity.getWindow().setStatusBarColor(Color.parseColor("#FF121212"));
            activity.getWindow().setNavigationBarColor(Color.parseColor("#FF121212"));
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setLightStatusBar(this);
        setContentView(R.layout.activity_login);
        
        TryLoginPHP(this);

        prefs = Prefs.with(this);
		
        TextView getKey = findViewById(R.id.getKey);
        getKey.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setData(Uri.parse("https://t.me/IAM_Putri"));
                startActivity(intent);
            }
        });
    }

    private void TryLoginPHP(final Context context) {
        final EditText mail = findViewById(R.id.textUsername);
        LinearLayout init = findViewById(R.id.btnSignIn);
        final String USER = "USER_KEY";
        prefs = Prefs.with(this);
        mail.setText(prefs.read(USER, ""));
        init.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                if (!mail.getText().toString().isEmpty()) {
                    prefs.write(USER, mail.getText().toString());
                    String username = mail.getText().toString().trim();
                    Login(context, username);
                }
                if (mail.getText().toString().isEmpty()) {
                    mail.setError("Please enter username");
                }
                if (mail.getText().toString().isEmpty()) {
                    mail.setError("Please enter username");
                }
            }
        });
    }

    static void StartFloatingService(Context context) {
        Intent intent = new Intent(context.getApplicationContext(), MainActivity.class);
        context.startActivity(intent);
    }

    private static void Login(final Context m_Context, final String userKey) {
        final ProgressDialog progressDialog = new ProgressDialog(m_Context, 5);
        progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        progressDialog.setMessage("Please wait...");
        progressDialog.setCancelable(false);
        progressDialog.show();

        final Handler loginHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 0) {
                    prefs.write("USER_KEY", userKey);
                    LoginActivity.StartFloatingService(m_Context);
                } else if (msg.what == 1) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(m_Context, 5);
                    builder.setTitle("key - Error");
                    builder.setMessage(msg.obj.toString());
                    builder.setCancelable(false);
                    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            prefs.remove("USER_KEY");
                            prefs.clear();
                        }
                    });
                    builder.show();
                }
                progressDialog.dismiss();
            }
        };

        new Thread(new Runnable() {
            @Override
            public void run() {
                String result = Check(m_Context, userKey);
                if (result.equals("OK")) {
                    loginHandler.sendEmptyMessage(0);
                } else {
                    Message msg = new Message();
                    msg.what = 1;
                    msg.obj = result;
                    loginHandler.sendMessage(msg);
                }
            }
        }).start();
    }

}

