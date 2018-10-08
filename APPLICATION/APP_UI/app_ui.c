#include "app_ui.h"
#include "sys_malloc.h"
#include "piclib.h"
#include "IconFrame.h"
#include "sys_parameters.h"
#include "includes.h"
#include <string.h>

#include "stamicroicon.h"


/******************* Icon Picture handles *******************************/
_IconPicHandle ICON_Rename = 
{
	.Path = "1:/ICON/grxx_n.jpg",
	.PathPress = "1:/ICON/grxx_h.jpg",
	.Size_X = MENU_ICON_WIDTH,
	.Size_Y = MENU_ICON_HEIGHT,
};

_IconPicHandle ICON_MeetInfo = 
{

	.Path = "1:/ICON/hyxx_n.jpg",
	.PathPress = "1:/ICON/hyxx_h.jpg",
	.Size_X = MENU_MEETINFO_WIDTH,
	.Size_Y = MENU_MEETINFO_HEIGHT,
};

_IconPicHandle ICON_Service = 
{
	.Path = "1:/ICON/hjfw_n.jpg",
	.PathPress = "1:/ICON/hjfw_h.jpg",
	.Size_X = MENU_ICON_WIDTH,
	.Size_Y = MENU_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle ICON_SMS = 
{

	.Path = "1:/ICON/dxfw_n.jpg",
	.PathPress = "1:/ICON/dxfw_h.jpg",
	.Size_X = MENU_ICON_WIDTH,
	.Size_Y = MENU_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle ICON_ShowName = 
{

	.Path = "1:/ICON/mpkj_n.jpg",
	.PathPress = "1:/ICON/mpkj_n.jpg",
	.Size_X = MENU_SHOWNAME_WIDTH,
	.Size_Y = MENU_SHOWNAME_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle ICON_Setting = 
{
	.Path = "1:/ICON/xtsz_n.jpg",
	.PathPress = "1:/ICON/xtsz_h.jpg",
	.Size_X = MENU_ICON_WIDTH,
	.Size_Y = MENU_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Coffee = 
{
	.Path = "1:/ICON/img_kafei_n.jpg",
	.PathPress = "1:/ICON/img_kafei_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Tea = 
{
	.Path = "1:/ICON/img_cha_n.jpg",
	.PathPress = "1:/ICON/img_cha_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Flower = 
{
	.Path = "1:/ICON/img_hua_n.jpg",
	.PathPress = "1:/ICON/img_hua_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Waiter = 
{
	.Path = "1:/ICON/img_fwy_n.jpg",
	.PathPress = "1:/ICON/img_fwy_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Microphone = 
{
	.Path = "1:/ICON/img_huatong_n.jpg",
	.PathPress = "1:/ICON/img_huatong_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Pen = 
{
	.Path = "1:/ICON/img_bi_n.jpg",
	.PathPress = "1:/ICON/img_bi_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Paper = 
{
	.Path = "1:/ICON/img_zhi_n.jpg",
	.PathPress = "1:/ICON/img_zhi_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SICON_Water = 
{
	.Path = "1:/ICON/img_shui_n.jpg",
	.PathPress = "1:/ICON/img_shui_n.jpg",
	.Size_X = SERVE_ICON_WIDTH,
	.Size_Y = SERVE_ICON_HEIGHT,
//	.Frame = gImage_128,
};

_IconPicHandle SMS_ICON_Send = 
{
	.Path = "1:/ICON/fasong_n.jpg",
	.PathPress = "1:/ICON/fasong_h.jpg",
	.Size_X = 95,
	.Size_Y = 296,
};

_IconPicHandle SMS_ICON_Choice = 
{
	.Path = "1:/ICON/xuanze_n.jpg",
	.PathPress = "1:/ICON/xuanze_h.jpg",
	.Size_X = 95,
	.Size_Y = 44,
};

//_IconPicHandle OICON_AddressSet = 
//{
//	.Path = "1:/ICON/oicon1.jpg",
//	.PathPress = "1:/ICON/opicon1.jpg",
//	.Size_X = ICON_Size,
//	.Size_Y = ICON_Size,
////	.Frame = gImage_128,
//};

//_IconPicHandle OICON_BackGroundSet = 
//{
//	.Path = "1:/ICON/oicon2.jpg",
//	.PathPress = "1:/ICON/opicon2.jpg",
//	.Size_X = ICON_Size,
//	.Size_Y = ICON_Size,
////	.Frame = gImage_128,
//};

//_IconPicHandle OICON_LightSet = 
//{
//	.Path = "1:/ICON/oicon3.jpg",
//	.PathPress = "1:/ICON/opicon3.jpg",
//	.Size_X = ICON_Size,
//	.Size_Y = ICON_Size,
////	.Frame = gImage_128,
//};

//_IconPicHandle OICON_SystemSta = 
//{
//	.Path = "1:/ICON/oicon4.jpg",
//	.PathPress = "1:/ICON/opicon4.jpg",
//	.Size_X = ICON_Size,
//	.Size_Y = ICON_Size,
////	.Frame = gImage_128,
//};

//_IconPicHandle OICON_WIFISet = 
//{
//	.Path = "1:/ICON/oicon5.jpg",
//	.PathPress = "1:/ICON/opicon5.jpg",
//	.Size_X = ICON_Size,
//	.Size_Y = ICON_Size,
////	.Frame = gImage_128,
//};

//_IconPicHandle MICON_Home = 
//{
//	.Path = "1:/ICON/micon1.jpg",
//	.PathPress = "1:/ICON/mpicon1.jpg",
//	.Size_X = MICON_XSize,
//	.Size_Y = MICON_YSize,
//	.Frame = NULL,
//};

//_IconPicHandle MICON_ShowName = 
//{
//	.Path = "1:/ICON/micon2.jpg",
//	.PathPress = "1:/ICON/mpicon2.jpg",
//	.Size_X = MICON_XSize,
//	.Size_Y = MICON_YSize,
//	.Frame = NULL,
//};

//_IconPicHandle MICON_SysMsg = 
//{
//	.Path = "1:/ICON/micon3.jpg",
//	.PathPress = "1:/ICON/mpicon3.jpg",
//	.Size_X = MICON_XSize,
//	.Size_Y = MICON_YSize,
//	.Frame = NULL,
//};

_IconPicHandle ICON_Return = 
{
	.Path = "1:/ICON/fanhui_n.jpg",
	.PathPress = "1:/ICON/fanhui_n.jpg",
	.Size_X = RETURN_ICON_WIDTH,
	.Size_Y = RETURN_ICON_HEIGHT,
//	.Frame = NULL,
};
/******************* Icon Black and White handles *******************************/
_SiglColIconHandle MicICON_WIFI1 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_wifi1,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 650,
	.Position_Y = 2,
};

_SiglColIconHandle MicICON_WIFI2 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_wifi2,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 650,
	.Position_Y = 2,
};

_SiglColIconHandle MicICON_WIFI3 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_wifi3,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 650,
	.Position_Y = 2,
};

_SiglColIconHandle MicICON_WIFI4 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_wifi4,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 650,
	.Position_Y = 2,
};

_SiglColIconHandle MicICON_Link1 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_link1,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 690,
	.Position_Y = 2,
};

_SiglColIconHandle MicICON_Battery1 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_bat1,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 740,
	.Position_Y = 2,
};

_SiglColIconHandle MicICON_Battery2 = 
{
	.Color = WHITE,
	.Cache = gImage_microicon_bat2,
	.Size_X = 32,
	.Size_Y = 15,
  .Position_X = 760,
	.Position_Y = 2,
};
/******************* Button handles *******************************/
//主页按钮
_ButtonHandle Button_Rename = 
{
	.Type = PICTURE,
	.IconPic = &ICON_Rename,
	.Position_X = 398,
	.Position_Y = 279,
	.ThemeStr = "本地改名",
	.Theme = &ButtonTheme,
};

_ButtonHandle Button_MeetMsg = 
{
	.Type = PICTURE,
	.IconPic = &ICON_MeetInfo,
	.Position_X = 44,
	.Position_Y = 95,
	.ThemeStr = "会议信息",
	.Theme = &MeetInfo_BtnTheme,
};

_ButtonHandle Button_Service = 
{
	.Type = PICTURE,
	.IconPic = &ICON_Service,
	.Position_X = 398,
	.Position_Y = 95,
	.ThemeStr = "呼叫服务",
	.Theme = &ButtonTheme,
};

_ButtonHandle Button_SMS = 
{
	.Type = PICTURE,
	.IconPic = &ICON_SMS,
	.Position_X = 584,
	.Position_Y = 95,
	.ThemeStr = "短信服务",
	.Theme = &ButtonTheme,
};

_ButtonHandle Button_ShowName = 
{
	.Type = PICTURE,
	.IconPic = &ICON_ShowName,
	.Position_X = 623,
	.Position_Y = 42,
	.ThemeStr = "显示人名",
	.Theme = &ShowName_BtnTheme,
};

_ButtonHandle Button_Setting = 
{
	.Type = PICTURE,
	.IconPic = &ICON_Setting,
	.Position_X = 584,
	.Position_Y = 279,
	.ThemeStr = "系统设置",
	.Theme = &ButtonTheme,
};

//本地改名页面按钮
_ButtonHandle Button_Rename_PerName = 
{
	.Type = COLOR,
	.Position_X = 141,
	.Position_Y = 110,
	.ThemeStr = NULL,
	.Size_X = 420,
	.Size_Y = 46,
	.MainColor = 0x18E3,
	.MainColorPress = GRAY,
	.BorderColorPress = WHITE,
	.BorderSizePress = 1,
	.Theme = &Rename_SetInfo,
};

_ButtonHandle Button_Rename_PerCom = 
{
	.Type = COLOR,
	.Position_X = 141,
	.Position_Y = 181,
	.ThemeStr = NULL,
	.Size_X = 420,
	.Size_Y = 46,
	.MainColor = 0x18E3,
	.MainColorPress = GRAY,
	.BorderColorPress = WHITE,
	.BorderSizePress = 1,
	.Theme = &Rename_SetInfo,
};

_ButtonHandle Button_Rename_PerPos = 
{
	.Type = COLOR,
	.Position_X = 141,
	.Position_Y = 250,
	.ThemeStr = NULL,
	.Size_X = 420,
	.Size_Y = 46,
	.MainColor = 0x18E3,
	.MainColorPress = GRAY,
	.BorderColorPress = WHITE,
	.BorderSizePress = 1,
	.Theme = &Rename_SetInfo,
};

_ButtonHandle Button_Rename_PerNameColor = 
{
	.Type = COLOR,
	.Position_X = 649,
	.Position_Y = 115,
	.ThemeStr = NULL,
	.Size_X = 80,
	.Size_Y = 36,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
};

_ButtonHandle Button_Rename_PerComColor = 
{
	.Type = COLOR,
	.Position_X = 649,
	.Position_Y = 186,
	.ThemeStr = NULL,
	.Size_X = 80,
	.Size_Y = 36,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
};

_ButtonHandle Button_Rename_PerPosColor = 
{
	.Type = COLOR,
	.Position_X = 649,
	.Position_Y = 255,
	.ThemeStr = NULL,
	.Size_X = 80,
	.Size_Y = 36,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
};

_ButtonHandle Button_Rename_SetBackGroundColor = 
{
	.Type = COLOR,
	.Position_X = 649,
	.Position_Y = 324,
	.ThemeStr = NULL,
	.Size_X = 80,
	.Size_Y = 36,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
};

_ButtonHandle Button_Rename_SetPicNameCar = 
{
	.Type = COLOR,
	.Position_X = 50,
	.Position_Y = 406,
	.ThemeStr = NULL,
	.Size_X = 160,
	.Size_Y = 50,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.ThemeStr = "使用图片名牌",
	.Theme = &Rename_FuncButton,
};


_ButtonHandle Button_Rename_Confirm = 
{
	.Type = COLOR,
	.Position_X = 662,
	.Position_Y = 406,
	.ThemeStr = NULL,
	.Size_X = 160,
	.Size_Y = 50,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 3,
	.ThemeStr = "确  认",
	.Theme = &Rename_FuncButton,
};

//呼叫服务按钮
_ButtonHandle ButtonS_Coffee = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Coffee,
	.Position_X = 30,
	.Position_Y = 95,
	.ThemeStr = "咖啡",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Tea = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Tea,
	.Position_X = 217,
	.Position_Y = 95,
	.ThemeStr = "茶",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Flower = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Flower,
	.Position_X = 30,
	.Position_Y = 280,
	.ThemeStr = "花",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Waiter = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Waiter,
	.Position_X = 593,
	.Position_Y = 280,
	.ThemeStr = "服务员",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Microphone = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Microphone,
	.Position_X = 593,
	.Position_Y = 95,
	.ThemeStr = "麦克风",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Pen = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Pen,
	.Position_X = 217,
	.Position_Y = 280,
	.ThemeStr = "笔",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Paper = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Paper,
	.Position_X = 406,
	.Position_Y = 280,
	.ThemeStr = "纸",
	.Theme = &Serve_BtnTheme,
};

_ButtonHandle ButtonS_Water = 
{
	.Type = PICTURE,
	.IconPic = &SICON_Water,
	.Position_X = 406,
	.Position_Y = 95,
	.ThemeStr = "水",
	.Theme = &Serve_BtnTheme,
};

//_ButtonHandle ButtonS_ExtraSer = 
//{
//	.Type = COLOR,
//	.Position_X = 84,
//	.Position_Y = 376,
//	.Size_X = 500,
//	.Size_Y = 40,
//	.MainColor = WHITE,
//	.BorderColor = LIGHTBLUE,
//	.BorderSize = 2,
//	.MainColorPress = LGRAY,
//	.BorderColorPress = LIGHTBLUE,
//	.BorderSizePress = 2,
//};

//_ButtonHandle ButtonS_Confirm = 
//{
//	.Type = COLOR,
//	.Position_X = 585,
//	.Position_Y = 376,
//	.Size_X = 128,
//	.Size_Y = 40,
//	.MainColor = LIGHTBLUE,
//	.BorderColor = WHITE,
//	.BorderSize = 1,
//	.MainColorPress = BLUE,
//	.BorderColorPress = WHITE,
//	.BorderSizePress = 2,
//	.ThemeStr = "确认呼叫",
//	.Theme = &Button_ConfirmTheme,
//};


//短信收发
_ButtonHandle Button_SMS_Addressee = 
{
	.Type = COLOR,
	.Position_X = 150,
	.Position_Y = 95,
	.Size_X = 500,
	.Size_Y = 44,
	.MainColor = 0x18C3,
	.MainColorPress = 0x18C3,
	.BorderColor = 0x6268,
	.BorderSize = 1,
	.ThemeStr = "收件人:",
	.Theme = &Button_SMSTheme,
};

_ButtonHandle Button_SMS_Content = 
{
	.Type = COLOR,
	.Position_X = 150,
	.Position_Y = 154,
	.Size_X = 500,
	.Size_Y = 296,
	.MainColor = 0x18C3,
	.MainColorPress = 0x18C3,
	.BorderColor = 0x6268,
	.BorderSize = 1,
	.ThemeStr = "内  容:",
	.Theme = &Button_SMSTheme,
};

_ButtonHandle Button_SMS_Send = 
{
	.Type = PICTURE,
	.Position_X = 662,
	.Position_Y = 153,
	.IconPic = &SMS_ICON_Send,
	.ThemeStr = "发 送",
	.Theme = &Button_SMSSendTheme,
};

_ButtonHandle Button_SMS_Choice= 
{
	.Type = PICTURE,
	.Position_X = 662,
	.Position_Y = 95,
	.IconPic = &SMS_ICON_Choice,
	.ThemeStr = "选 择",
	.Theme = &Button_SMSChoiceTheme,
};

//设置页面按钮
//_ButtonHandle ButtonO_AddressSet = 
//{
//	.Type = PICTURE,
//	.IconPic = &OICON_AddressSet,
//	.Position_X = OICON1_Xs,
//	.Position_Y = OICON1_Ys,
//	.ThemeStr = "地址设置",
//	.Theme = &ButtonTheme,
//};

//_ButtonHandle ButtonO_BackGroundSet = 
//{
//	.Type = PICTURE,
//	.IconPic = &OICON_BackGroundSet,
//	.Position_X = OICON2_Xs,
//	.Position_Y = OICON2_Ys,
//	.ThemeStr = "更换壁纸",
//	.Theme = &ButtonTheme,
//};

//_ButtonHandle ButtonO_LightSet = 
//{
//	.Type = PICTURE,
//	.IconPic = &OICON_LightSet,
//	.Position_X = OICON3_Xs,
//	.Position_Y = OICON3_Ys,
//	.ThemeStr = "亮度调整",
//	.Theme = &ButtonTheme,
//};

//_ButtonHandle ButtonO_SystemSta = 
//{
//	.Type = PICTURE,
//	.IconPic = &OICON_SystemSta,
//	.Position_X = OICON4_Xs,
//	.Position_Y = OICON4_Ys,
//	.ThemeStr = "系统状态",
//	.Theme = &ButtonTheme,
//};

//_ButtonHandle ButtonO_WIFISet = 
//{
//	.Type = PICTURE,
//	.IconPic = &OICON_WIFISet,
//	.Position_X = OICON5_Xs,
//	.Position_Y = OICON5_Ys,
//	.ThemeStr = "WIFI设置",
//	.Theme = &ButtonTheme,
//};

_ButtonHandle Button_Option_SetDeviceID = 
{
	.Type = COLOR,
	.Position_X = 550,
	.Position_Y = 240,
	.Size_X = 90,
	.Size_Y = 30,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.Theme = &Button_ModifyTheme,
};

_ButtonHandle Button_Option_SetServerIP = 
{
	.Type = COLOR,
	.Position_X = 550,
	.Position_Y = 280,
	.Size_X = 90,
	.Size_Y = 30,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.Theme = &Button_ModifyTheme,
};

_ButtonHandle Button_Option_SetServerPort = 
{
	.Type = COLOR,
	.Position_X = 550,
	.Position_Y = 320,
	.Size_X = 90,
	.Size_Y = 30,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.Theme = &Button_ModifyTheme,
};

_ButtonHandle Button_Option_SetLocalIP = 
{
	.Type = COLOR,
	.Position_X = 550,
	.Position_Y = 360,
	.Size_X = 90,
	.Size_Y = 30,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.Theme = &Button_ModifyTheme,
};

_ButtonHandle Button_Option_SSID = 
{
	.Type = COLOR,
	.Position_X = 550,
	.Position_Y = 280,
	.Size_X = 90,
	.Size_Y = 30,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.Theme = &Button_ModifyTheme,
};

_ButtonHandle Button_Option_WIFIPwd = 
{
	.Type = COLOR,
	.Position_X = 550,
	.Position_Y = 330,
	.Size_X = 90,
	.Size_Y = 30,
	.MainColor = LIGHTBLUE,
	.BorderColor = WHITE,
	.BorderSize = 1,
	.MainColorPress = BLUE,
	.BorderColorPress = WHITE,
	.BorderSizePress = 2,
	.Theme = &Button_ModifyTheme,
};

_ButtonHandle Button_Option_DHCP = 
{
	.Type = COLOR,
	.Position_X = 350,
	.Position_Y = 400,
	.Size_X = 30,
	.Size_Y = 30,
	.MainColor = WHITE,
	.BorderColor = BLACK,
	.BorderSize = 2,
	.MainColorPress = GREEN,
	.BorderColorPress = WHITE,
	.BorderSizePress = 3,
};


//底部按钮
//_ButtonHandle ButtonM_Home = 
//{
//	.Type = PICTURE,
//	.IconPic = &MICON_Home,
//	.Position_X = MICON1_Xs,
//	.Position_Y = MICON1_Ys,
//	.ThemeStr = NULL,
//};

//_ButtonHandle ButtonM_ShowName = 
//{
//	.Type = PICTURE,
//	.IconPic = &MICON_ShowName,
//	.Position_X = MICON2_Xs,
//	.Position_Y = MICON2_Ys,
//	.ThemeStr = NULL,
//};

//_ButtonHandle ButtonM_SysMsg = 
//{
//	.Type = PICTURE,
//	.IconPic = &MICON_SysMsg,
//	.Position_X = MICON3_Xs,
//	.Position_Y = MICON3_Ys,
//	.ThemeStr = NULL,
//};

_ButtonHandle ButtonM_Back = 
{
	.Type = PICTURE,
	.IconPic = &ICON_Return,
	.Position_X = 1,
	.Position_Y = 24,
	.ThemeStr = NULL,
};



/******************** BackGround Picture handles **************************/
_BackGroundHandle Main_BackGround = 
{
	.Type = PICTURE,
	.Path = NULL,
	.Size_X = 800,
	.Size_Y = 480,
};
/******************** Text Box handles ***********************************/
_TextBoxHandle MainPageHeading =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = "欢迎您 ",
	.FontColor = WHITE,
	.FontSize = 32,
	.Position_X = 10,
	.Position_Y = 23,
	.Size_X = 600,
	.Size_Y = 32,
};

_TextBoxHandle SecPageHeading =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 32,
	.Position_X = 0,
	.Position_Y = 23,
	.Size_X = 800,
	.Size_Y = 32,
};

_TextBoxHandle ButtonTheme =  
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 129,
	.Size_X = 170,
	.Size_Y = 24,
};

_TextBoxHandle Serve_BtnTheme =  
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 94,
	.Size_X = 176,
	.Size_Y = 24,
};

