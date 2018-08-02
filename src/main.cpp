

#include "GUIslice.h"
#include "GUIslice_ex.h"
#include "GUIslice_drv.h"

#include <Adafruit_GFX.h>


#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/TomThumb.h"



// Enumerations for pages, elements, fonts, images
enum {E_PG_MAIN,E_PG_KEYBOARD};
enum {E_ELEM_BOX_A,E_ELEM_BOX_B,E_ELEM_BTN_QUIT,E_ELEM_BTN_BTN,E_ELEM_TXT_COUNT};
enum {E_FONT_BTN,E_FONT_TXT};
enum {E_GROUP1};

bool        m_bQuit = false;

// Free-running counter for display
unsigned    m_nCount = 0;


// Instantiate the GUI
#define MAX_PAGE                2
#define MAX_FONT                2

// Define the maximum number of elements per page
#define MAX_ELEM_PG_MAIN          16                                        // # Elems total
#define MAX_ELEM_PG_MAIN_RAM      MAX_ELEM_PG_MAIN                          // # Elems in RAM

#define MAX_ELEM_PG_KEYBOARD          16                                        // # Elems total
#define MAX_ELEM_PG_KEYBOARD_RAM      MAX_ELEM_PG_MAIN

gslc_tsGui                  m_gui;
gslc_tsDriver               m_drv;
gslc_tsFont                 m_asFont[MAX_FONT];
gslc_tsPage                 m_asPage[MAX_PAGE];
gslc_tsElem                 m_asPageElem[MAX_ELEM_PG_MAIN_RAM];
gslc_tsElemRef              m_asPageElemRef[MAX_ELEM_PG_MAIN];

gslc_tsElem                 m_asPageElemKeyboard[MAX_ELEM_PG_KEYBOARD_RAM];
gslc_tsElemRef              m_asPageElemKeyboardRef[MAX_ELEM_PG_KEYBOARD];

// gslc_tsXGauge               m_sXGauge,m_sXGauge1;
// gslc_tsXCheckbox            m_asXCheck[3];
// gslc_tsXSlider              m_sXSlider;


#define MAX_STR             15

  // Save some element references for quick access
  gslc_tsElemRef*  m_pElemCnt        = NULL;
  gslc_tsElemRef*  m_pElemProgress   = NULL;
  gslc_tsElemRef*  m_pElemProgress1  = NULL;
  gslc_tsElemRef*  m_pElemSlider     = NULL;
  gslc_tsElemRef*  m_pElemSliderTxt  = NULL;

// Define debug message function
static int16_t DebugOut(char ch) { Serial.write(ch); return 0; }

// Button callbacks




//PAGINA PRINCIPAL

struct Channel{
  float voltage,current,voltageSet,currentSet;
  bool status = false;
  gslc_tsElemRef* txtVoltage=NULL;
  gslc_tsElemRef* txtCurrent=NULL;
  gslc_tsElemRef* txtVoltageSet=NULL;
  gslc_tsElemRef* txtCurrentSet=NULL;
  gslc_tsElemRef* txtPotenci=NULL;
  gslc_tsElemRef* btnOn=NULL;
}Ch_A,Ch_B;

bool CbBtnQuit(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  if (eTouch == GSLC_TOUCH_UP_IN) {
    // m_bQuit = true;
    gslc_SetPageCur(&m_gui,E_PG_KEYBOARD);
  }
  return true;
}

bool CbBtnOn(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY){
  if (eTouch == GSLC_TOUCH_UP_IN) {
    Ch_A.status = !Ch_A.status;
    // char str[3];
    if(Ch_A.status){
      gslc_ElemSetTxtStr(&m_gui,Ch_A.btnOn,(char*)"ON");
    }else{
      gslc_ElemSetTxtStr(&m_gui,Ch_A.btnOn,(char*)"OFF");
    }

  }
  return true;
}

// E_ELEM_TXT_VOLT_SET,E_ELEM_TXT_CURRENT_SET

