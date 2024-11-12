@JS('window')
library window;

import 'package:js/js.dart';
import 'dart:convert';
import 'dart:js';
import 'dart:async';

@JS('JSON.parse')
external dynamic parseJSON(String str);
@JS('JSON.stringify')
external String stringifyJSON(dynamic obj);

dynamic dartObjToJSON(dynamic obj) {
  String str = jsonEncode(obj);
  return parseJSON(str);
}
// Future<T> promiseToFuture1<T>(dynamic promise) {
//   final promise = JsPromise.fromBrowserObject(obj);
//   final completer = Completer<T>();
//  promiseToF

//   // 将JS Promise转换为Dart Future
//   promise.callMethod('then', [
//     allowInterop((result) {
//       completer.complete(result);
//     })
//   ]);

//   promise.callMethod('catch', [
//     allowInterop((error) {
//       completer.completeError(error);
//     })
//   ]);

//   return completer.future;
// }