#ifndef _APP_UI_H_
#define _APP_UI_H_ 
#include "sys.h"
#include "text.h"
#include "lcd.h"

#define MENU_ICON_WIDTH  					170
#define MENU_ICON_HEIGHT  				170

#define MENU_SHOWNAME_WIDTH 			146
#define MENU_SHOWNAME_HEIGHT  		39

#define MENU_MEETINFO_WIDTH 			340
#define MENU_MEETINFO_HEIGHT  		355

#define SERVE_ICON_WIDTH 			176
#define SERVE_ICON_HEIGHT  		170

#define RETURN_ICON_WIDTH 			113
#define RETURN_ICON_HEIGHT  		45

////按键类型
//typedef enum
//{
//	NORMAL=0,
//	ICON,
//}_ButtonType;

//类型
typedef enum
{
	COLOR=0,
	PICTURE,
}_UIType;

//位置类型
typedef enum
{
	ABSOLUTE=0,
	RELATIVE,
	
}_PosType;

//按键状态
typedef enum
{
	CLICK=0,
	UNCLICK,
}_ButtonSta;

//图标句柄
typedef struct
{
	u8 Path[30];
	u8 PathPress[30];
	
	u16 *Cache;
	u16 *CachePress;
	const u8 *Frame;
	
	u16 Size_X;
	u16 Size_Y;
	
}_IconPicHandle;

//单色图标句柄
typedef struct
{
	u16 Color;
	const u8 *Cache;
	
	u16 Size_X;
	u16 Size_Y;
		
  u16 Position_X;
	u16 Position_Y;
	
}_SiglColIconHandle;

//背景句柄
typedef struct
{
	_UIType Type;
	u8 Path[30];
	u16 *Cache;
	
	u16 Color;
	
	u16 Size_X;
	u16 Size_Y;
	
}_BackGroundHandle;


//文本框句柄
typedef struct
{
	_PosType PosType;
	_BOOL Border;
	_BOOL Filling;
	_BOOL Center;
	
	u16 BorderColor;
	u8 BorderSize;
	u16 FillingColor;
	u16 FontColor;
	
	const char *String;
	u8 FontSize;
	
	signed int Position_X;
	signed int Position_Y;
	
	u16 Size_X;
	u16 Size_Y;
	
}_TextBoxHandle;

//按键句柄
typedef struct
{
	_PosType PosType;
	_UIType Type;
	_ButtonSta Sta;
	//图片ICON
	_IconPicHandle *IconPic;
	
	char *ThemeStr;
	_TextBoxHandle *Theme;
	
	//纯色ICON
	u16 MainColor;
	u16 BorderColor;
	u8 BorderSize;
	
	u16 MainColorPress;
	u16 BorderColorPress;
	u8 BorderSizePress;
	
	u16 Position_X;
	u16 Position_Y;
	u16 Size_X;
	u16 Size_Y;
	
}_ButtonHandle;

//显示块句柄
typedef struct
{
	_UIType Type;
	_BOOL Border;
	_BOOL Filling;
	u8 Path[30];
	u16 *Cache;
	
	u16 BorderColor;
	u8 BorderSize;
	u16 FillingColor;
	u16 FontColor;

	u16 Position_X;
	u16 Position_Y;
	
	u16 Size_X;
	u16 Size_Y;
	
	const u8 *Frame;
	
}_FuncBlockHandle;

//滑动条句柄
typedef struct
{
	u16 BackColor;
	u16 SliderColor;
	u8 BorderSize;

	u16 Position_X;
	u16 Position_Y;
	
	u16 TchEfecPosInc_X;  //触摸有效位置增加参数
	u16 TchEfecPosInc_Y;
	
	u16 Size_X;
	u16 Size_Y;
	
	u8 ParaMax;
	u8 ParaMin;
		
	char *ThemeStr;
	_TextBoxHandle *Theme;
	
}_SliderHandle;

