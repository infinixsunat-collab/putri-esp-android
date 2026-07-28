package com.tencent.qq;

public class Shellll {
	private Thread executeThread;

	public static boolean rootAccess() {
		new Thread(new Runnable(){
				@Override
				public void run() {
					try {
						Runtime.getRuntime().exec("su");
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}).start();
		return true;
	}
	
	public static void requestRoot(){
		new Thread(new Runnable(){
				@Override
				public void run() {
					try {
						Runtime.getRuntime().exec("su");
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}).start();
	}
	public Shellll (final CharSequence command){
		this.executeThread = new Thread(new Runnable(){
				@Override
				public void run() {
					try {
						Runtime.getRuntime().exec("su -c " + command, null, null);
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			});
	}

    public static Shellll command(CharSequence command){
		return new Shellll(command);
	}

	public void start(){
		executeThread.start();
	}
}

