/**
 * @Author: Anderson Juncowski <anderson>
 * @Date:   2018-08-02T20:01:42-03:00
 * @Email:  andersonj@hotmail.rs
 * @Last modified by:   anderson
 * @Last modified time: 2018-08-02T20:03:48-03:00
 */

#include "GUIslice.h"
#include "GUIslice_ex.h"
#include "GUIslice_drv.h"
#include <string.h>

#include <Adafruit_GFX.h>


#include "Fonts/FreeSansBold9pt7b.h"
#include "Fonts/FreeSansBold18pt7b.h"



// Enumerations for pages, elements, fonts, images
enum {E_PG_MAIN,E_PG_KEYBOARD};
enum {/*PG_KEYBOARD*/
      E_ELEM_BOX_A,E_ELEM_BOX_B,E_ELEM_BTN_QUIT,E_ELEM_BTN_BTN,E_ELEM_TXT_COUNT
      ,E_ELEM_BTN_1,E_ELEM_BTN_2,E_ELEM_BTN_3,E_ELEM_BTN_PREV,E_ELEM_BTN_NEXT
      ,E_ELEM_BTN_4,E_ELEM_BTN_5,E_ELEM_BTN_6,E_ELEM_BTN_CANCEL,E_ELEM_BTN_OK
      ,E_ELEM_BTN_7,E_ELEM_BTN_8,E_ELEM_BTN_9,E_ELEM_BTN_0,E_ELEM_BTN_SEPARATOR
      /*PG_MAIN*/
      ,E_ELEM_BTN_CHAV,E_ELEM_BTN_CHAC,E_ELEM_BTN_CHASTATUS
};
enum {E_FONT_BIG,E_FONT_NORMAL,E_FONT_TXT};
enum {E_GROUP1};

bool        m_bQuit = false;

// Free-running counter for display
unsigned    m_nCount = 0;


// Instantiate the GUI
#define MAX_PAGE                2
#define MAX_FONT                3

// Define the maximum number of elements per page
#define MAX_ELEM_PG_MAIN          16                                        // # Elems total
#define MAX_ELEM_PG_MAIN_RAM      MAX_ELEM_PG_MAIN                          // # Elems in RAM

#define MAX_ELEM_PG_KEYBOARD          22                                        // # Elems total
#define MAX_ELEM_PG_KEYBOARD_RAM      MAX_ELEM_PG_KEYBOARD

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
  // gslc_tsElemRef*  m_pElemCnt        = NULL;
  gslc_tsElemRef*  m_pElemProgress   = NULL;
  gslc_tsElemRef*  m_pElemProgress1  = NULL;
  gslc_tsElemRef*  m_pElemSlider     = NULL;
  gslc_tsElemRef*  m_pElemSliderTxt  = NULL;

// Define debug message function
static int16_t DebugOut(char ch) { Serial.write(ch); return 0; }



#include "Channel.h"


Channel Ch_A('A');
Channel Ch_B('B');



// PAGINA E_PG_KEYBOARD

gslc_tsElemRef* elemRefValue = NULL;
uint8_t valueCursor = 0;
char valueStr[7] = "";
char channelStr[5]="Ch ?";
// char unityStr[5]="";
Channel::Type type;
Channel* chRef=NULL;



void cursorNext(){
  if(valueCursor<4){
    valueCursor++;
  }
  if(valueCursor==2)valueCursor++;
}

void cursorPrev(){
  if(valueCursor>0){
    valueCursor--;
  }
  if(valueCursor==2)valueCursor--;
}


void setCursor(uint8_t cur){
  valueCursor = cur;
}

void setValue(char val){
  valueStr[valueCursor] = val;
  cursorNext();
  gslc_ElemSetRedraw(&m_gui, elemRefValue, GSLC_REDRAW_INC);
}


void saveValue(){
  chRef->set(type,valueStr);
}



void loadKeyboard(Channel* ch, Channel::Type t){

  // valueStr=""
  setCursor(0);
  chRef = ch;
  type = t;
  channelStr[3]=ch->name;
  switch(t){
    case Channel::VOLTAGE:
      // strcpy(unityStr, "Volt");
      strcpy(valueStr,ch->voltageSetStr);
    break;
    case Channel::CURRENT:
      // strcpy(unityStr, "Amp");
      strcpy(valueStr,ch->currentSetStr);
    break;
  }
  gslc_SetPageCur(&m_gui,E_PG_KEYBOARD);
}

