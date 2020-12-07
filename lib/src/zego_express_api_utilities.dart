
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

}
