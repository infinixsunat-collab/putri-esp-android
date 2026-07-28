package com.tencent.qq;

import android.content.ClipboardManager;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.icu.text.SimpleDateFormat;
import android.media.MediaPlayer;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.sql.Timestamp;
import java.util.Random;

public class Util {


    private static MediaPlayer mMediaPlayer;
	
    public static boolean isRoot() {
        try {
            Process process = Runtime.getRuntime().exec("su");
            OutputStream os = process.getOutputStream();
            os.write("\n".getBytes());
            os.flush();
            os.write("exit".getBytes());
            os.flush();
            os.write("\n".getBytes());
            os.flush();
            int exitValue = process.waitFor();
            os.close();
            Runtime.getRuntime().exec("su -c setenforce 0", null, null);
            return exitValue == 0;
        } catch (Exception e) {
            return false;
        }
    }

    private static void load(Context context, File f, int a, String path) {
        for (int i = 1; i < a; i++) {
            File newFile = new File(f.getPath() + "/" + path + (i) + ".png");
            if (!newFile.exists()) {
                try {
                    InputStream in = context.getAssets().open("image/" + path + (i) + ".png");
                    FileOutputStream out = new FileOutputStream(newFile);
                    byte[] bs = new byte[1 * 1024 * 1024];
                    int count = 0;
                    while ((count = in.read(bs)) != -1) {
                        out.write(bs, 0, count);
                    }
                    out.flush();
                    in.close();
                    out.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    
    public static String getFileMd5(File file) {
        MessageDigest digest = null;
        FileInputStream fis = null;
        byte[] buffer = new byte[1024];
        try {
            if (!file.isFile()) {
                return "";
            }
            digest = MessageDigest.getInstance("MD5");
            fis = new FileInputStream(file);
            while (true) {
                int len;
                if ((len = fis.read(buffer, 0, 1024)) == -1) {
                    fis.close();
                    break;
                }
                digest.update(buffer, 0, len);
            }
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        BigInteger var5 = new BigInteger(1, digest.digest());
        return String.format("%1$032x", new Object[]{var5});
    }

    public static String getBinData() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 1024; i++) {
            String temp = Integer.toHexString(new Random().nextInt(255) & 0xFF);
            sb.append("\\x");
            if (temp.length() == 1) {
                sb.append("0");
            }
            sb.append(temp);
        }
        return sb.toString();
    }

    private static String getMd5(String text) throws NoSuchAlgorithmException, UnsupportedEncodingException {
        MessageDigest md5 = MessageDigest.getInstance("MD5");
        byte[] bytes = md5.digest(text.getBytes(StandardCharsets.UTF_8));
        StringBuilder builder = new StringBuilder();
        for (byte aByte : bytes) {
            builder.append(Integer.toHexString((0x000000FF & aByte) | 0xFFFFFF00).substring(6));
        }
        return builder.toString();
    }

    public static String getAndroidID() {
        String[] paths = new String[]{
			"/data/local/tmp/.system.r350.x29",
			"/data/app/install_check",
			"/data/data/android/code_cache/pmx32.dat"
        };
        StringBuilder sb = new StringBuilder();
        for (String path : paths) {
            File f = new File(path);
            if (!f.exists()) {
                Util.runShell("echo -e -n \"" + getBinData() + "\" > " + path + "\n" + "chmod 644 " + path, true);
            }
            sb.append(getFileMd5(f)).append("&");
        }
        try {
            return getMd5(sb.toString());
        } catch (NoSuchAlgorithmException | UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static byte[] runShell(String command, boolean isRoot) {
        try {
            Process process = Runtime.getRuntime().exec(isRoot ? "su" : "sh");
            InputStream ins = process.getInputStream();
            InputStream es = process.getErrorStream();
            OutputStream ous = process.getOutputStream();
            ous.write("\n".getBytes());
            ous.flush();
            ous.write(command.getBytes());
            ous.flush();
            ous.write("\n".getBytes());
            ous.flush();
            ous.write("exit".getBytes());
            ous.flush();
            ous.write("\n".getBytes());
            ous.flush();
            byte[] result = readInputStream(ins, false);
            byte[] error = readInputStream(es, false);
            process.waitFor();
            ins.close();
            es.close();
            ous.close();
            if (new String(error).trim().isEmpty()) {
                return result;
            } else {
                return ("Shell Result : \n" + new String(result) + "\n" + "Shell Error : \n" + new String(error) + "\n").getBytes();
            }
        } catch (Throwable th) {
            return ("Application Error : \n" + Log.getStackTraceString(th)).getBytes();
        }
    }

    public static byte[] readInputStream(InputStream ins, boolean close) {
        try {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            int i = -1;
            byte[] buf = new byte[1024];
            while ((i = ins.read(buf)) != -1) {
                bos.write(buf, 0, i);
            }
            if (close) {
                ins.close();
                bos.close();
            }
            return bos.toByteArray();
        } catch (Throwable th) {
            return Log.getStackTraceString(th).getBytes();
        }
    }

    /**
     * 停止播放逻辑
     */
    public static void stopPlay() {
        //释放播放器
        if (mMediaPlayer != null) {
            //充值监听器，防止内存泄漏
            mMediaPlayer.setOnCompletionListener(null);
            mMediaPlayer.setOnErrorListener(null);
            mMediaPlayer.stop();
            mMediaPlayer.reset();
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
    }

    public static void copyFileUsingFileStreams(File source, File dest)
	throws IOException {
        InputStream input = null;
        OutputStream output = null;
        try {
            input = new FileInputStream(source);
            output = new FileOutputStream(dest);
            byte[] buf = new byte[1024];
            int bytesRead;
            while ((bytesRead = input.read(buf)) != -1) {
                output.write(buf, 0, bytesRead);
            }
        } finally {
            input.close();
            output.close();
        }
    }

    public static Bitmap compressImage(Bitmap image, int size, int options) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        // 质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
        image.compress(Bitmap.CompressFormat.JPEG, 80, baos);
        // 循环判断如果压缩后图片是否大于100kb,大于继续压缩
        while (baos.toByteArray().length / 1024 > size) {
            options -= 10;// 每次都减少10
            baos.reset();// 重置baos即清空baos
            // 这里压缩options%，把压缩后的数据存放到baos中
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);
        }
        // 把压缩后的数据baos存放到ByteArrayInputStream中
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());
        // 把ByteArrayInputStream数据生成图片
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);
        return bitmap;
    }
}


