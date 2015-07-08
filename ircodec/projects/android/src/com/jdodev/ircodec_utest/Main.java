package com.jdodev.ircodec_utest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import com.jdodev.ircodec.IRcodec;

public class Main extends Activity {

	private final String TAG = "IRCODEC_UTEST";
			
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d(TAG, "hi!");		
		IRcodec t = new IRcodec();
		Log.d(TAG, t.stringFromJNI());		
	}
}