bool onClick_Keyboard(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElem);
  gslc_tsElem* pElem = pElemRef->pElem;
  int16_t nElemId = pElem->nId;
  if (eTouch == GSLC_TOUCH_UP_IN) {
    if (nElemId == E_ELEM_BTN_0) {
      setValue('0');
    } else if (nElemId == E_ELEM_BTN_1) {
      setValue('1');
    } else if (nElemId == E_ELEM_BTN_2) {
      setValue('2');
    }  else if (nElemId == E_ELEM_BTN_3) {
      setValue('3');
    }  else if (nElemId == E_ELEM_BTN_4) {
      setValue('4');
    }  else if (nElemId == E_ELEM_BTN_5) {
      setValue('5');
    }  else if (nElemId == E_ELEM_BTN_6) {
      setValue('6');
    }  else if (nElemId == E_ELEM_BTN_7) {
      setValue('7');
    }  else if (nElemId == E_ELEM_BTN_8) {
      setValue('8');
    }  else if (nElemId == E_ELEM_BTN_9) {
      setValue('9');
    } else if (nElemId == E_ELEM_BTN_NEXT) {
      cursorNext();
    }  else if (nElemId == E_ELEM_BTN_PREV) {
      cursorPrev();
    }  else if (nElemId == E_ELEM_BTN_OK) {
      saveValue();
      gslc_SetPageCur(&m_gui,E_PG_MAIN);
    }  else if (nElemId == E_ELEM_BTN_CANCEL) {
      gslc_SetPageCur(&m_gui,E_PG_MAIN);
    }  else if (nElemId == E_ELEM_BTN_SEPARATOR) {
      setCursor(3);
    }

  }
  return true;
}

void addKeyboardBtn(char* txt,uint16_t elementId){
  static const uint16_t btnW = m_gui.nDispW/5;
  static const uint16_t btnH = m_gui.nDispH/5;
  static int lin=2,col=-1;
  col++;
  if(col>=5){
    col=0;
    lin++;
  }
  gslc_ElemCreateBtnTxt(&m_gui,elementId,E_PG_KEYBOARD,(gslc_tsRect){col*btnW,lin*btnH,btnW,btnH},txt,0,E_FONT_BIG,&onClick_Keyboard);
}

bool initPgKeyboard(){
  gslc_tsElemRef* pElemRef;
  gslc_PageAdd(&m_gui,E_PG_KEYBOARD,m_asPageElemKeyboard,MAX_ELEM_PG_KEYBOARD_RAM,m_asPageElemKeyboardRef,MAX_ELEM_PG_KEYBOARD);
  // pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_BTN,E_PG_KEYBOARD,(gslc_tsRect){160,80,80,40},(char*)"BTN",0,E_FONT_BIG,&CbBtnBTN);

  addKeyboardBtn((char*)"1",E_ELEM_BTN_1);
  addKeyboardBtn((char*)"2",E_ELEM_BTN_2);
  addKeyboardBtn((char*)"3",E_ELEM_BTN_3);
  addKeyboardBtn((char*)"<",E_ELEM_BTN_PREV);
  addKeyboardBtn((char*)">",E_ELEM_BTN_NEXT);
  addKeyboardBtn((char*)"4",E_ELEM_BTN_4);
  addKeyboardBtn((char*)"5",E_ELEM_BTN_5);
  addKeyboardBtn((char*)"6",E_ELEM_BTN_6);
  addKeyboardBtn((char*)"X",E_ELEM_BTN_CANCEL);
  addKeyboardBtn((char*)"OK",E_ELEM_BTN_OK);
  addKeyboardBtn((char*)"7",E_ELEM_BTN_7);
  addKeyboardBtn((char*)"8",E_ELEM_BTN_8);
  addKeyboardBtn((char*)"9",E_ELEM_BTN_9);
  addKeyboardBtn((char*)"0",E_ELEM_BTN_0);
  addKeyboardBtn((char*)".",E_ELEM_BTN_SEPARATOR);

  // pElemRef = gslc_ElemCreateTxt(&m_gui, GSLC_ID_AUTO, E_PG_KEYBOARD, (gslc_tsRect){m_gui.nDispW/2+50,m_gui.nDispH/5-16,80,32},(char*)"Ampere",0,E_FONT_NORMAL);

  pElemRef = gslc_ElemCreateTxt(&m_gui, GSLC_ID_AUTO, E_PG_KEYBOARD, (gslc_tsRect){10,m_gui.nDispH/5-16,80,32},channelStr,0,E_FONT_BIG);
  pElemRef = gslc_ElemCreateTxt(&m_gui, GSLC_ID_AUTO, E_PG_KEYBOARD, (gslc_tsRect){m_gui.nDispW/2-50,m_gui.nDispH/5-16,120,32},valueStr,0,E_FONT_BIG);
  gslc_ElemSetTxtAlign(&m_gui, pElemRef, GSLC_ALIGN_MID_MID);
  elemRefValue=pElemRef;
  // pElemRef = gslc_ElemCreateTxt(&m_gui, GSLC_ID_AUTO, E_PG_KEYBOARD, (gslc_tsRect){m_gui.nDispW/2+60,m_gui.nDispH/5-16,80,32},unityStr,0,E_FONT_BIG);
  // pElemRef = gslc_ElemCreateTxt(&m_gui, GSLC_ID_AUTO, E_PG_KEYBOARD, (gslc_tsRect){m_gui.nDispW/2-100,m_gui.nDispH/5-16,200,32},(char*)"00.00",0,E_FONT_BIG);



  return true;
}


