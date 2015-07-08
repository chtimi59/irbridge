package com.jdodev.ircodec;

public class IRcodec
{

    static {
        System.loadLibrary("ircodec");
    }
    
    public native String stringFromJNI();

}