_TextBoxHandle MeetInfo_BtnTheme =  
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 22,
	.Position_Y = 21,
	.Size_X = 100,
	.Size_Y = 24,
};

_TextBoxHandle ShowName_BtnTheme =  
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 44,
	.Position_Y = 9,
	.Size_X = 100,
	.Size_Y = 24,
};

_TextBoxHandle Button_SMSSendTheme =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 155,
	.Size_X = 95,
	.Size_Y = 40,
};

_TextBoxHandle Button_SMSChoiceTheme =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 10,
	.Size_X = 95,
	.Size_Y = 24,
};

_TextBoxHandle Button_SMSTheme =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = 0x8C51,
	.FontSize = 24,
	.Position_X = -98,
	.Position_Y = 10,
	.Size_X = 90,
	.Size_Y = 24,

};

_TextBoxHandle Button_SliderTheme =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = -100,
	.Position_Y = -8,
	.Size_X = 90,
	.Size_Y = 24,
};

_TextBoxHandle Button_ModifyTheme =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 0,
	.String = "修 改",
	.Size_X = 90,
	.Size_Y = 30,
};


//_TextBoxHandle CalendarDate =
//{
//	.PosType = RELATIVE,
//	.Border = False,
//	.Filling = False,
//	.Center = True,
//	.String = NULL,
//	.FontColor = BLACK,
//	.FontSize = 32,
//	.Position_X = 0,
//	.Position_Y = 0,
//	.Size_X = ICON_CAL_X_Size,
//	.Size_Y = 32,
//};

