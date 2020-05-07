//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-04-03.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import android.content.Context;

import java.util.HashMap;

import io.flutter.plugin.common.MessageCodec;
import io.flutter.plugin.common.StandardMessageCodec;
import io.flutter.plugin.platform.PlatformView;
import io.flutter.plugin.platform.PlatformViewFactory;

public class ZegoPlatformViewFactory extends PlatformViewFactory {

    private static ZegoPlatformViewFactory instance;

    private HashMap<Integer, ZegoPlatformView> platformViews;

    private ZegoPlatformViewFactory(MessageCodec<Object> createArgsCodec) {
        super(createArgsCodec);
        this.platformViews = new HashMap<>();
    }

    static ZegoPlatformViewFactory getInstance() {
        if (instance == null) {
            synchronized (ZegoPlatformViewFactory.class) {
                if (instance == null) {
                    instance = new ZegoPlatformViewFactory(StandardMessageCodec.INSTANCE);
                }
            }
        }
        return instance;
    }

    /// Called when dart invoke `destroyPlatformView`
    void destroyPlatformView(int viewID) {
        this.platformViews.remove(viewID);
    }

    /// Get PlatfromView to pass to native when dart invoke `startPreview` or `startPlayingStream`
    ZegoPlatformView getPlatformView(int viewID) {
        return this.platformViews.get(viewID);
    }


    private void addView(int viewID, ZegoPlatformView view) {
        this.platformViews.put(viewID, view);
    }

    @Override
    public PlatformView create(Context context, int viewId, Object args) {
        ZegoPlatformView view = new ZegoPlatformView(context);
        this.addView(viewId, view);
        return view;
    }
}
