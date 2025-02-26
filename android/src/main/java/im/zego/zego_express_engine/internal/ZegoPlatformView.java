//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-04-03.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import android.content.Context;
import android.view.TextureView;
import android.view.View;

import io.flutter.plugin.platform.PlatformView;

public class ZegoPlatformView implements PlatformView {

    private TextureView surfaceView;

    ZegoPlatformView(Context context) {
        this.surfaceView = new TextureView(context);
        ZegoLog.log("[ZegoPlatformView] [init] surfaceView:%s", this.surfaceView.hashCode());
    }

    public TextureView getSurfaceView() {
        return this.surfaceView;
    }

    @Override
    public View getView() {
        return this.surfaceView;
    }

    @Override
    public void dispose() {
        ZegoLog.log("[ZegoPlatformView] [dispose] surfaceView:%s", this.surfaceView.hashCode());
        // Can not set surfaceView null, flutter may getView() after dispose()
    }
}
