package im.zego.zego_express_engine;

import im.zego.zegoexpress.callback.IZegoAudioMixingHandler;
import im.zego.zegoexpress.constants.ZegoAudioChannel;
import im.zego.zegoexpress.constants.ZegoAudioSampleRate;
import im.zego.zegoexpress.entity.ZegoAudioMixingData;
public class ZegoAudioMixingDataManager extends IZegoAudioMixingHandler {

    private static ZegoAudioMixingDataManager instance;
    private IZegoFlutterAudioMixingHandler handler;
    private ZegoAudioMixingData audioMixingData;

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
        if (audioMixingData == null) {
            audioMixingData = new ZegoAudioMixingData();
        }
        audioMixingData.audioData = data.audioData;
        audioMixingData.audioDataLength = data.audioDataLength;
        audioMixingData.SEIData = data.SEIData;
        audioMixingData.SEIDataLength = data.SEIDataLength;
        audioMixingData.param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(data.param.sampleRate.value());
        audioMixingData.param.channel = ZegoAudioChannel.getZegoAudioChannel(data.param.channel.value());
        return audioMixingData;
    }
}
