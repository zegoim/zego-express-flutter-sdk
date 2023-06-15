#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "ZegoLog.h"
#include <ZegoInternalPrivate.h>

#undef __MODULE__
#define __MODULE__ "Flutter"

void ZF::logInfo(const char* format, ...) {
    char log_buf[4096] = { 0 };
    va_list la;
    va_start(la, format);
    vsprintf(log_buf, format, la);
    va_end(la);

    printf("flutter: %s\n", log_buf);

    zego_express_custom_log(log_buf, __MODULE__);
}