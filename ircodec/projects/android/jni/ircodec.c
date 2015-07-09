#include <string.h>
#include <jni.h>
#include <android/log.h>
void android_log(android_LogPriority type, const char *fmt, ...);
static const char LOG_TAG[] = "IRCODEC_UTEST";
#define LOGI(...) android_log(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOGW(...) android_log(ANDROID_LOG_WARN, __VA_ARGS__)
#define LOGE(...) android_log(ANDROID_LOG_ERROR, __VA_ARGS__)
#define LOGD(...) android_log(ANDROID_LOG_DEBUG, __VA_ARGS__)

#include "ircodec.h"
#include "bitops.h"
void IRBuffer_read(JNIEnv*, jobject, lirc_t **, size_t*);
void IRBuffer_write(JNIEnv*, jobject, lirc_t *, size_t);



// UNKN0 CODEC
#if 1

/* reset UNKN0 decoder */
void Java_com_jdodev_ircodec_IRcodec_Unkn0DecodeReset( JNIEnv* env, jobject thiz) {
	unkn0_decode_reset();
}

/* ENCODE a input char string (ex: "10001100") into a lirc stream
*  @inputStream : a char input stream
*  @return : a JavaByteArray (i.e. lirc stream)
*/	
jbyteArray Java_com_jdodev_ircodec_IRcodec_Unkn0Encode( JNIEnv* env, jobject thiz, jstring inputStream)
{
	jbyteArray result;
	lirc_t* out = NULL;
	const char *in = NULL;
	
	if (inputStream == NULL) {
		return NULL;
	}
	
	in = (*env)->GetStringUTFChars(env, inputStream, 0);		
	size_t sz = unkn0_encode((char*)in,  &out);
	(*env)->ReleaseStringUTFChars(env, inputStream, in);

	#ifdef UNITARY_TEST
	LOGD("output buff size %d", sz);
	#endif
	
	if ((sz>0) && (out!=NULL)) {
		result=(*env)->NewByteArray(env, sz);
		(*env)->SetByteArrayRegion(env, result, 0, sz, (char*)out);
		free(out);
	} else {
		result=(*env)->NewByteArray(env, 0);
	}
	return result;
}

/* DECODE a lirc stream into 'unkn0_DECODE_BUF' char stream (ex: "10001100")
*  @irbuffer  : input IRBuffer object (i.e. lirc stream) 
*  @return : 1 if a frame is available in unkn0_DECODE_BUF
*
*  (*) stream will be updated on remaining part after decoder processing
*/
int Java_com_jdodev_ircodec_IRcodec_Unkn0Decode( JNIEnv* env, jobject thiz, jobject irbuffer)
{
	int ret;
	size_t len;
	lirc_t* buff = NULL;
	lirc_t* progress_buff = NULL;
	IRBuffer_read(env, irbuffer, &buff, &len);
	progress_buff = buff;
	ret = unkn0_decode(&progress_buff, &len);
	IRBuffer_write(env, irbuffer, progress_buff, len);
	if (buff!=NULL) free(buff);
    return ret;
}

/* DECODEDBUFF
 * @return: return the decoded char string (ex: "10001100")
 */
jstring Java_com_jdodev_ircodec_IRcodec_Unkn0DecodedBuff( JNIEnv* env, jobject thiz)
{
	jstring result = (*env)->NewStringUTF(env, unkn0_DECODE_BUFF);
	return result;
}

#ifdef UNITARY_TEST
void Java_com_jdodev_ircodec_IRcodec_Unkn0UTEST( JNIEnv* env, jobject thiz) {
	UTEST_unkn0();
}
#endif

#endif



// NEC CODEC
#if 1

/* reset NEC decoder */
void Java_com_jdodev_ircodec_IRcodec_NecDecodeReset( JNIEnv* env, jobject thiz) {
	nec_decode_reset();
}

