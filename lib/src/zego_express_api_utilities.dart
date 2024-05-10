import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineUtilities on ZegoExpressEngine {
  /// Start system performance monitoring.
  ///
  /// Available since: 1.19.0
  /// Description: Start system performance monitoring, monitor system or App's CPU usage and memory usage. Support set the monitoring interval.
  /// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Related callbacks: After starting monitoring, you can receive system performance status via [onPerformanceStatusUpdate] callback. [onPerformanceStatusUpdate] callback notification period is the value set by millisecond parameter.
  /// Related APIs: Call [stopPerformanceMonitor] to stop system performance monitoring.
  ///
  /// - [millisecond] Monitoring time period(in milliseconds), the value range is [1000, 10000]. Default value is 2000 ms.
  Future<void> startPerformanceMonitor({int? millisecond}) async {
    return await ZegoExpressImpl.instance
        .startPerformanceMonitor(millisecond: millisecond);
  }

  /// Stop system performance monitoring.
  ///
  /// Available since: 1.19.0
  /// Description: Stop system performance monitoring. After the monitoring is stopped, the [onPerformanceStatusUpdate] callback will not triggered.
  /// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Related APIs: Call [startPerformanceMonitor] to start system performance monitoring.
  Future<void> stopPerformanceMonitor() async {
    return await ZegoExpressImpl.instance.stopPerformanceMonitor();
  }

  /// Start network probe.
  ///
  /// Available since: 2.3.0
  /// Description: Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
  /// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will ignore subsequent calls of this function.
  /// Caution: The SDK detects http, tcp, and udp in sequence internally. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is null. Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe if needed. It is not recommended to start network probe during publishing/playing stream.
  /// Related APIs: Call [stopNetworkProbe] to stop network probe.
  ///
  /// - [config] network probe config.
  Future<ZegoNetworkProbeResult> startNetworkProbe(
      ZegoNetworkProbeConfig config) async {
    return await ZegoExpressImpl.instance.startNetworkProbe(config);
  }

  /// Stop network probe.
  ///
  /// Available since: 2.3.0
  /// Description: Stop network probe.
  /// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Related APIs: Call [startNetworkProbe] to start network probe.
  Future<void> stopNetworkProbe() async {
    return await ZegoExpressImpl.instance.stopNetworkProbe();
  }

  /// Start network speed test. Support set speed test interval。
  ///
  /// Available since: 1.20.0
  /// Description: This function supports uplink/downlink network speed test when the network can be connected.
  /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
  /// When to call: It needs to be called after [createEngine], and before [startPublishingStream]. If you call [startPublishingStream] while speed testing, the speed test will automatically stop.
  /// Restrictions: The default maximum allowable test time for a single network speed test is 30 seconds.
  /// Caution: Developers can register [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be triggered every 3 seconds. If an error occurs during the speed test process, [onNetworkSpeedTestError] callback will be triggered. If this function is repeatedly called multiple times, the last functioh call's configuration will be used.
  /// Related APIs: Call [stopNetworkSpeedTest] to stop network speed test.
  ///
  /// - [config] Network speed test configuration.
  /// - [interval] Interval of network speed test. In milliseconds, default is 3000 ms.
  Future<void> startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config,
      {int? interval}) async {
    return await ZegoExpressImpl.instance
        .startNetworkSpeedTest(config, interval: interval);
  }

  /// Stop network speed test.
  ///
  /// Available since: 1.20.0
  /// Description: Stop network speed test.
  /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Caution: After the network speed test stopped, [onNetworkSpeedTestQualityUpdate] callback will not be triggered.
  /// Related APIs: Call [startNetworkSpeedTest] to start network speed test.
  Future<void> stopNetworkSpeedTest() async {
    return await ZegoExpressImpl.instance.stopNetworkSpeedTest();
  }

  /// Obtain synchronization network time information.
  ///
  /// Available since: 2.9.0
  /// Description: Obtain synchronization network time(NTP), including timestamp and maximum deviation.
  /// Use cases: When performing multi-terminal synchronization behaviors, network time synchronization is required.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  Future<ZegoNetworkTimeInfo> getNetworkTimeInfo() async {
    return await ZegoExpressImpl.instance.getNetworkTimeInfo();
  }

  /// Dump audio and video data.
  ///
  /// Available since: 3.10.0
  /// Description: Dump audio and video data. Currently, only audio data is supported.
  /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Caution: It will trigger the [onStartDumpData] callback when data dumping starts.
  /// Related APIs: Call [stopDumpData] to stop dumping data.
  ///
  /// - [config] Dump data config.
  Future<void> startDumpData(ZegoDumpDataConfig config) async {
    return await ZegoExpressImpl.instance.startDumpData(config);
  }

  /// Stop dumping data.
  ///
  /// Available since: 3.10.0
  /// Description: Stop dumping data.
  /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
  /// When to call: It needs to be called after [startDumpData].
  /// Restrictions: None.
  /// Caution: It will trigger the [onUploadDumpData] callback.
  Future<void> stopDumpData() async {
    return await ZegoExpressImpl.instance.stopDumpData();
  }

  /// Upload dumped data to the ZEGO server.
  ///
  /// Available since: 3.10.0
  /// Description: Upload dumped data to the ZEGO server.
  /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
  /// When to call: It needs to be called after [stopDumpData].
  /// Restrictions: None.
  /// Caution: It will trigger the [onUploadDumpData] callback when dump data uploaded.
  Future<void> uploadDumpData() async {
    return await ZegoExpressImpl.instance.uploadDumpData();
  }

  /// Remove dumped data.
  ///
  /// Available since: 3.10.0
  /// Description: Remove dumped data.
  /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
  /// When to call: It needs to be called after [stopDumpData]. If the dump data is to be uploaded to the ZEGO server, it should be deleted after the upload is successful.
  /// Restrictions: None.
  Future<void> removeDumpData() async {
    return await ZegoExpressImpl.instance.removeDumpData();
  }
}
