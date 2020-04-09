import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/config/zego_config.dart';
import 'package:zego_express_engine_example/pages/publish_settings_page.dart';

class PublishStreamPage extends StatefulWidget {

  final int screenWidthPx;
  final int screenHeightPx;

  PublishStreamPage(this.screenWidthPx, this.screenHeightPx);

  @override
  _PublishStreamPageState createState() => new _PublishStreamPageState();
}

class _PublishStreamPageState extends State<PublishStreamPage> {

  String _title = '';
  bool _isPublishing = false;

  int _previewViewID = -1;
  Widget _previewViewWidget;
  ZegoCanvas _previewCanvas;

  int _publishWidth = 0;
  int _publishHeight = 0;
  double _publishFps = 0.0;
  double _publishBitrate = 0.0;
  bool _isHardwareEncode = false;
  String _networkQuality = '';

  bool _isUseMic = true;
  bool _isUseFrontCamera = true;

  TextEditingController _controller = new TextEditingController();

  @override
  void initState() {
    super.initState();

    _title = 'Step3 StartPublishing';

    if (ZegoConfig.instance.streamID.isNotEmpty) {
      _controller.text = ZegoConfig.instance.streamID;
    }

    setPublisherCallback();

    if (ZegoConfig.instance.enablePlatformView) {

      setState(() {
        // Create a PlatformView Widget
        _previewViewWidget = ZegoExpressEngine.instance.createPlatformView((viewID) {

          _previewViewID = viewID;

          // Start preview using platform view
          startPreview(viewID);

        });
      });

    } else {

      // Create a Texture Renderer
      ZegoExpressEngine.instance.createTextureRenderer(widget.screenWidthPx, widget.screenHeightPx).then((textureID) {

        _previewViewID = textureID;

        setState(() {
          // Create a Texture Widget
          _previewViewWidget = Texture(textureId: textureID);
        });

        // Start preview using texture renderer
        startPreview(textureID);
      });
    }
  }

  void setPublisherCallback() {

    // Set the publisher state callback
    ZegoExpressEngine.onPublisherStateUpdate = (String streamID, ZegoPublisherState state, int errorCode, Map<String, dynamic> extendedData) {

      if (errorCode == 0) {
        setState(() {
          _isPublishing = true;
          _title = 'Publishing';
        });

        ZegoConfig.instance.streamID = streamID;
        ZegoConfig.instance.saveConfig();

      } else {
        print('Publish error: $errorCode');
      }
    };

    // Set the publisher quality callback
    ZegoExpressEngine.onPublisherQualityUpdate = (String streamID, ZegoPublishStreamQuality quality) {

      setState(() {
        _publishFps = quality.videoSendFPS;
        _publishBitrate = quality.videoKBPS;
        _isHardwareEncode = quality.isHardwareEncode;

        switch (quality.level) {
          case ZegoStreamQualityLevel.Excellent:
            _networkQuality = '☀️';
            break;
          case ZegoStreamQualityLevel.Good:
            _networkQuality = '⛅️️';
            break;
          case ZegoStreamQualityLevel.Medium:
            _networkQuality = '☁️';
            break;
          case ZegoStreamQualityLevel.Bad:
            _networkQuality = '🌧';
            break;
          case ZegoStreamQualityLevel.Die:
            _networkQuality = '❌';
            break;
          default:
            break;
        }
      });
    };

    // Set the publisher video size changed callback
    ZegoExpressEngine.onPublisherVideoSizeChanged = (int width, int height, ZegoPublishChannel channel) {
      setState(() {
        _publishWidth = width;
        _publishHeight = height;
      });
    };
  }

  void startPreview(int viewID) {

    // Set the preview canvas
    _previewCanvas =  ZegoCanvas.view(viewID);

    // Start preview
    ZegoExpressEngine.instance.startPreview(canvas: _previewCanvas);
  }

  @override
  void dispose() {
    super.dispose();

    if (_isPublishing) {
      // Stop publishing
      ZegoExpressEngine.instance.stopPublishingStream();
    }

    // Stop preview
    ZegoExpressEngine.instance.stopPreview();

    // Unregister publisher callback
    ZegoExpressEngine.onPublisherStateUpdate = null;
    ZegoExpressEngine.onPublisherQualityUpdate = null;
    ZegoExpressEngine.onPublisherVideoSizeChanged = null;

    if (ZegoConfig.instance.enablePlatformView) {
      // Destroy preview platform view
      ZegoExpressEngine.instance.destroyPlatformView(_previewViewID);
    } else {
      // Destroy preview texture renderer
      ZegoExpressEngine.instance.destroyTextureRenderer(_previewViewID);
    }

    // Logout room
    ZegoExpressEngine.instance.logoutRoom(ZegoConfig.instance.roomID);

  }

  void onPublishButtonPressed() {

    String streamID = _controller.text.trim();

    // Start publishing stream
    ZegoExpressEngine.instance.startPublishingStream(streamID);

  }

  void onCamStateChanged() {

    _isUseFrontCamera = !_isUseFrontCamera;
    ZegoExpressEngine.instance.useFrontCamera(_isUseFrontCamera);
  }