//本地改名
_TextBoxHandle Rename_InfoHeading =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontSize = 24,
	.FontColor = 0xCE79,
	.Position_X = 0,
	.Position_Y = 0,
	.Size_X = 50,
	.Size_Y = 24,
};

_TextBoxHandle Rename_SetInfo=
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 5,
	.Position_Y = 8,
	.Size_X = 420,
	.Size_Y = 30,
};

_TextBoxHandle Rename_SetBGColor =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 0,
	.Size_X = 100,
	.Size_Y = 30,
};

_TextBoxHandle Rename_FuncButton =
{
	.PosType = RELATIVE,
	.Border = False,
	.Filling = False,
	.Center = True,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 0,
	.Position_Y = 0,
	.Size_X = 160,
	.Size_Y = 50,
};
//会议信息
_TextBoxHandle MeetMsg_MeetingTopic =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "会议主题:",
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 104,
	.Position_Y = 100,
	.Size_X = 550,
	.Size_Y = 24,
};

_TextBoxHandle MeetMsg_MeetingTitle =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "会议标题:",
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 104,
	.Position_Y = 150,
	.Size_X = 550,
	.Size_Y = 24,
};

_TextBoxHandle MeetMsg_MeetingContent =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "会议内容:",
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 104,
	.Position_Y = 200,
	.Size_X = 550,
	.Size_Y = 24,
};

