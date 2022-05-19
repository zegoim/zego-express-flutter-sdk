#include "ZegoExpressEngineEventHandler.h"
#include <flutter/encodable_value.h>
#include <memory>

std::shared_ptr<ZegoExpressEngineEventHandler> ZegoExpressEngineEventHandler::m_instance = nullptr;

void ZegoExpressEngineEventHandler::setEventSink(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& eventSink)
{
	eventSink_ = std::move(eventSink);
}

void ZegoExpressEngineEventHandler::clearEventSink()
{
	eventSink_.reset();
}

void ZegoExpressEngineEventHandler::onDebugError(int errorCode, const std::string& funcName, const std::string& info) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onDebugError");
		retMap[FTValue("errorCode")] = FTValue(errorCode);
		retMap[FTValue("funcName")] = FTValue(funcName);
		retMap[FTValue("info")] = FTValue(info);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onApiCalledResult(int errorCode, const std::string& funcName, const std::string& info) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onApiCalledResult");
		retMap[FTValue("errorCode")] = FTValue(errorCode);
		retMap[FTValue("funcName")] = FTValue(funcName);
		retMap[FTValue("info")] = FTValue(info);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onEngineStateUpdate(EXPRESS::ZegoEngineState state) {

	if (eventSink_) {
		flutter::EncodableMap retMap;
		retMap[FTValue("method")] = FTValue("onEngineStateUpdate");
		retMap[FTValue("state")] = FTValue(state);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomStateUpdate(const std::string& roomID, EXPRESS::ZegoRoomState state, int errorCode, const std::string& extendedData) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomStateUpdate");
		retMap[FTValue("state")] = FTValue(state);
		retMap[FTValue("errorCode")] = FTValue(errorCode);
		retMap[FTValue("roomID")] = FTValue(roomID);
		retMap[FTValue("extendedData")] = FTValue(extendedData);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomStateChanged(const std::string& roomID, EXPRESS::ZegoRoomStateChangedReason reason, int errorCode, const std::string& extendedData) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomStateChanged");
		retMap[FTValue("reason")] = FTValue(reason);
		retMap[FTValue("errorCode")] = FTValue(errorCode);
		retMap[FTValue("roomID")] = FTValue(roomID);

		std::string extendedData_ = extendedData.empty()? "{}": extendedData;
		retMap[FTValue("extendedData")] = FTValue(extendedData_);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomUserUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoUser>& userList) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomUserUpdate");
		retMap[FTValue("updateType")] = FTValue(updateType);
		retMap[FTValue("roomID")] = FTValue(roomID);

		FTArray userListArray;
		for (auto& user : userList) {
			FTMap userMap;
			userMap[FTValue("userID")] = FTValue(user.userID);
			userMap[FTValue("userName")] = FTValue(user.userName);
			userListArray.emplace_back(FTValue(userMap));
		}
		retMap[FTValue("userList")] = FTValue(userListArray);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomOnlineUserCountUpdate(const std::string& roomID, int count) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomOnlineUserCountUpdate");
		retMap[FTValue("count")] = FTValue(count);
		retMap[FTValue("roomID")] = FTValue(roomID);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomStreamUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoStream>& streamList, const std::string& extendedData) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomStreamUpdate");
		retMap[FTValue("updateType")] = FTValue(updateType);
		retMap[FTValue("roomID")] = FTValue(roomID);
		retMap[FTValue("extendedData")] = FTValue(extendedData);

		FTArray streamListArray;
		for (auto& stream : streamList) {
			FTMap streamMap;
			FTMap userMap;
			userMap[FTValue("userID")] = FTValue(stream.user.userID);
			userMap[FTValue("userName")] = FTValue(stream.user.userName);

			streamMap[FTValue("streamID")] = FTValue(stream.streamID);
			streamMap[FTValue("extraInfo")] = FTValue(stream.extraInfo);
			streamMap[FTValue("user")] = FTValue(userMap);

			streamListArray.emplace_back(FTValue(streamMap));
		}
		retMap[FTValue("streamList")] = FTValue(streamListArray);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoStream>& streamList) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomStreamExtraInfoUpdate");
		retMap[FTValue("roomID")] = FTValue(roomID);

		FTArray streamListArray;
		for (auto& stream : streamList) {
			FTMap streamMap;
			FTMap userMap;
			userMap[FTValue("userID")] = FTValue(stream.user.userID);
			userMap[FTValue("userName")] = FTValue(stream.user.userName);

			streamMap[FTValue("streamID")] = FTValue(stream.streamID);
			streamMap[FTValue("extraInfo")] = FTValue(stream.extraInfo);
			streamMap[FTValue("user")] = FTValue(userMap);

			streamListArray.emplace_back(FTValue(streamMap));
		}
		retMap[FTValue("streamList")] = FTValue(streamListArray);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoRoomExtraInfo>& roomExtraInfoList) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRoomExtraInfoUpdate");
		retMap[FTValue("roomID")] = FTValue(roomID);

		FTArray roomExtraInfoListArray;
		for (auto& roomExtraInfo : roomExtraInfoList) {
			FTMap infoMap;
			FTMap userMap;
			userMap[FTValue("userID")] = FTValue(roomExtraInfo.updateUser.userID);
			userMap[FTValue("userName")] = FTValue(roomExtraInfo.updateUser.userName);

			infoMap[FTValue("key")] = FTValue(roomExtraInfo.key);
			infoMap[FTValue("value")] = FTValue(roomExtraInfo.value);
			infoMap[FTValue("updateTime")] = FTValue((int64_t)roomExtraInfo.updateTime);
			infoMap[FTValue("updateUser")] = FTValue(userMap);

			roomExtraInfoListArray.emplace_back(FTValue(infoMap));
		}
		retMap[FTValue("roomExtraInfoList")] = flutter::EncodableValue(roomExtraInfoListArray);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPublisherStateUpdate(const std::string& streamID, EXPRESS::ZegoPublisherState state, int errorCode, const std::string& extendedData) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPublisherStateUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("state")] = flutter::EncodableValue(state);
		retMap[FTValue("errorCode")] = flutter::EncodableValue(errorCode);
		retMap[FTValue("extendedData")] = flutter::EncodableValue(extendedData);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPublisherQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPublishStreamQuality& quality) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPublisherQualityUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);

		FTMap qualityMap;
		qualityMap[FTValue("videoCaptureFPS")] = FTValue(quality.videoCaptureFPS);
		qualityMap[FTValue("videoEncodeFPS")] = FTValue(quality.videoEncodeFPS);
		qualityMap[FTValue("videoSendFPS")] = FTValue(quality.videoSendFPS);
		qualityMap[FTValue("videoKBPS")] = FTValue(quality.videoKBPS);
		qualityMap[FTValue("audioCaptureFPS")] = FTValue(quality.audioCaptureFPS);
		qualityMap[FTValue("audioSendFPS")] = FTValue(quality.audioSendFPS);
		qualityMap[FTValue("audioKBPS")] = FTValue(quality.audioKBPS);
		qualityMap[FTValue("rtt")] = FTValue(quality.rtt);
		qualityMap[FTValue("packetLostRate")] = FTValue(quality.packetLostRate);
		qualityMap[FTValue("level")] = FTValue(quality.level);
		qualityMap[FTValue("isHardwareEncode")] = FTValue(quality.isHardwareEncode);
		qualityMap[FTValue("videoCodecID")] = FTValue(quality.videoCodecID);
		qualityMap[FTValue("totalSendBytes")] = FTValue(quality.totalSendBytes);
		qualityMap[FTValue("audioSendBytes")] = FTValue(quality.audioSendBytes);
		qualityMap[FTValue("videoSendBytes")] = FTValue(quality.videoSendBytes);

		retMap[FTValue("quality")] = FTValue(qualityMap);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPublisherCapturedAudioFirstFrame() {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPublisherCapturedAudioFirstFrame");

		eventSink_->Success(retMap);
	}
}

