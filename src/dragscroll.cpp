#include <windows.h>
#include <stdint.h>

// Résout la chaîne de pointeurs vers l'objet caméra.
// Retourne 0 si un des pointeurs intermédiaires est invalide (ex: pas de partie en cours).
static uintptr_t ResolveCameraPointer()
{
    uintptr_t moduleBase = (uintptr_t)GetModuleHandleA("empires2.exe");
    if (!moduleBase)
    {
        return 0;
    }

    uintptr_t p1 = *(uintptr_t*)(moduleBase + 0x26FA60);
    if (!p1)
    {
        return 0;
    }

    uintptr_t p2 = *(uintptr_t*)(p1 + 0x4C);
    if (!p2)
    {
        return 0;
    }

    uintptr_t camera = *(uintptr_t*)(p2 + 0x4);
    return camera; // peut être 0 si pas encore initialisé
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
    // Attente initiale : on ne résout la caméra qu'une fois, au lancement.
    uintptr_t camera = 0;
    while (!camera)
    {
        camera = ResolveCameraPointer();
        if (!camera)
        {
            Sleep(500);
        }
    }

    // À partir d'ici, la caméra existe. On garde ce pointeur en cache
    // et on ne le re-résout que si une partie se termine/recommence
    // (voir la vérification périodique plus bas).

    bool wasPressed = false;
    POINT oldMouse;
    GetCursorPos(&oldMouse);

    int framesSinceLastCheck = 0;
    const int recheckInterval = 100; // ~1 seconde à 10ms/tick, ajuster si besoin

    while (true)
    {
        bool isPressed = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

        // Revalidation périodique et légère du pointeur caméra
        // (utile si une partie se termine ou qu'une nouvelle commence),
        // sans le faire à chaque tick (ancien système).
        framesSinceLastCheck++;
        if (framesSinceLastCheck >= recheckInterval)
        {
            framesSinceLastCheck = 0;
            uintptr_t freshCamera = ResolveCameraPointer();
            if (freshCamera && freshCamera != camera)
            {
                camera = freshCamera;
            }
            else if (!freshCamera)
            {
                // Plus de partie en cours : on retombe en attente.
                camera = 0;
                while (!camera)
                {
                    camera = ResolveCameraPointer();
                    if (!camera)
                    {
                        Sleep(500);
                    }
                }
                // On resynchronise pour éviter un delta basé sur une
                // position souris obsolète après cette pause.
                wasPressed = false;
                GetCursorPos(&oldMouse);
            }
        }

        if (isPressed)
        {
            POINT currentMouse;
            GetCursorPos(&currentMouse);

            if (!wasPressed)
            {
                // Front montant : on vient d'appuyer sur la molette.
                // On resynchronise oldMouse SANS appliquer de delta ce
                // tick-ci, pour éviter un delta énorme basé sur une
                // position potentiellement périmée (ex: après un délai
                // du scheduler pendant que le bouton était relâché).
                oldMouse = currentMouse;
            }
            else
            {
                int dx = currentMouse.x - oldMouse.x;
                int dy = currentMouse.y - oldMouse.y;

                // Filet de sécurité : borne le delta pour éviter tout
                // saut disproportionné, quelle qu'en soit la cause.
                const int maxDelta = 100;
                if (dx > maxDelta) dx = maxDelta;
                if (dx < -maxDelta) dx = -maxDelta;
                if (dy > maxDelta) dy = maxDelta;
                if (dy < -maxDelta) dy = -maxDelta;

                float* camA = (float*)(camera + 0x168);
                float* camB = (float*)(camera + 0x16C);
                float sensitivity = 0.02f;
                *camA -= (dx - dy) * sensitivity;
                *camB -= (dx + dy) * sensitivity;

                oldMouse = currentMouse;
            }
        }
        else
        {
            GetCursorPos(&oldMouse);
        }

        wasPressed = isPressed;
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
