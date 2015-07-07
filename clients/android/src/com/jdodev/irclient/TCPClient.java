package com.jdodev.irclient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.ServerSocket;
import java.net.Socket;

import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.util.Log;


public class TCPClient extends Thread
{
	public final static String TAG = "TCPCLIENT";
	
	public final static int TCP_PORT = 8000;
	public static String SERVER_ADDR = "172.16.32.74";
	private final static int MAXTCP_PAYLOADSZ = 65527;
	
	public interface Listener {
		public void OnThreadStart();
		public void OnDataReceived(Byte[] frame);
		public void OnThreadEnd();
	}
	
	private Activity mCtx = null;
	private Listener mListener = null;
	private volatile boolean done = false;
	private volatile OutputStreamWriter out = null;
	
	public TCPClient(Activity ctx, String address, Listener listener) {
		mCtx = ctx;
		mListener = listener;
		SERVER_ADDR = address;
       	done = false;
	}
		
	public void shutdown() {
	    done = true;
	}
	
	private void end() {		
		if (mListener!=null) mCtx.runOnUiThread(new Runnable() {
			@Override
			public void run() {
			mListener.OnThreadEnd();
		} });
		
		Log.i(TAG,"Rx Thread end");
	}
	
	
	@Override 
	public synchronized void run() { 
		Log.i(TAG,"Rx Thread start");
		Socket socket = null;
		
		// init
		try {
			socket = new Socket(SERVER_ADDR, TCP_PORT);
			socket.setSoTimeout(3000);
			OutputStreamWriter out = new OutputStreamWriter(socket.getOutputStream());
			
		} catch (IOException e) {
			e.printStackTrace();
			Log.e(TAG,e.toString());
			end(); 
		}
		
		if (mListener!=null) mCtx.runOnUiThread(new Runnable() {
			@Override
			public void run() {
			mListener.OnThreadStart();
		} });
		
		// read
		while (!done) {
			try {
		           
				InputStreamReader in = new InputStreamReader(socket.getInputStream());
				
				in.read(buffer) {
					mCtx.runOnUiThread(new Runnable() {
						@Override
						public void run() {
							mListener.OnDataReceived(frame);
					} });	
				}
				
								
			} catch (IOException e) {
				Log.e(TAG,e.toString());
			}
		}
		
		
		// end
		socket.close();
		socket = null;
		packet = null;
		Log.i(TAG,"Rx Thread proper end");
		end(); 		
	} 
}