/*
*********************************************************************************************************
*                                             uC/GUI V5.34
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2016, Micrium Inc., Weston, FL
*                       (c) Copyright 2016, SEGGER Microcontroller GmbH & Co. KG
*
*              uC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
*********************************************************************************************************
File        : GUIDEMO.h
Purpose     : Configuration file of GUIDemo
*********************************************************************************************************
*/

#ifndef GUIDEMO_H
#define GUIDEMO_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include "GUI.h"

#if GUI_WINSUPPORT
  #include "WM.h"

  #include "CHECKBOX.h"
  #include "FRAMEWIN.h"
  #include "PROGBAR.h"
  #include "TEXT.h"
  #include "BUTTON.h"
  #include "SLIDER.h"
  #include "HEADER.h"
  #include "GRAPH.h"
  #include "ICONVIEW.h"
  #include "LISTVIEW.h"
  #include "TREEVIEW.h"
#else
  #include <stdlib.h>  // Definition of NULL
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define CONTROL_SIZE_X    80
#define CONTROL_SIZE_Y    61
#define INFO_SIZE_Y       65
#define BUTTON_SIZE_X     36
#define BUTTON_SIZE_Y     22
#define PROGBAR_SIZE_X    74
#define PROGBAR_SIZE_Y    12
#define TEXT_SIZE_X       69
#define TEXT_SIZE_Y       7
#define SHOW_PROGBAR_AT   100
#define GUI_ID_HALT       (GUI_ID_USER + 0)
#define GUI_ID_NEXT       (GUI_ID_USER + 1)

#if 0
  #define GUI_BLUE          0x00FF0000
  #define GUI_GREEN         0x0000FF00
  #define GUI_RED           0x000000FF
  #define GUI_CYAN          0x00FFFF00
  #define GUI_MAGENTA       0x00FF00FF
  #define GUI_YELLOW        0x0000FFFF
  #define GUI_LIGHTBLUE     0x00FF8080
  #define GUI_LIGHTGREEN    0x0080FF80
  #define GUI_LIGHTRED      0x008080FF
  #define GUI_LIGHTCYAN     0x00FFFF80
  #define GUI_LIGHTMAGENTA  0x00FF80FF
  #define GUI_LIGHTYELLOW   0x0080FFFF
  #define GUI_DARKBLUE      0x00800000
  #define GUI_DARKGREEN     0x00008000
  #define GUI_DARKRED       0x00000080
  #define GUI_DARKCYAN      0x00808000
  #define GUI_DARKMAGENTA   0x00800080
  #define GUI_DARKYELLOW    0x00008080
  #define GUI_WHITE         0x00FFFFFF
  #define GUI_LIGHTGRAY     0x00D3D3D3
  #define GUI_GRAY          0x00808080
  #define GUI_DARKGRAY      0x00404040
  #define GUI_BLACK         0x00000000
  #define GUI_BROWN         0x002A2AA5
  #define GUI_ORANGE        0x0000A5FF
  #define GUI_TRANSPARENT   0xFF000000

  #define GUI_GRAY_3F       0x003F3F3F
  #define GUI_GRAY_50       0x00505050
  #define GUI_GRAY_55       0x00555555
  #define GUI_GRAY_60       0x00606060
  #define GUI_GRAY_7C       0x007C7C7C
  #define GUI_GRAY_9A       0x009A9A9A
  #define GUI_GRAY_AA       0x00AAAAAA
  #define GUI_GRAY_C0       0x00C0C0C0
  #define GUI_GRAY_C8       0x00C8C8C8
  #define GUI_GRAY_D0       0x00D0D0D0
  #define GUI_GRAY_E7       0x00E7E7E7
  #define GUI_BLUE_98       0x00980000

#endif

#define BK_COLOR_0        GUI_MAKE_COLOR(0xFF5555)
#define BK_COLOR_1        GUI_MAKE_COLOR(0x880000)

#define BK_BLUE_LIGHT     GUI_MAKE_COLOR(GUI_LIGHTBLUE)
#define BK_BLUE_DARK      GUI_MAKE_COLOR(GUI_DARKBLUE)

#define BK_GREEN_LIGHT    GUI_MAKE_COLOR(GUI_LIGHTGREEN)
#define BK_GREEN_DARK     GUI_MAKE_COLOR(GUI_DARKGREEN)

#define BK_RED_LIGHT      GUI_MAKE_COLOR(GUI_LIGHTRED)
#define BK_RED_DARK       GUI_MAKE_COLOR(GUI_DARKRED)

#define BK_ORANGE_LIGHT   GUI_MAKE_COLOR(GUI_ORANGE)
#define BK_ORANGE_DARK    GUI_MAKE_COLOR(0x000040A0)

#define BK_YELLOW_LIGHT   GUI_MAKE_COLOR(GUI_YELLOW)
#define BK_YELLOW_DARK    GUI_MAKE_COLOR(GUI_DARKYELLOW)

