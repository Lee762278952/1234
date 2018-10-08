#ifndef _SYS_PARAMETERS_H
#define _SYS_PARAMETERS_H

#include "sys.h"


#define   Version   "V1.0"

#define   Test_task     0
//#define   SD_CARD_ENABLE
#define   TP_DEBUG      1

#define 	BG_Path						"1:/BACKGROUND/"				//�����ļ���ͼƬ·��
#define   ICON_Path 				"1:/ICON/"


#define   ScreenDimTime			15
#define   HeartBeatTime			15

#define   MaxDeviceID				1000

#define   ScreenInside			LCD_2
#define   ScreenOutside			LCD_1
#define   ManuLayer         1  //�˵�����
#define   Lay1        			0	//��һ��


//Menuͼ������
#define		ICON_Size			128

#define		ICON1_Xs			84
#define		ICON1_Ys			100
#define		ICON1_Xe			ICON1_Xs+ICON_Size
#define		ICON1_Ye			ICON1_Ys+ICON_Size

#define		ICON2_Xs			252
#define		ICON2_Ys			100
#define		ICON2_Xe			ICON2_Xs+ICON_Size
#define		ICON2_Ye			ICON2_Ys+ICON_Size

#define		ICON3_Xs			84
#define		ICON3_Ys			282
#define		ICON3_Xe			ICON3_Xs+ICON_Size
#define		ICON3_Ye			ICON3_Ys+ICON_Size

#define		ICON4_Xs			252
#define		ICON4_Ys			282
#define		ICON4_Xe			ICON4_Xs+ICON_Size
#define		ICON4_Ye			ICON4_Ys+ICON_Size

#define		ICON5_Xs			420
#define		ICON5_Ys			282
#define		ICON5_Xe			ICON5_Xs+ICON_Size
#define		ICON5_Ye			ICON5_Ys+ICON_Size

#define		ICON6_Xs			588
#define		ICON6_Ys			282
#define		ICON6_Xe			ICON6_Xs+ICON_Size
#define		ICON6_Ye			ICON6_Ys+ICON_Size

#define   ICON_CAL_X_Size		155
#define   ICON_CAL_Y_Size		188

#define   ICON_CAL_Xs       555
#define   ICON_CAL_Ys       55
#define   ICON_CAL_Xe       ICON_CAL_Xs+ICON_CAL_X_Size
#define   ICON_CAL_Ye       ICON_CAL_Ys+ICON_CAL_Y_Size

//Serviceͼ������
#define		SICON1_Xs			84
#define		SICON1_Ys			60
#define		SICON1_Xe			SICON1_Xs+ICON_Size
#define		SICON1_Ye			SICON1_Ys+ICON_Size

#define		SICON2_Xs			252
#define		SICON2_Ys			60
#define		SICON2_Xe			SICON2_Xs+ICON_Size
#define		SICON2_Ye			SICON2_Ys+ICON_Size

#define		SICON3_Xs			420
#define		SICON3_Ys			60
#define		SICON3_Xe			SICON3_Xs+ICON_Size
#define		SICON3_Ye			SICON3_Ys+ICON_Size

#define		SICON4_Xs			588
#define		SICON4_Ys			60
#define		SICON4_Xe			SICON4_Xs+ICON_Size
#define		SICON4_Ye			SICON4_Ys+ICON_Size

#define		SICON5_Xs			84
#define		SICON5_Ys			218
#define		SICON5_Xe			SICON5_Xs+ICON_Size
#define		SICON5_Ye			SICON5_Ys+ICON_Size

#define		SICON6_Xs			252
#define		SICON6_Ys			218
#define		SICON6_Xe			SICON6_Xs+ICON_Size
#define		SICON6_Ye			SICON6_Ys+ICON_Size

#define		SICON7_Xs			420
#define		SICON7_Ys			218
#define		SICON7_Xe			SICON7_Xs+ICON_Size
#define		SICON7_Ye			SICON7_Ys+ICON_Size

#define		SICON8_Xs			588
#define		SICON8_Ys			218
#define		SICON8_Xe			SICON8_Xs+ICON_Size
#define		SICON8_Ye			SICON8_Ys+ICON_Size
//�ײ�ͼ��
//���ء�ϵͳ֪ͨͼ������
#define   MICON_XSize		145
#define   MICON_YSize		50

#define		MICON1_Xs			84
#define		MICON1_Ys			430
#define		MICON1_Xe			MICON1_Xs+MICON_XSize
#define		MICON1_Ye			MICON1_Ys+MICON_YSize

