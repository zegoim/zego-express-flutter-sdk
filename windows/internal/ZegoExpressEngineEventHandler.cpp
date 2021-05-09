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

/*std::shared_ptr<ZegoExpressEngineEventHandler> ZegoExpressEngineEventHandler::getSharedPtr()
{
	return shared_from_this();
}*/

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

void ZegoExpressEngineEventHandler::onEngineStateUpdate(EXPRESS::ZegoEngineState state) {

	if (eventSink_) {
		flutter::EncodableMap retMap;
		retMap[FTValue("method")] = FTValue("onEngineStateUpdate");
		retMap[FTValue("state")] = FTValue(state);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRoomStateUpdate(const std::string& roomID, EXPRESS::ZegoRoomState state, int errorCode, const std::string& extendedData) {

	std::cout << "on room state update" << std::endl;
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

/*void ZegoExpressEngineEventHandler::onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) {

}

void ZegoExpressEngineEventHandler::onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels) {
	
}*/

void ZegoExpressEngineEventHandler::onAudioDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType, const EXPRESS::ZegoDeviceInfo& deviceInfo) {


}

void ZegoExpressEngineEventHandler::onAudioDeviceVolumeChanged(EXPRESS::ZegoAudioDeviceType deviceType, const std::string& deviceID, int volume) {

}

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

void ZegoExpressEngineEventHandler::onRemoteCameraStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRemoteCameraStateUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("state")] = FTValue(state);

		eventSink_->Success(retMap);
	}
}

void ZegoExpressEngineEventHandler::onRemoteMicStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

	if (eventSink_) {
		FTMap retMap;
		retMap[FTValue("method")] = FTValue("onRemoteMicStateUpdate");
		retMap[FTValue("streamID")] = FTValue(streamID);
		retMap[FTValue("state")] = FTValue(state);

		eventSink_->Success(retMap);
	}
}