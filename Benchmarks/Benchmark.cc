// used for Benchmarks

#include "Utils/Memory.h"

#include "GameBase.h"

#include <benchmark/benchmark.h>

static IceSDK::Memory::Ptr<IceSDK::GameBase> g_GameBase;

class BenchmarkGame : public IceSDK::GameBase
{
};

IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase()
{
    if (g_GameBase == nullptr) g_GameBase = std::make_shared<BenchmarkGame>();

    return g_GameBase;
}
