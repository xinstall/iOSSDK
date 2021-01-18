//  1.1.11
//  XinstallSDK.h
//  XinstallSDK
//
//  Created by Xinstall on 2020/5/7.
//  Copyright © 2020 shu bao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XinstallData.h"
#import "XinstallDefault.h"

@protocol XinstallDelegate <NSObject>
@optional

/**
 * 一键拉起时获取 H5页面 携带的动态参数，参数中如果携带渠道，也会在方法中一起返回渠道号
 * @param appData 动态参数对象
 * appData 的 uo co 数据如果前端传入不是正常的json 数据，会返回前端传入的 String ，如果为正常 JSON 数据 会返回字典或数组
 */
- (void)xinstall_getWakeUpParams:(nullable XinstallData *)appData;

/**
 * 安装时获取 H5页面 携带的动态参数，参数中如果携带渠道，也会在方法中一起返回渠道号
 * @param appData 动态参数对象
 * appData 的 uo co 数据如果前端传入不是正常的json 数据，会返回前端传入的 String ，如果为正常 JSON 数据 会返回字典或数组
 */
- (void)xinstall_getInstallParams:(nullable XinstallData *)appData;

@end

@interface XinstallSDK : NSObject
#pragma mark - properties methods

/**
 * 获取 Xinstall SDK 当前版本
 */
+ (NSString *_Nullable)sdkVersion;

+ (instancetype _Nullable)defaultManager;

/**
 * 【重要】初始化 Xinstall SDK
 * 该方法只需要调用一次，调用实际尽量提前，一般在 App 启动时调用该方法进行初始化
 * 调用该方法前，需在 Info.plist 文件中配置键值对，键为固定值 com.xinstall.APP_KEY ，值为 Xinstall 后台对应应用的 appKey，可在 Xinstall 官方后台获取
 *
 * @param delegate 实现 XinstallDelegate 的对象
 */
+ (void)initWithDelegate:(id<XinstallDelegate> _Nonnull)delegate;

/// 对象为空则代表非本次安装
@property (nonatomic, strong) XinstallData * __nullable installData;

/**
 * 处理 通用链接
 * @param userActivity 由 AppDelegate 和 SceneDelegate 内对应方法中传入
 * @return 本次唤起是否被 Xinstall 正常处理
 */
+ (BOOL)continueUserActivity:(NSUserActivity *_Nullable)userActivity;

/**
 * 上报一次注册量
 *
 * 调用该方法后，会上报对应渠道的一次注册量，可以在 Xinstall 管理后台对应 App 的渠道报表中看到累计注册量等数据
 * 一般该方法会在 App 业务注册后进行调用，在实际使用场景中请注意不要重复调用，以免注册量上报次数过多
 */
+ (void)reportRegister;

/**
 * 上报一次事件（必须预先在 Xinstall 后台对应 App 内创建好事件ID，才能正确统计进去）
 *
 * 调用该方法后，会上报一次对应事件。上报机制非实时，会存在一定的延时。
 *
 * @param effectID 事件ID（在 Xinstall 后台预先创建）
 * @param effectValue 事件值（精确到整数）
 */
- (void)reportEffectPoint:(NSString *_Nonnull)effectID effectValue:(long)effectValue;

///**
// * 事件时长统计
//*调用该方法后，会上报一次对应事件。上报机制非实时，会存在一定的延时。
//* @param effectID 事件ID（在 Xinstall 后台预先创建）
//* @param effectValue 事件值（精确到整数）
// * @param duration 时间长度
// */
//- (void)reportEffectPoint:(NSString *_Nonnull)effectID effectValue:(long)effectValue duration:(NSInteger)duration;

@end