/*void ZegoExpressEngineEventHandler::onPublisherCapturedVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) {

}

void ZegoExpressEngineEventHandler::onPublisherRenderVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) {

}

void ZegoExpressEngineEventHandler::onPublisherVideoSizeChanged(int width, int height, EXPRESS::ZegoPublishChannel channel) {

}

void ZegoExpressEngineEventHandler::onPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) {

}*/

void ZegoExpressEngineEventHandler::onPublisherStreamEvent(EXPRESS::ZegoStreamEvent eventID, const std::string& streamID, const std::string& extraInfo) {

	if (eventSink_) {
		FTMap retMap;
		int eventID_ = -1;
		switch (eventID)
		{
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_START:
			eventID_ = 0;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_SUCCESS:
			eventID_ = 1;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_FAIL:
			eventID_ = 2;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PUBLISH_START:
			eventID_ = 3;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PUBLISH_SUCCESS:
			eventID_ = 4;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PUBLISH_FAIL:
			eventID_ = 5;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_END:
			eventID_ = 6;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_START:
			eventID_ = 7;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_SUCCESS:
			eventID_ = 8;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_FAIL:
			eventID_ = 9;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PLAY_START:
			eventID_ = 10;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PLAY_SUCCESS:
			eventID_ = 11;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PLAY_FAIL:
			eventID_ = 12;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_END:
			eventID_ = 13;
			break;
		default:
			break;
		}
		retMap[FTValue("method")] = FTValue("onPublisherStreamEvent");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("eventID")] = flutter::EncodableValue(eventID_);
		retMap[FTValue("streamID")] = flutter::EncodableValue(streamID);
		retMap[FTValue("extraInfo")] = flutter::EncodableValue(extraInfo);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerStateUpdate(const std::string& streamID, EXPRESS::ZegoPlayerState state, int errorCode, const std::string& extendedData) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerStateUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("state")] = flutter::EncodableValue(state);
		retMap[FTValue("errorCode")] = flutter::EncodableValue(errorCode);
		retMap[FTValue("extendedData")] = flutter::EncodableValue(extendedData);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPlayStreamQuality& quality) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerQualityUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);

		FTMap qualityMap;
		qualityMap[FTValue("videoRecvFPS")] = FTValue(quality.videoRecvFPS);
		qualityMap[FTValue("videoDejitterFPS")] = FTValue(quality.videoDejitterFPS);
		qualityMap[FTValue("videoDecodeFPS")] = FTValue(quality.videoDecodeFPS);
		qualityMap[FTValue("videoRenderFPS")] = FTValue(quality.videoRenderFPS);
		qualityMap[FTValue("videoKBPS")] = FTValue(quality.videoKBPS);
		qualityMap[FTValue("videoBreakRate")] = FTValue(quality.videoBreakRate);
		qualityMap[FTValue("audioRecvFPS")] = FTValue(quality.audioRecvFPS);
		qualityMap[FTValue("audioDejitterFPS")] = FTValue(quality.audioDejitterFPS);
		qualityMap[FTValue("audioDecodeFPS")] = FTValue(quality.audioDecodeFPS);
		qualityMap[FTValue("audioRenderFPS")] = FTValue(quality.audioRenderFPS);
		qualityMap[FTValue("audioKBPS")] = FTValue(quality.audioKBPS);
		qualityMap[FTValue("audioBreakRate")] = FTValue(quality.audioBreakRate);
		qualityMap[FTValue("mos")] = FTValue(quality.mos);
		qualityMap[FTValue("rtt")] = FTValue(quality.rtt);
		qualityMap[FTValue("packetLostRate")] = FTValue(quality.packetLostRate);
		qualityMap[FTValue("peerToPeerDelay")] = FTValue(quality.peerToPeerDelay);
		qualityMap[FTValue("peerToPeerPacketLostRate")] = FTValue(quality.peerToPeerPacketLostRate);
		qualityMap[FTValue("level")] = FTValue(quality.level);
		qualityMap[FTValue("delay")] = FTValue(quality.delay);
		qualityMap[FTValue("avTimestampDiff")] = FTValue(quality.avTimestampDiff);
		qualityMap[FTValue("isHardwareDecode")] = FTValue(quality.isHardwareDecode);
		qualityMap[FTValue("videoCodecID")] = FTValue(quality.videoCodecID);
		qualityMap[FTValue("totalRecvBytes")] = FTValue(quality.totalRecvBytes);
		qualityMap[FTValue("audioRecvBytes")] = FTValue(quality.audioRecvBytes);
		qualityMap[FTValue("videoRecvBytes")] = FTValue(quality.videoRecvBytes);

		retMap[FTValue("quality")] = FTValue(qualityMap);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerMediaEvent(const std::string& streamID, EXPRESS::ZegoPlayerMediaEvent event) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerMediaEvent");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("event")] = flutter::EncodableValue(event);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioFirstFrame(const std::string& streamID) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerRecvAudioFirstFrame");
		retMap[FTValue("streamID")] = FTValue(streamID);

		eventSink_->Success(retMap);
	}
}

