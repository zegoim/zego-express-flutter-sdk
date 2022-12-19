import 'dart:async';
import 'dart:math';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import '../zego_express_defines.dart';
import '../utils/zego_express_utils.dart';
import 'zego_express_impl.dart';

class ZegoExpressTextureRenderer {
  factory ZegoExpressTextureRenderer() => _instance;

  void init() async {
    if (!kIsWeb && !kIsAndroid) {
      _streamSubscriptionTextureRendererController ??=
          _textureRendererControllerEvent
              .receiveBroadcastStream()
              .listen(_textureRendererControllerEventListener);
    }
  }

  void uninit() async {
    if (_viewModeMap.isEmpty && !kIsWeb && !kIsAndroid) {
      await _streamSubscriptionTextureRendererController?.cancel();
      _streamSubscriptionTextureRendererController = null;
    }
  }

  /// Create a Texture renderer and return the texture ID
  Future<int> createTextureRenderer(int width, int height) async {
    final int textureID = await ZegoExpressImpl.methodChannel.invokeMethod(
        'createTextureRenderer', {'width': width, 'height': height});

    return textureID;
  }

  /// Update the size of the Texture renderer
  /// If it returns false, it's probably because the `textureID` to be updated doesn't exist.
  /// Note: Only used by Android!
  Future<bool> updateTextureRendererSize(
      int textureID, int width, int height) async {
    if (kIsAndroid) {
      return await ZegoExpressImpl.methodChannel.invokeMethod(
          'updateTextureRendererSize',
          {'textureID': textureID, 'width': width, 'height': height});
    } else {
      return true;
    }
  }

  /// Destroys the Texture renderer and releases its resources
  /// If it returns false, it's probably because the `textureID` to be destroyed doesn't exist.
  Future<bool> destroyTextureRenderer(int textureID) async {
    return await ZegoExpressImpl.methodChannel
        .invokeMethod('destroyTextureRenderer', {'textureID': textureID});
  }

  void setViewMode(int textureID, ZegoViewMode viewMode) {
    if (_viewModeMap.containsKey(textureID) &&
        _viewModeMap[textureID] != viewMode) {
      _updateController.sink.add({
        'textureID': textureID,
        'type': 'update',
      });
    }
    _viewModeMap[textureID] = viewMode;
  }

  void setBackgroundColor(int textureID, int backgroundColor) {
    _updateController.sink.add({'textureID': textureID, 'type': 'update'});
    _backgroundColorMap[textureID] = Color(backgroundColor | 0xff000000);
  }

  ZegoViewMode? getViewMode(int textureID) => _viewModeMap[textureID];
  Color? getBackgroundColor(int textureID) => _backgroundColorMap[textureID];
  Size? getSize(int textureID) => _sizeMap[textureID];
  int? getMirror(int textureID) => _mirrorMap[textureID];

  bool removeTexture(int textureID) {
    if (_viewModeMap.containsKey(textureID)) {
      _viewModeMap.remove(textureID);
      uninit();
      return true;
    } else {
      return false;
    }
  }

  Stream<Map<String, dynamic>> getStream(int textureID) {
    return _updateController.stream.where((event) {
      if (event['textureID'] == textureID) {
        return true;
      } else {
        return false;
      }
    });
  }

  static void _textureRendererControllerEventListener(dynamic data) {
    func(data) {
      final Map<dynamic, dynamic> map = data;
      switch (map['type']) {
        case 'update':
          int textureID = map['textureID'];
          _sizeMap[textureID] = Size(double.parse(map['width'].toString()),
              double.parse(map['height'].toString()));
          _mirrorMap[textureID] = map['isMirror'];
          _updateController.sink
              .add({'textureID': textureID, 'type': 'update'});
      }
    }

    Timer(const Duration(milliseconds: 10), () => func(data));
  }

  ZegoExpressTextureRenderer._();
  static final ZegoExpressTextureRenderer _instance =
      ZegoExpressTextureRenderer._();

  static final Map<int, ZegoViewMode> _viewModeMap = {};
  static final Map<int, Color> _backgroundColorMap = {};
  static final Map<int, Size> _sizeMap = {};
  static final Map<int, int?> _mirrorMap = {};

  static final StreamController<Map<String, dynamic>> _updateController =
      StreamController<Map<String, dynamic>>.broadcast();

  static const EventChannel _textureRendererControllerEvent = EventChannel(
      'plugins.zego.im/zego_texture_renderer_controller_event_handler');
  static StreamSubscription<dynamic>?
      _streamSubscriptionTextureRendererController;
}

