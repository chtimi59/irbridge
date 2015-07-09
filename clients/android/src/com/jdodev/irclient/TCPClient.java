package com.jdodev.irclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Arrays;

import android.app.Activity;
import android.util.Log;


public class TCPClient extends Thread
{
	public final static String TAG = "TCPCLIENT";
	
	public final static int TCP_PORT = 8000;
	public final static int BUFF_SIZE = 1024*1024;
	
	private String mServerAddr;
	
	public interface Listener {
		public void OnThreadStart();
		public void OnDataReceived(byte[] data, int sz);
		public void OnThreadEnd();
	}
	
	private Activity mCtx = null;
	private Listener mListener = null;
	private volatile boolean done = false;
	
	public TCPClient(Activity ctx, String address, Listener listener) {
		mCtx = ctx;
		mListener = listener;
		mServerAddr = address;
		Log.d(TAG,"'"+address+"'");
       	done = false;
	}
		
	public void shutdown() {
	    done = true;
	}
	
	byte[] mOutByte = null;
	public void send(byte[] data) {
		mOutByte = data;
	}
	
	private void end() {		
		if (mListener!=null) mCtx.runOnUiThread(new Runnable() {
			@Override
			public void run() {
			mListener.OnThreadEnd();
		} });
		
		Log.i(TAG,"Thread end");
	}
		
	@Override 
	public synchronized void run() { 
		Log.i(TAG,"Thread start");
		byte[] rxbuffer = new byte[BUFF_SIZE];
    	Socket socket = null;
    	InputStream in = null;
    	OutputStream out = null;
    	
		// init
		try {
			socket = new Socket(mServerAddr, TCP_PORT);
			socket.setSoTimeout(3000);
			in = socket.getInputStream();
			out = socket.getOutputStream();
		} catch (IOException e) {
			e.printStackTrace();
			Log.e(TAG,e.toString());
			end(); 
			return;
		}
				   
		if (mListener!=null) mCtx.runOnUiThread(new Runnable() {
			@Override
			public void run() {
			mListener.OnThreadStart();
		} });
		
		
		
		while (!done && socket.isConnected())
		{
			try {
				// rx
				if (in.available()>0) {
					final int sz = in.read(rxbuffer);	
					if (sz>0) {
						final byte[] copy = Arrays.copyOf(rxbuffer, sz);
						mCtx.runOnUiThread(new Runnable() {
							@Override
							public void run() {
								mListener.OnDataReceived(copy, sz);
						} });								
					}
				}
				// tx
				if (mOutByte!=null) {
					out.write(mOutByte);
					mOutByte = null;
				}
			} catch (IOException e) {
				e.printStackTrace();
				Log.e(TAG,e.toString());
				done = true;
			}
		}
				
		// end
		try { in.close(); } catch (IOException e) { }
		try { out.close(); } catch (IOException e) { }
		try { socket.close(); } catch (IOException e) { }
		Log.i(TAG,"Thread proper end");
		end(); 		
	} 
}