/*void ZegoExpressEngineEventHandler::onPlayerRecvVideoFirstFrame(const std::string& streamID) {

}

void ZegoExpressEngineEventHandler::onPlayerRenderVideoFirstFrame(const std::string& streamID) {

}

void ZegoExpressEngineEventHandler::onPlayerVideoSizeChanged(const std::string& streamID, int width, int height) {

}*/

void ZegoExpressEngineEventHandler::onPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerRecvSEI");
		retMap[FTValue("streamID")] = FTValue(streamID);

		auto nonConstData = const_cast<unsigned char*>(data);
		std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

		retMap[FTValue("data")] = FTValue(dataArray);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioSideInfo(const std::string& streamID, const unsigned char* data, unsigned int dataLength) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerRecvAudioSideInfo");
		retMap[FTValue("streamID")] = FTValue(streamID);

		auto nonConstData = const_cast<unsigned char*>(data);
		std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

		retMap[FTValue("data")] = FTValue(dataArray);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerStreamEvent(EXPRESS::ZegoStreamEvent eventID, const std::string& streamID, const std::string& extraInfo) {

	if (eventSink_) {
		FTMap retMap;
		int eventID_ = -1;
		switch (eventID)
		{
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_START:
			eventID_ = 0;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_SUCCESS:
			eventID_ = 1;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_FAIL:
			eventID_ = 2;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PUBLISH_START:
			eventID_ = 3;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PUBLISH_SUCCESS:
			eventID_ = 4;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PUBLISH_FAIL:
			eventID_ = 5;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PUBLISH_END:
			eventID_ = 6;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_START:
			eventID_ = 7;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_SUCCESS:
			eventID_ = 8;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_FAIL:
			eventID_ = 9;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PLAY_START:
			eventID_ = 10;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PLAY_SUCCESS:
			eventID_ = 11;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_RETRY_PLAY_FAIL:
			eventID_ = 12;
			break;
		case EXPRESS::ZEGO_STREAM_EVENT_PLAY_END:
			eventID_ = 13;
			break;
		default:
			break;
		}
		retMap[FTValue("method")] = FTValue("onPlayerStreamEvent");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("eventID")] = flutter::EncodableValue(eventID_);
		retMap[FTValue("streamID")] = flutter::EncodableValue(streamID);
		retMap[FTValue("extraInfo")] = flutter::EncodableValue(extraInfo);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) {
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixerRelayCDNStateUpdate");
        retMap[FTValue("taskID")] = FTValue(taskID);

        FTArray infoListArray;
        for (auto& info : infoList) {
            FTMap infoMap;
            infoMap[FTValue("url")] = FTValue(info.url);
            infoMap[FTValue("state")] = FTValue((int32_t)info.state);
            infoMap[FTValue("updateReason")] = FTValue((int32_t)info.updateReason);
            infoMap[FTValue("stateTime")] = FTValue((int32_t)info.stateTime);
            infoListArray.emplace_back(FTValue(infoMap));
        }
        retMap[FTValue("infoList")] = FTValue(infoListArray);
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels) {
	if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixerSoundLevelUpdate");

        FTMap soundLevelsMap;
        for (auto& soundlevel : soundLevels) {
            soundLevelsMap[FTValue((int32_t)soundlevel.first)] = FTValue(soundlevel.second);
        }
        retMap[FTValue("soundLevels")] = FTValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType, const EXPRESS::ZegoDeviceInfo& deviceInfo) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onAudioDeviceStateChanged");

		retMap[FTValue("updateType")] = FTValue((int32_t)updateType);
		retMap[FTValue("deviceType")] = FTValue((int32_t)deviceType);

		FTMap deviceInfoMap;
		deviceInfoMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
		deviceInfoMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);

		retMap[FTValue("deviceInfo")] = FTValue(deviceInfoMap);

		eventSink_->Success(retMap);
	}
}

