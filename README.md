# iOS SDK 集成指南

> 如需使用 Xinstall 的广告平台渠道功能，请按照本文档集成完毕后，继续阅读[《广告平台渠道 iOS 集成指南》](https://doc.xinstall.com/AD/iOSGuide.html)文档，完成额外的集成工作后，才能正常使用广告平台渠道功能。

## 一、导入 SDK

### 1. CocoaPods导入

在Podfile文件中添加命令: `pod 'XinstallSDK'`，执行 `pod install` 后重新打开项目，完成集成

注意：已经通过 pod 方式集成过 Xinstall，建议先 `pod update` 来升级本机 pod 仓库，否则会导致获取不到最新版本

如果搜索不到 **XinstallSDK**，请按下面操作:

1)、先执行: ```pod setup```

2)、再清空搜索索引，让pod重建索引(可能需要等待一会):
```xml
rm ~/Library/Caches/CocoaPods/search_ index.json
```

### 2. 手动导入

从[下载中心](https://xinstall.com/download_center/)下载 iOS 最新版本 SDK 压缩包
解压后，将文件夹导入工程中（导入时请选择 "Copy items if needed" )

## 二、初始化

在 AppDelegate 中，增加头文件的引用：
```objective-c
// Swift 开发时，请使用桥接方式引入
#import "XinstallSDK.h"
```

> 点击[这里](https://doc.xinstall.com/QA/iOSWrong.html#5、使用swift开发时如何桥接sdk)，查看如何在 Swift 开发时进行桥接

增加遵守的协议：

```objective-c
// Objective-C
@interface AppDelegate ()<XinstallDelegate>
```

```swift
// Swift
class AppDelegate: UIResponder, UIApplicationDelegate, XinstallDelegate
```

在Info.plist文件中配置appKey键值对，如下：

```xml
<key>com.xinstall.APP_KEY</key>
<string>xxxxxxx</string>
```

在 `application: didFinishLaunchingWithOptions:` 方法中调用 `initWithDelegate:` 方法来初始化SDK，如下代码所示：

```objective-c
// Objective-C
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  [XinstallSDK initWithDelegate:self];
  return YES;
}
```

```swift
// Swift
func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
  XinstallSDK.initWith(self);
  return true
}
```




## 三、功能集成

### 1. 快速下载
  如果只需要快速下载功能，无需其它功能（携带参数安装、渠道统计、一键拉起），完成初始化即可

### 2. 一键拉起

**Xinstall** 通过 Scheme (iOS < 9) 和 Universal Links (iOS ≥ 9) 两种技术，在 App 已安装的情况下，从各种浏览器（包括微信、QQ、新浪微博、钉钉等主流社交软件的内置浏览器）拉起 App 并传递拉起参数。

> 本文中描述的『唤醒』和『拉起』均指代该业务行为

##### 2.1 通用链接集成（Universal Link)

首先，我们需要到[苹果开发者网站](https://developer.apple.com/)，为当前的 App ID 开启关联域名 (Associated Domains) 服务：

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS1.png)

为刚才开启关联域名功能的 App ID 创建新的（或更新现有的）描述文件，下载并导入到 Xcode 中（通过 Xcode 自动生成的描述文件，可跳过这一步）：

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS2.png)在 Xcode 中配置 Xinstall 为当前应用生成的关联域名（Associated Domains）：**applinks:xxxx.xinstall.top** 和 **applinks:xxxx.xinstall.net**

> 具体的关联域名可在 Xinstall管理后台 - 对应的应用控制台 - iOS下载配置 页面中找到

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS3.png)

在 AppDelegate 的通用链接（Universal Links）相关方法，添加 Xinstall SDK 提供的处理代码，将拉起参数提交给 XinstallSDK 来处理

```objective-c
// Objective-C
- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray * _Nullable))restorationHandler{
  // 判断是否通过Xinstall Universal Link 唤起App
  if ([XinstallSDK continueUserActivity:userActivity]){//如果使用了Universal link ，此方法必写
    return YES;
  }
  // 其他第三方回调；
  return YES;
}
```