#define NUMBYTES_NEEDED   0x200000UL
#define CIRCLE_RADIUS     100
#define LOGO_DIST_BORDER  5
#define CHAR_READING_TIME 80
#define XSIZE_MIN         320
#define YSIZE_MIN         240

//
// Use an or-combination of the following flags to configure the
// GUIDemo container for the current application.
//
#define GUIDEMO_SHOW_CURSOR  (1 << 0)
#define GUIDEMO_SHOW_INFO    (1 << 1)
#define GUIDEMO_SHOW_CONTROL (1 << 2)

#define SHIFT_RIGHT_16(x)    ((x) / 65536)

/*********************************************************************
*
*       Configuration of modules to be used
*
**********************************************************************
*/
#ifndef   SHOW_GUIDEMO_AATEXT
  #define SHOW_GUIDEMO_AATEXT            (1)
#endif
#ifndef   SHOW_GUIDEMO_AUTOMOTIVE
  #define SHOW_GUIDEMO_AUTOMOTIVE        (0)
#endif
#ifndef   SHOW_GUIDEMO_BARGRAPH
  #define SHOW_GUIDEMO_BARGRAPH          (1)
#endif
#ifndef   SHOW_GUIDEMO_BITMAP
  #define SHOW_GUIDEMO_BITMAP            (1)
#endif
#ifndef   SHOW_GUIDEMO_COLORBAR
  #define SHOW_GUIDEMO_COLORBAR          (1)
#endif
#ifndef   SHOW_GUIDEMO_CURSOR
  #define SHOW_GUIDEMO_CURSOR            (1)
#endif
#ifndef   SHOW_GUIDEMO_FADING
  #define SHOW_GUIDEMO_FADING            (1)
#endif
#ifndef   SHOW_GUIDEMO_GRAPH
  #define SHOW_GUIDEMO_GRAPH             (1)
#endif
#ifndef   SHOW_GUIDEMO_ICONVIEW
  #define SHOW_GUIDEMO_ICONVIEW          (1)
#endif
#ifndef   SHOW_GUIDEMO_IMAGEFLOW
  #define SHOW_GUIDEMO_IMAGEFLOW         (1)
#endif
#ifndef   SHOW_GUIDEMO_LISTVIEW
  #define SHOW_GUIDEMO_LISTVIEW          (1)
#endif
#ifndef   SHOW_GUIDEMO_RADIALMENU
  #define SHOW_GUIDEMO_RADIALMENU        (1)
#endif
#ifndef   SHOW_GUIDEMO_SKINNING
  #define SHOW_GUIDEMO_SKINNING          (1)
#endif
#ifndef   SHOW_GUIDEMO_SPEED
  #define SHOW_GUIDEMO_SPEED             (1)
#endif
#ifndef   SHOW_GUIDEMO_SPEEDOMETER
  #define SHOW_GUIDEMO_SPEEDOMETER       (1)
#endif
#ifndef   SHOW_GUIDEMO_TRANSPARENTDIALOG
  #define SHOW_GUIDEMO_TRANSPARENTDIALOG (1)
#endif
#ifndef   SHOW_GUIDEMO_TREEVIEW
  #define SHOW_GUIDEMO_TREEVIEW          (1)
#endif
#ifndef   SHOW_GUIDEMO_VSCREEN
  #define SHOW_GUIDEMO_VSCREEN           (1)
#endif
#ifndef   SHOW_GUIDEMO_WASHINGMACHINE
  #define SHOW_GUIDEMO_WASHINGMACHINE    (1)
#endif
#ifndef   SHOW_GUIDEMO_ZOOMANDROTATE
  #define SHOW_GUIDEMO_ZOOMANDROTATE     (1)
#endif

/*********************************************************************
*
*       Configuration macros
*
**********************************************************************
*/
#ifndef   GUIDEMO_SHOW_SPRITES
  #define GUIDEMO_SHOW_SPRITES    (1)
#endif
#ifndef   GUIDEMO_USE_VNC
  #define GUIDEMO_USE_VNC         (0)
#endif
#ifndef   GUIDEMO_USE_AUTO_BK
  #define GUIDEMO_USE_AUTO_BK     (1)
#endif
#ifndef   GUIDEMO_SUPPORT_TOUCH
  #define GUIDEMO_SUPPORT_TOUCH   (1)
#endif
#ifndef   GUIDEMO_SUPPORT_CURSOR
  #define GUIDEMO_SUPPORT_CURSOR  (GUI_SUPPORT_CURSOR && GUI_SUPPORT_TOUCH)
#endif

#ifndef   GUIDEMO_CF_SHOW_SPRITES
  #define GUIDEMO_CF_SHOW_SPRITES   (GUIDEMO_SHOW_SPRITES                   <<  0)
#endif
#ifndef   GUIDEMO_CF_USE_VNC
  #define GUIDEMO_CF_USE_VNC        (GUIDEMO_USE_VNC                        <<  1)
#endif
#ifndef   GUIDEMO_CF_USE_AUTO_BK
  #define GUIDEMO_CF_USE_AUTO_BK    (GUIDEMO_USE_AUTO_BK                    <<  2)
