//
//  AppConfigs.h
//  AdsPopupDemo
//
//  Created by tang.wei on 13-12-13.
//  Copyright (c) 2013年 tang.wei. All rights reserved.
//

/*************************** MoPub账号设置 **************************/

#define IPHONE_BANNER_ID         @"406988cd0de14a66803d385f77c154b6"
#define IPHONE_FULLSCREEN_ID     @"b18b48f2e3a74fafae206fe81fa63308"

#define IPAD_BANNER_ID           @"5cb1a789d5364b1c85b790bcb5677c87"
#define IPAD_FULLSCREEN_ID       @"0bf7d00c62e64b67a6d56ea799657d59"

#define IPHONE_FULLSCREEN_ID_CROSSPROMO   @"5cd0a210619d44caa9740178d6436bb5"
#define IPAD_FULLSCREEN_ID_CROSSPROMO     @"590f650e477043af84ce76e1ed47d29a"
// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @""
#define CHARTBOOST_APP_SIGNATURE @""

/*************************** Flurry账号 ****************************/
// 如果留空不设置，那么表示不支持Flurry
#define FLURRY_KEY               @"RCXJ7FVVK39YPHKKH4S4"

/*************************** Privacy Page设置 ****************************/
// 如果留空默认为http://www.xxx.com/privacy.html, 其中xxx为bundle id中间部分
#define PRIVACY_PAGE_URL         @""