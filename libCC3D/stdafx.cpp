

#include "stdafx.h"

#include <stdio.h>
#include <cstdarg>

#include <map>

#include <emscripten.h>
#include <emscripten/html5.h>

 

// emulation for MFC CString 
CString::CString(LPCTSTR lpsz) 
{ 	s = NULL;
	if (lpsz)
        s = strdup(lpsz);
}

CString::CString(const CString &stringSrc)
{ 	s = NULL;
	if (!stringSrc.IsEmpty())
        s = strdup(stringSrc.s);
}



CString::CString(LPCTSTR lpsz, int count ) 
{ s = NULL;
  if (lpsz)
  {
      s = (char*) malloc ( count + 1 );
      strncpy( s, lpsz, count );
      s[ count ] = 0;
  }
}



void CString::Empty()

{

   if (s)

       free(s);

    s = NULL;

} // free up the data



const CString& CString ::operator=(const CString& stringSrc)
{ Empty();
  if (!stringSrc.IsEmpty())
      s = strdup(stringSrc.s);
  return *this;
}


const CString& CString::operator=(LPCTSTR lpsz)
{ 

	Empty();
	if (lpsz)
          s = strdup(lpsz);
    return *this; 

}

// string concatenation
const CString& CString::operator+=(LPCTSTR lpsz)
{
		char * p;
        int    ls = 0, lp = 0;

        if( !lpsz )
        {
            return *this;
        }
        else
        {
            lp = strlen( lpsz );
        }

        if( s )
        {
            ls = strlen( s );
        }

        p = (char*)malloc( ls + lp + 1 );
        if( p )
        {
            if( ls )
            {
                strncpy( p, s, ls );
            }
            if( lp )
            {
                strncpy( p + ls, lpsz, lp );
            }
            p[ ls + lp ] = 0;
        }

        Empty();

        s = p;

        return *this;
            
}


const CString& CString::operator+=(TCHAR ch)
{
    char p[ 2 ];
    p [ 0 ] = ch;
    p [ 1 ] = 0;
    return ( *this += p );
};


// trimming whitespace (either side)
void CString::TrimRight()

{ char * p;

  if( !s )
      return;

  p = s + strlen( s ) - 1;
  while( p >= s && isspace( *p ))
  {
     *p = 0;
	 p--;
  }
}

void CString::TrimLeft()

{ 

  char * t = s;
  char * p = s;

  if( !s )
      return;

  while( *p && isspace( *p ))
      p++;

  if( t == p )
      return;

  while( *p )
      *t++ = *p++;

  *t = 0;
}





// look for a specific sub-string
int  CString::Find(LPCTSTR lpszSub) const        // like "C" strstr
{
    char * p;

    if( !s )
    {
        return( -1 );
    }

    p = strstr( s, lpszSub );
    if( p )
    {
        return( p - s );
    }
    return( -1 );
}


int CString::Find(TCHAR ch) const                // like "C" strchr
{
    char * p;

    if( !s )
    {
        return( -1 );
    }

    p = strchr( s, ch );
    if( p )
    {
        return( p - s );
    }
    return( -1 );
}
int CString::ReverseFind(TCHAR ch) const                // like "C" strchr
{
    char * p;

    if( !s )
    {
        return( -1 );
    }

    p = strrchr( s, ch );
    if( p )
    {
        return( p - s );
    }
    return( -1 );
}



CString operator+(const CString& string1,
		          const CString& string2)
{
    CString s(string1);
    if (!string2.IsEmpty()) s += string2.s;
    return( s );
}

CString operator+(const CString& string1, LPCTSTR string2)
{
    CString s(string1);
    if (string2) s += string2;
    return( s );
}

CString operator+(LPCTSTR string1, const CString& string2)
{
    CString s(string1);
    if (!string2.IsEmpty()) s += string2;
    return( s );
}

bool  operator==(const CString& string, LPCTSTR lpsz)
{ 
    return string.s== lpsz || (strcmp(string.s,lpsz)==0); 
}


