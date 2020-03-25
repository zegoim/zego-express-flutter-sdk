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

    /// 房间状态变化通知
    ///
    /// 当房间的连接状态发生变化时将触发此回调，并告知发生变化的原因。开发者可根据此回调来判断当前用户在房间的状态，若长时间处于正在请求连接状态，一般情况大概率是因为用户端网络不稳定导致。
    /// [state] 变化后的的房间状态
    /// [errorCode] 错误码，详情请参考常用 [错误码文档](https://doc-zh.zego.im/zh/308.html)
    /// [extendedData] 扩展信息
    /// [roomID] 房间 ID，最大长度为 128 字节的字符串。
    static void Function(String roomID, ZegoRoomState state, int errorCode) onRoomStateUpdate;



    /// 房间内其他用户增加或减少的通知回调。
    ///
    /// 注意，只有在登录房间时传的 ZegoRoomConfig 中的 isUserStatusNotify 参数为 YES 时才会触发这个回调。
    /// [updateType] 更新类型（添加/删除）
    /// [userList] 当前房间内的用户列表
    /// [roomID] 用户已登录的房间 ID，最大长度为 128 字节的字符串。
    static void Function(String roomID, ZegoUpdateType updateType, List<ZegoUser> userList) onRoomUserUpdate;
}