package com.jdodev.irclient;

public class IRcodec {
	static byte[] int2byte(int[] v) {
		byte[] out = new byte[v.length];
		for (int i=0; i<v.length; i++) {
			out[i] = (byte)(v[i] & 0x7F);
			out[i] *= ((v[i] & 0x80)>0)?-1:0;
		}
		return out;
	}
}
