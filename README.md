# iOS SDK 集成指南

## 一、导入 SDK

### 1. CocoaPods导入

在Podfile文件中添加命令: pod 'XinstallSDK'，执行pod install后重新打开项目，完成集成.

注意：已经通过pod方式集成过Xinstall，建议先```pod update```来升级本机 pod 仓库，否则会导致获取不到最新版本

如果搜索不到 **XinstallSDK**, 请按下面操作:

1)、先执行: ```pod setup```

2)、再清空搜索索引，让pod重建索引(可能需要等待一会):
```xml
rm ~/Library/Caches/CocoaPods/search_ index.json
```

### 2. 手动导入

从[下载中心](https://xinstall.com/download_center/)下载 iOS 最新版本 SDK
解压 SDK 压缩包，将文件夹导入工程中（导入请选中 "Copy items if needed" )

## 二、初始化

在AppDelegate中，增加头文件的引用：
```objective-c
#import "XinstallSDK.h"
```

> Swift 开发时，请使用桥接方式引入

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
<string>8j7foaw</string>
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

**Xinstall** 通过 Scheme(iOS < 9)  和  universal link（iOS≥9） ,在app已安装的情况下，从各种浏览器（包括微信、QQ、新浪微博、钉钉等主流社交软件的内置浏览器）拉起app并传递动态参数，避免重复安装。

##### 2.1 通用链接集成（Universal Link)

首先，我们需要到[苹果开发者网站](https://developer.apple.com/)，为当前的 App ID 开启关联域名 (Associated Domains) 服务：

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS1.png)

为刚才开发关联域名功能的 App ID 创建新的（或更新现有的）描述文件，下载并导入到Xcode中(通过xcode自动生成的描述文件，可跳过这一步)：

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS2.png)在Xcode中配置Xinstall为当前应用生成的关联域名 (Associated Domains) ：**applinks:xxxx.xinstall.net**

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS3.png)

在AppDelegate中添加通用链接 (Universal Link) 回调方法，委托给 XinstallSDK 来处理

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

**注意iOS13之后如果有使用SceneDelegate, 则调用的方法需要放SceneDelegate,如果未使用SceneDelegate，可以直接跳过该配置**

```objective-c
// Objective-C
// 注意iOS13之后如果有SceneDelegate, 则调用的方法需要放SceneDelegate,低版本暂不支持SceneDelegate，可以直接跳过该配置

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions  API_AVAILABLE(ios(13.0)) {
    // Use this method to optionally configure and attach the UIWindow `window` to the provided UIWindowScene `scene`.
    // If using a storyboard, the `window` property will automatically be initialized and attached to the scene.
    // This delegate does not imply the connecting scene or session are new (see `application:configurationForConnectingSceneSession` instead).

    // 走scene 如果是universal Link 冷启动不会调SceneDelegate 的 - (void)scene:(UIScene *)scene continueUserActivity:(NSUserActivity *)userActivity 方法
    if (connectionOptions.userActivities.count > 0 ){
        [connectionOptions.userActivities enumerateObjectsUsingBlock:^(NSUserActivity * _Nonnull obj, BOOL * _Nonnull stop) {
            if ([XinstallSDK continueUserActivity:obj]) {
                 *stop = YES;
            }
        }];
    }
}

- (void)scene:(UIScene *)scene continueUserActivity:(NSUserActivity *)userActivity {
  NSLog(@"userActivity : %@",userActivity.webpageURL.description);
  if (![XinstallSDK continueUserActivity:userActivity]) {
    // 其他第三方回调
  }
}
```

```swift
// Swift
func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
  // Use this method to optionally configure and attach the UIWindow `window` to the provided UIWindowScene `scene`.
  // If using a storyboard, the `window` property will automatically be initialized and attached to the scene.
  // This delegate does not imply the connecting scene or session are new (see `application:configurationForConnectingSceneSession` instead).
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

  } else {
    //
  }
}
```



##### 2.2 集成Scheme

<font color=#FF0000 >此方法为XinstallSDK 1.2.2最新添加的方法，如果使用报错，请先更新SDK
更多问题见</font>[iOS常见问题](../QA/iOSWrong.html#4%E3%80%81%E4%B8%BA%E4%BB%80%E4%B9%88sdk%E7%9A%84%E6%96%B9%E6%B3%95%E5%92%8C%E6%96%87%E6%A1%A3%E4%B8%AD%E7%9A%84%E6%96%B9%E6%B3%95%E4%B8%8D%E4%B8%80%E8%87%B4)


2.2.1 在Xcode选中**Target**  ->  **Info**  ->  **URL Types**,配置**Xinstall** 为当前应用生成的 Scheme,如图所示：

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS6.png)