bool initPgMain(){
  gslc_tsElemRef* pElemRef;
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPageElem,MAX_ELEM_PG_MAIN_RAM,m_asPageElemRef,MAX_ELEM_PG_MAIN);

  // gslc_ElemCreateBox_P(&m_gui,200,E_PG_MAIN,10,10,140,150,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,true,true,NULL,NULL);
  gslc_ElemCreateBox_P(&m_gui,201,E_PG_MAIN,155,0,10,170,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,true,true,NULL,NULL);
  gslc_ElemCreateBox_P(&m_gui,202,E_PG_MAIN,0,170,320,70,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,true,true,NULL,NULL);

  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,(gslc_tsRect){160,80,80,40},(char*)"Quit",0,E_FONT_BTN,&CbBtnQuit);
  // Create counter
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,60,50,10},
    (char*)"Count:",0,E_FONT_TXT);
  static char mstr1[8] = "";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TXT_COUNT,E_PG_MAIN,(gslc_tsRect){80,60,50,10},
    mstr1,sizeof(mstr1),E_FONT_TXT);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  m_pElemCnt = pElemRef; // Save for quick access

  static char strVoltSet[8] = "25.00V";
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){10,20,70,15},strVoltSet,sizeof(strVoltSet),E_FONT_BTN);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  Ch_A.txtVoltageSet = pElemRef;

  static char strCurrSet[8] = "00.00A";
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){80,20,70,15},strCurrSet,sizeof(strCurrSet),E_FONT_BTN);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  Ch_A.txtCurrentSet = pElemRef;

  static char strVolt[8] = "00.00V";
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,50,100,20},strVolt,sizeof(strVolt),E_FONT_BTN);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  Ch_A.txtVoltage = pElemRef;

  static char strCurr[8] = "00.00A";
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,80,100,20},strCurr,sizeof(strCurr),E_FONT_BTN);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  Ch_A.txtCurrent = pElemRef;

  static char strBtnOn[8] = "ON";
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){10,120,70,50},strBtnOn,sizeof(strBtnOn),E_FONT_BTN,&CbBtnOn);
  // gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  Ch_A.btnOn = pElemRef;



// pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,(gslc_tsRect){160,80,80,40},(char*)"Quit",0,E_FONT_BTN,&CbBtnQuit);
  return true;
}


// PAGINA E_PG_KEYBOARD

bool CbBtnBTN(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  if (eTouch == GSLC_TOUCH_UP_IN) {
    gslc_SetPageCur(&m_gui,E_PG_MAIN);
  }
  return true;
}

bool initPgKeyboard(){
  gslc_tsElemRef* pElemRef;
  gslc_PageAdd(&m_gui,E_PG_KEYBOARD,m_asPageElemKeyboard,MAX_ELEM_PG_KEYBOARD_RAM,m_asPageElemKeyboardRef,MAX_ELEM_PG_KEYBOARD);
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_BTN,E_PG_KEYBOARD,(gslc_tsRect){160,80,80,40},(char*)"BTN",0,E_FONT_BTN,&CbBtnBTN);
  return true;
}

// Create page elements
bool InitOverlays()
{
  gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY_DK2);
  initPgMain();
  initPgKeyboard();
  return true;
}


void setup()
{
  Serial.begin(9600);
  gslc_InitDebug(&DebugOut);

  // Initialize
  if (!gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT)) { return; }


  if (!gslc_FontAdd(&m_gui,E_FONT_BTN,GSLC_FONTREF_PTR,&TomThumb,1)) { return; }

  if (!gslc_FontAdd(&m_gui,E_FONT_TXT,GSLC_FONTREF_PTR,NULL,1)) { return; }

  // Create graphic elements
  InitOverlays();

  // Start up display on main page
  gslc_SetPageCur(&m_gui,E_PG_MAIN);

  m_bQuit = false;
}

void loop()
{
  char                acTxt[MAX_STR];

  // General counter
  m_nCount++;

  // Update elements on active page

  snprintf(acTxt,MAX_STR,"%u",m_nCount/5);
  gslc_ElemSetTxtStr(&m_gui,m_pElemCnt,acTxt);



  gslc_ElemSetTxtStr(&m_gui,m_pElemSliderTxt,acTxt);



  // Periodically call GUIslice update function
  gslc_Update(&m_gui);

  // Slow down updates
  delay(10);

  // In a real program, we would detect the button press and take an action.
  // For this Arduino demo, we will pretend to exit by emulating it with an
  // infinite loop. Note that interrupts are not disabled so that any debug
  // messages via Serial have an opportunity to be transmitted.
  // if (m_bQuit) {
  //   // gslc_Quit(&m_gui);
  //   // while (1) { }
  // }
}
