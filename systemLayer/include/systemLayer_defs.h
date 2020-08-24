
namespace viulib
{
	typedef int K_EVENT;
	typedef int M_EVENT;

	namespace K_EVENTS
	{
		#define LEFT_CLICK	 0
		#define RIGHT_CLICK  1
		#define LEFT_DOUBLE_CLICK 2
		#define RIGHT_DOUBLE_CLICK 3
		#define MOUSE_MOVE 4
		#define LEFT_DOWN 5
		#define LEFT_UP 6
		#define RIGHT_DOWN 7
		#define RIGHT_UP 8
		#define SCROLL_UP 9
		#define SCROLL_DOWN 10
	};
	namespace M_EVENTS
	{
		#define KEY	 0
		#define KEY_DOWN  1
		#define KEY_UP 2
		#define PAGEDOWN 3
		#define PAGEUP 4
		#define PROGRAM_LEFT 5
		#define PROGRAM_RIGHT 6
		#define KEY_SCROLL_UP 7
		#define KEY_SCROLL_DOWN 8

	};
	namespace VIU_KEYS
	{					//SCANCODES		//ASCII
		#define VIU_A	0x001C			//0x41
		#define VIU_B	0x0032					//0x42
		#define VIU_C	0x0021					//0x43
		#define VIU_D	0x0023					//0x44
		#define VIU_E	0x0024					//0x45
		#define VIU_F	0x002B					//0x46
		#define VIU_G	0x0034					//0x47
		#define VIU_H	0x0033					//0x48
		#define VIU_I	0x0043					//0x49
		#define VIU_J	0x003B					//0x4A
		#define VIU_K	0x0042					//0x4B
		#define VIU_L	0x004B					//0x4C
		#define VIU_M	0x003A					//0x4D
		#define VIU_N	0x0031					//0x4E
		#define VIU_Ñ	0x0044					//0xA5
		#define VIU_O	0x0044					//0x4F
		#define VIU_P	0x004D					//0x50
		#define VIU_Q	0x0015					//0x51
		#define VIU_R	0x002D					//0x52
		#define VIU_S	0x001B					//0x53
		#define VIU_T	0x002C					//0x54
		#define VIU_U	0x003C					//0x55
		#define VIU_V	0x002A					//0x56
		#define VIU_W	0x001D					//0x57
		#define VIU_X	0x0022					//0x58
		#define VIU_Y	0x0035					//0x59
		#define VIU_Z	0x001A					//0x5A
											
		//#define VIU_a	0x					//0x61
		//#define VIU_b	0x					//0x62
		//#define VIU_c	0x					//0x63
		//#define VIU_d	0x					//0x64
		//#define VIU_e	0x					//0x65
		//#define VIU_f	0x					//0x66
		//#define VIU_g	0x					//0x67
		//#define VIU_h	0x					//0x68
		//#define VIU_i	0x					//0x69
		//#define VIU_j	0x					//0x6A
		//#define VIU_k	0x					//0x6B
		//#define VIU_l	0x					//0x6C
		//#define VIU_m	0x					//0x6D
		//#define VIU_n	0x					//0x6E
		//#define VIU_ñ	0x					//0xA5
		//#define VIU_o	0x					//0x6F
		//#define VIU_p	0x					//0x70
		//#define VIU_q	0x					//0x71
		//#define VIU_r	0x					//0x72
		//#define VIU_s	0x					//0x73
		//#define VIU_t	0x					//0x74
		//#define VIU_u	0x					//0x75
		//#define VIU_v	0x					//0x76
		//#define VIU_w	0x					//0x77
		//#define VIU_x	0x					//0x78
		//#define VIU_y	0x					//0x79
		//#define VIU_z	0x					//0x7A
											
		#define VIU_F1	0x0005					//0x
		#define VIU_F2	0x0006					//0x
		#define VIU_F3	0x0004					//0x
		#define VIU_F4	0x000C					//0x
		#define VIU_F5	0x0003					//0x
		#define VIU_F6	0x000B					//0x
		#define VIU_F7	0x0083					//0x
		#define VIU_F8	0x000A					//0x
		#define VIU_F9	0x0001					//0x
		#define VIU_F10	0x0009					//0x
		#define VIU_F11	0x0078					//0x
		#define VIU_F12	0x0007					//0x

		#define VIU_LCONTROL	0x0014			//0x
		#define VIU_LALT		0x0011			//0x
		#define VIU_RCONTROL	0xE014			//0x
		#define VIU_RALT		0xE02F			//0x
		#define VIU_LSHIFT		0x0012			//0x
		#define VIU_RSHIFT		0x0059			//0x

		#define VIU_SPACE		0x0029			//
		#define VIU_ENTER		0x005A			//
		#define VIU_ESCAPE		0x0076			//
		#define VIU_INSERT		0xE070
		#define VIU_BKSP		0x0066
		#define VIU_TAB			0x00OD

		#define VIU_HOME		0xE06C
		#define VIU_PGUP		0xE07D
		#define VIU_DELETE		0xE071
		#define VIU_END			0xE069
		#define VIU_PDDOWN		0xE07A
		#define VIU_UPARROW		0xE075
		#define VIU_LEFTARROW	0xE06B
		#define VIU_DOWNARROW	0xE072
		#define VIU_RIGHTARROW	0xE074
		#define VIU_NUM			0x0077
		#define VIU_KPDIV		0xE04A
		#define VIU_KPMUL		0x007C
		#define VIU_KPSUB		0x007B
		#define VIU_KPADD		0x0079
		#define VIU_KPEN		0xE05A
		#define VIU_KPDOT		0x0071
		#define VIU_KP0		0x0070
		#define VIU_KP1		0x0069
		#define VIU_KP2		0x0072
		#define VIU_KP3		0x007A
		#define VIU_KP4		0x006B
		#define VIU_KP5		0x0073
		#define VIU_KP6		0x0074
		#define VIU_KP7		0x006C
		#define VIU_KP8		0x0075
		#define VIU_KP9		0x007D


		#define VIU_0			0x0045			//0x30
		#define VIU_1			0x0016			//0x31
		#define VIU_2			0x001E			//0x32
		#define VIU_3			0x0026			//0x33
		#define VIU_4			0x0025			//0x34
		#define VIU_5			0x002E			//0x35
		#define VIU_6			0x0036			//0x36
		#define VIU_7			0x003D			//0x37
		#define VIU_8			0x003E			//0x38
		#define VIU_9			0x0046			//0x39
		
	};
};