/*void ZegoExpressEngineEventHandler::onAudioDeviceVolumeChanged(EXPRESS::ZegoAudioDeviceType deviceType, const std::string& deviceID, int volume) {

}*/

/*void ZegoExpressEngineEventHandler::onVideoDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, const EXPRESS::ZegoDeviceInfo& deviceInfo) {

}*/

void ZegoExpressEngineEventHandler::onCapturedSoundLevelUpdate(float soundLevel) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onCapturedSoundLevelUpdate");
		retMap[FTValue("soundLevel")] = FTValue(soundLevel);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRemoteSoundLevelUpdate");

		FTMap soundLevelsMap;
		for (auto& soundlevel : soundLevels) {
			soundLevelsMap[FTValue(soundlevel.first)] = FTValue(soundlevel.second);
		}
		retMap[FTValue("soundLevels")] = FTValue(soundLevelsMap);

		eventSink_->Success(retMap);
	}
}

/*void ZegoExpressEngineEventHandler::onCapturedAudioSpectrumUpdate(const EXPRESS::ZegoAudioSpectrum& audioSpectrum) {

}

void ZegoExpressEngineEventHandler::onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum>& audioSpectrums) {

}*/

void ZegoExpressEngineEventHandler::onDeviceError(int errorCode, const std::string& deviceName) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onDeviceError");
		retMap[FTValue("errorCode")] = FTValue(errorCode);
		retMap[FTValue("deviceName")] = FTValue(deviceName);

		eventSink_->Success(retMap);
	}
}

