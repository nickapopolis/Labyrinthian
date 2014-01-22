#pragma once


#define SAFE_RELEASE(x) {if ((x) != NULL) {(x)->Release(); (x) = NULL;}}
#define SAFE_IO_RELEASE(x) {if ((x) != NULL) {(x)->Unacquire(); (x)->Release(); (x) = NULL;}}