_TextBoxHandle MeetMsg_MeetingSTime =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "开始时间:",
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 104,
	.Position_Y = 250,
	.Size_X = 550,
	.Size_Y = 24,
};

_TextBoxHandle MeetMsg_MeetingETime =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "结束时间:",
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 104,
	.Position_Y = 300,
	.Size_X = 550,
	.Size_Y = 24,
};

_TextBoxHandle MeetMsg_RecMsgTitle =
{
	.PosType = ABSOLUTE,
	.Border = True,
	.BorderColor = BLUE,
	.BorderSize = 1,
	.Filling = False,
	.Center = False,
	.String = "来自:",
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 120,
	.Position_Y = 100,
	.Size_X = 555,
	.Size_Y = 26,
};

_TextBoxHandle MeetMsg_RecMsgContent =
{
	.PosType = ABSOLUTE,
	.Border = True,
	.BorderColor = BLUE,
	.BorderSize = 1,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 120,
	.Position_Y = 150,
	.Size_X = 555,
	.Size_Y = 200,
};

//短信服务文本框
_TextBoxHandle SMS_AddresseeName =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 250,
	.Position_Y = 93,
	.Size_X = 460,
	.Size_Y = 30,
};

_TextBoxHandle SMS_Content =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 250,
	.Position_Y = 160,
	.Size_X = 440,
	.Size_Y = 185,
};

//服务请求文本框
_TextBoxHandle CallService_ExtraSer =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = BLACK,
	.FontSize = 24,
	.Position_X = 90,
	.Position_Y = 385,
	.Size_X = 480,
	.Size_Y = 24,
};
//设置页面文本框
_TextBoxHandle Option_DeviceID =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "设备ID: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 220,
	.Position_Y = 240,
	.Size_X = 300,
	.Size_Y = 24,
};

_TextBoxHandle Option_ServerIP =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "服务器IP: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 200,
	.Position_Y = 280,
	.Size_X = 300,
	.Size_Y = 24,
};

_TextBoxHandle Option_ServerPort =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "服务器端口: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 176,
	.Position_Y = 320,
	.Size_X = 300,
	.Size_Y = 24,
};

_TextBoxHandle Option_LocalIP =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "本机IP: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 224,
	.Position_Y = 360,
	.Size_X = 300,
	.Size_Y = 24,
};

_TextBoxHandle Option_DHCP =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "使用DHCP: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 200,
	.Position_Y = 400,
	.Size_X = 120,
	.Size_Y = 24,
};

_TextBoxHandle Option_DeviceMac =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "设备MAC: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 130,
	.Position_Y = 250,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle Option_CodeVersion =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "软件版本: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 130,
	.Position_Y = 350,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle Option_MCUUsage =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "CPU使用率: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 450,
	.Position_Y = 250,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle Option_FLASHUsage =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "FLASH使用率: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 450,
	.Position_Y = 300,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle Option_WIFISignal=
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "WIFI强度: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 450,
	.Position_Y = 350,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle Option_SSID =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "WIFI账号: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 220,
	.Position_Y = 280,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle Option_WIFIPwd =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = "WIFI密码: ",
	.FontColor = WHITE,
	.FontSize = 24,
	.Position_X = 220,
	.Position_Y = 330,
	.Size_X = 280,
	.Size_Y = 24,
};

_TextBoxHandle StatusBar_TimeDate =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 16,
	.Position_X = 15,
	.Position_Y = 2,
	.Size_X = 300,
	.Size_Y = 16,
};

_TextBoxHandle Updating_Prompt  =
{
	.PosType = ABSOLUTE,
	.Border = False,
	.Filling = False,
	.Center = False,
	.String = NULL,
	.FontColor = WHITE,
	.FontSize = 32,
	.Position_X = 100,
	.Position_Y = 230,
	.Size_X = 600,
	.Size_Y = 64,
};
/******************** Function Block handles **************************/
//_FuncBlockHandle Calendar = 
//{
//	.Type = PICTURE,
//	.Path = "1:/ICON/calendar.jpg",
//	.Position_X = ICON_CAL_Xs,
//	.Position_Y = ICON_CAL_Ys,
//	.Size_X = ICON_CAL_X_Size,
//	.Size_Y = ICON_CAL_Y_Size,
//};