  void onMicStateChanged() {

    setState(() {
      _isUseMic = !_isUseMic;
      ZegoExpressEngine.instance.muteMicrophone(!_isUseMic);
    });
  }

  void onVideoMirroModeChanged(int mode) {
    ZegoExpressEngine.instance.setVideoMirrorMode(ZegoVideoMirrorMode.values[mode]);
  }


  Widget showPreviewToolPage() {
    return GestureDetector(

      behavior: HitTestBehavior.translucent,
      onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),

      child: Container(
        padding: const EdgeInsets.symmetric(horizontal: 30.0),
        child: Column(
          children: <Widget>[
            Padding(
              padding: const EdgeInsets.only(top: 50.0),
            ),
            Row(
              children: <Widget>[
                Text('StreamID: ',
                  style: TextStyle(
                    color: Colors.white
                  ),
                )
              ],
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            TextField(
              controller: _controller,
              style: TextStyle(
                color: Colors.white
              ),
              decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: 'Please enter streamID',
                  hintStyle: TextStyle(
                    color: Color.fromRGBO(255, 255, 255, 0.8)
                  ),
                  enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: Colors.white
                    )
                  ),
                  focusedBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                          color: Color(0xff0e88eb)
                      )
                  )
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            Text(
              'StreamID must be globally unique and the length should not exceed 255 bytes',
              style: TextStyle(
                color: Colors.white
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 30.0),
            ),
            Container(
              padding: const EdgeInsets.all(0.0),
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(12.0),
                color: Color(0xee0e88eb),
              ),
              width: 240.0,
              height: 60.0,
              child: CupertinoButton(
                child: Text('Start Publishing',
                  style: TextStyle(
                      color: Colors.white
                  ),
                ),
                onPressed: onPublishButtonPressed,
              ),
            )
          ],
        ),
      ),
    );
  }

  Widget showPublishingToolPage() {
    return Container(
      padding: EdgeInsets.only(left: 10.0, right: 10.0, bottom: MediaQuery.of(context).padding.bottom + 20.0),
      child: Column(
        children: <Widget>[
          Padding(
            padding: const EdgeInsets.only(top: 10.0),
          ),
          Row(
            children: <Widget>[
              Text('RoomID: ${ZegoConfig.instance.roomID}',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('StreamID: ${ZegoConfig.instance.streamID}',
                style: TextStyle(
                  color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Rendering with: ${ZegoConfig.instance.enablePlatformView ? 'PlatformView' : 'TextureRenderer'}',
                style: TextStyle(
                  color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Resolution: $_publishWidth x $_publishHeight',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('FPS: ${_publishFps.toStringAsFixed(2)}',
                style: TextStyle(
                  color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Bitrate: ${_publishBitrate.toStringAsFixed(2)} kb/s',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('HardwareEncode: ${_isHardwareEncode ? '✅' : '❎'}',
                style: TextStyle(
                  color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('NetworkQuality: $_networkQuality',
                style: TextStyle(
                  color: Colors.white
                ),
              ),
            ],
          ),
          Expanded(
            child: Padding(padding: const EdgeInsets.only(top: 10.0)),
          ),
          Row(
            children: <Widget>[
              CupertinoButton(
                padding: const EdgeInsets.all(0.0),
                pressedOpacity: 1.0,
                borderRadius: BorderRadius.circular(
                    0.0),
                child: Image(
                  width: 44.0,
                  image:AssetImage('resources/images/bottom_switchcamera_icon.png'),
                ),
                onPressed: onCamStateChanged,
              ),
              Padding(
                padding: const EdgeInsets.only(left: 10.0),
              ),
              CupertinoButton(
                padding: const EdgeInsets.all(0.0),
                pressedOpacity: 1.0,
                borderRadius: BorderRadius.circular(
                    0.0),
                child: Image(
                  width: 44.0,
                  image: _isUseMic
                      ? AssetImage('resources/images/bottom_microphone_on_icon.png')
                      : AssetImage('resources/images/bottom_microphone_off_icon.png'),
                ),
                onPressed: onMicStateChanged,
              ),
              Padding(
                padding: const EdgeInsets.only(left: 10.0),
              )
            ],
          ),
        ],
      ),
    );
  }

  void onSettingsButtonClicked() {
    Navigator.push(context, MaterialPageRoute(builder: (BuildContext context) {
      return PublishSettingsPage();
    },fullscreenDialog: true));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      resizeToAvoidBottomPadding: false,
      appBar: AppBar(
        title: Text(_title),
      ),
      floatingActionButton: CupertinoButton(
          child: Text(
            'Settings',
            style: TextStyle(
              color: Colors.white
            ),
          ),
          onPressed: onSettingsButtonClicked
      ),
      body: Stack(
        children: <Widget>[
          Container(
            width: MediaQuery.of(context).size.width,
            height: MediaQuery.of(context).size.height - MediaQuery.of(context).padding.top,
            child: _previewViewWidget,
          ),
          _isPublishing ? showPublishingToolPage() : showPreviewToolPage(),
        ],
      )
    );
  }

}