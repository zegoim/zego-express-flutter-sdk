package im.zego.zego_express_engine;

import im.zego.zegoexpress.callback.IZegoAudioMixingHandler;
import im.zego.zegoexpress.constants.ZegoAudioChannel;
import im.zego.zegoexpress.constants.ZegoAudioSampleRate;
import im.zego.zegoexpress.entity.ZegoAudioMixingData;
public class ZegoAudioMixingDataManager extends IZegoAudioMixingHandler {

    private static ZegoAudioMixingDataManager instance;
    private IZegoFlutterAudioMixingHandler handler;

    private ZegoAudioMixingDataManager() {

    }

    public static ZegoAudioMixingDataManager getInstance() {
        if (instance == null) {
            instance = new ZegoAudioMixingDataManager();
        }
        return instance;
    }

    public void setAudioMixingHandler(IZegoFlutterAudioMixingHandler handler) {
        if (handler == null) {
            return;
        }
        this.handler = handler;
    }

    @Override
    public ZegoAudioMixingData onAudioMixingCopyData(int expectedDataLength) {
        if (handler == null) {
            return null;
        }
        ZGFlutterAudioMixData data = handler.onAudioMixingCopyData(expectedDataLength);
        if (data == null) {
            return null;
        }
        ZegoAudioMixingData result = new ZegoAudioMixingData();
        result.audioData = data.audioData;
        result.audioDataLength = data.audioDataLength;
        result.SEIData = data.SEIData;
        result.SEIDataLength = data.SEIDataLength;
        result.param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(data.param.sampleRate.value());
        result.param.channel = ZegoAudioChannel.getZegoAudioChannel(data.param.channel.value());
        return result;
    }
}