_FuncBlockHandle StatusBar = 
{
	.Type = PICTURE,
	.Path = NULL,
	.Position_X = 0,
	.Position_Y = 0,
	.Size_X = 800,
	.Size_Y = 20,
};

//_FuncBlockHandle Msg_Blackboard = 
//{
//	.Type = COLOR,
//	.Position_X = 90,
//	.Position_Y = 60,
//	.Size_X = 632,
//	.Size_Y = 336,
//	.Filling = True,
//	.FillingColor = WHITE,
//	.Border = True,
//	.BorderColor = BLUE,
//	.BorderSize = 3,
//};

_FuncBlockHandle Rename_Blackboard = 
{
	.Type = COLOR,
	.Position_X = 50,
	.Position_Y = 95,
	.Size_X = 708,
	.Size_Y = 290,
	.Filling = True,
	.FillingColor = 0x5A48,
};

/************************ Slider handles ******************************/
_SliderHandle INScreenBrightness = 
{
	.BackColor = WHITE,
	.SliderColor = LIGHTBLUE,
	.BorderSize = 2,
	.Position_X = 250,
	.Position_Y = 260,
	.Size_X = 400,
	.Size_Y = 10,
	.TchEfecPosInc_X = 50,
	.TchEfecPosInc_Y = 50,
  .ParaMax = 100,
	.ParaMin = 20,
	.ThemeStr = "内屏亮度",
	.Theme = &Button_SliderTheme,
};

//_SliderHandle OUTScreenBrightness = 
//{
//	.BackColor = WHITE,
//	.SliderColor = LIGHTBLUE,
//	.BorderSize = 2,
//	.Position_X = 250,
//	.Position_Y = 360,
//	.Size_X = 400,
//	.Size_Y = 10,
//	.ParaMax = 100,
//	.ParaMin = 20,
//	.ThemeStr = "外屏亮度",
//	.Theme = &Button_SliderTheme,
//};
/************************* Button Library *****************************/
_ButtonHandle *MainButtonLib[6] = 
{
	&Button_Rename,
	&Button_MeetMsg,
	&Button_Service,
	&Button_SMS,
	&Button_ShowName,
	&Button_Setting,
};

_ButtonHandle *ServiceButtonLib[8] = 
{
	&ButtonS_Coffee,
	&ButtonS_Tea,
	&ButtonS_Flower,
	&ButtonS_Waiter,
	&ButtonS_Microphone,
	&ButtonS_Pen,
	&ButtonS_Paper,
	&ButtonS_Water,
//	&ButtonS_ExtraSer,
//	&ButtonS_Confirm,
};

//_ButtonHandle *OptionButtonLib[3] = 
//{
//	&ButtonO_AddressSet,
//	&ButtonO_BackGroundSet,
//	&ButtonO_LightSet,
//	&ButtonO_SystemSta,
//	&ButtonO_WIFISet,
//};

_ButtonHandle *OptionAddressSet_ButtonLib[4] = 
{
	&Button_Option_SetDeviceID,
	&Button_Option_SetServerIP,
	&Button_Option_SetServerPort,
	&Button_Option_SetLocalIP,
};

_ButtonHandle *OptionWIFISet_ButtonLib[2] = 
{
	&Button_Option_SSID,
	&Button_Option_WIFIPwd,
};

//_ButtonHandle *Button_M_Lib[4] = 
//{
//	&ButtonM_Home,
//	&ButtonM_ShowName,
//	&ButtonM_SysMsg,
//	&ButtonM_Back,
//};

_ButtonHandle *Button_RenameLib[9] =
{
	&Button_Rename_PerName,
	&Button_Rename_PerCom,
	&Button_Rename_PerPos,
	&Button_Rename_PerNameColor,
	&Button_Rename_PerComColor,
	&Button_Rename_PerPosColor,
	&Button_Rename_SetBackGroundColor,
	&Button_Rename_SetPicNameCar,
	&Button_Rename_Confirm,
};

_ButtonHandle *Button_SMSLib[3] =
{
	&Button_SMS_Choice,
	&Button_SMS_Content,
	&Button_SMS_Send,
};

/******************** UI Page handles **************************/
//_UIPageHandle HomePage = 
//{
//	.PageButtonLib[0] = &Button_Rename,
//	.PageButtonLib[1] = &Button_MeetMsg,
//	.PageButtonLib[2] = &Button_Service,
//	.PageButtonLib[3] = &Button_SMS,
//	.PageButtonLib[4] = &Button_ShowName,
//	.PageButtonLib[5] = &Button_Setting,
//	.PageBackGround = &Main_BackGround,

//	
//};
/************************************************************************/

/******************* declaration **********************************/
static u8 UI_LoadingIconPic(_IconPicHandle *IconPic);
static u8 UI_LoadingBGPic(_BackGroundHandle *BackGround);
static u8 UI_LoadingFBPic(_FuncBlockHandle *FuncBlock);
static u8 UI_SetButton(_ButtonHandle *Button,_ButtonSta Sta);
static u8 UI_SetBackGround(_BackGroundHandle *BackGround,u16 sx,u16 sy,u16 ex,u16 ey);
static u8 UI_SetTextBox(_TextBoxHandle *TextBox,char *Str,u16 RelPos_X,u16 RelPos_Y);
static u8 UI_SetFuncBlock(_FuncBlockHandle *FuncBlock,u16 *SetColor);
static void UI_Config(void);
static u8 UI_DarkerRAM(u16 *RamCache,u32 len,u8 coefficient);
static u8 UI_ButtonTouch(_ButtonHandle *Button,u16 tp_x,u16 tp_y,u8 opt);
static u8 UI_ButtonLibTouch(_ButtonHandle *(*ButtonLib),u16 tp_x,u16 tp_y,u8 opt,u8 num);
static u8 UI_SetSlider(_SliderHandle *Slider,u8 init_value);
static u8 UI_SliderTouch(_SliderHandle *Slider,u16 tp_x,u16 tp_y,u8 opt,u8 *CorrelaPara);
static u8 UI_BacklightGradient(u8 Init_Lum,u8 Final_Lum,u8 dly);
static u8 UI_SingleColorIconSet(_SiglColIconHandle * Icon);
/****************************************************************/

_ui_class UI = 
{
	.Config = UI_Config,
	.LoadingIconPic = UI_LoadingIconPic,
	.SetButton = UI_SetButton,
	.LoadingBGPic = UI_LoadingBGPic,
	.SetBackGround = UI_SetBackGround,
	.SetTextBox = UI_SetTextBox,
	.LoadingFBPic = UI_LoadingFBPic,
	.SetFuncBlock = UI_SetFuncBlock,
	.ButtonTouch = UI_ButtonTouch,
	.ButtonLibTouch = UI_ButtonLibTouch,
	.SetSlider = UI_SetSlider,
	.SliderTouch = UI_SliderTouch,
	.BacklightGradient = UI_BacklightGradient,
	.SingleColorIconSet = UI_SingleColorIconSet,
};


