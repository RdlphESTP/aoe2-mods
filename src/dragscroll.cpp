#include <windows.h>
#include <stdint.h>


DWORD WINAPI MainThread(LPVOID lpParam)
{
    uintptr_t camera = 0;

    while (!camera)
    {
        uintptr_t moduleBase =
            (uintptr_t)GetModuleHandleA("empires2.exe");

        uintptr_t p1 =
            *(uintptr_t*)(moduleBase + 0x26FA60);

        if (p1)
        {
            uintptr_t p2 =
                *(uintptr_t*)(p1 + 0x4C);

            if (p2)
            {
                camera =
                    *(uintptr_t*)(p2 + 0x4);

                if (!camera)
                {
                    Sleep(500);
                }
            }
            else
            {
                Sleep(500);
            }
        }
        else
        {
            Sleep(500);
        }
    }

    // ici on sait que la caméra existe

    POINT oldMouse;
    GetCursorPos(&oldMouse);

    while (true)
    {
        if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
        {
            uintptr_t moduleBase =
                (uintptr_t)GetModuleHandleA("empires2.exe");

            uintptr_t p1 =
                *(uintptr_t*)(moduleBase + 0x26FA60);

            if (!p1)
            {
                Sleep(10);
                continue;
            }

            uintptr_t p2 =
                *(uintptr_t*)(p1 + 0x4C);

            if (!p2)
            {
                Sleep(10);
                continue;
            }

            uintptr_t camera =
                *(uintptr_t*)(p2 + 0x4);

            if (!camera)
            {
                Sleep(10);
                continue;
            }

            POINT currentMouse;
            GetCursorPos(&currentMouse);

            int dx = currentMouse.x - oldMouse.x;
            int dy = currentMouse.y - oldMouse.y;

            float* camA =
                (float*)(camera + 0x168);

            float* camB =
                (float*)(camera + 0x16C);

            float sensitivity = 0.02f;

            *camA -= (dx - dy) * sensitivity;
            *camB -= (dx + dy) * sensitivity;

            oldMouse = currentMouse;
        }
        else
        {
            GetCursorPos(&oldMouse);
        }

        Sleep(10);
    }

    return 0;
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        CreateThread(
            NULL,
            0,
            MainThread,
            NULL,
            0,
            NULL);
    }

    return TRUE;
}