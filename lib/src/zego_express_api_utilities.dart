
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
  Future<void> startPerformanceMonitor({int? millisecond}) async {
    return await ZegoExpressImpl.instance.startPerformanceMonitor(millisecond: millisecond);
  }

  /// Stops system performance status monitoring.
  ///
  /// After the monitoring is stopped, the callback of the system performance status will be stopped.
  Future<void> stopPerformanceMonitor() async {
    return await ZegoExpressImpl.instance.stopPerformanceMonitor();
  }

  /// start network probe.
  ///
  /// Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
  /// The SDK internally detects http, tcp, and udp in sequence. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is null.
  /// The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will not work if you call this method repeatedly.
  /// Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe as needed.
  ///
  /// - [config] network probe config
  Future<ZegoNetworkProbeResult> startNetworkProbe(ZegoNetworkProbeConfig config) async {
    return await ZegoExpressImpl.instance.startNetworkProbe(config);
  }

  /// stop network probe.
  Future<void> stopNetworkProbe() async {
    return await ZegoExpressImpl.instance.stopNetworkProbe();
  }

  /// Start network speed test. (Support to set the speed test interval)
  ///
  /// This function should be called before [startPublishingStream], if you call [startPublishingStream] while speed testing, the speed test will automatically stop.
  /// Developers can listen to the [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be called back every 3 seconds.
  /// If an error occurs during the speed measurement process, [onNetworkSpeedTestError] callback will be triggered.
  /// If this function is repeatedly called multiple times, the last invoke's configuration will be used.
  ///
  /// - [config] Network speed test configuration.
  /// - [interval] Interval of network speed test. in milliseconds. Default is 3000 ms."
  Future<void> startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config, {int? interval}) async {
    return await ZegoExpressImpl.instance.startNetworkSpeedTest(config, interval: interval);
  }

  /// Stop network speed test.
  ///
  /// After stopping the speed test, [onNetworkSpeedTestQualityUpdate] will no longer call back.
  Future<void> stopNetworkSpeedTest() async {
    return await ZegoExpressImpl.instance.stopNetworkSpeedTest();
  }

  /// Obtain synchronization network time information, including timestamp and maximum deviation
  Future<ZegoNetworkTimeInfo> getNetworkTimeInfo() async {
    return await ZegoExpressImpl.instance.getNetworkTimeInfo();
  }

}
