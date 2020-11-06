
#include "Client.h"
#include "Engine.h"
#include "Client/Client.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    if (!GET_SINGLE(CEngine)->Init(TEXT("GameClient"),
        TEXT("GameClient"), hInstance, IDI_ICON1,
        IDI_ICON1, 1280, 720))
    {
        DESTROY_SINGLE(CEngine);
        return 0;
    }

    //GlobalSetting();

    int iRetVal = GET_SINGLE(CEngine)->Run();

    DESTROY_SINGLE(CEngine);

    return iRetVal;
}