图中4中的值为实例值，具体为xi+appkey格式，如xi4z9yxu8，您也可以在[iOS集成-功能集成-一键拉起](https://www.xinstall.com/admin/integration/ios#2)的文档中找到您对应的Scheme值

2.2.2 在 **AppDelegate** 的两个**scheme**回调方法中添加**Xinstall**的Scheme方法

```objective-c
// Objective-C
// iOS9以上会优先走这个方法
- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	[XinstallSDK handleSchemeURL:url];
	return YES;
}

// iOS9以下调用这个方法
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(id)annotation {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	[XinstallSDK handleSchemeURL:url];
	return YES;
}
```

```swift
// Swift
// iOS9以上会优先走这个方法
func application(_ app: UIApplication, open url: URL, options: [UIApplication.OpenURLOptionsKey : Any] = [:]) -> Bool {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	XinstallSDK.handleSchemeURL(url)
	return true
}

// iOS9以下调用这个方法
func application(_ application: UIApplication, open url: URL, sourceApplication: String?, annotation: Any) -> Bool {
	// 处理通过Xinstall URL SchemeURL 唤起App的数据
	XinstallSDK.handleSchemeURL(url)
	return true
}
```

2.2.3 iOS13之后如果有SceneDelegate, 则调用的方法需要放`SceneDelegate.m`中。如果未使用SceneDelegate， 可以直接跳过该配置

```objective-c
// Objective-C
- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions  API_AVAILABLE(ios(13.0)) {
    // Use this method to optionally configure and attach the UIWindow `window` to the provided UIWindowScene `scene`.
    // If using a storyboard, the `window` property will automatically be initialized and attached to the scene.
    // This delegate does not imply the connecting scene or session are new (see `application:configurationForConnectingSceneSession` instead).

    //一般放最底部
    //scheme相关代码
    for (UIOpenURLContext *urlcontext in connectionOptions.URLContexts) {
            [XinstallSDK handleSchemeURL:urlcontext.URL];
    }
}

- (void)scene:(UIScene *)scene openURLContexts:(NSSet<UIOpenURLContext *> *)URLContexts API_AVAILABLE(ios(13.0)) {
    for (UIOpenURLContext *urlcontext in URLContexts) {
    	//scheme相关代码
        [XinstallSDK handleSchemeURL:urlcontext.URL];
    }
}
```

```swift
// Swift
func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
	// Use this method to optionally configure and attach the UIWindow `window` to the provided UIWindowScene `scene`.
	// If using a storyboard, the `window` property will automatically be initialized and attached to the scene.
	// This delegate does not imply the connecting scene or session are new (see `application:configurationForConnectingSceneSession` instead).
	guard let _ = (scene as? UIWindowScene) else { return }

​	//一般放最底部
​	//scheme相关代码
​	for urlcontext: UIOpenURLContext in connectionOptions.urlContexts {
​		XinstallSDK.handleSchemeURL(urlcontext.url);
​	}

}

func scene(_ scene: UIScene, openURLContexts URLContexts: Set<UIOpenURLContext>) {
	//scheme相关代码
	for urlcontext in URLContexts {
		XinstallSDK.handleSchemeURL(urlcontext.url);
	}
}
```



##### 2.3 添加跳转参数回调方法

首先，在AppDelegate中遵守协议并且添加一键拉起的回调，获取来自H5页面的动态参数，根据参数值执行相关操作，如跳转体验页面等

```objective-c
// Objective-C
// 通过Xinstall获取已经安装App被唤醒时的参数（如果是通过渠道页面唤醒App时，会返回渠道编号）
- (void)xinstall_getWakeUpParams:(nullable XinstallData *)appData {
    if (appData.data) {//(动态唤醒参数)
        //e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
        // uo 为H5网址后拼接的参数，如下载地址 https://app.xinstall.com/app/8l4hkz1?channelCode=zhihu&key1=value1&key2=value2，则uo为{channelCode=zhihu;key1=value1;key2=value2};
        id uo = [appData.data objectForKey:@"uo"];
        // co 通过H5按钮点击事件传递参数， Xinstall支持单页面传递多事件参数。
        id co = [appData.data objectForKey:@"co"];
    }
    if (appData.channelCode) {//(通过渠道链接或二维码唤醒会返回渠道编号)
        //e.g.可自己统计渠道相关数据等
    }
    if (appData.timeSpan > 20 ) {
        //e.g. 超过20s不处理
    }
    NSLog(@"XinstallSDK:\n动态参数：%@;\n渠道编号：%@",appData.data,appData.channelCode);

}
```

```swift
// Swift
func xinstall_getWakeUpParams(_ appData: XinstallData?) {
  if appData?.data != nil { //(动态唤醒参数)
    // e.g.如免填邀请码建立邀请关系、自动加好友、自动进入某个群组或房间等
  }
  if appData?.data != nil {//(通过渠道链接或二维码唤醒会返回渠道编号)
    // e.g.可自己统计渠道相关数据等
  }
  if appData != nil && appData!.timeSpan > 20 {
    // e.g. 超过20s不处理
  }
  print("唤醒参数 data = \(String(describing: appData?.data)),channelCode = \(String(describing: appData?.channelCode))")
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

**【注意】若 installData 不为空，但 installData.channelCode 为空时，代表本次安装不来自于渠道 **

### 4. 高级数据统计

SDK初始化时会自动完成访问量、点击量、安装量、活跃量、留存率等统计工作。 除此之外我们还提供了一下两种数据统计服务：

#### 注册量统计

 如需统计每个渠道的注册量（对评估渠道质量很重要），可根据自身的业务规则，在确保用户完成app注册的情况下调用相关api

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

#### 事件统计

事件统计，主要用来统计终端用户对于某些特殊业务的使用效果，如充值金额，分享次数，广告浏览次数等等。

调用接口前，需要先进入管理后台**事件统计**然后点击新增事件

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS5.png)

调用接口时，需要保证代码中的事件ID与后台创建的事件ID一致

```objective-c
// Objective-C
[[XinstallSDK defaultManager] reportEventPoint:@"事件ID" eventValue:123];
```

```swift
// Swift
XinstallSDK.defaultManager()?.reportEventPoint(@"事件ID", eventValue: 123)
```

**注：**只有后台创建事件统计，并且代码中传递的事件ID与后台创建的ID一致时，上报数据才会被统计.

## 四、导出ipa包并上传

**点击上传ipa**，我们会帮您做ipa相关信息判断，如果符合标准，则可以点击上传，如果存在相关警告，则需要您注意相关设置。

![](https://cdn.xinstall.com//iOS_SDK%E7%B4%A0%E6%9D%90/iOS4.png)