bool  operator==(const CString& string1,const CString& string2)
{ 
    return string1.s== string2.s || (strcmp(string1.s,string2.s)==0); 
}

void CString::Format(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buf[255];
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    *this += buf;
}

void Translate(const char * text, CString& cstr) {
	cstr = text;
}


void CCmdUI::Enable(BOOL enable) {
    printf("CCmdUI::Enable not implemented yet!\n");
}

void CCmdUI::SetCheck(int check) {
    printf("CCmdUI::SetCheck not implemented yet!\n");   
}

void CCmdUI::SetRadio(BOOL radio) {
    printf("CCmdUI::SetRadio not implemented yet!\n");   
}

void CCmdUI::SetText(LPCTSTR text) {
    printf("CCmdUI::SetText not implemented yet!\n");   
}

HCURSOR SetCursor(HCURSOR cursor) {
    return (HCURSOR)EM_ASM_INT({
        let old = Module.cursorPtr ?? 0;
        Module.cursorPtr = $0;
        Module.canvas.style.cursor = UTF8ToString($0);
        return old;
    }, cursor);
}

HCURSOR GetCursor() {
    return (HCURSOR)EM_ASM_INT({
        return Module.cursorPtr ?? 0;
    });
}

void BeginWaitCursor() {
    EM_ASM({
        Module.canvas.style.cursor = 'wait';
    });
}

void EndWaitCursor() {
    EM_ASM({
        Module.canvas.style.cursor = Module.cursorPtr ? UTF8ToString(Module.cursorPtr) : 'default';
    });
}

void SetCapture() {}
void ReleaseCapture() {}

BOOL GetFocus() {
    return EM_ASM_INT({
        return document.activeElement === Module.canvas ? 1 : 0;
    });
}

static int TIMERS[] = {0, 0, 0, 0, 0};

uint32_t SetTimer(uint32_t *id, uint32_t elapse, void (*timerfunc)(void *userData), void *userData) {
    // if(elapse < 0x0A) {
    //     elapse = 0x0A;
    // }
    if(*id >= sizeof(TIMERS)) {
        printf("Timer ID exceeded maximum!\n");
        return 0;
    }
    if(TIMERS[*id]) {
        emscripten_clear_interval(TIMERS[*id]);
    }
    TIMERS[*id] = emscripten_set_interval(timerfunc, elapse, userData);
    return *id;
}

BOOL KillTimer(uint32_t id) {
    emscripten_clear_interval(TIMERS[id]);
    TIMERS[id] = 0;
    return 1;
}

BOOL GetClientRect(RECT *rect) {
	rect->left = 0;
	rect->top = 0;
	EM_ASM({
		setValue($0, Module.canvas.clientHeight, 'i32');
		setValue($1, Module.canvas.clientWidth, 'i32');
	}, &rect->bottom, &rect->right);
    return 1;
}


static POINT cursor_pos_internal;
void SetCursorPosInternal(LONG x, LONG y) {
    cursor_pos_internal.x = x;
    cursor_pos_internal.y = y;
}

BOOL GetCursorPos(POINT *point) {
    point->x = cursor_pos_internal.x;
    point->y = cursor_pos_internal.y;
    return 1;
}

static std::map<int, SHORT> key_states_internal;
void SetKeyStateInternal(int key, SHORT state) {
    key_states_internal[key] = state;
}

SHORT GetKeyState(int key) {
    return key_states_internal[key];
}

static SHORT ctrl_state_internal;
static SHORT shift_state_internal;
static SHORT alt_state_internal;

EM_BOOL keyboard_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    ctrl_state_internal = keyEvent->ctrlKey ? 0x8000 : 0;
    shift_state_internal = keyEvent->shiftKey ? 0x8000 : 0;
    alt_state_internal = keyEvent->altKey ? 0x8000 : 0;
    return false;
}

void InitModifierListeners() {
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, false, keyboard_callback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, false, keyboard_callback);
}

SHORT GetCtrlState() {
    return ctrl_state_internal;
}
SHORT GetShiftState() {
    return shift_state_internal;
}
SHORT GetAltState() {
    return alt_state_internal;
}