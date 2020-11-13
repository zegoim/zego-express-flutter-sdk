import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';
import 'package:permission_handler/permission_handler.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/config/zego_config.dart';
import 'package:zego_express_engine_example/pages/publish_stream_page.dart';
import 'package:zego_express_engine_example/pages/play_stream_page.dart';

class LoginRoomPage extends StatefulWidget {

  final bool isPublish;

  LoginRoomPage(this.isPublish);

  @override
  _LoginRoomPageState createState() => new _LoginRoomPageState();
}

class _LoginRoomPageState extends State<LoginRoomPage> {

  final TextEditingController _controller = new TextEditingController();

  @override
  void initState() {
    super.initState();

    if(ZegoConfig.instance.roomID.isNotEmpty) {
      _controller.text = ZegoConfig.instance.roomID;
    }
  }

  @override
  void dispose() {
    super.dispose();

    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    ZegoExpressEngine.destroyEngine();
  }

  Future<bool> requestPermission() async {
    PermissionStatus microphoneStatus = await Permission.microphone.request();
    PermissionStatus cameraStatus = await Permission.camera.request();
    return microphoneStatus.isGranted && cameraStatus.isGranted;
  }

  void onButtonPressed() async {
    if (widget.isPublish) {
      // Publishing stream requires permission
      // Check the permissions before logging into the room
      bool isPermissionGranted = await requestPermission();

      if (!isPermissionGranted) {
        // The authorization is not allowed, the pop-up window prompts the user to open the permission
        showSettingsLink();
      } else {
        // Authorization is complete, allowing login room
        _loginRoom();
      }
    } else {
      // Playing stream does not need to apply for permission
      _loginRoom();
    }
  }

  void _loginRoom() {
    String roomID = _controller.text.trim();

    ZegoUser user = ZegoUser(ZegoConfig.instance.userID, ZegoConfig.instance.userName);

    // Step2 LoginRoom
    ZegoExpressEngine.instance.loginRoom(roomID, user);

    ZegoConfig.instance.roomID = roomID;
    ZegoConfig.instance.saveConfig();

    Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {

      int screenWidthPx = MediaQuery.of(context).size.width.toInt() * MediaQuery.of(context).devicePixelRatio.toInt();
      int screenHeightPx = (MediaQuery.of(context).size.height - MediaQuery.of(context).padding.top - 56.0).toInt() * MediaQuery.of(context).devicePixelRatio.toInt();

      if (widget.isPublish) {

        return PublishStreamPage(screenWidthPx, screenHeightPx);

      } else {

        return PlayStreamPage(screenWidthPx, screenHeightPx);

      }
    }));

  }

  void showSettingsLink() {
    showDialog(context: context, builder: (BuildContext context) {
      return AlertDialog(
        title: Text('Tips'),
        content: Text('Please go to the settings page to open the camera/microphone permissions'),
        actions: <Widget>[
          FlatButton(
            child: Text('OK'),
            onPressed: () => Navigator.of(context).pop()
          ),
        ],
      );
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Step2 LoginRoom'),
      ),
      body: GestureDetector(

        behavior: HitTestBehavior.translucent,
        onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),

        child: Container(
          padding: const EdgeInsets.symmetric(horizontal: 40.0),
          child: Column(
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.only(bottom: 20.0),
              ),
              Row(
                children: <Widget>[
                  Text('RoomID: '),
                ],
              ),
              Padding(
                padding: const EdgeInsets.only(bottom: 10.0),
              ),
              TextField(
                controller: _controller,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: 'Please enter the room ID:',
                  enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                        color: Colors.grey,
                      )
                  ),
                  focusedBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                        color: Color(0xff0e88eb),
                      )
                  ),
                ),
              ),
              Padding(
                padding: const EdgeInsets.only(bottom: 10.0),
              ),
              Text('RoomID represents the identification of a room, it needs to ensure that the RoomID is globally unique, and no longer than 255 bytes',
                style: TextStyle(
                    fontSize: 12.0,
                    color: Colors.black45
                ),
                maxLines: 2,
                softWrap: true,
              ),
              Padding(
                padding: const EdgeInsets.only(bottom: 50.0),
              ),
              Container(
                padding: const EdgeInsets.all(0.0),
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(12.0),
                  color: Color(0xff0e88eb),
                ),
                width: 240.0,
                height: 60.0,
                child: CupertinoButton(
                  child: Text('Login Room',
                    style: TextStyle(
                        color: Colors.white
                    ),
                  ),
                  onPressed: onButtonPressed,
                ),
              )
            ],
          ),
        )
      ),
    );
  }

}