//PAGINA PRINCIPAL

bool onClick_Main(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY){
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElem);
  gslc_tsElem* pElem = pElemRef->pElem;
  int16_t nElemId = pElem->nId;
  if (eTouch == GSLC_TOUCH_UP_IN) {
    if (nElemId == E_ELEM_BTN_CHAV) {
      loadKeyboard(&Ch_A,Channel::VOLTAGE);
    } else if (nElemId == E_ELEM_BTN_CHAC) {
      loadKeyboard(&Ch_A,Channel::CURRENT);
    } else if (nElemId == E_ELEM_BTN_CHASTATUS) {
      Ch_A.toogleStatus();
    }
  }
  return true;
}

// E_ELEM_TXT_VOLT_SET,E_ELEM_TXT_CURRENT_SET

bool initPgMain(){
  gslc_tsElemRef* pElemRef;
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPageElem,MAX_ELEM_PG_MAIN_RAM,m_asPageElemRef,MAX_ELEM_PG_MAIN);

  // gslc_ElemCreateBox_P(&m_gui,200,E_PG_MAIN,10,10,140,150,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,true,true,NULL,NULL);
  gslc_ElemCreateBox_P(&m_gui,201,E_PG_MAIN,155,0,10,180,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,true,true,NULL,NULL);
  gslc_ElemCreateBox_P(&m_gui,202,E_PG_MAIN,0,180,320,60,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,true,true,NULL,NULL);
  //
  // pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,(gslc_tsRect){160,80,80,40},(char*)"Quit",0,E_FONT_BIG,&CbBtnQuit);
  // Create counter
  // pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,60,50,10},
  //   (char*)"Count:",0,E_FONT_TXT);
  // static char mstr1[8] = "";
  // pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TXT_COUNT,E_PG_MAIN,(gslc_tsRect){80,60,50,10},
  //   mstr1,sizeof(mstr1),E_FONT_TXT);
  // gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_YELLOW);
  // m_pElemCnt = pElemRef; // Save for quick access

  // static char strVoltSet[8] = "25.00V";
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_CHAV,E_PG_MAIN,(gslc_tsRect){5,5,70,32},Ch_A.voltageSetStr,sizeof(Ch_A.voltageSetStr),E_FONT_NORMAL,&onClick_Main);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  // Ch_A.txtVoltageSet = pElemRef;

  // static char strCurrSet[8] = "00.00A";

  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_CHAC,E_PG_MAIN,(gslc_tsRect){80,5,70,32},Ch_A.currentSetStr,sizeof(Ch_A.currentSetStr),E_FONT_NORMAL,&onClick_Main);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  // Ch_A.txtCurrentSet = pElemRef;

  // static char strVolt[8] = "00.00V";
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,50,100,20},Ch_A.voltageStr,sizeof(Ch_A.voltageStr),E_FONT_BIG);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  // Ch_A.txtVoltage = pElemRef;

  // static char strCurr[8] = "00.00A";
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,80,100,20},Ch_A.currentStr,sizeof(Ch_A.currentStr),E_FONT_BIG);

  // static char strBtnOn[8] = "ON";
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_CHASTATUS,E_PG_MAIN,(gslc_tsRect){5,120,80,50},Ch_A.statusStr,sizeof(Ch_A.statusStr),E_FONT_BIG,&onClick_Main);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  // Ch_A.btnOn = pElemRef;



// pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,(gslc_tsRect){160,80,80,40},(char*)"Quit",0,E_FONT_BTN,&CbBtnQuit);
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

  if (!gslc_FontAdd(&m_gui,E_FONT_BIG,GSLC_FONTREF_PTR,&FreeSansBold18pt7b,1)) { return; }
  if (!gslc_FontAdd(&m_gui,E_FONT_NORMAL,GSLC_FONTREF_PTR,&FreeSansBold9pt7b,1)) { return; }

  if (!gslc_FontAdd(&m_gui,E_FONT_TXT,GSLC_FONTREF_PTR,NULL,1)) { return; }

  // Create graphic elements
  InitOverlays();

  // Start up display on main page
  gslc_SetPageCur(&m_gui,E_PG_MAIN);

  m_bQuit = false;
}

void loop()
{
  // char                acTxt[MAX_STR];

  // General counter
  // m_nCount++;

  // Update elements on active page

  // snprintf(acTxt,MAX_STR,"%u",m_nCount/5);
  // gslc_ElemSetTxtStr(&m_gui,m_pElemCnt,acTxt);



  // gslc_ElemSetTxtStr(&m_gui,m_pElemSliderTxt,acTxt);



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
