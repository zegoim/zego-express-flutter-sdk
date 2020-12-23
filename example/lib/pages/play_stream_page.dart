import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/config/zego_config.dart';
import 'package:zego_express_engine_example/ui/zego_ui_tool.dart';

class PlayStreamPage extends StatefulWidget {

  final int screenWidthPx;
  final int screenHeightPx;

  PlayStreamPage(this.screenWidthPx, this.screenHeightPx);

  @override
  _PlayStreamPageState createState() => new _PlayStreamPageState();
}

class _PlayStreamPageState extends State<PlayStreamPage> {

  String _title = '';
  bool _isPlaying = false;

  int _playViewID = -1;
  Widget _playViewWidget;
  ZegoCanvas _playCanvas;

  int _playWidth = 0;
  int _playHeight = 0;
  double _playRecvFPS = 0.0;
  double _playDecodeFPS = 0.0;
  double _playRenderFPS = 0.0;
  double _playVideoBitrate = 0.0;
  double _playAudioBitrate = 0.0;
  bool _isHardwareDecode = false;
  String _networkQuality = '';

  bool _isUseSpeaker = true;

  TextEditingController _controller = new TextEditingController();

  @override
  void initState() {
    super.initState();

    _title = 'Step3 StartPlaying';

    if (ZegoConfig.instance.streamID.isNotEmpty) {
      _controller.text = ZegoConfig.instance.streamID;
    }

    setPlayerCallback();

  }