class ZegoTextureWidget extends StatefulWidget {
  const ZegoTextureWidget(
      {Key? key,
      required this.textureID,
      required this.stream,
      required this.updateTextureRendererSize})
      : super(key: key);

  final int textureID;

  final Stream<Map<String, dynamic>> stream;

  final Function(int, int, int) updateTextureRendererSize;

  @override
  State<ZegoTextureWidget> createState() => _ZegoTextureWidgetState();
}

class _ZegoTextureWidgetState extends State<ZegoTextureWidget> {
  bool _isInit = false;

  @override
  void initState() {
    super.initState();
    widget.stream.listen((map) {
      if (_isInit) {
        setState(() {});
      }
    });
    _isInit = true;
  }

  @override
  void dispose() {
    _isInit = false;
    super.dispose();
  }

  Rect _calculateHeight(Size size, double width, double height) {
    var textureWidth = width;
    var textureHeight = textureWidth * size.height / size.width;
    var x = (width - textureWidth) / 2;
    var y = (height - textureHeight) / 2;
    return Rect.fromLTWH(x, y, textureWidth, textureHeight);
  }

  Rect _calculateWidth(Size size, double width, double height) {
    var textureHeight = height;
    var textureWidth = textureHeight * size.width / size.height;
    var x = (width - textureWidth) / 2;
    var y = (height - textureHeight) / 2;
    return Rect.fromLTWH(x, y, textureWidth, textureHeight);
  }

  Rect _viewModeCalculate(double width, double height) {
    final double pixelRatio = MediaQuery.of(context).devicePixelRatio;
    var rect = Rect.fromLTWH(0, 0, width, height);

    Size? size = ZegoExpressTextureRenderer().getSize(widget.textureID);
    if (size != null) {
      var viewMode =
          ZegoExpressTextureRenderer().getViewMode(widget.textureID) ??
              ZegoViewMode.AspectFit;

      switch (viewMode) {
        case ZegoViewMode.AspectFit:
          {
            if (size.width > size.height) {
              rect = _calculateHeight(size, width, height);
              if (rect.height > height) {
                rect = _calculateWidth(size, width, height);
              }
            } else {
              rect = _calculateWidth(size, width, height);
              if (rect.width > width) {
                rect = _calculateHeight(size, width, height);
              }
            }
            widget.updateTextureRendererSize(
                widget.textureID,
                (rect.width * pixelRatio).toInt(),
                (rect.height * pixelRatio).toInt());
          }
          break;
        case ZegoViewMode.AspectFill:
          {
            if (size.width > size.height) {
              rect = _calculateWidth(size, width, height);
              if (rect.width < width) {
                rect = _calculateHeight(size, width, height);
              }
            } else {
              rect = _calculateHeight(size, width, height);
              if (rect.height < height) {
                rect = _calculateWidth(size, width, height);
              }
            }
            widget.updateTextureRendererSize(
                widget.textureID,
                (rect.width * pixelRatio).toInt(),
                (rect.height * pixelRatio).toInt());
          }
          break;
        case ZegoViewMode.ScaleToFill:
          break;
      }
    } else {
      widget.updateTextureRendererSize(widget.textureID,
          (width * pixelRatio).toInt(), (height * pixelRatio).toInt());
    }
    return rect;
  }

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(builder: ((context, constraints) {
      // Calculate the scaled size
      var rect = _viewModeCalculate(
          constraints.biggest.width, constraints.biggest.height);
      var textureWidth = rect.width;
      var textureHeight = rect.height;
      var x = rect.left;
      var y = rect.top;

      var backgroundColor =
          ZegoExpressTextureRenderer().getBackgroundColor(widget.textureID) ??
              Colors.black;

      Widget child = Texture(
        textureId: widget.textureID,
      );
      // mirror
      int? isMirror = ZegoExpressTextureRenderer().getMirror(widget.textureID);
      if (isMirror != null && isMirror == 1) {
        var matrix4 = Matrix4.rotationY(pi);
        matrix4.setTranslationRaw(textureWidth, 0, 0);
        child = Transform(
          transform: matrix4,
          child: Texture(
            textureId: widget.textureID,
          ),
        );
      }
      return Stack(
        children: [
          Container(
            color: backgroundColor,
          ),
          Positioned(
            left: x,
            top: y,
            width: textureWidth,
            height: textureHeight,
            child: child,
          )
        ],
      );
    }));
  }
}
