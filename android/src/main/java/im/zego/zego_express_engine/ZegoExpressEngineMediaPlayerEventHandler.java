//
//  ZegoExpressEngineMediaPlayerEventHandler.java
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020/04/29.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import java.util.HashMap;

import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.callback.IZegoMediaPlayerEventHandler;
import im.zego.zegoexpress.constants.ZegoMediaPlayerNetworkEvent;
import im.zego.zegoexpress.constants.ZegoMediaPlayerState;
import io.flutter.plugin.common.EventChannel;

public class ZegoExpressEngineMediaPlayerEventHandler extends IZegoMediaPlayerEventHandler {

    private final EventChannel.EventSink sink;

    ZegoExpressEngineMediaPlayerEventHandler(EventChannel.EventSink sink) {
        super();
        this.sink = sink;
    }

    @Override
    public void onMediaPlayerStateUpdate(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerState state, int errorCode) {
        super.onMediaPlayerStateUpdate(mediaPlayer, state, errorCode);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onMediaPlayerStateUpdate");
        map.put("mediaPlayerIndex", mediaPlayer.getIndex());
        map.put("state", state.value());
        map.put("errorCode", errorCode);

        sink.success(map);
    }

    @Override
    public void onMediaPlayerNetworkEvent(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerNetworkEvent networkEvent) {
        super.onMediaPlayerNetworkEvent(mediaPlayer, networkEvent);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onMediaPlayerNetworkEvent");
        map.put("mediaPlayerIndex", mediaPlayer.getIndex());
        map.put("networkEvent", networkEvent.value());

        sink.success(map);
    }

    @Override
    public void onMediaPlayerPlayingProgress(ZegoMediaPlayer mediaPlayer, long millisecond) {
        super.onMediaPlayerPlayingProgress(mediaPlayer, millisecond);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onMediaPlayerPlayingProgress");
        map.put("mediaPlayerIndex", mediaPlayer.getIndex());
        map.put("millisecond", millisecond);

        sink.success(map);
    }
}
