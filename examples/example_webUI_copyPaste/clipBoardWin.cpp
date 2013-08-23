#include "clipBoardWin.h"
#include <Windows.h>

std::string getClipBoardString() {

	std::string clipBoardText = "";  
	if (OpenClipboard(NULL)){  
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);  
		if(IsClipboardFormatAvailable(CF_TEXT)){  
			char *pchData = (char*)GlobalLock(hClipboardData);  
			clipBoardText.assign(pchData);  
			GlobalUnlock(hClipboardData);  
			CloseClipboard();  
		}  
	}  
	return clipBoardText;  
}