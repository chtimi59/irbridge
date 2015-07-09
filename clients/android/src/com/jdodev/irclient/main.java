package com.jdodev.irclient;

import java.util.Date;

import com.jdodev.ircodec.IRBuffer;
import com.jdodev.ircodec.IRcodec;

import android.app.Activity;
import android.content.SharedPreferences;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.ToggleButton;

public class main extends Activity implements TCPClient.Listener, SensorEventListener  {
	
	public final static String TAG = "IRCLIENT";
	private main mCtx = null;

	private SensorManager mSensorManager=null;
	
	private Button       mStartStopBtn = null;
	private EditText     mEditText1 = null;
	
	private TextView     mTxtInformation = null;
	private ToggleButton mToggleAngleMeasurement = null;
	private ToggleButton mToggleLedSwitch = null;

	private TCPClient mTCP = null;
	private SharedPreferences mPrefs = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mCtx = this;
        
        mStartStopBtn = (Button)findViewById(R.id.toggleButton1);
        mEditText1   = (EditText)findViewById(R.id.editText1);
        mTxtInformation = (TextView) findViewById(R.id.textView1);
        mToggleAngleMeasurement = (ToggleButton)findViewById(R.id.toggleAngleMeasurement);
        mToggleLedSwitch = (ToggleButton)findViewById(R.id.buttonLedSwitch);
        mTxtInformation.setBackgroundColor(0x0);
		mTxtInformation.setTextColor(0xffffffff);
		mTxtInformation.setText("Off line");
		
		mSensorManager=(SensorManager)getSystemService(SENSOR_SERVICE);
		
