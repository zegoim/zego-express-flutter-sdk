#pragma once

#define ZEGO_API __attribute__((visibility("default")))


extern ZEGO_API void zego_liveroom_custom_log(const char* content);

extern ZEGO_API void ZegoExternalLogWithNotice(const char* content);
//inline void log_string_notice(const char* content) { zego_liveroom_custom_log(content);}
