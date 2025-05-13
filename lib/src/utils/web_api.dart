@JS('window')
library window;

import 'package:js/js.dart';
import 'dart:convert';

@JS('JSON.parse')
external dynamic parseJSON(String str);
@JS('JSON.stringify')
external String stringifyJSON(dynamic obj);

dynamic dartObjToJSON(dynamic obj) {
  String str = jsonEncode(obj);
  return parseJSON(str);
}

@JS('Object.keys')
external dynamic objectKeys(dynamic obj);
