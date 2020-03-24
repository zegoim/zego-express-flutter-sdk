import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

class ZegoExpressEngine {
    static ZegoExpressEngine _instance;

    ZegoExpressEngine._internal();

    static ZegoExpressEngine get instance {
        return _instance;
    }

    /// 获取 ZegoExpressEngine 版本号。
    ///
    /// ZegoExpressEngine 在运行过程中，当开发者发现与预期情况不符时，可将问题与相关日志提交给 ZEGO 技术人员定位，ZEGO 技术人员可能需要 engine 的版本的信息来辅助定位问题。
    /// 开发者也可以收集此信息作为 App 所使用的 engine 的版本信息。
    /// Returns ZegoExpressEngine 版本号
    static Future<String> getVersion() async {
        return await ZegoExpressImpl.getVersion();
    }

    static Future<void> destroyEngine() async {
        return await ZegoExpressImpl.destroyEngine();
    }

    Future<void> loginRoom(String roomID, ZegoUser user, {ZegoRoomConfig config}) async {
        return await ZegoExpressImpl.instance.loginRoom(roomID, user, config: config);
    }

    /// 退出房间
    ///
    /// 退出房间会停止该用户的所有推拉流并且 ZegoExpressEngine 内部会主动停止本地预览，调用该接口后会收到 [onRoomStateUpdate] 回调通知成功退出房间，同时同房间其他用户会接收到 [onRoomUserUpdate] 回调通知。
    /// [roomID] 用户已登录的房间 ID，最大长度为 128 字节的字符串。
    Future<void> logoutRoom(String roomID) async {
        return await ZegoExpressImpl.instance.logoutRoom(roomID);
    }
}