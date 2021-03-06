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
File        : GUIDEMO_Fading.c
Purpose     : Demo of fading animation function
*********************************************************************************************************
*/

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_FADING && GUI_SUPPORT_MEMDEV)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NUM_FADINGS 3
#define NUM_SCREENS 2
#define TIME_FADING 1500

//
// Memory device format in dependence of color format
//
#if (GUI_USE_ARGB)
  #define GUI_COLOR_CONV GUICC_M8888I
#else
  #define GUI_COLOR_CONV GUICC_8888
#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawFrame
*/
static void _DrawFrame(int x0, int y0, int x1, int y1, int b) {
  GUI_FillRect(x0, y0, x1, y0 + b - 1);
  GUI_FillRect(x0, y0 + b, x0 + b - 1, y1 - b);
  GUI_FillRect(x1 - b + 1, y0 + b, x1, y1 - b);
  GUI_FillRect(x0, y1 - b + 1, x1, y1);
}

/*********************************************************************
*
*       _DrawLogo
*/
static void _DrawLogo(int xSize, GUI_MEMDEV_Handle hLogo, GUI_MEMDEV_Handle hMemWork, GUI_RECT * pRect) {
  int xSizeLogo, ySizeLogo, Factor, xFactor, yFactor;

  xSizeLogo = GUI_MEMDEV_GetXSize(hLogo);
  ySizeLogo = GUI_MEMDEV_GetYSize(hLogo);
  if ((xSizeLogo < (pRect->x1 - pRect->x0 + 1)) && (ySizeLogo < (pRect->y1 - pRect->y0 + 1))) {
    GUI_MEMDEV_WriteAt(hLogo,
                       pRect->x0 + (pRect->x1 - pRect->x0 + 1 - xSizeLogo) / 2,
                       pRect->y0 + (pRect->y1 - pRect->y0 + 1 - ySizeLogo) / 2);
  } else {
    xFactor = ((pRect->x1 - pRect->x0 + 1) * 1000) / xSizeLogo;
    yFactor = ((pRect->y1 - pRect->y0 + 1) * 1000) / ySizeLogo;
    Factor = (xFactor > yFactor) ? yFactor : xFactor;
    GUI_MEMDEV_RotateHQ(hLogo, hMemWork,
                        (xSize - xSizeLogo) / 2,
                        pRect->y0 + ((pRect->y1 - pRect->y0 + 1) - ySizeLogo) / 2,
                        0, Factor);
  }
}