/* ENCODE a input char string (ex: "10001100") into a lirc stream
*  @inputStream : a char input stream
*  @return : a JavaByteArray (i.e. lirc stream)
*/	
jbyteArray Java_com_jdodev_ircodec_IRcodec_NecEncode( JNIEnv* env, jobject thiz, jstring inputStream)
{
	jbyteArray result;
	lirc_t* out = NULL;
	const char *in = NULL;
	
	if (inputStream == NULL) {
		return NULL;
	}
	
	in = (*env)->GetStringUTFChars(env, inputStream, 0);		
	size_t sz = nec_encode((char*)in,  &out);
	(*env)->ReleaseStringUTFChars(env, inputStream, in);

	#ifdef UNITARY_TEST
	LOGD("output buff size %d", sz);
	#endif
	
	if ((sz>0) && (out!=NULL)) {
		result=(*env)->NewByteArray(env, sz);
		(*env)->SetByteArrayRegion(env, result, 0, sz, (char*)out);
		free(out);
	} else {
		result=(*env)->NewByteArray(env, 0);
	}
	return result;
}

/* DECODE a lirc stream into 'nec_DECODE_BUF' char stream (ex: "10001100")
*  @irbuffer  : input IRBuffer object (i.e. lirc stream) 
*  @return : 1 if a frame is available in nec_DECODE_BUF
*
*  (*) stream will be updated on remaining part after decoder processing
*/
int Java_com_jdodev_ircodec_IRcodec_NecDecode( JNIEnv* env, jobject thiz, jobject irbuffer)
{
	int ret;
	size_t len;
	lirc_t* buff = NULL;
	lirc_t* progress_buff = NULL;
	IRBuffer_read(env, irbuffer, &buff, &len);
	progress_buff = buff;
	ret = nec_decode(&progress_buff, &len);
	IRBuffer_write(env, irbuffer, progress_buff, len);
	if (buff!=NULL) free(buff);
    return ret;
}

/* DECODEDBUFF
 * @return: return the decoded char string (ex: "10001100")
 */
jstring Java_com_jdodev_ircodec_IRcodec_NecDecodedBuff( JNIEnv* env, jobject thiz)
{
	jstring result = (*env)->NewStringUTF(env, nec_DECODE_BUFF);
	return result;
}

#ifdef UNITARY_TEST
void Java_com_jdodev_ircodec_IRcodec_NecUTEST( JNIEnv* env, jobject thiz) {
	UTEST_nec();
}
#endif

#endif



// bitsOPS
#if 1

/* Convert an input char stream into a IntArray array
*  example:
*       string2bytes("10001000");  => return { 0x88 }
*       string2bytes("1");         => return { 0x80 }
*       string2bytes("100001001"); => return { 0x84, 0x80 }
*
*  @inputStream : a char input stream
*  @return : a Int Array (1byte per array elements)
*
*/
jintArray Java_com_jdodev_ircodec_IRcodec_String2Bytes(JNIEnv* env, jobject thiz, jstring inputStream)
{
	jintArray result;
	sample_t* out = NULL;	
	int i, count=0;
	
	if (inputStream==NULL) {
		return NULL;
	}
	
	const char *in = (*env)->GetStringUTFChars(env, inputStream, 0);		
	size_t sz = string2bytes((char*)in, &out);
	count = sz / sizeof(sample_t);
	(*env)->ReleaseStringUTFChars(env, inputStream, in);
	
	#ifdef UNITARY_TEST
	LOGD("output array count %d", count);
	#endif
	
	if ((count>0) && (out!=NULL)) {
		jint* out2 = NULL;
		out2 = malloc(count * sizeof(sample_t));	
		for(i=0; i<count; i++) out2[i] = (jint)out[i];
		result=(*env)->NewIntArray(env, count);
		(*env)->SetIntArrayRegion(env, result, 0, count, (int*)out2);
		free(out2);
	} else {
		result=(*env)->NewIntArray(env, 0);
	}
	
	if (out!=NULL) free(out);
	return result;
}

