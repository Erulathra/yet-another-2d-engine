#include "MainEngine.h"
#include "LoggingMacros.h"

int main(int, char**)
{
    LoggingMacros::InitializeSPDLog();

    MainEngine Engine = MainEngine();
    if(Engine.Init() == 0)
    {
        Engine.PrepareScene();
        int32_t ReturnCode = Engine.MainLoop();

        if (ReturnCode != 0) {
            return ReturnCode;
        }
    }

    return 0;
}
