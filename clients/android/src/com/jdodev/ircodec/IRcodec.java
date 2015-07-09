package com.jdodev.ircodec;

public class IRcodec
{

    static {
        System.loadLibrary("ircodec");
    }

    
    /* RESET decoder */
    public static native void Unkn0DecodeReset();

    /* ENCODE a input char string (ex: "10001100") into a lirc stream */	
    public static native byte[] Unkn0Encode(String bitstream);
 
    /* DECODE an input lirc stream, return 1 if something is ready in Unkn0DecodedBuff  */
    public static native int Unkn0Decode(IRBuffer obj);
    
    /* DECODEBUFF return the decoded buffer  */
    public static native String Unkn0DecodedBuff();
    
    
    
    /* RESET decoder */
    public static native void NecDecodeReset();

    /* ENCODE a input char string (ex: "10001100") into a lirc stream */	
    public static native byte[] NecEncode(String bitstream);
 
    /* DECODE an input lirc stream, return 1 if something is ready in Unkn0DecodedBuff  */
    public static native int NecDecode(IRBuffer obj);
    
    /* DECODEBUFF return the decoded buffer  */
    public static native String NecDecodedBuff();
    
    
    
    
    /* Convert an input char stream (ex: "10001100") into a IntArray array */
    public static native int[] String2Bytes(String bitsream);
    
    /* Convert an IntArray array into a char stream */
    public static native String Bytes2String(int[] array, int nbbits);
    
}