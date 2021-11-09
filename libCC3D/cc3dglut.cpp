/*
 * CC3D minimal demo
 * using GLUT
 *
 * started from glutskel.c
 * A skeleton/template GLUT program - Written by Brian Paul and in the public domain.
 */

#include <gl4esinit.h>

// CC3D global headers
#include "stdafx.h"
#include "greporter.h"
#include "gversion.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// GL / Mesa includes

//#include <GL/gl.h>
#include <GL/glut.h>
#include <emscripten.h>
#include <emscripten/bind.h>


//#include <stdio.h>
#include <stdlib.h>

// CC3D includes

#include "glrender.h"
#include "gmodel.h"
#include "gvdb.h"

#include "blaxxuncc3d.h"
#include "blaxxuncc3dctl.h"


// the CC3D toplevel view object
static CGLViewCtrlCtrl *ctl=NULL;


static GLfloat Xrot = 0, Yrot = 0, Zrot = 0;
static GLboolean Anim = GL_FALSE;
static BOOL mouse_left;
static BOOL mouse_middle;
static BOOL mouse_right;



// idle function
// static void OnIdle( void )
// {

// 	// check for a camera animation
//     if (view->AnimatingCamera()) {
//         if (view->AnimateCameraStep()==2) {
//         };
// 		view->setRedraw();
//     }
//     view->TriggerTimeSensors(); // update browser time stamp and trigger timesensors
	
//     if (view->getRedraw()) // we should redraw
//        glutPostRedisplay();
// }

// static void Idle( void )
// {
//    Xrot += 0.0;
//    Yrot += 2.0;
//    Zrot += 2.0;

//    GCamera &camera = view->GetCamera();
//    camera.Orbit(Yrot*0.01,Yrot*0.01);
//    view->setRedraw();

//    OnIdle();
// }


// repaint
static void OnPaint( void )
{
   if (ctl) {
	ctl->OnPaint();
   }	

   glutSwapBuffers();
}

static void OnIdle( void ) {
   if(ctl) {
      ctl->OnIdle(1);
   }
}


static void OnSize( int width, int height )
{
   if (ctl)
       ctl->OnSize(0, width, height);
}

static float rotateStep = 0.2;
static float walkStep = 0.2;

// static void OnCameraSeekto(int x, int y) 
// {
//    if (view->Select(x,y,0,GView::JUMP_OBJECT)) {
// 		//Message("Jumping to object");
// 		//Redraw();
//    }
// }

// static void OnMouseDblClick(int x, int y,int nFlags)
// {
//     if (view->Select(x,y,nFlags,GView::JUMP_WWWAnchor)) {
//  	  // camera selection is already done	at this stage 
// 	  if (view->executeNode) {
// 			//ExecuteAnchorNode((GvNode*)view->executeNode);
// 	  }
// 	}
// }



static void OnKey( unsigned char key, int x, int y )
{
   SetKeyStateInternal(key, 0x8000);
   if (!ctl) return;
   ctl->OnKeyDown(key, 0, 0);
   //glutPostRedisplay();
}

static void OnKeyUp(unsigned char key, int x, int y) {
   SetKeyStateInternal(key, 0);
   if(!ctl) return;
   ctl->OnKeyUp(key, 0, 0);
}


static void OnSpecialKey( int key, int x, int y )
{
   SetKeyStateInternal(key<<8, 0x8000);
   if (!ctl) return;
   
   int modifiers =  glutGetModifiers();
   //GLUT_ACTIVE_SHIFT GLUT_ACTIVE_CTRL GLUT_ACTIVE_ALT
  
   ctl->OnKeyDown(key<<8, 0, 0);
   //glutPostRedisplay();
}

static void OnSpecialKeyUp( int key, int x, int y )
{
   SetKeyStateInternal(key<<8, 0);
   if (!ctl) return;
   
   int modifiers =  glutGetModifiers();
   //GLUT_ACTIVE_SHIFT GLUT_ACTIVE_CTRL GLUT_ACTIVE_ALT
  
   ctl->OnKeyUp(key<<8, 0, 0);
   //glutPostRedisplay();
}

static void OnMouse(int button, int state, int x, int y )
{
   SetCursorPosInternal(x, y);
 
   if (!ctl) return;
   
   int modifiers =  glutGetModifiers();
   //GLUT_ACTIVE_SHIFT GLUT_ACTIVE_CTRL GLUT_ACTIVE_ALT
   UINT flags = 0;
   if(modifiers & GLUT_ACTIVE_SHIFT) flags |= MK_SHIFT;
   if(modifiers & GLUT_ACTIVE_CTRL) flags |= MK_CONTROL;
   CPoint point;
   point.x = x;
   point.y = y;
   
   if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      mouse_left = TRUE;
      ctl->OnLButtonDown(flags, point);
   } else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      mouse_left = FALSE;
      ctl->OnLButtonUp(flags, point);
   } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      mouse_right = TRUE;
      ctl->OnRButtonDown(flags, point);
   } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
      mouse_right = FALSE;
      ctl->OnRButtonUp(flags, point);
   } else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
      mouse_middle = TRUE;
      ctl->OnMButtonDown(flags, point);
   } else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
      mouse_middle = FALSE;
      ctl->OnMButtonUp(flags, point);
   } else {
      printf("Unknown OnMouse(%i,%i,%i,%i)\n", button, state, x, y);
   }
   
}