/* Convert a IntArray into a char stream
*  example:
*       jInt* array = { 0x42,  0x8F };
*       out = bytes2string(array, 8) => out = "100001001"
*
*  @array  : a IntArray array
*  @nbbits : nb of bits to deals with in the input array
*  @return : a char stream
*/
jstring Java_com_jdodev_ircodec_IRcodec_Bytes2String(JNIEnv* env, jobject thiz, jintArray array, jint nbbits)
{	
	int count;
	sample_t* in;
	jint* in2;
	jstring outputStream = NULL;
	int i;
	
	if (array==NULL) {
		return outputStream;
	}

	count = (*env)->GetArrayLength(env, array);
	if ((count==0) || (nbbits==0)) {
		return outputStream;
	}
	
	in = malloc(count * sizeof(sample_t));
	in2 = malloc(count * sizeof(jint));
	
	#ifdef UNITARY_TEST
	LOGD("input array count %d", count);
	#endif
	
	(*env)->GetIntArrayRegion(env, array, 0, count, in2);
	for(i=0; i<count; i++) in[i] = (sample_t)in2[i];
	
	char* str = bytes2string(in, nbbits);		
	if (str!=NULL) {
		outputStream = (*env)->NewStringUTF(env, str);
	}
	
	free(in);
	free(in2);
	return outputStream;
}

#ifdef UNITARY_TEST
void Java_com_jdodev_ircodec_IRcodec_BitsopsTEST(JNIEnv* env, jobject thiz) {
	void UTEST_bitsops();
}
#endif

#endif




// ----------- HELPERS -------------
#if 1

/* Read from IRBuffer class instance and allocate/fill pBuff accordignly */ 
void IRBuffer_read(JNIEnv* env, jobject obj, lirc_t **pbuff, size_t* size)
{
	jclass cls = (*env)->FindClass(env, "com/jdodev/ircodec/IRBuffer");	
	// GET LEN member
	{
		jfieldID idx = (*env)->GetFieldID(env, cls, "len", "I");
		jint value = (*env)->GetIntField(env, obj, idx);
		*size = value;
		#ifdef UNITARY_TEST
		LOGD("IRBuffer.len = %d", value);
		#endif
	}
	// GET buffer member
	{
		jfieldID idx = (*env)->GetFieldID(env, cls, "buffer", "[B");
		jobject* o = ((*env)->GetObjectField (env, obj, idx));
		jbyteArray array = *((jbyteArray*)(&o));
		int arraylen = (*env)->GetArrayLength(env, array);
		*pbuff = malloc(arraylen);
		(*env)->GetByteArrayRegion(env, array, 0, arraylen, (jbyte*)(*pbuff));
		#ifdef UNITARY_TEST
		LOGD("IRBuffer.buffer.lenght = %d", arraylen);
		#endif
	}
}

/* Write to IRBuffer class instance from buff */ 
void IRBuffer_write(JNIEnv* env, jobject obj, lirc_t *buff, size_t size)
{
	jclass cls = (*env)->FindClass(env, "com/jdodev/ircodec/IRBuffer");	
	// SET LEN member
	{
		jfieldID idx = (*env)->GetFieldID(env, cls, "len", "I");
		(*env)->SetIntField(env, obj, idx, size);
	}
	// SET buffer member
	{
		// release previous one
		jfieldID idx = (*env)->GetFieldID(env, cls, "buffer", "[B");
		jobject* o = ((*env)->GetObjectField (env, obj, idx));
		jbyteArray array = *((jbyteArray*)(&o));
		jbyte*     elts = (*env)->GetByteArrayElements(env, array, 0);
		(*env)->ReleaseByteArrayElements(env, array, elts, 0);
		(*env)->SetObjectField (env, obj, idx, NULL);
		
		// create a new one
		jbyteArray result;
		if ((size==0) || (buff==NULL)) {
			result = (*env)->NewByteArray(env, 0);			
		} else {
			result =(*env)->NewByteArray(env, size);
			(*env)->SetByteArrayRegion(env, result, 0, size, (char*)buff);
		}
		(*env)->SetObjectField (env, obj, idx, result);
	}
}

/* Debug log facilities */ 
void android_log(android_LogPriority type, const char *fmt, ...)
{
    static char buf[1024];
    static char *bp = buf;
    va_list vl;
    va_start(vl, fmt);
    int available = sizeof(buf) - (bp - buf);
    int nout = vsnprintf(bp, available, fmt, vl);
	__android_log_write(type, LOG_TAG, buf);	
    if (nout >= available) {
        __android_log_write(ANDROID_LOG_WARN, LOG_TAG, "previous log line has been truncated!");
        bp = buf;
    }
    va_end(vl);
}
#endif
