/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/

#include "stdafx.h"


#include "gtrace.h"


GTrace * GTrace::current = NULL;
BOOL GTrace::doTrace = TRUE;

/*
GTrace::Open(const char *relPath) {
{
	logFile = fopen(relPath,"w");
}
*/



int GTrace::Warning1(const char * message)
{
//  TRACE("Warning : %s\n",message);
  if (doTrace) {
	  fprintf(logFile,"%s", message);
	  Flush();
  }
  return(0);
}

	

int G__cdecl GTrace::Warning(const char *formatString, ...)
{
    char	buf[10000];
    va_list	ap;

    va_start(ap, formatString);
    _vsnprintf(buf, sizeof(buf),formatString, ap);
    va_end(ap);
	return(Warning1(buf));
}


int GEaiTrace::eaiLevel=0;

