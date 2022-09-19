//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-04-03.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import android.view.Surface;

import java.util.HashMap;
import java.util.Locale;

import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.entity.ZegoCanvas;
import im.zego.zegoexpress.entity.ZegoPlayerConfig;
import io.flutter.view.TextureRegistry;

public class ZegoTextureRendererController {

    private volatile static ZegoTextureRendererController instance;

    private HashMap<Long, ZegoTextureRenderer> renderers;

    public HashMap<ZegoPublishChannel, ZegoCanvas> previewCanvasInUse = new HashMap<>();

    public HashMap<String, ZegoCanvas> playerCanvasInUse = new HashMap<>(); // Key is playing streamID

    public HashMap<String, ZegoPlayerConfig> playerConfigInUse = new HashMap<>(); // Key is playing streamID

    public HashMap<Integer, ZegoCanvas> mediaPlayerCanvasInUse = new HashMap<>(); // Key is media player index

    public static ZegoTextureRendererController getInstance() {
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

        ZegoLog.log("[createTextureRenderer] textureID:%d, renderer:%s", renderer.textureID, renderer.hashCode());

        this.renderers.put(renderer.textureID, renderer);

        logCurrentRenderers();

        return renderer.textureID;
    }

    /// Called when dart invoke `updateTextureRendererSize`
    Boolean updateTextureRendererSize(Long textureID, int viewWidth, int viewHeight) {

        ZegoTextureRenderer renderer = this.renderers.get(textureID);

        if (renderer == null) {
            ZegoLog.log("[updateTextureRendererSize] renderer for textureID:%d not exists", textureID);
            logCurrentRenderers();
            return false;
        }

        // ZegoLog.log("[updateTextureRendererSize] textureID:%d, renderer:%s", textureID, renderer.hashCode());

        Surface originSurface = renderer.getSurface();

        renderer.updateRenderSize(viewWidth, viewHeight);

        for (ZegoPublishChannel channel : previewCanvasInUse.keySet()) {
            ZegoCanvas canvas = previewCanvasInUse.get(channel);
            if (canvas != null && originSurface.equals(canvas.view)) {
                canvas.view = renderer.getSurface();
                ZegoExpressEngine.getEngine().startPreview(canvas, channel);
                return true;
            }
        }

        for (String streamID : playerCanvasInUse.keySet()) {
            ZegoCanvas canvas = playerCanvasInUse.get(streamID);
            ZegoPlayerConfig config = playerConfigInUse.get(streamID);
            if (canvas != null && originSurface.equals(canvas.view)) {
                canvas.view = renderer.getSurface();
                ZegoExpressEngine.getEngine().startPlayingStream(streamID, canvas, config);
                return true;
            }
        }

        for (Integer index : mediaPlayerCanvasInUse.keySet()) {
            ZegoCanvas canvas = mediaPlayerCanvasInUse.get(index);
            if (canvas != null && originSurface.equals(canvas.view)) {
                ZegoMediaPlayer mediaPlayer = ZegoExpressEngineMethodHandler.getMediaPlayer(index);
                if (mediaPlayer != null) {
                    canvas.view = renderer.getSurface();
                    mediaPlayer.setPlayerCanvas(canvas);
                }
            }
        }

        logCurrentRenderers();

        return true;
    }

    /// Called when dart invoke `destroyTextureRenderer`
    Boolean destroyTextureRenderer(Long textureID) {

        ZegoTextureRenderer renderer = this.renderers.get(textureID);

        if (renderer == null) {
            ZegoLog.log("[destroyTextureRenderer] renderer for textureID:%d not exists", textureID);
            logCurrentRenderers();
            return false;
        }

        ZegoLog.log("[destroyTextureRenderer] textureID:%d, renderer: %d", textureID, renderer.hashCode());

        this.renderers.remove(textureID);
        renderer.release();

        logCurrentRenderers();

        return true;
    }

    /// Get TextureRenderer to pass to native when dart invoke `startPreview` or `startPlayingStream`
    ZegoTextureRenderer getTextureRenderer(Long textureID) {

        ZegoTextureRenderer renderer = this.renderers.get(textureID);

        ZegoLog.log("[getTextureRenderer] textureID:%d, renderer:%s", textureID, renderer == null ? "null" : renderer.hashCode());

        logCurrentRenderers();

        return renderer;
    }

    private void logCurrentRenderers() {
        StringBuilder desc = new StringBuilder();
        for (Long id: this.renderers.keySet()) {
            ZegoTextureRenderer eachRenderer = this.renderers.get(id);
            desc.append(String.format(Locale.ENGLISH, "[ID:%d|Rnd:%s] ", id, eachRenderer == null ? "null" : eachRenderer.hashCode()));
        }
        ZegoLog.log("[ZegoTextureRendererController] currentRenderers: %s", desc.toString());
    }

}