/***********************************************************/
//加载按钮图标图标
static u8 UI_LoadingIconPic(_IconPicHandle *IconPic)
{
	u8 Path[50];	
	u16 width,height;
	u16 i,j;
	
	width = IconPic->Size_X;
	height = IconPic->Size_Y;
	
//	IconPic->Cache = mymalloc(SRAMEX,width*height*2);
//	IconPic->CachePress = mymalloc(SRAMEX,width*height*2);
	SYS_MALLOC(IconPic->Cache,SRAMEX,width*height*2);
	SYS_MALLOC(IconPic->CachePress,SRAMEX,width*height*2);
	
	if(IconPic->Cache == NULL || IconPic->CachePress == NULL) return 1;
	
	if(IconPic->Path == NULL || IconPic->PathPress == NULL) return 2;
	
//	strcpy((char*)Path,"1:/");
	strcpy((char*)Path,(const char*)IconPic->Path);
	if(ai_load_picfile(Path,0,0,width,height,1)) return 3;
	
	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			IconPic->Cache[width*j+i] = LCD.ReadPoint(i,j);
		}
	}
	
//	strcpy((char*)Path,"1:/");
	strcpy((char*)Path,(const char*)IconPic->PathPress);
	if(ai_load_picfile(Path,0,0,width,height,1)) return 3;

	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			IconPic->CachePress[width*j+i] = LCD.ReadPoint(i,j);
		}
	}
	
	return 0;
}


/***********************************************************/

//加载背景图片
static u8 UI_LoadingBGPic(_BackGroundHandle *BackGround)
{
	u8 Path[50];	
	u16 width,height;
	u16 i,j;
	
	if(BackGround->Type != PICTURE) return 4;
	
	width = BackGround->Size_X;
	height = BackGround->Size_Y;
	
//	BackGround->Cache = mymalloc(SRAMEX,width*height*2);
	SYS_MALLOC(BackGround->Cache,SRAMEX,width*height*2);
	
	if(BackGround->Cache == NULL) return 1;
	if(BackGround->Path == NULL) return 2;
	
//	strcpy((char*)Path,"1:/");
	strcpy((char*)Path,(const char*)BackGround->Path);
	if(ai_load_picfile(Path,0,0,width,height,1)) return 3;
	
	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			BackGround->Cache[width*j+i] = LCD.ReadPoint(i,j);
		}
	}
	
	return 0;
}

//加载功能块背景图
static u8 UI_LoadingFBPic(_FuncBlockHandle *FuncBlock)
{
		u8 Path[50];	
	u16 width,height;
	u16 i,j;
	
	if(FuncBlock->Type != PICTURE) return 4;
	
	width = FuncBlock->Size_X;
	height = FuncBlock->Size_Y;
	
//	FuncBlock->Cache = mymalloc(SRAMEX,width*height*2);
	SYS_MALLOC(FuncBlock->Cache,SRAMEX,width*height*2);
	
	if(FuncBlock->Cache == NULL) return 1;
	if(FuncBlock->Path == NULL) return 2;
	
//	strcpy((char*)Path,"1:/");
	strcpy((char*)Path,(const char*)FuncBlock->Path);
	if(ai_load_picfile(Path,0,0,width,height,1)) return 3;
	
	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			FuncBlock->Cache[width*j+i] = LCD.ReadPoint(i,j);
		}
	}
	
	return 0;
}
	

//设置按钮
static u8 UI_SetButton(_ButtonHandle *Button,_ButtonSta Sta)
{
	CPU_SR_ALLOC();
	
	u16 *Cache_temp;
	u16 i,j,k;
	u16 width,height;
	u16 x = Button->Position_X ,y = Button->Position_Y;
	
	u16 MainColorTemp,BorderColorTemp;
	u8 BorderSizeTemp;
	
	if(Button->Type == PICTURE)
	{
		width = Button->IconPic->Size_X;
		height = Button->IconPic->Size_Y;
		
		if(Sta == CLICK) Cache_temp = Button->IconPic->CachePress;
		else Cache_temp = Button->IconPic->Cache;
		
		if(Button->IconPic->Cache == NULL) return 1;
		
		OS_CRITICAL_ENTER();	//进入临界区
		if(Button->IconPic->Frame != NULL) 
		{
			for(j=0;j<height;j++)
			{
				for(i=0;i<width/8;i++)
				{
					for(k=0;k<8;k++)
					{
						if(Button->IconPic->Frame[j*16+i] & (0x80 >> k))
						LCD.Fast_DrawPoint(i*8+k+x,j+y,Cache_temp[width*j+(i*8+k)]);
					}
					
				}
			}
		}
		else LCD.Color_Fill(x,y,x+width-1,y+height-1,Cache_temp);
		OS_CRITICAL_EXIT();	//退出临界区
		Button->Sta = Sta;
	}
	else if(Button->Type == COLOR)
	{
		width = Button->Size_X;
		height = Button->Size_Y;
		
		
	  if(Sta == CLICK) 
		{
			MainColorTemp = Button->MainColorPress;
			BorderColorTemp = Button->BorderColorPress;
		  BorderSizeTemp = Button->BorderSizePress;
		}
		else 
		{
			MainColorTemp = Button->MainColor;
			BorderColorTemp = Button->BorderColor;
		  BorderSizeTemp = Button->BorderSize;
		}
		
		OS_CRITICAL_ENTER();	//进入临界区
		LCD.Fill(x,y,x+width-1,y+height-1,MainColorTemp);
		POINT_COLOR = BorderColorTemp;
		for(i=0;i<BorderSizeTemp;i++)
		{
			LCD.DrawRectangle(x+i,y+i,x+width-i-1,y+height-i-1);
		}
		OS_CRITICAL_EXIT();	//退出临界区
		Button->Sta = Sta;
		
	}
	
	if(Button->Theme != NULL && (Button->ThemeStr != NULL || Button->Theme->String != NULL))
	{
		UI.SetTextBox(Button->Theme,(char *)Button->ThemeStr,Button->Position_X,Button->Position_Y);
	}
	
	return 0;
}


//检测按钮触摸
//按键被触摸后 opt: 	0 = 只返回案件被触摸真值
//                 	1 =	按键显示未点击图标
//                 	2 =	按键显示已点击图标
//                 	3 =	按键显示当前状态的反转
static u8 UI_ButtonTouch(_ButtonHandle *Button,u16 tp_x,u16 tp_y,u8 opt)
{
	OS_ERR err;
	u16 sx = Button->Position_X ,sy = Button->Position_Y;
	u16 ex , ey ;
	
	if(Button->Type == PICTURE) 
	{
		ex = sx + Button->IconPic->Size_X;
		ey = sy + Button->IconPic->Size_Y;
	}		
	else
	{
		ex = sx + Button->Size_X;
		ey = sy + Button->Size_Y;
	}
	
	if(tp_x >= sx && tp_x <= ex && tp_y >= sy && tp_y <= ey)
	{
		switch(opt)
		{
			case 0:return 1;
			case 1:
			{
				UI.SetButton(Button,UNCLICK);
				while(SYS->TP.Status == Press) OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				return 1;
			}
			case 2:
			{
				UI.SetButton(Button,CLICK);
				while(SYS->TP.Status == Press) OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				return 1;
			}
			case 3:
			{
				UI.SetButton(Button,(_ButtonSta)!Button->Sta);
				while(SYS->TP.Status == Press) OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				return 1;
			}
			case 4:
			{
				UI.SetButton(Button,CLICK);
				while(SYS->TP.Status == Press) OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				UI.SetButton(Button,UNCLICK);
				return 1;
			}
			
			default:return 0;
		}
	}
	else return 0;
}

