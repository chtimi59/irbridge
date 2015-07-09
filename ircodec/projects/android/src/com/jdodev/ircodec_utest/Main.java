package com.jdodev.ircodec_utest;

import junit.framework.Assert;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.jdodev.ircodec.IRBuffer;
import com.jdodev.ircodec.IRcodec;

public class Main extends Activity {

	private final String TAG = "IRCODEC_UTEST";

	private final int PULSE_BIT = 0x01000000;
	private final int UNKN0_T0  = 500;
	private final int UNKN0_T1  = 1000;
	private final int  NEC_THEAD_PULSE = 9000;
	private final int  NEC_THEAD_SPACE = (NEC_THEAD_PULSE/2);
	private final int  NEC_T0 = 560;
	private final int  NEC_T1 = 1680;
	
	
	private void test_nec()
	{
		{
			IRBuffer in = new IRBuffer();
			int n,p;
			String out = null;

			
			IRcodec.NecDecodeReset();
			in = new IRBuffer(2*1024);
			in.len = (2*1024) *4;
			p = 0;			
			n=IRcodec.NecDecode(in);
			out = IRcodec.NecDecodedBuff();
			Assert.assertTrue(out.length()==1022);
			
			IRcodec.NecDecodeReset();
			in = new IRBuffer(1024);
			p = 0;			
			n=IRcodec.NecDecode(in);
			out = IRcodec.NecDecodedBuff();
			Assert.assertTrue(n == 0);
			Assert.assertTrue(out.length()==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);

			IRcodec.NecDecodeReset();
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put(p, 2672565);
			n = IRcodec.NecDecode(in);
			out = IRcodec.NecDecodedBuff();
			Assert.assertTrue(n == 0);
			Assert.assertTrue(out.length()==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);
			
			IRcodec.NecDecodeReset();			
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put( p, PULSE_BIT | 576);
			p = in.put( p,514); /*0*/
			p = in.put( p,PULSE_BIT | 583);
			p = in.put( p,1624); /*1*/
			p = in.put( p,2672565);
			n = IRcodec.NecDecode(in);
			out = IRcodec.NecDecodedBuff();
			Assert.assertTrue(n == 1);
			Assert.assertTrue(out.compareTo("01") == 0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);
			
			
			IRcodec.NecDecodeReset();
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put( p,  PULSE_BIT | 576);
			p = in.put( p,  514); /*0*/
			p = in.put( p,  PULSE_BIT | 583);
			p = in.put( p, 1624);/*1*/
			p = in.put( p, PULSE_BIT | 568);
			p = in.put( p, 531);/*0*/
			p = in.put( p, PULSE_BIT | 525);
			p = in.put( p, 1680);/*1*/
			n = IRcodec.NecDecode(in);
			out = IRcodec.NecDecodedBuff();
			Assert.assertTrue(n == 0);
			Assert.assertTrue(out.compareTo("0101")==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);			
			p = 0;
			in = new IRBuffer(1024);		
			p = in.put( p, PULSE_BIT | 569);
			p = in.put( p, 1634); /*1*/
			p = in.put( p, PULSE_BIT | 598);
			p = in.put( p, 1603); /*1*/
			p = in.put( p, PULSE_BIT | 593);
			p = in.put( p, 510); /*0*/
			p = in.put( p, 77000);
			n = IRcodec.NecDecode(in);
			out = IRcodec.NecDecodedBuff();
			Assert.assertTrue(n == 1);
			Assert.assertTrue(out.compareTo("0101110")==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);		
			

			IRcodec.NecDecodeReset();
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put( p, 2672565);
			p = in.put( p, PULSE_BIT | 8825);
			p = in.put( p, 4407);
			p = in.put( p, PULSE_BIT | 576);
			p = in.put( p, 514);
			p = in.put( p, PULSE_BIT | 583);
			p = in.put( p, 1624);
			p = in.put( p, PULSE_BIT | 568);
			p = in.put( p, 531);
			p = in.put( p, PULSE_BIT | 525);
			p = in.put( p, 1680);
			p = in.put( p, PULSE_BIT | 569);
			p = in.put( p, 1634);
			p = in.put( p, PULSE_BIT | 598);
			p = in.put( p, 1603);
			p = in.put( p, PULSE_BIT | 593);
			p = in.put( p, 510);
			p = in.put( p, PULSE_BIT | 558);
			p = in.put( p, 1646);
			p = in.put( p, PULSE_BIT | 587);
			p = in.put( p, 511);
			p = in.put( p, PULSE_BIT | 562);
			p = in.put( p, 539);
			p = in.put( p, PULSE_BIT | 522);
			p = in.put( p, 579);
			p = in.put( p, PULSE_BIT | 519);
			p = in.put( p, 591);
			p = in.put( p, PULSE_BIT | 564);
			p = in.put( p, 525);
			p = in.put( p, PULSE_BIT | 527);
			p = in.put( p, 1682);
			p = in.put( p, PULSE_BIT | 584);
			p = in.put( p, 514);
			p = in.put( p, PULSE_BIT | 568);
			p = in.put( p, 1639);
			p = in.put( p, PULSE_BIT | 592);
			p = in.put( p, 1612);
			p = in.put( p, PULSE_BIT | 571);
			p = in.put( p, 1632);
			p = in.put( p, PULSE_BIT | 583);
			p = in.put( p, 514);
			p = in.put( p, PULSE_BIT | 570);
			p = in.put( p, 533);
			p = in.put( p, PULSE_BIT | 571);
			p = in.put( p, 553);
			p = in.put( p, PULSE_BIT | 496);
			p = in.put( p, 585);
			p = in.put( p, PULSE_BIT | 521);
			p = in.put( p, 579);
			p = in.put( p, PULSE_BIT | 563);
			p = in.put( p, 553);
			p = in.put( p, PULSE_BIT | 549);
			p = in.put( p, 536);
			p = in.put( p, PULSE_BIT | 587);
			p = in.put( p, 513);
			p = in.put( p, PULSE_BIT | 521);
			p = in.put( p, 1686);
			p = in.put( p, PULSE_BIT | 567);
			p = in.put( p, 1637);
			p = in.put( p, PULSE_BIT | 586);
			p = in.put( p, 1617);
			p = in.put( p, PULSE_BIT | 582);
			p = in.put( p, 1621);
			p = in.put( p, PULSE_BIT | 657);
			p = in.put( p, 1545);
			p = in.put( p, PULSE_BIT | 600);
			p = in.put( p, 1604);
			p = in.put( p, PULSE_BIT | 600);
			p = in.put( p, 2672565);
			p = in.put( p, PULSE_BIT | 8825);
			p = in.put( p, 4407);
			p = in.put( p, PULSE_BIT | 576);
			p = in.put( p, 514);
			p = in.put( p, PULSE_BIT | 583);
			p = in.put( p, 1624);
			p = in.put( p, PULSE_BIT | 568);
			p = in.put( p, 531);
			p = in.put( p, PULSE_BIT | 525);
			p = in.put( p, 1680);
			p = in.put( p, PULSE_BIT | 569);
			p = in.put( p, 1634);
			p = in.put( p, PULSE_BIT | 598);
			p = in.put( p, 1603);
			p = in.put( p, PULSE_BIT | 593);
			p = in.put( p, 510);
			p = in.put( p, PULSE_BIT | 558);
			p = in.put( p, 1646);
			p = in.put( p, PULSE_BIT | 587);
			p = in.put( p, 511);
			p = in.put( p, PULSE_BIT | 562);
			p = in.put( p, 539);
			p = in.put( p, PULSE_BIT | 522);
			p = in.put( p, 579);
			p = in.put( p, PULSE_BIT | 519);
			p = in.put( p, 591);
			p = in.put( p, PULSE_BIT | 564);
			p = in.put( p, 525);
			p = in.put( p, PULSE_BIT | 527);
			p = in.put( p, 1682);
			p = in.put( p, PULSE_BIT | 584);
			p = in.put( p, 514);
			p = in.put( p, PULSE_BIT | 568);
			p = in.put( p, 1639);
			p = in.put( p, PULSE_BIT | 592);
			p = in.put( p, 1612);
			p = in.put( p, PULSE_BIT | 571);
			p = in.put( p, 1632);
			p = in.put( p, PULSE_BIT | 583);
			p = in.put( p, 514);
			p = in.put( p, PULSE_BIT | 570);
			p = in.put( p, 533);
			p = in.put( p, PULSE_BIT | 571);
			p = in.put( p, 553);
			p = in.put( p, PULSE_BIT | 496);
			p = in.put( p, 585);
			p = in.put( p, PULSE_BIT | 521);
			p = in.put( p, 579);
			p = in.put( p, PULSE_BIT | 563);
			p = in.put( p, 553);
			p = in.put( p, PULSE_BIT | 549);
			p = in.put( p, 536);
			p = in.put( p, PULSE_BIT | 587);
			p = in.put( p, 513);
			p = in.put( p, PULSE_BIT | 521);
			p = in.put( p, 1686);
			p = in.put( p, PULSE_BIT | 567);
			p = in.put( p, 1637);
			p = in.put( p, PULSE_BIT | 586);
			p = in.put( p, 1617);
			p = in.put( p, PULSE_BIT | 582);
			p = in.put( p, 1621);
			p = in.put( p, PULSE_BIT | 657);
			p = in.put( p, 1545);
			p = in.put( p, PULSE_BIT | 600);
			p = in.put( p, 1604);
			p = in.put( p, PULSE_BIT | 600);
			p = in.put( p, 2672565);
			p = in.put( p, PULSE_BIT | 8825);
			int i = 0;
			do {
				i++; // test no
				n = IRcodec.NecDecode(in);
				out = IRcodec.NecDecodedBuff();
				switch (i) {
					case 1:
						Assert.assertTrue(n == 0);
						Assert.assertTrue(out.length()==0);
						Assert.assertTrue(in.len == (138-1)*4);
						Assert.assertTrue(in.len==in.buffer.length);							
						break;
					case 2:
						Assert.assertTrue(n == 0);
						Assert.assertTrue(out.length()==0);
						Assert.assertTrue(in.len == (138-2)*4);
						Assert.assertTrue(in.len==in.buffer.length);							
						break;
					case 3:
						Assert.assertTrue(n == 0);
						Assert.assertTrue(out.length()==0);
						Assert.assertTrue(in.len == (138-3)*4);
						Assert.assertTrue(in.len==in.buffer.length);							
						break;						
					case 4:
						Assert.assertTrue(n == 1);
						Assert.assertTrue(out.compareTo("01011101000001011100000000111111")==0);
						break;
					case 5:						
					case 6:
						Assert.assertTrue(n == 0);
						Assert.assertTrue(out.length()==0);
						break;
					case 7:
						Assert.assertTrue(n == 1);
						Assert.assertTrue(out.compareTo("01011101000001011100000000111111")==0);
						break;					
				}
				if (n == 1) IRcodec.NecDecodeReset();
			} while (in.available());
			
			Log.d(TAG, "NecDecode() -- tests OK");
		}

		{
			byte[] out = null;
			
			out = IRcodec.NecEncode(null);
			Assert.assertTrue(out==null);
			
			out = IRcodec.NecEncode("");
			Assert.assertTrue(out.length==0);
			
			out = IRcodec.NecEncode("1");
			Assert.assertTrue(out.length == 4*(2*1+2+1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 0) == NEC_THEAD_PULSE) && (IRBuffer.FourBytesToInt(out, 1) == NEC_THEAD_SPACE));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 2) == NEC_T0) && (IRBuffer.FourBytesToInt(out, 3) == NEC_T1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 4) == NEC_T0));
			

			out =IRcodec.NecEncode("0");
			Assert.assertTrue(out.length == 4*(2*1+2+1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 0) == NEC_THEAD_PULSE) && (IRBuffer.FourBytesToInt(out, 1) == NEC_THEAD_SPACE));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 2) == NEC_T0) && (IRBuffer.FourBytesToInt(out, 3) == NEC_T0));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 4) == NEC_T0));

			out =IRcodec.NecEncode("110");
			Assert.assertTrue(out.length == 4*(2*3+2+1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 0) == NEC_THEAD_PULSE) && (IRBuffer.FourBytesToInt(out, 1) == NEC_THEAD_SPACE));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 2) == NEC_T0) && (IRBuffer.FourBytesToInt(out, 3) == NEC_T1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 4) == NEC_T0) && (IRBuffer.FourBytesToInt(out, 5) == NEC_T1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 6) == NEC_T0) && (IRBuffer.FourBytesToInt(out, 7) == NEC_T0));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 8) == NEC_T0));
			
			Log.d(TAG, "NecEncodeReset() -- tests OK");
		}		
	}
	
	private void test_nec_combine()
	{		
		int[] test = new int[] { 0xAD, 0x13 };
		
		/* 1 - create a bit string */
		String bits = IRcodec.Bytes2String(test, 15);
		Log.d(TAG, ""+bits);
		
		/* 2 - encode it into unkn0 */
		byte[] ir = IRcodec.NecEncode(bits);
		
		/* 3 - simulate this nec ir code reception */
		int cir = ir.length/4 ;
		IRBuffer ir2 = new IRBuffer(cir+1);
		for (int i = 0; i < cir; i++) {
			int v = IRBuffer.FourBytesToInt(ir, i);
			if (i % 2 == 0) v |= PULSE_BIT;
			ir2.put(i, v);
		}
		ir2.put(cir, 77000);
		
		/* 4 - decode nec ir code reception */
		IRcodec.NecDecodeReset();
		do {
			if (IRcodec.NecDecode(ir2)==1)
			{
				String decodedbits = IRcodec.NecDecodedBuff();
				Log.d(TAG, ""+decodedbits);
				Assert.assertTrue(decodedbits.compareTo(bits)==0);
				
				/* 5 - convert bit string to bytes */
				int[] decoded = IRcodec.String2Bytes(decodedbits);
				for (int i = 0; i<decoded.length; i++) {
					Log.d(TAG, ""+decoded[i]);
					if (i==0) { Assert.assertTrue(decoded[i]==test[i]); }
					if (i==1) { Assert.assertTrue(decoded[i]==(test[i] & 0xFE)); }
				}
				
				IRcodec.NecDecodeReset();
			}
		} while(ir2.available());
		
		Log.d(TAG, "test_nec_combine() -- tests OK");
	}
	
	
	
	private void test_unkn0()
	{
		
		{
			IRBuffer in = new IRBuffer();
			int n,p;
			String out = null;

			
			IRcodec.Unkn0DecodeReset();
			in = new IRBuffer(2*1024);
			in.len = (2*1024) *4;
			p = 0;			
			n=IRcodec.Unkn0Decode(in);
			out = IRcodec.Unkn0DecodedBuff();
			Assert.assertTrue(out.length()==1022);
			
			IRcodec.Unkn0DecodeReset();
			in = new IRBuffer(1024);
			p = 0;			
			n=IRcodec.Unkn0Decode(in);
			out = IRcodec.Unkn0DecodedBuff();
			Assert.assertTrue(n == 0);
			Assert.assertTrue(out.length()==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);

			IRcodec.Unkn0DecodeReset();
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put(p, 77000);
			n = IRcodec.Unkn0Decode(in);
			out = IRcodec.Unkn0DecodedBuff();
			Assert.assertTrue(n == 0);
			Assert.assertTrue(out.length()==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);
			
			IRcodec.Unkn0DecodeReset();			
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 77000);
			n = IRcodec.Unkn0Decode(in);
			out = IRcodec.Unkn0DecodedBuff();
			Assert.assertTrue(n == 1);
			Assert.assertTrue(out.compareTo("11011100") == 0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);
			
			
			IRcodec.Unkn0DecodeReset();
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			n = IRcodec.Unkn0Decode(in);
			out = IRcodec.Unkn0DecodedBuff();
			Assert.assertTrue(n == 0);
			Assert.assertTrue(out.compareTo("1101")==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);			
			p = 0;
			in = new IRBuffer(1024);		
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 77000);
			n = IRcodec.Unkn0Decode(in);
			out = IRcodec.Unkn0DecodedBuff();
			Assert.assertTrue(n == 1);
			Assert.assertTrue(out.compareTo("11011100")==0);
			Assert.assertTrue(in.len==0);
			Assert.assertTrue(in.len==in.buffer.length);		
			

			IRcodec.Unkn0DecodeReset();
			in = new IRBuffer(1024);		
			p = 0;
			p = in.put( p, 77000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 77000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 1
			p = in.put( p, 1000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 77000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			p = in.put( p, 77000);
			p = in.put( p, 500 | PULSE_BIT); // 0
			p = in.put( p, 500);
			int i = 0;
			do {
				i++; // test no
				n = IRcodec.Unkn0Decode(in);
				out = IRcodec.Unkn0DecodedBuff();
				switch (i) {
					case 1:
						Assert.assertTrue(n == 0);
						Assert.assertTrue(out.length()==0);
						Assert.assertTrue(in.len == (22-1)*4);
						Assert.assertTrue(in.len==in.buffer.length);							
						break;
					case 2:
						Assert.assertTrue(n == 1);
						Assert.assertTrue(out.compareTo("1101")==0);
						Assert.assertTrue(in.len == (22-10)*4);
						Assert.assertTrue(in.len==in.buffer.length);	
						break;
					case 3:
						Assert.assertTrue(n == 1);
						Assert.assertTrue(out.compareTo("110")==0);
						Assert.assertTrue(in.len == (22-17)*4);
						Assert.assertTrue(in.len==in.buffer.length);	
						break;
					case 4:
						Assert.assertTrue(n == 1);
						Assert.assertTrue(out.compareTo("0")==0);
						Assert.assertTrue(in.len == (22-20)*4);
						Assert.assertTrue(in.len==in.buffer.length);	
						break;
					case 5:
						Assert.assertTrue(n == 0);
						Assert.assertTrue(out.compareTo("0")==0);
						Assert.assertTrue(in.len == 0);
						Assert.assertTrue(in.len==in.buffer.length);	
						break;
				}
				if (n == 1) IRcodec.Unkn0DecodeReset();
			} while (in.available());
			
			Log.d(TAG, "Unkn0Decode() -- tests OK");
		}

		{
			byte[] out = null;
			
			out = IRcodec.Unkn0Encode(null);
			Assert.assertTrue(out==null);
			
			out = IRcodec.Unkn0Encode("");
			Assert.assertTrue(out.length==0);
			
			out = IRcodec.Unkn0Encode("1");
			Assert.assertTrue(out.length == 4*(2*1+1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 0) == UNKN0_T0) && (IRBuffer.FourBytesToInt(out, 1) == UNKN0_T1));

			out =IRcodec.Unkn0Encode("0");
			Assert.assertTrue(out.length == 4*(2*1+1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 0) == UNKN0_T0) && (IRBuffer.FourBytesToInt(out, 1) == UNKN0_T0));

			out =IRcodec.Unkn0Encode("110");
			Assert.assertTrue(out.length == 4*(2*3+1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 0) == UNKN0_T0) && (IRBuffer.FourBytesToInt(out, 1) == UNKN0_T1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 2) == UNKN0_T0) && (IRBuffer.FourBytesToInt(out, 3) == UNKN0_T1));
			Assert.assertTrue((IRBuffer.FourBytesToInt(out, 4) == UNKN0_T0) && (IRBuffer.FourBytesToInt(out, 5) == UNKN0_T0));
			
			Log.d(TAG, "Unkn0EncodeReset() -- tests OK");
		}		
	}
	
	private void test_unkn0_combine()
	{
		int[] test = new int[] { 0xAD, 0x13 };
		
		/* 1 - create a bit string */
		String bits = IRcodec.Bytes2String(test, 15);
		Log.d(TAG, ""+bits);
		
		/* 2 - encode it into unkn0 */
		byte[] ir = IRcodec.Unkn0Encode(bits);
		
		/* 3 - simulate this unkn0 ir code reception */
		int cir = ir.length/4 ;
		IRBuffer ir2 = new IRBuffer(cir+1);
		for (int i = 0; i < cir; i++) {
			int v = IRBuffer.FourBytesToInt(ir, i);
			if (i % 2 == 0) v |= PULSE_BIT;
			ir2.put(i, v);
		}
		ir2.put(cir, 77000);
		
		/* 4 - decode unkn0 ir code reception */
		IRcodec.Unkn0DecodeReset();
		do {
			if (IRcodec.Unkn0Decode(ir2)==1)
			{
				String decodedbits = IRcodec.Unkn0DecodedBuff();
				Log.d(TAG, ""+decodedbits);
				Assert.assertTrue(decodedbits.compareTo(bits)==0);
				
				/* 5 - convert bit string to bytes */
				int[] decoded = IRcodec.String2Bytes(decodedbits);
				for (int i = 0; i<decoded.length; i++) {
					Log.d(TAG, ""+decoded[i]);
					if (i==0) { Assert.assertTrue(decoded[i]==test[i]); }
					if (i==1) { Assert.assertTrue(decoded[i]==(test[i] & 0xFE)); }
				}
				
				IRcodec.Unkn0DecodeReset();
			}
		} while(ir2.available());
		
		Log.d(TAG, "test_unkn0_combine() -- tests OK");
	}
	
	private void test_bitsops()
	{
		{
			int[] out;
			
			out = IRcodec.String2Bytes(null);
			Assert.assertTrue(out == null);

			out = IRcodec.String2Bytes("");
			Assert.assertTrue(out.length==0);
			
			out = IRcodec.String2Bytes("10001000");
			Assert.assertTrue(out.length==1);
			Assert.assertTrue(out[0] == 0x88);
		
			out = IRcodec.String2Bytes("1");
			Assert.assertTrue(out.length==1);
			Assert.assertTrue(out[0] == 0x80);

			out = IRcodec.String2Bytes("");
			Assert.assertTrue(out.length==0);
	
			out = IRcodec.String2Bytes("100001001");
			Assert.assertTrue(out.length==2);
			Assert.assertTrue(out[0]==0x84);
			Assert.assertTrue(out[1]==0x80);
			
			Log.d(TAG, "String2Bytes() -- tests OK");
		}	
		
		{
			String out = null;
			int[] in = new int[1024];
			
			out = IRcodec.Bytes2String(null, 999);
			Assert.assertTrue(out == null);
			
			out = IRcodec.Bytes2String(in, 0);
			Assert.assertTrue(out == null);

			in[0] = 0x01;
			out = IRcodec.Bytes2String(in, 8);
			Assert.assertTrue(out.compareTo("00000001") == 0);

			in[0] = 0x01;
			out = IRcodec.Bytes2String(in, 7);
			Assert.assertTrue(out.compareTo("0000000") == 0);

			in[0] = 0x84;
			in[1] = 0x8F;
			out = IRcodec.Bytes2String(in, 9);
			Assert.assertTrue(out.compareTo("100001001") == 0);

			in[0] = 0x85;
			in[1] = 0x8F;
			out = IRcodec.Bytes2String(in, 16);
			Assert.assertTrue(out.compareTo("1000010110001111") == 0);
			
			Log.d(TAG, "Bytes2String() -- tests OK");
		}
		
	}
	
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		test_bitsops();
		
		test_unkn0();
		test_unkn0_combine();
		
		test_nec();
		test_nec_combine();
	}
}
