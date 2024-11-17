#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES saProcess, saThread;
    char cmdLine[] = "notepad.exe";
    DWORD exitCode;
    const DWORD timeout = 10000; 

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    saProcess.nLength = sizeof(SECURITY_ATTRIBUTES);
    saProcess.lpSecurityDescriptor = NULL; 
    saProcess.bInheritHandle = FALSE;

    saThread.nLength = sizeof(SECURITY_ATTRIBUTES);
    saThread.lpSecurityDescriptor = NULL; 
    saThread.bInheritHandle = FALSE;

    if (!CreateProcessA(
        NULL,            
        cmdLine,          
        &saProcess,       
        &saThread,       
        FALSE,            
        0,                
        NULL,             
        NULL,             
        &si,              
        &pi))            
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

  
    DWORD waitResult = WaitForSingleObject(pi.hProcess, timeout);

    if (waitResult == WAIT_TIMEOUT) {
        
        if (!TerminateProcess(pi.hProcess, 1)) {
            printf("TerminateProcess failed (%d).\n", GetLastError());
        }
        else {
            printf("Process was terminated because it ran too long.\n");
        }
    }
    else if (waitResult == WAIT_OBJECT_0) {
       
        if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
            printf("GetExitCodeProcess failed (%d).\n", GetLastError());
        }
        else if (exitCode == 0) {
            printf("Process exited successfully.\n");
        }
        else {
            printf("Process exited with error code %d.\n", exitCode);
        }
    }
    else {
        printf("WaitForSingleObject failed (%d).\n", GetLastError());
    }

 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