/*void ZegoExpressEngineEventHandler::onRemoteCameraStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRemoteCameraStateUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("state")] = FTValue(state);

		eventSink_->Success(retMap);
	}
}*/

void ZegoExpressEngineEventHandler::onRemoteMicStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRemoteMicStateUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("state")] = FTValue(state);

		eventSink_->Success(retMap);
	}
}

// Audio Effect Player
void ZegoExpressEngineEventHandler::onAudioEffectPlayStateUpdate(EXPRESS::IZegoAudioEffectPlayer* audioEffectPlayer, unsigned int audioEffectID, EXPRESS::ZegoAudioEffectPlayState state, int errorCode)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onAudioEffectPlayStateUpdate");
		retMap[FTValue("audioEffectPlayerIndex")] = FTValue(audioEffectPlayer->getIndex());
		retMap[FTValue("audioEffectID")] = FTValue((int32_t)audioEffectID);
		retMap[FTValue("state")] = FTValue(state);
		retMap[FTValue("errorCode")] = FTValue(errorCode);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMediaPlayerStateUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerState state, int errorCode)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMediaPlayerStateUpdate");
		retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
		retMap[FTValue("state")] = FTValue(state);
		retMap[FTValue("errorCode")] = FTValue(errorCode);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMediaPlayerNetworkEvent(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerNetworkEvent networkEvent)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMediaPlayerNetworkEvent");
		retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
		retMap[FTValue("networkEvent")] = FTValue(networkEvent);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMediaPlayerPlayingProgress(EXPRESS::IZegoMediaPlayer* mediaPlayer, unsigned long long millisecond)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMediaPlayerPlayingProgress");
		retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
		// TODO: convert need test?
		retMap[FTValue("millisecond")] = FTValue((int64_t)millisecond);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMediaPlayerRecvSEI(EXPRESS::IZegoMediaPlayer* mediaPlayer, const unsigned char* data, unsigned int dataLength)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMediaPlayerRecvSEI");
		retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
		// TODO: convert need test?
		std::vector<uint8_t> vec_data(data, data + dataLength);
		retMap[FTValue("data")] = FTValue(vec_data);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMediaPlayerSoundLevelUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, float soundLevel)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMediaPlayerSoundLevelUpdate");
		retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

		retMap[FTValue("soundLevel")] = FTValue(soundLevel);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMediaPlayerFrequencySpectrumUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, const EXPRESS::ZegoAudioSpectrum& spectrumList)
{
	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMediaPlayerFrequencySpectrumUpdate");
		retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

		retMap[FTValue("soundLevel")] = FTValue(spectrumList);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onCapturedAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param)
{
	if(eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onCapturedAudioData");
		std::vector<uint8_t> dataVec(data, data + dataLength);
		retMap[FTValue("data")] = dataVec;
		retMap[FTValue("dataLength")] = (int)dataLength;
		FTMap paramMap;
		paramMap[FTValue("sampleRate")] = getAudioSampleRateIndex(param.sampleRate);
		paramMap[FTValue("channel")] = (int32_t)param.channel;
		retMap[FTValue("param")] = paramMap;

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlaybackAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param)
{
	if(eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlaybackAudioData");
		std::vector<uint8_t> dataVec(data, data + dataLength);
		retMap[FTValue("data")] = dataVec;
		retMap[FTValue("dataLength")] = (int)dataLength;
		FTMap paramMap;
		paramMap[FTValue("sampleRate")] = getAudioSampleRateIndex(param.sampleRate);
		paramMap[FTValue("channel")] = param.channel;
		retMap[FTValue("param")] = paramMap;

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onMixedAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param)
{
	if(eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onMixedAudioData");
		std::vector<uint8_t> dataVec(data, data + dataLength);
		retMap[FTValue("data")] = dataVec;
		retMap[FTValue("dataLength")] = (int)dataLength;
		FTMap paramMap;
		paramMap[FTValue("sampleRate")] = getAudioSampleRateIndex(param.sampleRate);
		paramMap[FTValue("channel")] = param.channel;
		retMap[FTValue("param")] = paramMap;

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onPlayerAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param, const std::string& streamID)
{
	if(eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onPlayerAudioData");
		std::vector<uint8_t> dataVec(data, data + dataLength);
		retMap[FTValue("data")] = dataVec;
		retMap[FTValue("dataLength")] = (int)dataLength;
		FTMap paramMap;
		paramMap[FTValue("sampleRate")] = getAudioSampleRateIndex(param.sampleRate);
		paramMap[FTValue("channel")] = param.channel;
		retMap[FTValue("param")] = paramMap;
		retMap[FTValue("streamID")] = streamID;

		eventSink_->Success(retMap);
	}
}

int ZegoExpressEngineEventHandler::getAudioSampleRateIndex(EXPRESS::ZegoAudioSampleRate sampleRate)
{
	switch (sampleRate)
	{
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_UNKNOWN:
		return 0;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_8K:
		return 1;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_16K:
		return 2;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_22K:
		return 3;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_24K:
		return 4;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_32K:
		return 5;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_44K:
		return 6;
	case EXPRESS::ZegoAudioSampleRate::ZEGO_AUDIO_SAMPLE_RATE_48K:
		return 7;
	}
	return 0;
}

void ZegoExpressEngineEventHandler::onCapturedDataRecordStateUpdate(EXPRESS::ZegoDataRecordState state, int errorCode, EXPRESS::ZegoDataRecordConfig config, EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        FTMap configMap;
		retMap[FTValue("method")] = FTValue("onCapturedDataRecordStateUpdate");

        configMap[FTValue("filePath")] = FTValue(config.filePath);
        configMap[FTValue("recordType")] = FTValue(config.recordType);

        retMap[FTValue("state")] = FTValue(state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("config")] = FTValue(configMap);
        retMap[FTValue("channel")] = FTValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedDataRecordProgressUpdate(EXPRESS::ZegoDataRecordProgress progress, EXPRESS::ZegoDataRecordConfig config, EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        FTMap progressMap;
        FTMap configMap;
		retMap[FTValue("method")] = FTValue("onCapturedDataRecordProgressUpdate");

        progressMap[FTValue("currentFileSize")] = FTValue((int32_t)progress.currentFileSize);
        progressMap[FTValue("duration")] = FTValue((int32_t)progress.duration);

        configMap[FTValue("filePath")] = FTValue(config.filePath);
        configMap[FTValue("recordType")] = FTValue(config.recordType);

        retMap[FTValue("config")] = FTValue(configMap);
        retMap[FTValue("progress")] = FTValue(progressMap);
        retMap[FTValue("channel")] = FTValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onDownloadProgressUpdate(EXPRESS::IZegoCopyrightedMusic* copyrightedMusic, const std::string& resourceID, float progressRate)
{
	if (eventSink_) {
        FTMap retMap;
		retMap[FTValue("method")] = FTValue("onDownloadProgressUpdate");

        retMap[FTValue("resourceID")] = FTValue(resourceID);
        retMap[FTValue("progressRate")] = FTValue(progressRate);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCurrentPitchValueUpdate(EXPRESS::IZegoCopyrightedMusic* copyrightedMusic, const std::string& resourceID, int currentDuration, int pitchValue) 
{
		if (eventSink_) {
        FTMap retMap;
		retMap[FTValue("method")] = FTValue("onCurrentPitchValueUpdate");

        retMap[FTValue("resourceID")] = FTValue(resourceID);
        retMap[FTValue("currentDuration")] = FTValue(currentDuration);
		retMap[FTValue("pitchValue")] = FTValue(pitchValue);

        eventSink_->Success(retMap);
    }
}
