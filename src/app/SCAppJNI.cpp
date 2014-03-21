//
//  SCAppJNI.cpp
//  SnowCat
//
//  Created by Moky on 13-9-29.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCAppJNI.h"

/**
 *  SnowCat.java
 *
package com.slanissue.libs;

import java.lang.reflect.Constructor;

import android.content.Context;
import android.view.View;

public class SnowCat {
	
	public static native void onCreate(View view, Context ctx);
	
	public static View createView(String className, Context ctx) {
		try {
			Class<?> clazz = Class.forName(className);
			Constructor<?> cr = clazz.getConstructor(Context.class);
			return (View)cr.newInstance(ctx);
		} catch (Exception e) {
			return null;
		}
	}
}
 */

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)

extern "C"
{
	static const char * s_java_classes[] = {
		"android/view/View",
		"android/widget/ImageView",
		"android/widget/AbsoluteLayout",
		
		"android/net/Uri",
		
		"com/slanissue/libs/SnowCat",
	};
	
	typedef enum {
		class_view,
		class_imageview,
		class_layout,
		
		class_uri,
		
		class_snowcat,
	} class_index;
	
	static inline const char * getClassName_(const unsigned int idx) {
		return s_java_classes[idx];
	}
	
	static const char * s_java_methods[] = {
		// View
		"layout",          "(IIII)V",
		// ImageView
		"setImageURI",     "(Landroid/net/Uri;)V",
		// AbsoluteLayout
		"addView",         "(Landroid/view/View;I)V",
		"removeViewAt",    "(I)V",
		// Uri
		"parse",           "(Ljava/lang/String;)Landroid/net/Uri;", // static
		// SnowCat
		"createView",      "(Ljava/lang/String;Landroid/content/Context;)Landroid/view/View;", // static
		"setImage",        "(Landroid/widget/ImageView;Ljava/lang/String;Landroid/content/Context;)V", // static
	};
	
	typedef enum {
		// View
		method_setframe,
		// ImageView
		method_setimage,
		// AbsoluteLayout
		method_addview,
		method_removeviewat,
		// Uri
		static_method_parse,
		// SnowCat
		static_method_createview,
		static_method_setimage,
	} method_index;
	
	static inline const char * getMethodName_(const unsigned int idx) {
		return s_java_methods[idx * 2];
	}
	static inline const char * getParamCode_(const unsigned int idx) {
		return s_java_methods[idx * 2 + 1];
	}
	
	static void preload_methods_(void)
	{
		unsigned int methods[] = {
			class_view,      method_setframe,
			class_imageview, method_setimage,
			class_layout,    method_addview,
			class_layout,    method_removeviewat,
		};
		for (int i = 0; i < 4; i++)
		{
			const char * className = getClassName_(methods[i * 2]);
			const char * methodName = getMethodName_(methods[i * 2 + 1]);
			const char * paramCode = getParamCode_(methods[i * 2 + 1]);
			JniMethodInfo t;
			if (JniHelper::getMethodInfo(t, className, methodName, paramCode))
			{
				t.env->DeleteLocalRef(t.classID);
			}
		}
	}
	
	static void preload_static_methods_(void)
	{
		unsigned int static_methods[] = {
			class_uri,     static_method_parse,
			class_snowcat, static_method_createview,
			class_snowcat, static_method_setimage,
		};
		for (int i = 0; i < 3; i++)
		{
			const char * className = getClassName_(static_methods[i * 2]);
			const char * methodName = getMethodName_(static_methods[i * 2 + 1]);
			const char * paramCode = getParamCode_(static_methods[i * 2 + 1]);
			JniMethodInfo t;
			if (JniHelper::getStaticMethodInfo(t, className, methodName, paramCode))
			{
				t.env->DeleteLocalRef(t.classID);
			}
		}
	}
	
#pragma mark -
	
	static jobject s_current_activity = NULL;
	static jobject s_root_view = NULL;
	
	jobject rootViewJNI(void)
	{
		return s_root_view;
	}
	
	void Java_com_slanissue_libs_SnowCat_onCreate(JNIEnv*  env, jobject thiz, jobject view, jobject ctx)
	{
		// preload classes and methods
		preload_methods_();
		preload_static_methods_();
		
		// save current context & root view
		SCLog("activity: %d, view: %d", ctx, view);
		s_current_activity = ctx;
		s_root_view = view;
	}
	
	jobject createViewJNI(const char * className)
	{
		SCLog("class: %s", className);
		
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, getClassName_(class_snowcat), getMethodName_(static_method_createview), getParamCode_(static_method_createview)))
		{
			jstring clazz = t.env->NewStringUTF(className);
			jobject view = t.env->CallStaticObjectMethod(t.classID, t.methodID, clazz, s_current_activity);
			t.env->DeleteLocalRef(t.classID);
			t.env->DeleteLocalRef(clazz);
			
			SCLog("OK");
			return view;
		}
		
		SCError("Error");
		return 0;
	}
	
	void insertSubviewJNI(jobject parent, jobject child, int index)
	{
		SCLog("index: %d", index);
		
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t, getClassName_(class_layout), getMethodName_(method_addview), getParamCode_(method_addview)))
		{
			t.env->CallVoidMethod(parent, t.methodID, child, index);
			t.env->DeleteLocalRef(t.classID);
			
			SCLog("OK");
			return;
		}
		
		SCError("Error");
	}
	
	void removeSubviewJNI(jobject view, int index)
	{
		SCLog("index: %d", index);
		
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t, getClassName_(class_layout), getMethodName_(method_removeviewat), getParamCode_(method_removeviewat)))
		{
			t.env->CallVoidMethod(view, t.methodID, index);
			t.env->DeleteLocalRef(t.classID);
			
			SCLog("OK");
			return;
		}
		
		SCError("Error");
	}
	
	void setViewFrameJNI(jobject view, const Size & size, const Point & center)
	{
		if (!view)
		{
			SCError("invalid view, size: {%.2f, %.2f}, center: {%.2f, %.2f}", size.width, size.height, center.x, center.y);
			return;
		}
		SCLog("size: {%.2f, %.2f}, center: {%.2f, %.2f}", size.width, size.height, center.x, center.y);
		int left   = center.x - size.width * 0.5;
		int top    = center.y - size.height * 0.5;
		int right  = left + size.width;
		int bottom = top + size.height;
		
//		JniMethodInfo t;
//		if (JniHelper::getMethodInfo(t, getClassName_(class_view), getMethodName_(method_setframe), getParamCode_(method_setframe)))
//		{
//			SCLog("calling: %d, %d, %d, %d, view: %d", left, top, right, bottom, view);
//			t.env->CallVoidMethod(view, t.methodID, left, top, right, bottom);
//			SCLog("deleting");
//			t.env->DeleteLocalRef(t.classID);
//			
//			SCLog("OK");
//			return;
//		}
		
		
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, getClassName_(class_snowcat), "setFrame", "(Landroid/view/View;IIII)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, view, left, top, right, bottom);
			t.env->DeleteLocalRef(t.classID);
			
			SCLog("OK");
			return;
		}
		
		
		SCError("Error");
	}
	
