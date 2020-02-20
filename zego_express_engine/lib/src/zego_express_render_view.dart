import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'zego_express_defines.dart';
import 'zego_express_api.dart';

enum ZegoRenderViewMode {
  ASPECT_FIT,
  ASPECT_FILL,
  SCALE_FILL
}

class ZegoRenderView extends StatefulWidget {

  const ZegoRenderView({
    Key key,
    //@required this.controller,
    this.viewMode = ZegoRenderViewMode.ASPECT_FIT,
    this.backgroundColor = Colors.black,
    @required this.onViewCreated
  }) :  assert(onViewCreated != null),
        super(key: key);

  //final ZegoRenderViewController controller;

  final ZegoRenderViewMode viewMode;

  final Color backgroundColor;

  final ZegoViewCreatedCallback onViewCreated;

  @override
  _ZegoRenderViewState createState() => new _ZegoRenderViewState();
}

class _ZegoRenderViewState extends State<ZegoRenderView> {

  Widget _renderView;
  int _viewID;

  @override
  void initState() {
    super.initState();

    if(ZegoExpressEngine.isUsePlatformView) {

    } else {

      //_viewID =
      //_renderView = Texture(textureId: widget.controller.viewID);

    }
  }

  @override
  Widget build(BuildContext context) {
    return _renderView;
  }

}