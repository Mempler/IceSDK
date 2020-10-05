// used for unit tests

#include "Utils/Memory.h"

#include "GameBase.h"


static IceSDK::Memory::Ptr<IceSDK::GameBase> g_GameBase;

class TestGame : public IceSDK::GameBase
{
};

IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase()
{
    if (g_GameBase == nullptr) g_GameBase = std::make_shared<TestGame>();

    return g_GameBase;
}