```swift
// Swift
func application(_ application: UIApplication, continue userActivity: NSUserActivity, restorationHandler: @escaping ([Any]?) -> Void) -> Bool{
  if XinstallSDK.continue(userActivity) {
    return true
  } // 如果使用了Universal link ，此方法必写
  // 其他第三方回调:
  return true
}
// 注意，Swift4.2版本，系统方法修改为：
func application(_ application: UIApplication, continue userActivity: NSUserActivity, restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
  if XinstallSDK.continue(userActivity) {
    return true
  } // 如果使用了Universal link ，此方法必写
  // 其他第三方回调:
  return true
}
```

**注意 iOS13 之后如果使用了 SceneDelegate，则需要在 SceneDelegate 内相关方法中添加 Xinstall SDK 提供的处理代码。如果未使用 SceneDelegate，可以直接跳过该配置**

```objective-c
// Objective-C
// 注意 iOS13 之后如果有 SceneDelegate，则调用的方法需要放 SceneDelegate，低版本暂不支持 SceneDelegate，可以直接跳过该配置
- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions {
    // 走 scene 如果是 Universal Links 冷启动不会调用 SceneDelegate 的 - (void)scene:(UIScene *)scene continueUserActivity:(NSUserActivity *)userActivity 方法
    if (connectionOptions.userActivities.count > 0 ) {
        [connectionOptions.userActivities enumerateObjectsUsingBlock:^(NSUserActivity * _Nonnull obj, BOOL * _Nonnull stop) {
            if ([XinstallSDK continueUserActivity:obj]) {
                 *stop = YES;
            }
        }];
    }
}

- (void)scene:(UIScene *)scene continueUserActivity:(NSUserActivity *)userActivity {
  if (![XinstallSDK continueUserActivity:userActivity]) {
    // 其他第三方回调
  }
}
```

```swift
// Swift
func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
  guard let _ = (scene as? UIWindowScene) else { return }
  
  if connectionOptions.userActivities.count > 0 {
    for (_, userActivity) in connectionOptions.userActivities.enumerated() {
    if XinstallSDK.continue(userActivity) {
          break;
      }
    }
  }
}

func scene(_ scene: UIScene, continue userActivity: NSUserActivity) {
  if XinstallSDK.continue(userActivity) {

  }
}
```



##### 2.2 集成Scheme

<font color=#FF0000 >此方法为 XinstallSDK 1.2.2 最新添加的方法，如果使用报错，请先更新SDK。更多问题见</font>[iOS常见问题](../QA/iOSWrong.html#4%E3%80%81%E4%B8%BA%E4%BB%80%E4%B9%88sdk%E7%9A%84%E6%96%B9%E6%B3%95%E5%92%8C%E6%96%87%E6%A1%A3%E4%B8%AD%E7%9A%84%E6%96%B9%E6%B3%95%E4%B8%8D%E4%B8%80%E8%87%B4)

2.2.1 在 Xcode 选中**Target** -> **Info** -> **URL Types**，配置 **Xinstall** 为当前应用生成的 Scheme，如图所示：

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS6.png)