////UI页面句柄
//typedef struct
//{
//	u8 PageNum;
//	
//	_ButtonHandle *PageButtonLib[16];
//	_BackGroundHandle *PageBackGround;
//	_TextBoxHandle *PageTextBoxLib[8];
//	_FuncBlockHandle *PageFuncBlockLib[4];
//	
//}_UIPageHandle;


//UI功能类
typedef struct
{
	u8 CurrentPageNum;
	
	u8 (*LoadingIconPic)(_IconPicHandle *);
	u8 (*LoadingBGPic)(_BackGroundHandle *);
	u8 (*LoadingFBPic)(_FuncBlockHandle *);
	u8 (*CreateButton)(_ButtonHandle *);
	u8 (*SetButton)(_ButtonHandle *,_ButtonSta);
	u8 (*SetBackGround)(_BackGroundHandle *,u16,u16,u16,u16);
	u8 (*SetTextBox)(_TextBoxHandle *,char *,u16 ,u16);
	u8 (*SetFuncBlock)(_FuncBlockHandle *,u16 *);
	u8 (*ButtonTouch)(_ButtonHandle *,u16 ,u16 ,u8 );
	u8 (*ButtonLibTouch)(_ButtonHandle **,u16 ,u16 ,u8 ,u8 );
	u8 (*SetSlider)(_SliderHandle *,u8 );
	u8 (*SliderTouch)(_SliderHandle *,u16 ,u16 ,u8 ,u8 *);
	u8 (*BacklightGradient)(u8 ,u8 ,u8 );
	u8 (*SingleColorIconSet)(_SiglColIconHandle * );
	void (*Config)(void);
	
}_ui_class;


extern _ui_class UI;

extern _SiglColIconHandle MicICON_WIFI1;
extern _SiglColIconHandle MicICON_WIFI2;
extern _SiglColIconHandle MicICON_WIFI3;
extern _SiglColIconHandle MicICON_WIFI4;
extern _SiglColIconHandle MicICON_Link1;
extern _SiglColIconHandle MicICON_Battery1;
extern _SiglColIconHandle MicICON_Battery2;

extern _ButtonHandle Button_Rename;
extern _ButtonHandle Button_MeetMsg;
extern _ButtonHandle Button_Service;
extern _ButtonHandle Button_SMS;
extern _ButtonHandle Button_ShowName;
extern _ButtonHandle Button_Setting;

extern _ButtonHandle ButtonS_Coffee;
extern _ButtonHandle ButtonS_Tea;
extern _ButtonHandle ButtonS_Flower;
extern _ButtonHandle ButtonS_Waiter;
extern _ButtonHandle ButtonS_Microphone;
extern _ButtonHandle ButtonS_Pen;
extern _ButtonHandle ButtonS_Paper;
extern _ButtonHandle ButtonS_Water;
//extern _ButtonHandle ButtonS_ExtraSer;
//extern _ButtonHandle ButtonS_Confirm;

extern _ButtonHandle ButtonO_AddressSet;
//extern _ButtonHandle ButtonO_BackGroundSet;
//extern _ButtonHandle ButtonO_LightSet;
//extern _ButtonHandle ButtonO_SystemSta;
//extern _ButtonHandle ButtonO_WIFISet;
extern _ButtonHandle Button_Option_SetDeviceID;
extern _ButtonHandle Button_Option_SetServerIP;
extern _ButtonHandle Button_Option_SetServerPort;
extern _ButtonHandle Button_Option_SetLocalIP;
extern _ButtonHandle Button_Option_SSID;
extern _ButtonHandle Button_Option_WIFIPwd;
extern _ButtonHandle Button_Option_DHCP;

//extern _ButtonHandle ButtonM_Home;
//extern _ButtonHandle ButtonM_ShowName;
//extern _ButtonHandle ButtonM_SysMsg;
extern _ButtonHandle ButtonM_Back;

