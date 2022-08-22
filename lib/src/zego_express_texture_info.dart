import 'dart:async';
import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import 'zego_express_defines.dart';
import 'zego_express_impl.dart';

class ZegoExpressTextureInfo {
  factory ZegoExpressTextureInfo() => _instance;

  void init() async {
    _streamSubscriptionTextureRendererController ??=
        _textureRendererControllerEvent
            .receiveBroadcastStream()
            .listen(_textureRendererControllerEventListener);
  }

  void uninit() async {
    if (_textRenderViewModes.isEmpty) {
      await _streamSubscriptionTextureRendererController?.cancel();
      _streamSubscriptionTextureRendererController = null;
    }
  }

  void setViewMode(int textureID, ZegoViewMode viewMode) {
    bool usePlatformView =
        ZegoExpressImpl.enablePlatformView && !Platform.isWindows;
    usePlatformView = usePlatformView || kIsWeb;
    if (!usePlatformView) {
      if (_textRenderViewModes.containsKey(textureID) &&
          _textRenderViewModes[textureID] != viewMode) {
        _updateController.sink.add({
          'textureID': textureID,
          'type': 'update',
        });
      }
      _textRenderViewModes[textureID] = viewMode;
    }
  }

  void setBackgroundColor(int textureID, int backgroundColor) {
    bool usePlatformView =
        ZegoExpressImpl.enablePlatformView && !Platform.isWindows;
    usePlatformView = usePlatformView || kIsWeb;
    if (!usePlatformView) {
      _updateController.sink.add({'textureID': textureID, 'type': 'update'});
      _textRenderColorss[textureID] = Color(backgroundColor | 0xff000000);
    }
  }

  bool isContains(int textureID) =>
      _textRenderViewModes.keys.contains(textureID);

  ZegoViewMode? getViewMode(int textureID) => _textRenderViewModes[textureID];
  Color? getColor(int textureID) => _textRenderColorss[textureID];

  bool removeTexture(int textureID) {
    if (_textRenderViewModes.containsKey(textureID)) {
      _textRenderViewModes.remove(textureID);
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
    final Map<dynamic, dynamic> map = data;
    switch (map['type']) {
      case 'update':
        int textureID = map['textureID'];
        _updateController.sink.add({
          'textureID': textureID,
          'type': 'resize',
          'width': double.parse(map['width'].toString()),
          'height': double.parse(map['height'].toString())
        });
    }
  }

  ZegoExpressTextureInfo._();
  static final ZegoExpressTextureInfo _instance = ZegoExpressTextureInfo._();

  static final Map<int, ZegoViewMode> _textRenderViewModes = {};
  static final Map<int, Color> _textRenderColorss = {};

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
  Size? _size;

  @override
  void initState() {
    super.initState();

    widget.stream.listen((map) {
      if (map['type'] == 'resize') {
        _size = Size(map['width'], map['height']);
      }
      setState(() {});
    });
  }

  Rect _viewModeCalculate(double width, double height) {
    final double pixelRatio = MediaQuery.of(context).devicePixelRatio;
    var textureWidth = width;
    var textureHeight = height;
    var x = 0.0;
    var y = 0.0;

    if (_size != null) {
      Size size = _size!;
      var viewMode = ZegoExpressTextureInfo().getViewMode(widget.textureID) ??
          ZegoViewMode.AspectFit;
      switch (viewMode) {
        case ZegoViewMode.AspectFit:
          {
            if (size.width > size.height) {
              textureWidth = width;
              textureHeight = textureWidth * size.height / size.width;
              x = (width - textureWidth) / 2;
              y = (height - textureHeight) / 2;
            } else {
              textureHeight = height;
              textureWidth = textureHeight * size.width / size.height;
              x = (width - textureWidth) / 2;
              y = (height - textureHeight) / 2;
            }
            widget.updateTextureRendererSize(
                widget.textureID,
                (textureWidth * pixelRatio).toInt(),
                (textureHeight * pixelRatio).toInt());
          }
          break;
        case ZegoViewMode.AspectFill:
          {
            if (size.width > size.height) {
              textureHeight = height;
              textureWidth = textureHeight * size.width / size.height;

              x = (width - textureWidth) / 2;
              y = (height - textureHeight) / 2;
            } else {
              textureWidth = width;
              textureHeight = textureWidth * size.height / size.width;
              x = (width - textureWidth) / 2;
              y = (height - textureHeight) / 2;
            }
            widget.updateTextureRendererSize(
                widget.textureID,
                (textureWidth * pixelRatio).toInt(),
                (textureHeight * pixelRatio).toInt());
          }
          break;
        case ZegoViewMode.ScaleToFill:
          break;
      }
    } else {
      widget.updateTextureRendererSize(widget.textureID,
          (width * pixelRatio).toInt(), (height * pixelRatio).toInt());
    }
    return Rect.fromLTWH(x, y, textureWidth, textureHeight);
  }

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(builder: ((context, constraints) {
      var rect = _viewModeCalculate(
          constraints.biggest.width, constraints.biggest.height);
      var textureWidth = rect.width;
      var textureHeight = rect.height;
      var x = rect.left;
      var y = rect.top;

      var backgroundColor =
          ZegoExpressTextureInfo().getColor(widget.textureID) ?? Colors.black;
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
            child: Texture(
              textureId: widget.textureID,
            ),
          )
        ],
      );
    }));
  }
}