#define		MICON2_Xs			252
#define		MICON2_Ys			430
#define		MICON2_Xe			MICON2_Xs+MICON_XSize
#define		MICON2_Ye			MICON2_Ys+MICON_YSize

#define		MICON3_Xs			420
#define		MICON3_Ys			430
#define		MICON3_Xe			MICON3_Xs+MICON_XSize
#define		MICON3_Ye			MICON3_Ys+MICON_YSize

#define		MICON4_Xs			588
#define		MICON4_Ys			430
#define		MICON4_Xe			MICON4_Xs+MICON_XSize
#define		MICON4_Ye			MICON4_Ys+MICON_YSize

//���ý���ͼ������
#define  	OICON1_Xs			30
#define  	OICON1_Ys			60
#define		OICON1_Xe			OICON1_Xs+ICON_Size
#define		OICON1_Ye			OICON1_Ys+ICON_Size

#define  	OICON2_Xs			183
#define  	OICON2_Ys			60
#define		OICON2_Xe			OICON2_Xs+ICON_Size
#define		OICON2_Ye			OICON2_Ys+ICON_Size

#define  	OICON3_Xs			336
#define  	OICON3_Ys			60
#define		OICON3_Xe			OICON3_Xs+ICON_Size
#define		OICON3_Ye			OICON3_Ys+ICON_Size

#define  	OICON4_Xs			489
#define  	OICON4_Ys			60
#define		OICON4_Xe			OICON4_Xs+ICON_Size
#define		OICON4_Ye			OICON4_Ys+ICON_Size

#define  	OICON5_Xs			642
#define  	OICON5_Ys			60
#define		OICON5_Xe			OICON5_Xs+ICON_Size
#define		OICON5_Ye			OICON5_Ys+ICON_Size


/********************/
#define 	Name_PerLen		17		//������չ��20�ֽ� 10����
#define 	Name_PosLen		21		//������չ��30�ֽ� 15����
#define 	Name_ComLen		21		//������չ��40�ֽ� 20����

//ϵͳ�ؼ�������ַ������
#define   Save											1
#define   Load											0

#define   Flag_Baseaddr							1024*1024*6+100

//�ļ�ϵͳ��ɸ��±�־
#define   FatfsUDFlag								0x14
#define		FatfsUDFlag_addr					Flag_Baseaddr + 0

#define   WIFI_InfoUDFlag						0x11
#define		WIFI_InfoUDFlag_addr			Flag_Baseaddr + 1

#define   FontUDFlag								0x11
#define		FontUDFlag_addr						Flag_Baseaddr + 2

#define   SYSParaUDFlag							0x11
#define		SYSParaUDFlag_addr				Flag_Baseaddr + 3

#define   UIUpdataFlag							0x11
#define		UIUpdataFlag_addr					Flag_Baseaddr + 4

#define   FontUpdataFlag						0x11
#define		FontUpdataFlag_addr				Flag_Baseaddr + 5


//ϵͳ��Ϣ
#define  	SYSPara_Baseaddr					1024*1024*6+512

#define   Device_UID_addr						SYSPara_Baseaddr + 0              	 
#define   Device_UIDLen							2

#define		Name_Person_addr					Device_UID_addr + Device_UIDLen						
#define		Name_PersonLen						20

#define   Name_Position_addr				Name_Person_addr + Name_PersonLen						 
#define   Name_PositionLen					30

#define   Name_Company_addr					Name_Position_addr + Name_PositionLen						
#define   Name_CompanyLen						40

#define		Name_PerColor_addr				Name_Company_addr + Name_CompanyLen						
#define		Name_PerColorLen					2	

#define   Name_PosColor_addr				Name_PerColor_addr + Name_PerColorLen
#define   Name_PosColorLen					2

#define   Name_ComColor_addr				Name_PosColor_addr + Name_PosColorLen
#define   Name_ComColorLen					2

#define		Name_PerSize_addr					Name_ComColor_addr + Name_ComSizeLen						
#define		Name_PerSizeLen						1	   

#define   Name_PosSize_addr					Name_PerSize_addr + Name_PerSizeLen
#define   Name_PosSizeLen						1

#define   Name_ComSize_addr					Name_PosSize_addr + Name_PosSizeLen		
#define   Name_ComSizeLen						1	

#define  	OuterBG_JPG_addr					Name_ComSize_addr + Name_ComSizeLen	
#define  	OuterBG_JPGLen						1