extern _ButtonHandle Button_Rename_PerName;
extern _ButtonHandle Button_Rename_PerCom;
extern _ButtonHandle Button_Rename_PerPos;
extern _ButtonHandle Button_Rename_PerNameColor;
extern _ButtonHandle Button_Rename_PerComColor;
extern _ButtonHandle Button_Rename_PerPosColor;
extern _ButtonHandle Button_Rename_SetBackGroundColor;
extern _ButtonHandle Button_Rename_SetPicNameCar;
extern _ButtonHandle Button_Rename_Confirm;

extern _ButtonHandle Button_SMS_Addressee;
extern _ButtonHandle Button_SMS_Content;
extern _ButtonHandle Button_SMS_Send;
extern _ButtonHandle Button_SMS_Choice;

extern _ButtonHandle *MainButtonLib[6];
extern _ButtonHandle *Button_M_Lib[4];
extern _ButtonHandle *ServiceButtonLib[8];
//extern _ButtonHandle *OptionButtonLib[3];
extern _ButtonHandle *OptionAddressSet_ButtonLib[4];
extern _ButtonHandle *OptionWIFISet_ButtonLib[2];
extern _ButtonHandle *Button_RenameLib[9];
extern _ButtonHandle *Button_SMSLib[3];

extern _BackGroundHandle Main_BackGround;

extern _TextBoxHandle MainPageHeading;
extern _TextBoxHandle SecPageHeading;
extern _TextBoxHandle ButtonTheme;
extern _TextBoxHandle Serve_BtnTheme;
extern _TextBoxHandle MeetInfo_BtnTheme;
extern _TextBoxHandle ShowName_BtnTheme;
extern _TextBoxHandle Button_SMSSendTheme;
extern _TextBoxHandle Button_SMSChoiceTheme;
extern _TextBoxHandle Button_SMSTheme;
extern _TextBoxHandle Button_SliderTheme;
extern _TextBoxHandle Button_ModifyTheme;
//extern _TextBoxHandle CalendarDate;
extern _TextBoxHandle Rename_InfoHeading;
extern _TextBoxHandle Rename_SetInfo;
extern _TextBoxHandle Rename_SetBGColor;
extern _TextBoxHandle Rename_FuncButton;
extern _TextBoxHandle MeetMsg_MeetingTopic;
extern _TextBoxHandle MeetMsg_MeetingTitle;
extern _TextBoxHandle MeetMsg_MeetingContent;
extern _TextBoxHandle MeetMsg_MeetingSTime;
extern _TextBoxHandle MeetMsg_MeetingETime;
extern _TextBoxHandle MeetMsg_RecMsgTitle;
extern _TextBoxHandle MeetMsg_RecMsgContent;
extern _TextBoxHandle SMS_AddresseeName;
extern _TextBoxHandle SMS_Content;
extern _TextBoxHandle CallService_ExtraSer;
extern _TextBoxHandle Option_DeviceID;
extern _TextBoxHandle Option_ServerIP;
extern _TextBoxHandle Option_ServerPort;
extern _TextBoxHandle Option_LocalIP;
extern _TextBoxHandle Option_DHCP;
extern _TextBoxHandle Option_DeviceMac;
extern _TextBoxHandle Option_CodeVersion;
extern _TextBoxHandle Option_MCUUsage;
extern _TextBoxHandle Option_FLASHUsage;
extern _TextBoxHandle Option_WIFISignal;
extern _TextBoxHandle Option_SSID;
extern _TextBoxHandle Option_WIFIPwd;
extern _TextBoxHandle StatusBar_TimeDate;
extern _TextBoxHandle Updating_Prompt;


//extern _FuncBlockHandle Calendar;
extern _FuncBlockHandle StatusBar;
extern _FuncBlockHandle Rename_Blackboard;
//extern _FuncBlockHandle Msg_Blackboard;

extern _SliderHandle INScreenBrightness;
extern _SliderHandle OUTScreenBrightness;
#endif