上图中的值仅为示例，具体为 xi+appkey 格式，如 xi4z9yxu8，您也可以在[iOS集成-功能集成-一键拉起](https://www.xinstall.com/admin/integration/ios#2)的文档中找到您对应的 Scheme 值

2.2.2 在 **AppDelegate** 的两个 **scheme** 回调方法中添加 **Xinstall** 的 Scheme 方法

```objective-c
// Objective-C
// iOS9 以上会走这个方法
- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	[XinstallSDK handleSchemeURL:url];
	return YES;
}

// iOS9 以下会走这个方法
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(id)annotation {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	[XinstallSDK handleSchemeURL:url];
	return YES;
}
```

```swift
// Swift
// iOS9 以上会走这个方法
func application(_ app: UIApplication, open url: URL, options: [UIApplication.OpenURLOptionsKey : Any] = [:]) -> Bool {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	XinstallSDK.handleSchemeURL(url)
	return true
}

// iOS9 以下会走这个方法
func application(_ application: UIApplication, open url: URL, sourceApplication: String?, annotation: Any) -> Bool {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	XinstallSDK.handleSchemeURL(url)
	return true
}
```

2.2.3 iOS13之后如果有 SceneDelegate，则调用的方法需要放在 `SceneDelegate.m` 中。如果未使用 SceneDelegate， 可以直接跳过该配置

```objective-c
// Objective-C
- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions {
    // scheme相关代码
    for (UIOpenURLContext *urlcontext in connectionOptions.URLContexts) {
            [XinstallSDK handleSchemeURL:urlcontext.URL];
    }
}

- (void)scene:(UIScene *)scene openURLContexts:(NSSet<UIOpenURLContext *> *)URLContexts {
    // scheme相关代码
    for (UIOpenURLContext *urlcontext in URLContexts) {
        [XinstallSDK handleSchemeURL:urlcontext.URL];
    }
}
```

```swift
// Swift
func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
	guard let _ = (scene as? UIWindowScene) else { return }
​	// scheme相关代码
​	for urlcontext: UIOpenURLContext in connectionOptions.urlContexts {
​		XinstallSDK.handleSchemeURL(urlcontext.url);
​	}
}

func scene(_ scene: UIScene, openURLContexts URLContexts: Set<UIOpenURLContext>) {
	// scheme相关代码
	for urlcontext in URLContexts {
		XinstallSDK.handleSchemeURL(urlcontext.url);
	}
}
```



##### 2.3 添加拉起参数回调方法

在 AppDelegate 中遵守 XinstallDelegate 协议并且添加一键拉起的回调，获取来自 H5页面 的动态参数，根据参数值执行相关操作，如跳转体验页面等

> 您可以在下述两种回调方法中任选一个进行实现，不同的回调方法有不同的逻辑，请选择最符合您实际场景的方法进行实现，请勿同时实现两个方法。

**【方法一】：`- (void)xinstall_getWakeUpParams:(nullable XinstallData \*)appData;` 该方法只会在成功获取到拉起参数时，才会回调。如果无法成功获取到拉起参数，例如不是集成了 Xinstall Web SDK 的页面拉起您的 App 时，将会无法获取到拉起参数，也就不会执行该回调方法。**

```objective-c
// Objective-C
// 通过 Xinstall 获取已经安装 App 被拉起时的参数（如果是通过渠道页面拉起 App 时，会返回渠道编号）
- (void)xinstall_getWakeUpParams:(nullable XinstallData *)appData {
    if (appData.data) {  // 拉起参数
        // e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
        // uo 为H5网址后拼接的参数，如下载地址 https://app.xinstall.com/app/8l4hkz1?channelCode=zhihu&key1=value1&key2=value2，则uo为{channelCode=zhihu;key1=value1;key2=value2};
        id uo = [appData.data objectForKey:@"uo"];
        // co 为通过H5按钮点击事件传递参数， Xinstall 支持单页面传递多事件参数。
        id co = [appData.data objectForKey:@"co"];
    }
    if (appData.channelCode) {  // 通过渠道链接拉起时，会返回渠道编号
        // e.g.可自己统计渠道相关数据等
    }
    NSLog(@"XinstallSDK:\n拉起参数：%@;\n渠道编号：%@", appData.data, appData.channelCode);
}
```

```swift
// Swift
func xinstall_getWakeUpParams(_ appData: XinstallData?) {
  if appData?.data != nil { // 拉起参数
    // e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
  }
  if appData?.channelCode != nil {  // 通过渠道链接拉起时，会返回渠道编号
    // e.g.可自己统计渠道相关数据等
  }
  print("唤醒参数 data = \(String(describing: appData?.data)), channelCode = \(String(describing: appData?.channelCode))")
}
```

**【方法二】：`- (void)xinstall_getWakeUpParams:(nullable XinstallData \*)appData error:(nullable XinstallError \*)error;` 该方法无论是否成功获取到拉起参数，均会回调。如果成功获取到拉起参数，则 appData != nil 并且 error == nil；如果没有获取到拉起参数，则 appData == nil 并且 error != nil。**

```objective-c
// Objective-C
// 通过 Xinstall 获取已经安装 App 被拉起时的参数（如果是通过渠道页面拉起 App 时，会返回渠道编号）
- (void)xinstall_getWakeUpParams:(XinstallData *)appData error:(XinstallError *)error {
    if (error) {
        NSLog(@"没有获取到拉起参数，错误类型：%li，错误描述：%@", (long)error.type, error.errorMsg);
    } else {
        if (appData.data) {  // 拉起参数
            // e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
            // uo 为H5网址后拼接的参数，如下载地址 https://app.xinstall.com/app/8l4hkz1?channelCode=zhihu&key1=value1&key2=value2，则uo为{channelCode=zhihu;key1=value1;key2=value2};
            id uo = [appData.data objectForKey:@"uo"];
            // co 为通过H5按钮点击事件传递参数， Xinstall 支持单页面传递多事件参数。
            id co = [appData.data objectForKey:@"co"];
        }
        if (appData.channelCode) {  // 通过渠道链接拉起时，会返回渠道编号
            // e.g.可自己统计渠道相关数据等
        }
        NSLog(@"XinstallSDK:\n拉起参数：%@;\n渠道编号：%@", appData.data, appData.channelCode);
    }
}
```

```swift
// Swift
func xinstall_getWakeUpParams(_ appData: XinstallData?, error: XinstallError?) {
    if error != nil {
        print("没有获取到拉起参数，错误类型：\(String(describing: error?.type))，错误描述：\(String(describing: error?.errorMsg))")
    } else {
        if appData?.data != nil { // 拉起参数
            // e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
        }
        if appData?.channelCode != nil {  // 通过渠道链接拉起时，会返回渠道编号
            // e.g.可自己统计渠道相关数据等
        }
        print("唤醒参数 data = \(String(describing: appData?.data)), channelCode = \(String(describing: appData?.channelCode))")
    }
}
```

### 3. 携带参数安装/唤起

在 App 业务中，需要获取到安装参数时，可以调用 `getInstallParamsWithCompletion:` 方法获取来自H5页面的动态参数。在回调中获取到安装参数后，可以执行下一步业务操作，比如上报渠道信息，直达对应游戏房间，跳转指定页面等：

```objective-c
// Objective-C
// 安装App后，通过 XinstallSDK 获取到传递的参数（如果是通过渠道页面安装App时，会返回渠道编号）
[[XinstallSDK defaultManager] getInstallParamsWithCompletion:^(XinstallData * _Nullable installData, XinstallError * _Nullable error) {
    if (error == nil) {
        // error 为 nil 时代表获取安装参数成功
        if (installData.data) { // 安装参数
            // e.g. 如免填邀请码绑定邀请关系、自动加好友、自动进入某个群组或房间等
            // uo 为H5网址后拼接的参数，如下载地址 https://app.xinstall.com/app/8l4hkz1?channelCode=zhihu&key1=value1&key2=value2，则uo为{channelCode=zhihu;key1=value1;key2=value2};
            id uo = [installData.data objectForKey:@"uo"];
            // co 通过H5按钮点击事件传递参数， Xinstall 支持单页面传递多事件参数。
            id co = [installData.data objectForKey:@"co"];
        }
        if (installData.channelCode) {// 通过渠道链接或二维码唤醒会返回渠道编号
            // e.g.可自己统计渠道相关数据等
        }
        if (installData.timeSpan > 20 ) {
            // e.g. 超过20s不处理
        }
        if (installData.isFirstFetch) {
            // e.g. 第一次获取到安装参数
        }
        NSLog(@"XinstallSDK:\n动态参数：%@;\n渠道编号：%@", installData.data, installData.channelCode);
    } else {
        // 获取安装参数时发生错误，具体可根据 error.type 和 error.errorMsg 的值进行下一步处理
    }
}];
```

```swift
// Swift
// 安装App后，通过 XinstallSDK 获取到传递的参数（如果是通过渠道页面安装App时，会返回渠道编号）
XinstallSDK.defaultManager()?.getInstallParams(completion: { (installData: XinstallData?, error: XinstallError?) in
    if error == nil {
        if installData?.data != nil { // 安装参数
            // e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
            // uo 为H5网址后拼接的参数，如下载地址 https://app.xinstall.com/app/8l4hkz1?channelCode=zhihu&key1=value1&key2=value2，则uo为{channelCode=zhihu;key1=value1;key2=value2};
            var uo = installData?.data["uo"];
            // co 通过H5按钮点击事件传递参数， Xinstall 支持单页面传递多事件参数。
            var co = installData?.data["co"];
        }
        if installData?.channelCode != nil { // 通过渠道链接或二维码安装会返回渠道编号
            // e.g.可自己统计渠道相关数据等
        }
        if installData != nil && installData!.timeSpan > 20 {
            // e.g. 超过20s不处理
        }
        if installData?.isFirstFetch == true {
            // e.g. 第一次获取到安装参数
        }
        print("唤醒参数 data = \(String(describing: installData?.data)),channelCode = \(String(describing: installData?.channelCode))")
    } else {
        // 获取安装参数时发生错误，具体可根据 error.type 和 error.errorMsg 的值进行下一步处理
    }
})
```

**【注意】若 installData 不为空，但 installData.channelCode 为空时，代表本次安装不来自于渠道**

> 您可以在 Xinstall 管理后台对应的 App 内，看到所有的传递参数以及参数出现的次数，方便你们做运营统计分析，如通过该报表知道哪些页面或代理带来了最多客户，客户最感兴趣的 App 页面是什么等。具体参数名和值，运营人员可以和技术协商定义，或联系 Xinstall 客服咨询。具体效果如下图：
>
> ![传参报表](https://cdn.xinstall.com/iOS_SDK%E7%B4%A0%E6%9D%90/paramsTable.png)



### 4. 高级数据统计

SDK初始化时会自动完成访问量、点击量、安装量、活跃量、留存率等统计工作。 除此之外我们还提供了以下三种数据统计服务：

#### 注册量统计

如需统计每个渠道的注册量（对评估渠道质量很重要），可根据自身的业务规则，在确保用户完成 App 注册的情况下调用如下方法，即可上报一次注册量：

```objective-c
// Objective-C
// 用户注册成功后调用
[XinstallSDK reportRegister];
```

```swift
// Swift
// 用户注册成功后调用
XinstallSDK.reportRegister()
```

> 您可在 Xinstall 管理后台对应的 App 的『渠道报表』页面中查看每个渠道上报的注册量

#### 事件统计

事件统计，主要用来统计终端用户对于某些特殊业务的使用效果，如充值金额，分享次数，广告浏览次数等等。

调用接口前，需要先进入管理后台**事件统计**然后点击新增事件

![](https://cdn.xinstall.com/iOS_SDK%E7%B4%A0%E6%9D%90/addEvent.jpg)

调用接口时，需要保证代码中的事件ID与后台创建的事件ID一致

```objective-c
// Objective-C
[[XinstallSDK defaultManager] reportEventPoint:@"事件ID" eventValue:123];
```

```swift
// Swift
XinstallSDK.defaultManager()?.reportEventPoint(@"事件ID", eventValue: 123)
```

> 注：只有后台创建事件统计，并且代码中传递的事件ID与后台创建的ID一致时，上报数据才会被统计.

您可以设置不同事件来满足不同的业务场景，如下图所示，您可以设置充值事件，进行上报。就可以统计到总的充值设备数为32个，累积充值金额为364元

![](https://cdn.xinstall.com/iOS_SDK%E7%B4%A0%E6%9D%90/eventTable.jpg)

### 5. 场景定制统计

#### 5.1 分享统计

场景业务介绍，可到[分享数据统计](https://doc.xinstall.com/environment/分享数据统计.html)页面查看

> 分享统计主要用来统计分享业务相关的数据，例如分享次数、分享查看人数、分享新增用户等。在用户分享操作触发后（注：此处为分享事件触发，非分享完成或成功），可调用如下方法上报一次分享数据：

```objective-c
// Objective-C
[[XinstallSDK defaultManager] reportShareByXinShareId:@"填写分享人或UID"];
```

```swift
// Swift
XinstallSDK.defaultManager()?.reportShare(byXinShareId: "填写分享人或UID")
```

> **注：**分享人或UID 可由您自行定义，只需要用以区分用户即可

您可在 Xinstall 管理后台 对应 App 中查看详细分享数据报表，表中的「分享人/UID」即为调用方法时携带的参数，其余字段含义可将鼠标移到字段右边的小问号上进行查看：

![](https://doc.xinstall.com/integrationGuide/share.jpg)



## 四、导出ipa包并上传

**上传 ipa 包时**，我们会帮您做 SDK 集成配置判断，如果符合标准，则可以点击上传，如果存在相关警告，则需要您注意相关设置。

![](https://doc.xinstall.com/integrationGuide/iOS4.jpg)
