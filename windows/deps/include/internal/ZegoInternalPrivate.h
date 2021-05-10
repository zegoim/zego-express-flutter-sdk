#pragma once

#include "./include/zego-express-defines.h"

ZEGO_BEGIN_DECLS

extern const char *zego_express_get_print_debug_info(int module, const char *func_name, zego_error error_code);
extern void zego_express_trigger_on_debug_error(int error_code, const char *func_name, const char *info);
extern void zego_express_trigger_on_api_called_result(int error_code, const char* func_name, const char* info);
extern void zego_express_show_debug_message_box(zego_language language, zego_error error_code, const char *func_name, const char *error_info);
extern void zego_express_post_windows_message(void *message);
extern void zego_register_recv_windows_message_callback(void *callback, void *user_context);
extern void zego_express_set_platform_language(enum zego_platform_language);
extern void zego_express_custom_log(const char *log, const char *module);

ZEGO_END_DECLS
