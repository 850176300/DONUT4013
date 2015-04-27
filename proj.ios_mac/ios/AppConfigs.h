//
//  AppConfigs.h
//  AdsPopupDemo
//
//  Created by tang.wei on 13-12-13.
//  Copyright (c) 2013年 tang.wei. All rights reserved.
//

/*************************** MoPub账号设置 **************************/

#define IPHONE_BANNER_ID         @"a41992b322aa4850a63e8bd37485332f"
#define IPHONE_FULLSCREEN_ID     @"311d40887bde4c768629c55beb84c089"

#define IPAD_BANNER_ID           @"9f694c799597405785cd8f421fb5061d"
#define IPAD_FULLSCREEN_ID       @"eaf72b51aee946769368d11883266d38"

#define IPHONE_FULLSCREEN_ID_CROSSPROMO   @"32eb4e3850844b57a3f61c1471fcf5a2"
#define IPAD_FULLSCREEN_ID_CROSSPROMO     @"7ea405c38c574501aeff0427f80198aa"
// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @""
#define CHARTBOOST_APP_SIGNATURE @""

/*************************** Flurry账号 ****************************/
// 如果留空不设置，那么表示不支持Flurry
#define FLURRY_KEY               @"8SKWR4J9SZT5KKG8XCJK"

/*************************** Privacy Page设置 ****************************/
// 如果留空默认为http://www.xxx.com/privacy.html, 其中xxx为bundle id中间部分
#define PRIVACY_PAGE_URL         @""