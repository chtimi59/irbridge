package com.jdodev.ircodec_utest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class Main extends Activity {

	private final String TAG = "IRCODEC_UTEST";
			
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d(TAG, "hi!");
	}
}