  void setPlayerCallback() {

    // Set the player state callback
    ZegoExpressEngine.onPlayerStateUpdate = (String streamID, ZegoPlayerState state, int errorCode, Map<String, dynamic> extendedData) {

      if(errorCode == 0) {
        setState(() {
          _isPlaying = true;
          _title = 'Playing';
        });

        ZegoConfig.instance.streamID = streamID;
        ZegoConfig.instance.saveConfig();

      } else {
        print('Play error: $errorCode');
      }
    };

    // Set the player quality callback
    ZegoExpressEngine.onPlayerQualityUpdate = (String streamID, ZegoPlayStreamQuality quality) {

      setState(() {
        _playRecvFPS = quality.videoRecvFPS;
        _playDecodeFPS = quality.videoDecodeFPS;
        _playRenderFPS = quality.videoRenderFPS;
        _playVideoBitrate = quality.videoKBPS;
        _playAudioBitrate = quality.audioKBPS;
        _isHardwareDecode = quality.isHardwareDecode;

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

    // Set the player video size changed callback
    ZegoExpressEngine.onPlayerVideoSizeChanged = (String streamID, int width, int height) {
      setState(() {
        _playWidth = width;
        _playHeight = height;
      });
    };
  }

  @override
  void dispose() {
    super.dispose();

    if (_isPlaying) {
      // Stop playing
      ZegoExpressEngine.instance.stopPlayingStream(ZegoConfig.instance.streamID);
    }

    // Unregister player callback
    ZegoExpressEngine.onPlayerStateUpdate = null;
    ZegoExpressEngine.onPlayerQualityUpdate = null;
    ZegoExpressEngine.onPlayerVideoSizeChanged = null;

    if (ZegoConfig.instance.enablePlatformView) {
      // Destroy play platform view
      ZegoExpressEngine.instance.destroyPlatformView(_playViewID);
    } else {
      // Destroy play texture renderer
      ZegoExpressEngine.instance.destroyTextureRenderer(_playViewID);
    }

    // Logout room
    ZegoExpressEngine.instance.logoutRoom(ZegoConfig.instance.roomID);
  }

  void onPlayButtonPressed() {

    String streamID = _controller.text.trim();

    if (ZegoConfig.instance.enablePlatformView) {

      setState(() {
        // Create a PlatformView Widget
        _playViewWidget = ZegoExpressEngine.instance.createPlatformView((viewID) {

          _playViewID = viewID;

          // Start playing stream using platform view
          startPlayingStream(viewID, streamID);

        });
      });

    } else {


      // Create a Texture Renderer
      ZegoExpressEngine.instance.createTextureRenderer(widget.screenWidthPx, widget.screenHeightPx).then((textureID) {

        _playViewID = textureID;

        setState(() {
          // Create a Texture Widget
          _playViewWidget = Texture(textureId: textureID);
        });

        // Start playing stream using texture renderer
        startPlayingStream(textureID, streamID);
      });
    }

  }

  void startPlayingStream(int viewID, String streamID) {
    setState(() {

      // Set the play canvas
      _playCanvas = ZegoCanvas.view(viewID);

      // Start playing stream
      ZegoExpressEngine.instance.startPlayingStream(streamID, canvas: _playCanvas);
    });
  }

  void onSpeakerStateChanged() {
    setState(() {
      _isUseSpeaker = !_isUseSpeaker;
      ZegoExpressEngine.instance.muteSpeaker(!_isUseSpeaker);
    });
  }

  void onSnapshotButtonClicked() {
    ZegoExpressEngine.instance.takePlayStreamSnapshot(_controller.text.trim()).then((result) {
      print('[takePublishStreamSnapshot], errorCode: ${result.errorCode}, is null image?: ${result.image != null ? "false" : "true"}');
      ZegoUITool.showImage(context, result.image);
    });
  }

  Widget prepareToolWidget() {
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
                Text('StreamID: ')
              ],
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            TextField(
              controller: _controller,

              decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: 'Please enter streamID',
                  enabledBorder: OutlineInputBorder(borderSide: BorderSide(color: Colors.grey)),
                  focusedBorder: OutlineInputBorder(borderSide: BorderSide(color: Color(0xff0e88eb)))
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            Text(
              'StreamID must be globally unique and the length should not exceed 255 bytes',
              style: TextStyle(fontSize: 12.0, color: Colors.black45),
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
                child: Text('Start Playing', style: TextStyle(color: Colors.white)),
                onPressed: onPlayButtonPressed,
              ),
            )
          ],
        ),
      ),
    );
  }

  Widget playingToolWidget() {
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
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('StreamID: ${ZegoConfig.instance.streamID}',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Rendering with: ${ZegoConfig.instance.enablePlatformView ? 'PlatformView' : 'TextureRenderer'}',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Resolution: $_playWidth x $_playHeight',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('FPS(Recv): ${_playRecvFPS.toStringAsFixed(2)}',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('FPS(Decode): ${_playDecodeFPS.toStringAsFixed(2)}',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('FPS(Render): ${_playRenderFPS.toStringAsFixed(2)}',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Bitrate(Video): ${_playVideoBitrate.toStringAsFixed(2)} kb/s',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('Bitrate(Audio): ${_playAudioBitrate.toStringAsFixed(2)} kb/s',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('HardwareDecode: ${_isHardwareDecode ? '✅' : '❎'}',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('NetworkQuality: $_networkQuality',
                style: TextStyle(color: Colors.white, fontSize: 9),
              ),
            ],
          ),
          Expanded(child: SizedBox()),
          Row(
            children: <Widget>[
              CupertinoButton(
                padding: const EdgeInsets.all(0.0),
                pressedOpacity: 1.0,
                borderRadius: BorderRadius.circular(0.0),
                child: Icon(
                  _isUseSpeaker ? Icons.volume_up : Icons.volume_off,
                  size: 44.0,
                  color: Colors.white
                ),
                onPressed: onSpeakerStateChanged,
              ),
              SizedBox(width: 10.0),
              CupertinoButton(
                padding: const EdgeInsets.all(0.0),
                pressedOpacity: 1.0,
                borderRadius: BorderRadius.circular(0.0),
                child: Icon(Icons.camera, size: 44.0, color: Colors.white),
                onPressed: onSnapshotButtonClicked,
              ),
            ],
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        resizeToAvoidBottomPadding: false,
        appBar: AppBar(title: Text(_title)),
        body: Stack(
          children: <Widget>[
            Container(
              width: MediaQuery.of(context).size.width,
              height: MediaQuery.of(context).size.height - MediaQuery.of(context).padding.top,
              child: _playViewWidget,
            ),
            _isPlaying ? playingToolWidget() : prepareToolWidget(),
          ],
        )
    );
  }

}