#define  	OuterBG_Color_addr				OuterBG_JPG_addr + OuterBG_JPGLen
#define  	OuterBG_ColorLen					2	

#define  	OuterJPG_Path_addr				OuterBG_Color_addr + OuterBG_ColorLen				 
#define  	OuterJPG_PathLen					30

#define   InnerJPG_Path_addr				OuterJPG_Path_addr + OuterJPG_PathLen	
#define   InnerJPG_PathLen					30

#define   WIFI_SSID_addr						InnerJPG_Path_addr + InnerJPG_PathLen
#define   WIFI_SSIDLen							30

#define   WIFI_PWD_addr							WIFI_SSID_addr + WIFI_SSIDLen
#define   WIFI_PWDLen								30

#define   TCP_IPadd_addr						WIFI_PWD_addr + WIFI_PWDLen
#define   TCP_IPaddLen							20
	
#define   TCP_Port_addr							TCP_IPadd_addr + TCP_IPaddLen
#define   TCP_PortLen								5

#define   Device_IP_addr						TCP_Port_addr + TCP_PortLen
#define   Device_IPLen							20

#define   DHCP_Enable_addr					Device_IP_addr + Device_IPLen
#define   DHCP_EnableLen						1

/***********************************************/



//������״̬
typedef enum
{
	Press=0,
	Loosen,
	Slippage,
}_tp_sta;

typedef struct
{
	_BOOL   StaUpdata;
	_BOOL   CoUpdata;
	
	_tp_sta	Status;					//TP״̬
	u16 		Coordinate_X;		//TP������
	u16 		Coordinate_Y;		//TP������
	u16     BeforeCo_X;
	u16     BeforeCo_Y;
	
}_tp_para;



//��ʾ��ҳ��
typedef enum
{
	WelcomePage = 0,
	MainPage,
	Rename,
	MeetInfo,
	CallService,
	SMS,
	ShowName,
	Setting,	
	Updating,
	
}_menu_page;

//��ʾ��ҳ�������
typedef struct
{
	_BOOL PageUpdata;
	_menu_page Page;
	u32 PageSta[ManuLayer];
	u32 Para;

//	_BOOL OperatUpdata;
//	u8 OperatSta;
//	u16 OperatPara;
}_menu;


//�û�����
typedef struct
{
	char _lstUserName[20];
	u16 _lstUserID;
}_user_lst;	 	

//WIFI�б�
typedef struct
{
	char _lstWIFISSID[30];
	char _lstWIFISIGNAL[5];
}_wifi_lst;

typedef struct
{
	u16 RecMsg_ID;
	u8 	RecMsgCont[200];
}_recmsg_lst;

typedef struct
{
	u8 Year;
	u8 Month; 
	u8 Date;
	u8 WeekDay; 
	u8 Hours;           
  u8 Minutes;          
  u8 Seconds;
	
	char YearStr[10];
	char MonthDateStr[10]; 
  char WeekDayStrCN[7]; 
    
}_timedate_;