/*********************************************************************
*
*       _DrawText
*/
static void _DrawText(GUI_RECT * pRect) {
  GUI_SetFont(GUI_FONT_20_ASCII);
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringInRectWrap("Fading between two screens", pRect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
}

/*********************************************************************
*
*       _Paint0
*/
static void _Paint0(int xSize, int ySize, GUI_MEMDEV_Handle hLogo, GUI_MEMDEV_Handle hMemWork) {
  int ySizeText;
  GUI_RECT rLogo, rText;

  //
  // Background
  //
  GUI_SetBkColor(GUI_MAGENTA);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  _DrawFrame(10, 10, xSize - 11, ySize - 11, 5);
  GUI_SetColor(GUI_BLUE);
  GUI_FillRect(15, 15, xSize - 16, ySize - 16);
  //
  // Screen0
  //
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(GUI_FONT_24B_ASCII);
  GUI_SetTextMode(GUI_TM_TRANS);
  ySizeText = GUI_GetFontDistY();
  GUI_DispStringHCenterAt("Screen 0", xSize / 2, (ySize - ySizeText) / 2);
  //
  // Logo
  //
  rLogo.x0 = 25;
  rLogo.y0 = 25;
  rLogo.x1 = xSize - 25 - 1;
  rLogo.y1 = (ySize - ySizeText) / 2 - 10 - 1;
  _DrawLogo(xSize, hLogo, hMemWork, &rLogo);
  //
  // Message
  //
  rText.x0 = 25;
  rText.y0 = (ySize - ySizeText) / 2 + ySizeText + 10;
  rText.x1 = xSize - 25 - 1;
  rText.y1 = ySize - 25;
  _DrawText(&rText);
}

/*********************************************************************
*
*       _Paint1
*/
static void _Paint1(int xSize, int ySize, GUI_MEMDEV_Handle hLogo, GUI_MEMDEV_Handle hMemWork) {
  int ySizeText;
  GUI_RECT rLogo, rText;

  //
  // Background
  //
  GUI_SetBkColor(GUI_CYAN);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  _DrawFrame(10, 10, xSize - 11, ySize - 11, 5);
  GUI_SetColor(GUI_BLUE);
  GUI_FillRect(15, 15, xSize - 16, ySize - 16);
  //
  // Screen1
  //
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(GUI_FONT_24B_ASCII);
  GUI_SetTextMode(GUI_TM_TRANS);
  ySizeText = GUI_GetFontDistY();
  GUI_DispStringHCenterAt("Screen 1", xSize / 2, (ySize - ySizeText) / 2);
  //
  // Logo
  //
  rLogo.x0 = 25;
  rLogo.y0 = (ySize - ySizeText) / 2 + ySizeText + 10;
  rLogo.x1 = xSize - 25 - 1;
  rLogo.y1 = ySize - 25;
  _DrawLogo(xSize, hLogo, hMemWork, &rLogo);
  //
  // Message
  //
  rText.x0 = 25;
  rText.y0 = 25;
  rText.x1 = xSize - 25 - 1;
  rText.y1 = (ySize - ySizeText) / 2 - 10 - 1;
  _DrawText(&rText);
}

/*********************************************************************
*
*       _FadeScreens
*/
static int _FadeScreens(GUI_MEMDEV_Handle hScreen0, GUI_MEMDEV_Handle hScreen1) {
  GUI_MEMDEV_Write(hScreen1);
  //
  // Fade in Title and check if the user wants to cancel the demo
  //
  GUI_MEMDEV_FadeDevices(hScreen0, hScreen1, TIME_FADING);
  if (GUIDEMO_CheckCancel() == 1) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       _FadingDemo
*/
static void _FadingDemo(void) {
  #if GUI_WINSUPPORT
    GUI_MEMDEV_Handle    hMemControl;
  #endif
  GUI_MEMDEV_Handle    ahMem[NUM_SCREENS];
  GUI_MEMDEV_Handle    hLogo;
  void              (* apFunc[])(int, int, GUI_MEMDEV_Handle, GUI_MEMDEV_Handle) = { _Paint0, _Paint1 };
  int                  xSize, ySize, i, j;

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  //
  // Store Control window in MEMDEV
  //
  GUI_Exec();
  #if GUI_WINSUPPORT
    hMemControl = GUI_MEMDEV_CreateEx(xSize - CONTROL_SIZE_X, ySize - CONTROL_SIZE_Y, CONTROL_SIZE_X, CONTROL_SIZE_Y, GUI_MEMDEV_NOTRANS);
    GUI_MEMDEV_CopyFromLCD(hMemControl);
  #endif
  //
  // Create logo device
  //
  hLogo = GUI_MEMDEV_CreateFixed(0, 0, bmMicriumLogo.XSize, bmMicriumLogo.YSize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV);
  GUI_MEMDEV_Select(hLogo);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_DrawBitmap(&bmMicriumLogo, 0, 0);
  //
  // Create screen devices
  //
  for (i = 0; i < NUM_SCREENS; i++) {
    ahMem[i] = GUI_MEMDEV_CreateFixed(0, 0, xSize, ySize, GUI_MEMDEV_HASTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV);
    GUI_MEMDEV_Select(ahMem[i]);
    apFunc[i](xSize, ySize, hLogo, ahMem[i]);
    #if GUI_WINSUPPORT
      GUI_MEMDEV_Write(hMemControl);
    #endif
  }
  GUI_MEMDEV_Select(0);
  //
  // Show fading
  //
  for (j = 0; j < NUM_FADINGS; j++) {
    for (i = 0; i < NUM_SCREENS; i++) {
      if (_FadeScreens(ahMem[i], ahMem[i ^ 1])) {
        j = NUM_FADINGS;
        break;
      }
    }
  }
  //
  // Delete devices
  //
  #if GUI_WINSUPPORT
    GUI_MEMDEV_Delete(hMemControl);
  #endif
  GUI_MEMDEV_Delete(hLogo);
  for (i = 0; i < NUM_SCREENS; i++) {
    if (ahMem[i]) {
      GUI_MEMDEV_Delete(ahMem[i]);
    }
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Fading
*/
void GUIDEMO_Fading(void) {
  GUIDEMO_ConfigureDemo("Fading", "Fading between\ntwo screens", GUIDEMO_SHOW_CURSOR | GUIDEMO_SHOW_CONTROL);
  _FadingDemo();
}

#else

void GUIDEMO_Fading_C(void);
void GUIDEMO_Fading_C(void) {}

#endif  // SHOW_GUIDEMO_FADING && GUI_SUPPORT_MEMDEV

/*************************** End of file ****************************/
