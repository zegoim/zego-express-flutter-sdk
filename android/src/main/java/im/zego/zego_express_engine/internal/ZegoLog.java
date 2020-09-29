//
//  ZegoLog.java
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020/08/29.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import android.util.Log;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Locale;

public class ZegoLog {

    static Class<?> engineClass;
    static Method logMethod;

    public static void log(String format, Object... args) {
        String message = String.format(Locale.ENGLISH, format, args);
        Log.d("ZEGO", "[Flutter] " + message);
        ZegoLog.logNotice(message);
    }

    public static void error(String format, Object... args) {
        String message = String.format(Locale.ENGLISH, format, args);
        Log.e("ZEGO", "[Flutter] [ERROR] " + message);
        ZegoLog.logNotice(message);
    }

    private static void logNotice(String message) {
        if (engineClass == null || logMethod == null) {
            try {
                engineClass = Class.forName("im.zego.zegoexpress.internal.ZegoExpressEngineInternalImpl");
                logMethod = engineClass.getDeclaredMethod("logNotice", String.class, String.class);
                logMethod.setAccessible(true);
            } catch (ClassNotFoundException e) {
                Log.e("ZEGO", "[Flutter] logNotice failed, class ZegoExpressEngineInternalImpl not found.");
            } catch (NoSuchMethodException e) {
                Log.e("ZEGO", "[Flutter] logNotice failed, method logNotice not found.");
            }
        }

        try {
            if (logMethod != null) {
                logMethod.invoke(null, message, "Flutter");
            }
        } catch (IllegalAccessException e) {
            Log.e("ZEGO", "[Flutter] logNotice failed, IllegalAccessException.");
        } catch (InvocationTargetException e) {
            Log.e("ZEGO", "[Flutter] logNotice failed, InvocationTargetException.");
        }
    }
}