typedef struct
{
/************ϵͳ�ؼ���������浽FLASH��***************/
	
	u16 Device_UID;  							//�豸UID��
	u8 InnerJPG_Path[30];					//��¼����ͼƬ·��
	
//
	u8	Name_Person[Name_PerLen];
	u8  Name_Position[Name_PosLen];
	u8  Name_Company[Name_ComLen];
	
	u16 Name_PerColor;
	u16 Name_PosColor;
	u16 Name_ComColor;
	
	u8 Name_PerSize;
	u8 Name_PosSize;
	u8 Name_ComSize;
	
//������������
	_BOOL OuterBG_JPG;			//�����Ƿ���ͼƬ��Ϊ����
	u16 OuterBG_Color;			//�Դ���ɫʱ��ɫ��ֵ
	u8  OuterJPG_Path[30];	//��ͼƬʱͼƬ·��
	
//WIFI
	u8 WIFI_SSID[30];
	u8 WIFI_PWD[30];
	u8 TCP_IPadd[20];
	u8 TCP_Port[5];
	//�豸STA IP
	u8 Device_IP[20];
	
	_BOOL DHCP_Enable;
	
/********************ϵͳ����************************/
//	u16 *bg_cache;				//����ͼ����
	_timedate_ TimeDate;
	
	//ϵͳ��Ӳ������
	char CompileTime[10];  //�������� ������汾�ţ�
	char CompileDate[10];	//����ʱ��
  u8 Device_Mac[20];			//MAC��ַ
  u32 MCU_UID;
	
	_wifi_lst *WIFI_List;
	u8   WIFI_ListNum;
	
	//�豸���� �������ϴ��� ֵ ��1~100
	u8 WIFI_SignalSt; //WIFI�ź�ֵ
	u8 ElectricQua;		//����
	u8 Luminance;			//��������
	u8 O_Luminance;		//��������
	
	
	//��ʼ������
	_BOOL Fatfs_INIT;				
	_BOOL Font_INIT;
	_BOOL FLASH_INIT;
	
#ifdef SD_CARD_ENABLE
	_BOOL SD_CARD_INIT;
#endif

	_BOOL TimeSet;
	u32 TimeStamp;
	
	_BOOL Device_ShutDown;
	_BOOL Charging_EN;
/*******FuncFrame���ܲ���*********/
	u8 	FuncMode; //�ⲿ����
	u16 FuncFrame_temp;
  u8 *FuncFrame_buf;
  u8  FuncInpLen;
	
	_BOOL T9_ENABLE;
	_BOOL T9_UPDATA;
	
	_BOOL ColorSet_ENABLE;
	_BOOL ColorSet_UPDATA;
	
	_BOOL WordSizeSet_ENABLE;
	_BOOL WordSizeSet_UPDATA;
	
	_BOOL AddresseeSel_ENABLE;
	_BOOL AddresseeSel_UPDATA;
	
	_BOOL WIFI_SSID_Get_ENABLE;
	_BOOL WIFI_SSID_Get_UPDATA;
	
//	_BOOL BACKGROUND_Set_ENABLE;
//	_BOOL BACKGROUND_Set_UPDATA;
	
	_BOOL WARNING_ENABLE;


	_BOOL INQUIRY_ENABLE;
	_BOOL INQUIRY_ENABLE_UPDATA;
//	_BOOL WIFI_SSID_Get_UPDATA;
/******************************/	
	//TP������
	_tp_para TP;  
	/*Status;					//TP״̬
	Coordinate_X;		//TP������
	Coordinate_Y;		//TP������*/
	
	//�����˵�������
	_menu Menu;  //��ʾ
	_menu MenuOperat; //����
	/*	PageUpdata  //�˵����±�־
	    Page;       //ҳ�����
	    PageSta[ManuLayer];  //ҳ���ڲ��� */
	u8	DisplayInside_SET;   //������ʾ���ݲ���
	u8	DisplayOutside_SET;   //������ʾ���ݲ���
	
	
	
/*********��������Խ���ز���*******************/
	//�������������ݴ����־
	_BOOL ServerCommand;
	
	//HTTP����ͼƬ��־
	_BOOL HttpGeting;
	
	//ϵͳTCP����
	_BOOL HeartBeat;
	//����ʱ���ۼ���
	u8 heartbeat_Timer;


	//�豸ע���־
	_BOOL DeviceRegister;


	
	//��ȡ�û���Ϣ
	_BOOL GetUserinfo;
	
	//��ȡ�᳡�û��б�
  _BOOL GetUserList;
	
	//����������
	_BOOL ReqService;
	
	//�����������
	u8 *ServCont;
	u8 *ExtraSerCont;
	_BOOL ExtraSer;
	u8 SerID;
	
  //���ŷ���
	_BOOL SendMassage;
	//��������
	u8 *MsgCont;
	//����Ŀ���ַ
  u16 MsgReceive_ID;
	
	//���ն�������
	_recmsg_lst *RecMsgList[10];
	u8 RecMsgContNum;

	//������Ϣ
	u16 MeetingID; //����ID
	u8 MeetingName[20]; //��������
	u8 MeetingSlogan[40]; //�������
	u8 MeetingContent[100]; //��������
	u8 MeetingStartTime[25]; //���鿪ʼʱ��
	u8 MeetingEndTime[25]; //�������ʱ��

  //�û���Ϣ
	u8 *BGpicURL;//����ͼƬURL
	u8 *BGnameURL;//�ϳ�ͼƬURL
	u8 *BG_IP;
	u8 *BG_Port;
	_BOOL BGpicUpdata;
	_BOOL BGnameUpdata;
	
	//�û��б�
	u16 UserNum;//�᳡���û�������
	_user_lst *UserList;
/***************************************/	
	
	//ʡ��������ʱ�µ�����
	u8 dim_bl;
	
}_sys_para_class; 

extern _sys_para_class *SYS;
//extern _sys_para_class SYS;

void SYSPara_SaveNLoad(u8 operation,u8 section);

#endif




