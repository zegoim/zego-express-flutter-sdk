//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-04-03.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import java.util.HashMap;

import io.flutter.view.TextureRegistry;

class ZegoTextureRendererController {

    private static ZegoTextureRendererController instance;

    private HashMap<Long, ZegoTextureRenderer> renderers;

    static ZegoTextureRendererController getInstance() {
        if (instance == null) {
            synchronized (ZegoTextureRendererController.class) {
                if (instance == null) {
                    instance = new ZegoTextureRendererController();
                    instance.renderers = new HashMap<>();
                }
            }
        }
        return instance;
    }

    /// Called when dart invoke `createTextureRenderer`
    /// @return textureID
    Long createTextureRenderer(TextureRegistry.SurfaceTextureEntry textureEntry, int viewWidth, int viewHeight) {

        ZegoTextureRenderer renderer = new ZegoTextureRenderer(textureEntry, viewWidth, viewHeight);

        this.renderers.put(renderer.textureID, renderer);

        return renderer.textureID;
    }

    /// Called when dart invoke `updateTextureRendererSize`
    void updateTextureRendererSize(Long textureID, int viewWidth, int viewHeight) {

        ZegoTextureRenderer renderer = this.renderers.get(textureID);

        if (renderer == null) {
            return;
        }

        renderer.updateRenderSize(viewWidth, viewHeight);
    }

    /// Called when dart invoke `destroyTextureRenderer`
    void destroyTextureRenderer(Long textureID) {

        ZegoTextureRenderer renderer = this.renderers.get(textureID);

        if (renderer == null) {
            return;
        }

        this.renderers.remove(textureID);
        renderer.release();
    }

    /// Get TextureRenderer to pass to native when dart invoke `startPreview` or `startPlayingStream`
    ZegoTextureRenderer getTextureRenderer(Long textureID) {
        return this.renderers.get(textureID);
    }

}