//检测按钮触摸(按键库)
static u8 UI_ButtonLibTouch(_ButtonHandle *(*ButtonLib),u16 tp_x,u16 tp_y,u8 opt,u8 num)
{
	u8 i,res;
	
	for(i=0;i<num;i++)
	{
		res = UI.ButtonTouch(ButtonLib[i],tp_x,tp_y,opt);
		if(res) return i+1;
	}
	return 0;
}


//设置背景图
static u8 UI_SetBackGround(_BackGroundHandle *BackGround,u16 sx,u16 sy,u16 ex,u16 ey)
{
	CPU_SR_ALLOC();
	
	u16 i;
	u16 width = BackGround->Size_X ,height = BackGround->Size_Y;

	if(BackGround->Cache == NULL) return 2;
	

	if( sx==0 && sy==0 && ex==width && ey==height) 
	{
		OS_CRITICAL_ENTER();	//进入临界区
		LCD.Color_Fill(sx,sy,ex-1,ey-1,BackGround->Cache);
		OS_CRITICAL_EXIT();	//退出临界区
	}
	else if( sx<ex && sy<ey && ex<=width && ey<=height)
	{
		OS_CRITICAL_ENTER();	//进入临界区
		for(i=sy;i<ey;i++)
		{
			LCD.Color_Fill(sx,i,ex,i,&BackGround->Cache[width*i+sx]);		
		}
		OS_CRITICAL_EXIT();	//退出临界区
	}
	else return 1;
	
	return 0;
}

//设置文本框
static u8 UI_SetTextBox(_TextBoxHandle *TextBox,char *Str,u16 RelPos_X,u16 RelPos_Y)
{
	CPU_SR_ALLOC();
	
	char *StrTemp = NULL;
	u8 CstrLen,StrLen;
	u8 i;
	u16 x , y;
	u16 width = TextBox->Size_X,height = TextBox->Size_Y;
	u8 csize = TextBox->FontSize;
	
	if(TextBox->PosType == ABSOLUTE)
	{
		x = TextBox->Position_X;
		y = TextBox->Position_Y;
	}
	else
	{
		x = RelPos_X + TextBox->Position_X;
		y = RelPos_Y + TextBox->Position_Y;
	}
	
	if(TextBox->String != NULL) CstrLen = strlen(TextBox->String);
	else CstrLen = 0;
	if(Str != NULL) StrLen = strlen(Str);
	else StrLen = 0;
	
//	StrTemp = mymalloc(SRAMIN,CstrLen+StrLen+1);
	SYS_MALLOC(StrTemp,SRAMIN,CstrLen+StrLen+1);
	
	if(StrTemp == NULL) return 1;
	memset(StrTemp,'\0',CstrLen+StrLen+1);
	
	if(CstrLen) 
	{
		strcpy(StrTemp,TextBox->String);
		if(StrLen) strcat(StrTemp,Str);
	}
	else
	{
		if(StrLen) strcpy(StrTemp,Str);
		else return 2;
	}
	
	if(TextBox->Filling)
	{
		OS_CRITICAL_ENTER();	//进入临界区
		LCD.Fill(x,y,x+width-1,y+height-1,TextBox->FillingColor);
		OS_CRITICAL_EXIT();	//退出临界区
	}
	if(TextBox->Border)
	{
		if(TextBox->BorderSize*2+csize > height) return 3;
		else
		{
			POINT_COLOR = TextBox->BorderColor;
			for(i=0;i<TextBox->BorderSize;i++)
			{
				LCD.DrawRectangle(x+i,y+i,x+width-i-1,y+height-i-1);
			}
		}
		POINT_COLOR = TextBox->FontColor;
		if(TextBox->Center) TEXT.show_str_mid(x+TextBox->BorderSize,y+TextBox->BorderSize+(height-csize)/2,(u8*)StrTemp,csize,width-2*TextBox->BorderSize);
		else TEXT.show_str(x+TextBox->BorderSize,y+TextBox->BorderSize,width-2*TextBox->BorderSize,height-2*TextBox->BorderSize,(u8*)StrTemp,csize,1);
	}
	else
	{
		POINT_COLOR = TextBox->FontColor;
		if(TextBox->Center) TEXT.show_str_mid(x,y+(height-csize)/2,(u8*)StrTemp,csize,width);
		else TEXT.show_str(x,y,width,height,(u8*)StrTemp,csize,1);
	}

//	myfree(SRAMIN,StrTemp);
	SYS_FREE(StrTemp,SRAMIN);
	
	return 0;
}

//设置功能块
static u8 UI_SetFuncBlock(_FuncBlockHandle *FuncBlock,u16 *SetColor)
{
	CPU_SR_ALLOC();
//	u16 *Cache_temp;
	u16 x = FuncBlock->Position_X,y = FuncBlock->Position_Y;
	u16 width = FuncBlock->Size_X ,height = FuncBlock->Size_Y;
  u8 i;
	
	if(FuncBlock->Type == PICTURE) 
	{
		if(FuncBlock->Cache == NULL) return 1;
		OS_CRITICAL_ENTER();	//进入临界区
		LCD.Color_Fill(x,y,x+width-1,y+height-1,FuncBlock->Cache);
		OS_CRITICAL_EXIT();	//退出临界区
		
	}
	else if(FuncBlock->Type == COLOR) 
	{
		if(FuncBlock->Filling == True)
		{
			OS_CRITICAL_ENTER();	//进入临界区
			if(SetColor != NULL) LCD.Fill(x,y,x+width-1,y+height-1,(*SetColor));
			else LCD.Fill(x,y,x+width-1,y+height-1,FuncBlock->FillingColor);
			OS_CRITICAL_EXIT();	//退出临界区
		}
		if(FuncBlock->Border == True)
		{
			POINT_COLOR = FuncBlock->BorderColor;
			for(i=0;i<FuncBlock->BorderSize;i++)
			{
				LCD.DrawRectangle(x+i,y+i,x+width-i-1,y+height-i-1);
			}
		}
	}	
	return 0;
}