//	static jobject getUriJNI_(const char * psz)
//	{
//		SCLog("string: %s", psz);
//		
//		JniMethodInfo t;
//		if (JniHelper::getStaticMethodInfo(t, getClassName_(class_snowcat), getMethodName_(static_method_geturi), getParamCode_(static_method_geturi)))
//		{
//			jstring string = t.env->NewStringUTF(psz);
//			jobject uri = t.env->CallStaticObjectMethod(t.classID, t.methodID, string);
//			t.env->DeleteLocalRef(t.classID);
//			t.env->DeleteLocalRef(string);
//			
//			SCLog("OK");
//			return uri;
//		}
////		JniMethodInfo t;
////		if (JniHelper::getStaticMethodInfo(t, getClassName_(class_uri), getMethodName_(static_method_parse), getParamCode_(static_method_parse)))
////		{
////			jstring string = t.env->NewStringUTF(psz);
////			jobject uri = t.env->CallStaticObjectMethod(t.classID, t.methodID, string);
////			t.env->DeleteLocalRef(t.classID);
////			t.env->DeleteLocalRef(string);
////			
////			SCLog("OK");
////			return uri;
////		}
//		
//		SCError("Error");
//		return NULL;
//	}
	
	void setViewImageJNI(jobject view, const char * image)
	{
		SCLog("view: %d, image: %s, context: %d", view, image, s_current_activity);
		
//		jobject uri = getUriJNI_(image);
//		if (!uri)
//		{
//			SCError("");
//			return;
//		}
//		
//		JniMethodInfo t;
//		if (JniHelper::getMethodInfo(t, getClassName_(class_imageview), getMethodName_(method_setimage), getParamCode_(method_setimage)))
//		{
//			t.env->CallVoidMethod(view, t.methodID, uri);
//			t.env->DeleteLocalRef(t.classID);
//			t.env->DeleteLocalRef(uri);
//			
//			SCLog("OK");
//		}
		
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, getClassName_(class_snowcat), getMethodName_(static_method_setimage), getParamCode_(static_method_setimage)))
		{
			jstring string = t.env->NewStringUTF(image);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, view, string, s_current_activity);
			t.env->DeleteLocalRef(t.classID);
			t.env->DeleteLocalRef(string);
			
			SCLog("OK");
			return;
		}
		
		SCError("Error");
	}
	
}

#endif // EOF '#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)'

NAMESPACE_END
NAMESPACE_END