		mPrefs = this.getSharedPreferences("com.jdodev.irclient", 0);
		mEditText1.setText(mPrefs.getString("SRVADDR", "192.168.2.13"));
		mEditText1.addTextChangedListener(new TextWatcher() {
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after) { }
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) { 
				String str = s.toString().trim();
				SharedPreferences.Editor editor = mPrefs.edit();
				editor.putString("SRVADDR", str);
				editor.commit();
				editor.apply();
			}
			@Override
			public void afterTextChanged(Editable s) { }			
		});
		
		mStartStopBtn.setOnClickListener(new OnClickListener() {
	        	@Override
				public void onClick(View v) {
	        		if (mTCP == null) {        			
	        			mStartStopBtn.setText("DISCONNECT");
	        			mTCP = new TCPClient(mCtx, mEditText1.getText().toString().trim(), mCtx);
	        			mTCP.start();
	        		} else {
	        			tcpstop() ;
	        		}
				}
		});
		
		// LED
		mToggleLedSwitch.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if (mTCP == null) return;
				int[] LEDON  =  new int[] { 0x00, 0xF7, 0xC0, 0x3F };
				int[] LEDOFF =  new int[] { 0x00, 0xF7, 0x40, 0xBF };
				String bits = IRcodec.Bytes2String(!mToggleLedSwitch.isChecked()?LEDOFF:LEDON, 32);
				Log.d(TAG, "send "+bits.length()+" bits!");
				byte[] ir = IRcodec.NecEncode(bits);					
				mTCP.send(ir);
			}
        });
        
		
		// ANGLE MEASURMENT
		mToggleAngleMeasurement.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				sensor(mToggleAngleMeasurement.isChecked());
			}
        });

  
    }
	
	private void tcpstop() {
		mStartStopBtn.setText("CONNECT");
		if (mTCP!=null) mTCP.shutdown();
	}

	@Override
	public void onBackPressed() {
		this.finish();
	    super.onBackPressed();
	}
	
	@Override
	public void onDestroy() {
		tcpstop();
	    super.onDestroy();
	}

	
	@Override
	public void OnThreadStart() {
		mTxtInformation.setBackgroundColor(0xffffffff);
		mTxtInformation.setTextColor(0xff000000);
		mTxtInformation.setText("...Kg");
		IRcodec.Unkn0DecodeReset();
		IRcodec.NecDecodeReset();
	}

	@Override
	public void OnThreadEnd() {
		tcpstop();
		mTCP = null;
		mTxtInformation.setBackgroundColor(0xff000000);
		mTxtInformation.setTextColor(0xffffffff);
		mTxtInformation.setText("Off line");
	}
	
	@Override
	public void OnDataReceived(byte[] data, int sz) {
		IRBuffer ir = new IRBuffer();
		ir.buffer = data;
		ir.len = sz;		
		do {
			if (IRcodec.Unkn0Decode(ir)==1)
			{				
				String decodedbits = IRcodec.Unkn0DecodedBuff();
				//Log.d(TAG, "received "+decodedbits.length()+" bits!");
				int[] decoded = IRcodec.String2Bytes(decodedbits);				
				if(decodedbits.length()==39) {					
					if (decoded[0]==0x54)
					{		
						int trail  = (~decoded[0]) & 0xFF;
						int head   = (~decoded[1]) & 0xFF;
						int msb    = (~decoded[2]) & 0xFF;
						int lsb    = (~decoded[3]) & 0xFF;
						int check  = (~decoded[4]) & 0xFF;
						
						float weight = (float)((msb<<8) | lsb)/10;
						
						int sum = trail+head+msb+lsb;
						int sumFF = (sum & 0xFF) | 0x1;
						int remain = (sum - sumFF);
						if (remain>=256) sumFF += 2;						

						if (sumFF==check) {							
							//Log.i(TAG, ""+trail+" "+head+" "+msb+" "+lsb+" "+check+" -- "+sumFF+"/"+remain+" "+weight);
							if ((head & 0x80)==0x80) {
								mTxtInformation.setTextColor(0xff000000);
							} else {
								mTxtInformation.setTextColor(0xff00ff00);
							}
							mTxtInformation.setText(""+weight+"Kg");
						} else {
							Log.e(TAG, ""+trail+" "+head+" "+msb+" "+lsb+" "+check+" -- "+sumFF+"/"+remain+" "+weight);
						}
												
					}
				}				
				IRcodec.Unkn0DecodeReset();
			}
		} while(ir.available());		
	}

	private byte[] createFigure(int v) {
		int trail  = 171;
		int head   = 128;
		int msb    = (v >> 8) & 0xFF;
		int lsb    = (v >> 0) & 0xFF;

		int sum = trail+head+msb+lsb;
		int sumFF = (sum & 0xFF) | 0x1;
		int remain = (sum - sumFF);
		if (remain>=256) sumFF += 2;
		
		int check  = sumFF;
	
		int[] data = new int[5];
		data[0] = (~trail) & 0xFF;
		data[1] = (~head) & 0xFF;
		data[2] = (~msb) & 0xFF;
		data[3] = (~lsb) & 0xFF;
		data[4] = (~check) & 0xFF;
		
		String bits = IRcodec.Bytes2String(data, 39);
		byte[] ir = IRcodec.Unkn0Encode(bits);	
		return ir;
	}

	float[] aValues = null;
	float[] mValues = null;
	Date    mLastDate = null;
	
	@Override
	public void onSensorChanged(SensorEvent event)
	{
		switch (event.sensor.getType ()){
            case Sensor.TYPE_ACCELEROMETER:
                aValues = event.values.clone ();
                break;
            case Sensor.TYPE_MAGNETIC_FIELD:
                mValues = event.values.clone ();
                break;
        }
		
		if (aValues==null) return;
		if (mValues==null) return;
		
        float[] R = new float[16];
        float[] orientationValues = new float[3];

        SensorManager.getRotationMatrix (R, null, aValues, mValues);
        SensorManager.getOrientation (R, orientationValues);
        int angle = Math.abs((int)(Math.toDegrees (orientationValues[1])*10));
        
        Date date = new Date();
        if (mLastDate != null) {
        	long diffms = date.getTime() - mLastDate.getTime();
        	if (diffms>500) mLastDate = null;
        }
        
        if (mLastDate == null) {
        	mLastDate = date;
        	Log.d(TAG, " "+((float)angle)/10);
			if (mTCP != null) mTCP.send(createFigure(angle));
        }
	}

	private void sensor(boolean on) {
		mLastDate = null;
		if (on) {
			mSensorManager.registerListener(this,  mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
			mSensorManager.registerListener(this,  mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD), SensorManager.SENSOR_DELAY_NORMAL);
		} else {
			mSensorManager.unregisterListener(this);
		}
	}
	
	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) { }
}
