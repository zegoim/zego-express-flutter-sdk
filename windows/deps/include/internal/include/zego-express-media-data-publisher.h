
#ifndef __ZEGO_EXPRESS_MEDIA_DATA_PUBLISHER_H__
#define __ZEGO_EXPRESS_MEDIA_DATA_PUBLISHER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API zego_instance EXP_CALL zego_express_create_media_data_publisher();


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API void EXP_CALL zego_express_destroy_media_data_publisher(zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_add_media_file_path(const char * path, bool is_clear, zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_reset(zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_set_video_send_delay_time(int delay_time, zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_seek_to(unsigned long long millisecond, zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API unsigned long long EXP_CALL zego_express_media_data_publisher_get_total_duration(zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
ZEGOEXP_API unsigned long long EXP_CALL zego_express_media_data_publisher_get_current_duration(zego_instance instance_index);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
typedef void(*zego_on_media_data_publisher_file_open)(const char * path, zego_instance instance_index, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_open_callback(zego_on_media_data_publisher_file_open callback_func, void * user_context);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
typedef void(*zego_on_media_data_publisher_file_close)(zego_error error_code, const char * path, zego_instance instance_index, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_close_callback(zego_on_media_data_publisher_file_close callback_func, void * user_context);


/**
 * @deprecated This API is invalid in this SDK package and should not be used. If you need to use this API, please contact ZEGO technical support.
 */
typedef void(*zego_on_media_data_publisher_file_data_begin)(const char * path, zego_instance instance_index, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_data_begin_callback(zego_on_media_data_publisher_file_data_begin callback_func, void * user_context);



ZEGO_END_DECLS

#endif
