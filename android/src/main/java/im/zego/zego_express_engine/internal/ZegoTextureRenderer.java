//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-04-03.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import android.graphics.SurfaceTexture;
import android.view.Surface;

import io.flutter.view.TextureRegistry;

public class ZegoTextureRenderer {

    final long textureID;

    private final TextureRegistry.SurfaceTextureEntry textureEntry;
    private final SurfaceTexture surfaceTexture;
    private Surface surface;

    private int viewWidth;
    private int viewHeight;

    ZegoTextureRenderer(TextureRegistry.SurfaceTextureEntry textureEntry, int viewWidth, int viewHeight) {
        this.textureEntry = textureEntry;
        this.textureID = textureEntry.id();
        this.surfaceTexture = textureEntry.surfaceTexture();
        this.surfaceTexture.setDefaultBufferSize(viewWidth, viewHeight);
        this.surface = new Surface(this.surfaceTexture);

        this.viewWidth = viewWidth;
        this.viewHeight = viewHeight;

        ZegoLog.log("[ZegoTextureRenderer] [init] renderer:%s", this.hashCode());
    }

    void updateRenderSize(final int width, final int height) {

        if (this.viewWidth != width || this.viewHeight != height) {
            this.viewWidth = width;
            this.viewHeight = height;

            this.surfaceTexture.setDefaultBufferSize(viewWidth, viewHeight);
            this.surface.release();
            this.surface = new Surface(this.surfaceTexture);
        }
    }

    void release() {
        this.textureEntry.release();

        if (this.surface != null) {
            surface.release();
        }

        ZegoLog.log("[ZegoTextureRenderer] [release] renderer:%s", this.hashCode());
    }

    Surface getSurface() {
        return this.surface;
    }

}
