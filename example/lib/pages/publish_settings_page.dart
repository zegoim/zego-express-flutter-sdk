import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/config/zego_config.dart';

class PublishSettingsPage extends StatefulWidget {

  @override
  _PublishSettingsPageState createState() => new _PublishSettingsPageState();
}

class _PublishSettingsPageState extends State<PublishSettingsPage> {

  bool _isPreviewMirror;
  bool _isPublishMirror;


  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    _isPreviewMirror = ZegoConfig.instance.isPreviewMirror;
    _isPublishMirror = ZegoConfig.instance.isPublishMirror;

  }

  void checkMirrorMode() {

    if (!_isPreviewMirror && !_isPublishMirror) {

      ZegoExpressEngine.instance.setVideoMirrorMode(ZegoVideoMirrorMode.NoMirror);

    } else if (!_isPreviewMirror && _isPublishMirror) {

      ZegoExpressEngine.instance.setVideoMirrorMode(ZegoVideoMirrorMode.OnlyPublishMirror);

    } else if (_isPreviewMirror && !_isPublishMirror) {

      ZegoExpressEngine.instance.setVideoMirrorMode(ZegoVideoMirrorMode.OnlyPreviewMirror);

    } else {

      ZegoExpressEngine.instance.setVideoMirrorMode(ZegoVideoMirrorMode.BothMirror);

    }
  }

  void onPreviewMirrorValueChanged(bool value) {
    setState(() {
      _isPreviewMirror = value;
      ZegoConfig.instance.isPreviewMirror = _isPreviewMirror;
      checkMirrorMode();
    });
  }

  void onPublishMirrorValueChanged(bool value) {
    setState(() {
      _isPublishMirror = value;
      ZegoConfig.instance.isPublishMirror = _isPublishMirror;
      checkMirrorMode();
    });
  }


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Setting'),
      ),
      body: Container(
        child: ListView(
          children: <Widget>[
            /// 镜像模式
            Container(
              padding: EdgeInsets.symmetric(horizontal: 20.0),
              decoration: BoxDecoration(
                border: Border(
                  bottom: BorderSide(
                    color: Colors.black26
                  )
                )
              ),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Text(
                    'Video Mirror Mode',
                    style: TextStyle(
                        fontSize: 18.0
                    ),
                  ),
                  Column(
                    children: <Widget>[
                      Row(
                        children: <Widget>[
                          Text(
                              'Enable Preview Mirror'
                          ),
                          CupertinoSwitch(
                            value: _isPreviewMirror,
                            onChanged: onPreviewMirrorValueChanged,
                          )
                        ],
                      ),
                      Row(
                        children: <Widget>[
                          Text(
                              'Enable Publish Mirror'
                          ),
                          CupertinoSwitch(
                            value: _isPublishMirror,
                            onChanged: onPublishMirrorValueChanged,
                          )
                        ],
                      ),
                    ],
                  )
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

}