package com.jdodev.irclient;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ToggleButton;

public class main extends Activity implements TCPClient.Listener {
	
	public final static String TAG = "IRCLIENT";
	private main mCtx = null;
	
	private TextView mTxtInformation = null;
	private ToggleButton mStartStopBtn = null;
	private TCPClient mTCP = null;
	private ToggleButton mToggleAngleMeasurement = null;
	private Button mButtonLedSwitch = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mCtx = this;
                
        mTxtInformation = (TextView) findViewById(R.id.textView1);
        mStartStopBtn = (ToggleButton)findViewById(R.id.toggleButton1);
        mToggleAngleMeasurement = (ToggleButton)findViewById(R.id.toggleAngleMeasurement);
        mButtonLedSwitch = (Button)findViewById(R.id.buttonLedSwitch);
        
        mButtonLedSwitch.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if (mTCP != null) {
					int[] data =  new int[] { 0xFF, 0xF7, 0xC0, 0x3F};
					mTCP.send(IRcodec.int2byte(data));
				}
			}
        });
        
        mStartStopBtn.setOnClickListener(new OnClickListener() {
        	@Override
			public void onClick(View v) {
        		if (mStartStopBtn.isChecked()) {
        			if (mTCP == null) {
        				mTCP = new TCPClient(mCtx, "192.168.2.13", mCtx);
        				mTCP.start();
        			}
        		} else {
        			if (mTCP!=null) {
        				mTxtInformation.setText("Disconnecting...");
        				mTCP.shutdown();
        				mStartStopBtn.setChecked(true);
        			}
        		}
			}
		});
      
        
        
    }
	
	private void tcpstop() {
		mTxtInformation.setText("Off line");
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
		mTxtInformation.setText("Wait...");
	}

	@Override
	public void OnThreadEnd() {
		tcpstop();
		mTCP = null;
		mStartStopBtn.setChecked(false);
	}
	
	@Override
	public void OnDataReceived(byte[] data, int sz) {
		mTxtInformation.setText("data received");
	}

	
}