static void OnMotion(int x, int y )
{
   SetCursorPosInternal(x, y);
   if(!ctl) return;
   UINT flags = 0;
   CPoint point;
   point.x = x;
   point.y = y;
   int modifiers =  glutGetModifiers();
   if(modifiers & GLUT_ACTIVE_SHIFT) flags |= MK_SHIFT;
   if(modifiers & GLUT_ACTIVE_CTRL) flags |= MK_CONTROL;
   if(mouse_left) flags |= MK_LBUTTON;
   if(mouse_middle) flags |= MK_MBUTTON;
   if(mouse_right) flags |= MK_RBUTTON;
   ctl->OnMouseMove(flags, point);
}

static void OnVisibility(int state)
{
	printf("ON Visibility state = %d \n",state);
}


// load a new world, for now only local files are supported in GFile for Linux

// static int LoadWorld(const char *url, const char* homeUrl = NULL)
// {
// 	int ret;
// 	GFile *loader;
	
// 	loader = new GFile();
// 	if (!loader) return -1;
	
// 	loader->ref();
	
// 	if (homeUrl) {
// 		loader->SetHome(homeUrl);
// 		loader->SetRefedFrom(homeUrl,"Load");
// 	}
   	
// 	loader->Set(url,0,0);

//    	// process url 
//    	ret = loader->Process();
	
// 	// we should have some local file name 	
//     	if (ret >=0) {
// 		if (loader->localFileUnzipped.GetLength()>0) {
// 			ret = view->ReadModel(loader);
// 			if (ret>=0) {
// 				view->setRedraw();
// 			}				
// 		}	
// 	}
// 	loader->unref();

// 	return ret;
// }


const char *helptext = 
"a tiny VRML browser\n"
"by holger@blaxxun.com\n"
"Keyboard controls: \n"
"\ta s/ d f - look left-right / up-down\n"
"\t * / walk speed \n"
"\t w z/ e r / c v - walk in-out / left-right / up-down\n"
"\t Page Up / Down / Home - viewpoints\n"
"\t q <ESC> - exit\n"
;


void help(const char *progname=_PROGRAM)
{
	printf("usage : %s someVrmlFile.wrl\n",progname);

	printf(helptext);

}


int main( int argc, char *argv[] )
{
   int argi=1;	
   int ret=0;

   // init reporter object (needed !)

   int width, height;
   EM_ASM({
      if(getComputedStyle(Module.canvas).padding !== '0px') {
         console.warn('Padding non-0:', getComputedStyle(Module.canvas).padding);
      }
      setValue($0, Module.canvas.clientWidth, 'i32');
      setValue($1, Module.canvas.clientHeight, 'i32');
   }, &width, &height);
   printf("Emscripten screen size: %ix%i\n", width, height);


   initialize_gl4es();

   CGLViewCtrlApp app;
   app.InitInstance();

   // create a view	
   ctl = new CGLViewCtrlCtrl();
   if (!ctl) return(-1);
   GFile::hGlobalPostMsgWnd = ctl;
   //view->SetReporter(reporter);

   InitModifierListeners();

   // glut init stuff

   
   glutInit( &argc, argv );
   glutInitWindowPosition( 0, 0 );
   glutInitWindowSize( width, height );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutCreateWindow(argv[0]);
   glutReshapeFunc( OnSize );
   glutKeyboardFunc( OnKey );
   glutKeyboardUpFunc( OnKeyUp );
   glutSpecialFunc( OnSpecialKey );
   glutSpecialUpFunc( OnSpecialKeyUp );
   glutDisplayFunc( OnPaint );
   
   // more 
   glutMouseFunc( OnMouse );
   glutMotionFunc( OnMotion );
   glutPassiveMotionFunc( OnMotion );
   
   ctl->DoPropExchange();
   // glutEntryFunc
   // glutMenuStateFunc
   //glutVisibilityFunc( OnVisibility );
   // joystick etc
   
   // init view	
   //ctl->Initialize(NULL); //OnPaint will auto initialize, including initial URL

   ret=0;
   
   if (argc==1 || (argi<argc && streq(argv[argi],"-h"))) {
   	   argi++;
	   help(argv[0]);
   }

   // file argument ????????????�
 
   // if (argi<argc) {
	//  ret=LoadWorld(argv[argi]);
	//  argi++;
   // }	  
   // else {  
	// //ret=LoadWorld("/home/holger/x/0cache/www.blaxxun.com/vrml/avatars/boygirl/boygirl.wrl");
	// //ret=LoadWorld("/home/holger/x/0cache/catilaporte.com/vrml/TalentShow/catizap.wrl");
   // }
   
   if (ret<0)  {
	//reporter->Error("Can not read input file");
	exit(-1);
   }		                        
   
   // the idle function, sampling timeSensors & checking the redraw flag
   glutIdleFunc(OnIdle);

   glutMainLoop();

   // cleanup	
   if (ctl) {
	//delete ctl;
	ctl = NULL;
   }	
   GvDB::term();

   return 0;
}

CGLViewCtrlCtrl *Browser() {
   return ctl;
}

EMSCRIPTEN_BINDINGS(bindings_top) {
   emscripten::function("Browser", &Browser, emscripten::allow_raw_pointers());
}