#endif
#ifndef   GUIDEMO_CF_SUPPORT_TOUCH
  #define GUIDEMO_CF_SUPPORT_TOUCH  (GUI_WINSUPPORT ? GUIDEMO_SUPPORT_TOUCH <<  3 : 0)
#endif

/*********************************************************************
*
*       GUIDEMO_CONFIG
*/
typedef struct GUIDEMO_CONFIG {
  void (* * apFunc)(void);
  int       NumDemos;
  U16       Flags;
  #if GUIDEMO_USE_VNC
    int  (* pGUI_VNC_X_StartServer)(int LayerIndex, int ServerIndex);
  #endif
} GUIDEMO_CONFIG;

#if (GUI_WINSUPPORT == 0)
  #define GUIDEMO_NotifyStartNext  GUIDEMO_ClearHalt
  #define GUIDEMO_Delay            GUI_Delay
#else
  void    GUIDEMO_NotifyStartNext  (void);
  void    GUIDEMO_Delay            (int Time);
#endif

/*********************************************************************
*
*       Internal functions
*
**********************************************************************
*/
void      GUIDEMO_AddIntToString   (char * pText, U32 Number);
void      GUIDEMO_AddStringToString(char * pText, const char * pAdd);
int       GUIDEMO_CheckCancel      (void);
int       GUIDEMO_CheckCancelDelay (int Delay);
void      GUIDEMO_ClearHalt        (void);
void      GUIDEMO_ClearText        (char * pText);
void      GUIDEMO_Config           (GUIDEMO_CONFIG * pConfig);
void      GUIDEMO_ConfigureDemo    (char * pTitle, char * pDescription, unsigned Flags);
void      GUIDEMO_DispHint         (char * pHint);
void      GUIDEMO_DispTitle        (char * pTitle);
void      GUIDEMO_DrawBk           (void);
U16       GUIDEMO_GetConfFlag      (U16 Flag);
int       GUIDEMO_GetTime          (void);
int       GUIDEMO_GetTitleSizeY    (void);
void      GUIDEMO_HideCursor       (void);
void      GUIDEMO_Intro            (void);
void      GUIDEMO_Main             (void);
GUI_COLOR GUIDEMO_MixColors        (GUI_COLOR Color0, GUI_COLOR Color1, U8 Intens);
void      GUIDEMO_SetInfoText      (const char * pInfo);
int       GUIDEMO_ShiftRight       (int Value, U8 NumBits);
void      GUIDEMO_ShowCursor       (void);
void      GUIDEMO_Wait             (int TimeWait);

/*********************************************************************
*
*       Demo modules
*
**********************************************************************
*/
void GUIDEMO_AntialiasedText  (void);
void GUIDEMO_Automotive       (void);
void GUIDEMO_BarGraph         (void);
void GUIDEMO_Bitmap           (void);
void GUIDEMO_ColorBar         (void);
void GUIDEMO_Cursor           (void);
void GUIDEMO_Fading           (void);
void GUIDEMO_Graph            (void);
void GUIDEMO_IconView         (void);
void GUIDEMO_ImageFlow        (void);
void GUIDEMO_Listview         (void);
void GUIDEMO_RadialMenu       (void);
void GUIDEMO_Skinning         (void);
void GUIDEMO_Speed            (void);
void GUIDEMO_Speedometer      (void);
void GUIDEMO_TransparentDialog(void);
void GUIDEMO_Treeview         (void);
void GUIDEMO_VScreen          (void);
void GUIDEMO_WashingMachine   (void);
void GUIDEMO_ZoomAndRotate    (void);

/*********************************************************************
*
*       Externs
*
**********************************************************************
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmMicriumLogoFrame69x32;
extern GUI_CONST_STORAGE GUI_BITMAP bmMicrium_Logo_200x56;
extern GUI_CONST_STORAGE GUI_BITMAP bmuCOSIIIBook;
extern GUI_CONST_STORAGE GUI_BITMAP bmMicriumLogo;
extern GUI_CONST_STORAGE GUI_BITMAP bmMicriumLogoLarge;

extern GUI_CONST_STORAGE GUI_BITMAP bmOS2_Logo;
extern GUI_CONST_STORAGE GUI_BITMAP bmOS3_Logo;
extern GUI_CONST_STORAGE GUI_BITMAP bmGUI_Logo;
extern GUI_CONST_STORAGE GUI_BITMAP bmplay;
extern GUI_CONST_STORAGE GUI_BITMAP bmforward;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_0;
extern GUI_CONST_STORAGE GUI_BITMAP bmgrad_32x16;

extern GUI_CONST_STORAGE GUI_FONT   GUI_FontRounded16;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontRounded22;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontSouvenir18;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontD6x8;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontAA2_32;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontAA4_32;

extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_00;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_01;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_02;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_03;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_04;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_10;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_11;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_12;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_13;
extern GUI_CONST_STORAGE GUI_BITMAP bmDolphin_14;

#if defined(__cplusplus)
  }
#endif

#endif // Avoid multiple inclusion

/*************************** End of file ****************************/