/*==============================================================================

	Title		: ERRORMSG
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _ERRORMSG_H_
#define _ERRORMSG_H_


/*------------------------------------------------------------------------------
* includeêÈåæ
------------------------------------------------------------------------------*/
#include <windows.h>

/*------------------------------------------------------------------------------
* defineêÈåæ
------------------------------------------------------------------------------*/
#define LMESSAGE(title, content) MessageBox(NULL, L##content, L##title, NULL)
#define MESSAGE(title, content) MessageBox(NULL, content, title, NULL)

#define ERROR_CHECK(variable, content) if (variable < 0) {\
LMESSAGE("Error", content);\
return -1;\
}\

#define NULL_CHECK(pointer) if (pointer == nullptr) {\
LMESSAGE("Error", "Error nullptr.");\
return -1;\
}\

#define CREATE_CHECK(pointer) if (pointer != nullptr) {\
LMESSAGE("Error", "Error created.");\
return -1;\
}\

#endif
