package com.jdodev.irclient;

import com.jdodev.ircodec.IRBuffer;
import com.jdodev.ircodec.IRcodec;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
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
					String bits = IRcodec.Bytes2String(data, 32);
					Log.d(TAG, "send "+bits.length()+" bits!");
					byte[] ir = IRcodec.NecEncode(bits);					
					mTCP.send(ir);
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
		IRcodec.Unkn0DecodeReset();
		IRcodec.NecDecodeReset();
	}

	@Override
	public void OnThreadEnd() {
		tcpstop();
		mTCP = null;
		mStartStopBtn.setChecked(false);
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
				Log.d(TAG, "received "+decodedbits.length()+" bits!");
				int[] decoded = IRcodec.String2Bytes(decodedbits);				
				if(decodedbits.length()==39 && decoded[0]==0xAB) {
					if (decoded[0]==0xAB) {
						float weight = (float)(decoded[2]<<8 | decoded[3])/10;
						mTxtInformation.setText(""+weight+"Kg");
					}
				}				
				IRcodec.Unkn0DecodeReset();
			}
		} while(ir.available());		
	}

	
}
