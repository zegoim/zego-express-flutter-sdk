
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineUtilities on ZegoExpressEngine {

  /// Starts system performance status monitoring. Support setting the listening interval.
  ///
  /// After starting monitoring, you can receive system performance status via [onPerformanceStatusUpdate] callback.
  /// [onPerformanceStatusUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [millisecond] Monitoring time period of the audio spectrum, in milliseconds, has a value range of [1000, 10000]. Default is 2000 ms.
  Future<void> startPerformanceMonitor({int millisecond}) async {
    return await ZegoExpressImpl.instance.startPerformanceMonitor(millisecond: millisecond);
  }

  /// Stops system performance status monitoring.
  ///
  /// After the monitoring is stopped, the callback of the system performance status will be stopped.
  Future<void> stopPerformanceMonitor() async {
    return await ZegoExpressImpl.instance.stopPerformanceMonitor();
  }

  /// Start network speed test.
  ///
  /// Developers can listen to the [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be called back every 3 seconds.
  /// If an error occurs during the speed measurement process, [onNetworkSpeedTestError] callback will be triggered.
  /// If this function is repeatedly called multiple times, the last invoke's configuration will be used.
  ///
  /// - [config] Network speed test configuration.
  Future<void> startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config) async {
    return await ZegoExpressImpl.instance.startNetworkSpeedTest(config);
  }

  /// Stop network speed test.
  ///
  /// After stopping the speed test, [onNetworkSpeedTestQualityUpdate] will no longer call back.
  Future<void> stopNetworkSpeedTest() async {
    return await ZegoExpressImpl.instance.stopNetworkSpeedTest();
  }

}
