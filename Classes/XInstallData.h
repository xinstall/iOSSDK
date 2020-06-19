//
//  XInstallData.h
//  XInstall
//
//  Created by huawenjie on 2020/5/7.
//  Copyright © 2020 jiwei. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface XInstallData : NSObject <NSCopying>

- (instancetype)initWithData:(NSDictionary *)data
                 channelCode:(NSString *)channelCode;
                

@property (nonatomic, strong) NSDictionary *data;//动态参数
@property (nonatomic, copy) NSString *channelCode;//渠道编号
@property (nonatomic, assign) NSInteger timeSpan;//时间间隔s

/// 安装网络请求是否成功
@property (nonatomic,assign) BOOL installRequestSuccess;

@end

NS_ASSUME_NONNULL_END