//设置滑动条
static u8 UI_SetSlider(_SliderHandle *Slider,u8 init_value)
{
	CPU_SR_ALLOC();
	
	u16 x = Slider->Position_X,y = Slider->Position_Y;
	u16 width = Slider->Size_X ,height = Slider->Size_Y;
	u8 bordersize = Slider->BorderSize;
	
	if(init_value > 100) init_value = 100;
	if(bordersize == 0) bordersize = 1;
	
	OS_CRITICAL_ENTER();	//进入临界区
	LCD.Fill(x,y,x+width-1,y+height-1,Slider->BackColor);
//	POINT_COLOR = ;
	LCD.Fill(x+bordersize,y+bordersize,x+bordersize+((width-2*bordersize)*init_value/100),y+height-1-bordersize,Slider->SliderColor);
	OS_CRITICAL_EXIT();	//退出临界区
	
	if(Slider->Theme != NULL && (Slider->ThemeStr != NULL || Slider->Theme->String != NULL))
	{
		UI.SetTextBox(Slider->Theme,(char *)Slider->ThemeStr,Slider->Position_X,Slider->Position_Y);
	}
  return 0;
}

//检测滑动条触摸
static u8 UI_SliderTouch(_SliderHandle *Slider,u16 tp_x,u16 tp_y,u8 opt,u8 *CorrelaPara)
{
	OS_ERR err;
	u16 sx = Slider->Position_X ,sy = Slider->Position_Y;
	u16 ex = Slider->Position_X+Slider->Size_X , ey = Slider->Position_Y+Slider->Size_Y;
	u8 min = Slider->ParaMin,max = Slider->ParaMax;
	
	u8 ret,percent;
	
	if(tp_x >= sx - Slider->TchEfecPosInc_X && tp_x <= ex + Slider->TchEfecPosInc_X && tp_y >= sy  - Slider->TchEfecPosInc_Y && tp_y <= ey + Slider->TchEfecPosInc_Y)
	{
			while(SYS->TP.Status == Press)
			{
				if(SYS->TP.Coordinate_X < sx)
				{
					UI.SetSlider(Slider,0);
					ret = min;
					if(opt) *CorrelaPara = min;
				}
				else if(SYS->TP.Coordinate_X >= sx && SYS->TP.Coordinate_X <= ex)
				{						
					percent = 100*(SYS->TP.Coordinate_X - sx)/(ex-sx);
					ret = (max-min)*percent/100+min;
					if(opt) *CorrelaPara = ret;
					UI.SetSlider(Slider,percent);
				}
				else if(SYS->TP.Coordinate_X > ex) 
				{
					UI.SetSlider(Slider,100);
					ret = max;
					if(opt) *CorrelaPara = max;
				}
					
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			return ret;
	
	}
	else return 0;
	
	
}

static u8 UI_DarkerRAM(u16 *RamCache,u32 len,u8 coefficient)
{
	u16 RR,GG,BB;
	u32 temp;
	
	if(coefficient<1 || coefficient >99) return 1;
	if(RamCache == NULL) return 2;
	
	for(temp=0;temp<len;temp++)
	{
			RR = RamCache[temp] >> 11;
			GG = (RamCache[temp] & 0x7FF) >> 5;
			BB = RamCache[temp] & 0x1F;
		
			RR = (RR * coefficient / 100);
			GG = (GG * coefficient / 100);
			BB = (BB * coefficient / 100);
		
			RamCache[temp] = 	((RR << 11) | (GG << 5) | (BB));
	}
	
	return 0;
}

//亮度渐变
//dly 1~10
static u8 UI_BacklightGradient(u8 Init_Lum,u8 Final_Lum,u8 dly)
{
	OS_ERR err;
	u8 res;
	
	if(Init_Lum>100  || Final_Lum>100 || dly<1 || dly>10 ) return 1;
	

	LCD.BackLightSet(Init_Lum);
	
	if(Init_Lum > Final_Lum)
	{
		for(res = Init_Lum;res > Final_Lum;res--)
		{
			LCD.BackLightSet(res);
			OSTimeDlyHMSM(0,0,0,dly,OS_OPT_TIME_HMSM_STRICT,&err);
		}
	}
	else
	{
		for(res = Init_Lum;res < Final_Lum;res++)
		{
			LCD.BackLightSet(res);
			OSTimeDlyHMSM(0,0,0,dly,OS_OPT_TIME_HMSM_STRICT,&err);
		}
	}
	return 0;
}

static u8 UI_SingleColorIconSet(_SiglColIconHandle * Icon)
{
	u8 h = Icon->Size_Y,w = Icon->Size_X/8, i,temp;
	
	for(h=0;h<15;h++)
	{   						
		for(w=0;w<4;w++)
		{
			temp=Icon->Cache[h*4+w];
			for(i=0;i<8;i++)
			{
				if(temp&0x80)LCD.Fast_DrawPoint(Icon->Position_X+(w*8+i),Icon->Position_Y+h,Icon->Color);
				temp<<=1;
			}
		}
	}
	
	return 0;
}



static void UI_Config(void)
{
	//加载主页图标
	UI.LoadingIconPic(&ICON_Rename);
	UI.LoadingIconPic(&ICON_MeetInfo);
	UI.LoadingIconPic(&ICON_Service);
	UI.LoadingIconPic(&ICON_SMS);
	UI.LoadingIconPic(&ICON_ShowName);
	UI.LoadingIconPic(&ICON_Setting);
	
	//加载服务图标
	UI.LoadingIconPic(&SICON_Coffee);
	UI.LoadingIconPic(&SICON_Tea);
	UI.LoadingIconPic(&SICON_Flower);
	UI.LoadingIconPic(&SICON_Waiter);
	UI.LoadingIconPic(&SICON_Microphone);
	UI.LoadingIconPic(&SICON_Pen);
	UI.LoadingIconPic(&SICON_Paper);
	UI.LoadingIconPic(&SICON_Water);
	//短信功能
	UI.LoadingIconPic(&SMS_ICON_Send);
	UI.LoadingIconPic(&SMS_ICON_Choice);
	
	//加载设置图标
//	UI.LoadingIconPic(&OICON_AddressSet);
//	UI.LoadingIconPic(&OICON_BackGroundSet);
//	UI.LoadingIconPic(&OICON_LightSet);
//	UI.LoadingIconPic(&OICON_SystemSta);
//	UI.LoadingIconPic(&OICON_WIFISet);
	
	//加载底部图标
//	UI.LoadingIconPic(&MICON_Home);
//	UI.LoadingIconPic(&MICON_ShowName);
//	UI.LoadingIconPic(&MICON_SysMsg);
	
	UI.LoadingIconPic(&ICON_Return);
	
	//加载日历图标
//	UI.LoadingFBPic(&Calendar);
	
	//加载背景
	strcpy((char*)Main_BackGround.Path,(char*)SYS->InnerJPG_Path);
	UI.LoadingBGPic(&Main_BackGround);
	
	//加载状态栏背景
	UI.LoadingFBPic(&StatusBar);
	memcpy(StatusBar.Cache,Main_BackGround.Cache,StatusBar.Size_X*StatusBar.Size_Y*2);
	UI_DarkerRAM(StatusBar.Cache,StatusBar.Size_X*StatusBar.Size_Y,60);
	
	Button_Rename_PerName.ThemeStr = (char*)SYS->Name_Person;
	Button_Rename_PerCom.ThemeStr = (char*)SYS->Name_Company;
	Button_Rename_PerPos.ThemeStr = (char*)SYS->Name_Position;
	
	
}



