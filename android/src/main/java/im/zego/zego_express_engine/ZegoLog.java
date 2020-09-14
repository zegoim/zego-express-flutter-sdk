//
//  ZegoLog.java
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020/08/29.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import android.util.Log;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Locale;

public class ZegoLog {

    static Class<?> engineClass;
    static Method logMethod;

    public static void log(String format, Object... args) {
        if (engineClass == null || logMethod == null) {
            try {
                engineClass = Class.forName("im.zego.zegoexpress.internal.ZegoExpressEngineJniAPI");
                logMethod = engineClass.getMethod("logNoticeJni", String.class, String.class);
            } catch (ClassNotFoundException e) {
                Log.e("ZEGO", "[Flutter] logNotice failed, class ZegoExpressEngineJniAPI not found.");
            } catch (NoSuchMethodException e) {
                Log.e("ZEGO", "[Flutter] logNotice failed, method logNoticeJni not found.");
            }
        }

        String message = String.format(Locale.ENGLISH, format, args);
        Log.d("ZEGO", "[Flutter] " + message);

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
