package com.jdodev.ircodec;

public class IRBuffer
{	
	public IRBuffer() {
		len = 0;
		buffer = null;
	}
	
	public IRBuffer(int count) {
		len = 0;
		buffer = new byte[count*4];
	}
	
	public int put(int pos, int v) {
		for (int t=0; t<4;t++)
			buffer[4*pos+t] = (byte)(v >> (t*8));
		len += 4;
		return pos+1;
	}
	
	public int get(int pos) {
		return FourBytesToInt(buffer, pos);
	}
	
	public static int FourBytesToInt(byte[] b, int pos) {
		int v = 0;
		for (int t=0; t<4;t++) {
			int tmp = (int)(b[4*pos+t]);
			if (tmp<0)  tmp = (b[4*pos+t] & 0x7F) | 0x80;				
			v = v | (tmp << (t*8));
		}
		return v;
	}
		
	public int count() {
		return len/4;
	}
	
	public boolean available() {
		return len>0;
	}
	
   	public byte[] buffer;  
   	public int    len;     /* in bytes */
}
