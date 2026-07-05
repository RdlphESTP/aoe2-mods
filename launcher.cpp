#include <windows.h>

bool InjectDLL(DWORD processId, const char* dllPath)
{
    HANDLE hProcess =
        OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

    if (!hProcess)
        return false;

    void* remoteString =
        VirtualAllocEx(
            hProcess,
            NULL,
            MAX_PATH,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE);

    if (!remoteString)
    {
        CloseHandle(hProcess);
        return false;
    }

    WriteProcessMemory(
        hProcess,
        remoteString,
        dllPath,
        strlen(dllPath) + 1,
        NULL);

    HMODULE kernel32 =
        GetModuleHandleA("kernel32.dll");

    FARPROC loadLibrary =
        GetProcAddress(kernel32, "LoadLibraryA");

    HANDLE hThread =
        CreateRemoteThread(
            hProcess,
            NULL,
            0,
            (LPTHREAD_START_ROUTINE)loadLibrary,
            remoteString,
            0,
            NULL);

    if (!hThread)
    {
        VirtualFreeEx(
            hProcess,
            remoteString,
            0,
            MEM_RELEASE);

        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

    VirtualFreeEx(
        hProcess,
        remoteString,
        0,
        MEM_RELEASE);

    CloseHandle(hProcess);

    return true;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};

    si.cb = sizeof(si);

    BOOL result =
        CreateProcessA(
            "empires2.exe",
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi);

    if (!result)
    {
        MessageBoxA(
            NULL,
            "Impossible de lancer empires2.exe",
            "Erreur",
            MB_OK);

        return 1;
    }

    char dllPath[MAX_PATH];

    GetFullPathNameA(
        "mods\\dragscroll.dll",
        MAX_PATH,
        dllPath,
        NULL);

    Sleep(2000);

    if (!InjectDLL(pi.dwProcessId, dllPath))
    {
        MessageBoxA(
            NULL,
            "Injection DLL echouee",
            "Erreur",
            MB_OK);

        return 1;
    }

    return 0;
}