#include "usb_func.h"

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;
u8 usb_connect = 0;

u8 USH_User_App(void)
{
	if(HCD_IsDeviceConnected(&USB_OTG_Core))
	{
		printf("usb is connected\r\n");
		usb_connect = 1;
	}
	else usb_connect = 0;
	return usb_connect;
}


