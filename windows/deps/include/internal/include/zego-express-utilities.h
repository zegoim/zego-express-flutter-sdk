
#ifndef __ZEGO_EXPRESS_UTILITIES_H__
#define __ZEGO_EXPRESS_UTILITIES_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Starts system performance status monitoring. Support setting the listening interval.
 *
 * After starting monitoring, you can receive system performance status via [onPerformanceStatusUpdate] callback.
 * [onPerformanceStatusUpdate] callback notification period is the value set by the parameter.
 *
 * @param millisecond Monitoring time period of the audio spectrum, in milliseconds, has a value range of [1000, 10000]. Default is 2000 ms.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_performance_monitor(unsigned int millisecond);


/**
 * Stops system performance status monitoring.
 *
 * After the monitoring is stopped, the callback of the system performance status will be stopped.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_performance_monitor();


/**
 * start network probe.
 *
 * Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
 * The SDK internally detects http, tcp, and udp in sequence. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is NULL.
 * The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will not work if you call this method repeatedly.
 * Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe as needed.
 *
 * @param config network probe config
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_start_network_probe(struct zego_network_probe_config config);


/**
 * stop network probe.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_network_probe();


/**
 * Test network connectivity.
 *
 * Test network connectivity.
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_test_network_connectivity();


/**
 * Start network speed test.
 *
 * This function cannot be called together with [startPublishingStream], otherwise the network probe will automatically stop.
 * Developers can listen to the [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be called back every 3 seconds.
 * If an error occurs during the speed measurement process, [onNetworkSpeedTestError] callback will be triggered.
 * If this function is repeatedly called multiple times, the last invoke's configuration will be used.
 *
 * @param config Network speed test configuration.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_network_speed_test(struct zego_network_speed_test_config config);


/**
 * Stop network speed test.
 *
 * After stopping the speed test, [onNetworkSpeedTestQualityUpdate] will no longer call back.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_network_speed_test();


/**
 * The system performance status callback.
 *
 * To trigger this callback function, the [startPerformanceMonitor] function must be called to start the system performance monitor.
 * The callback notification period is the setting parameter of [startPerformanceMonitor].
 *
 * @param status The system performance status.
 * @param user_context Context of user.
 */
typedef void(*zego_on_performance_status_update)(struct zego_performance_status status, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_performance_status_update_callback(zego_on_performance_status_update callback_func, void * user_context);


/**
 * Callback for network mode changed.
 *
 * This callback will be called when the device's network mode changes, such as switching from WiFi to 5G, or when the network is disconnected.
 *
 * @param mode Current network mode.
 * @param user_context Context of user.
 */
typedef void(*zego_on_network_mode_changed)(enum zego_network_mode mode, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_network_mode_changed_callback(zego_on_network_mode_changed callback_func, void * user_context);


/**
 * The callback triggered when error occurred when testing network speed.
 *
 * @param error_code The error code corresponding to the network speed test, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param type Uplink or downlink
 * @param user_context Context of user.
 */
typedef void(*zego_on_network_speed_test_error)(int error_code, enum zego_network_speed_test_type type, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_network_speed_test_error_callback(zego_on_network_speed_test_error callback_func, void * user_context);


/**
 * The callback triggered when quality updated when testing network speed.
 *
 * When error occurs or called stopNetworkSpeedTest, this callback will be stopped.
 *
 * @param quality Network speed quality
 * @param type Uplink or downlink
 * @param user_context Context of user.
 */
typedef void(*zego_on_network_speed_test_quality_update)(const struct zego_network_speed_test_quality quality, enum zego_network_speed_test_type type, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_network_speed_test_quality_update_callback(zego_on_network_speed_test_quality_update callback_func, void * user_context);


/**
 * Callback for test network connectivity.
 *
 * @param seq Sequence.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param result Network connectivity test results
 * @param user_context Context of user.
 */
typedef void(*zego_on_test_network_connectivity)(zego_seq seq, zego_error error_code, const struct zego_test_network_connectivity_result result, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_test_network_connectivity_callback(zego_on_test_network_connectivity callback_func, void * user_context);


/**
 * Callback for network probe.
 *
 * @param seq Sequence.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param result Network probe result
 * @param user_context Context of user.
 */
typedef void(*zego_on_network_probe_result)(zego_seq seq, zego_error error_code, const struct zego_network_probe_result result, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_network_probe_result_callback(zego_on_network_probe_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
