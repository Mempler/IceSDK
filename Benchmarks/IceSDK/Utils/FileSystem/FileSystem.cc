#include "Utils/FileSystem.h"

#include <benchmark/benchmark.h>

using namespace IceSDK;

static void BM_FileSystem_JoinPath(benchmark::State& state)
{
    for (auto _ : state)
    { FileSystem::JoinPath("C:\\Path\\To\\IceSDK", "out"); }
}

static void BM_FileSystem_Exists(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::Exists("."); }
}

static void BM_FileSystem_IsDirectory(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::IsDirectory("."); }
}

static void BM_FileSystem_ReadDirectory(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::ReadDirectory(".", true); }
}

static void BM_FileSystem_ResolveFullPath(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::ResolveFullPath("."); }
}

static void BM_FileSystem_ReadBinaryFile(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::ReadBinaryFile("."); }
}

static void BM_FileSystem_WriteBinaryFile(benchmark::State& state)
{
    std::vector<uint8_t> data = { 0xF0, 0x0D, 0xBA, 0xBE };
    for (auto _ : state) { FileSystem::WriteBinaryFile("TestFile", data); }
}

static void BM_FileSystem_HasExtension(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::HasExtension("TestFile", ".mp4"); }
}

static void BM_FileSystem_GetFileName(benchmark::State& state)
{
    for (auto _ : state)
    { FileSystem::GetFileName("./Test/File/TestFile.mp4"); }
}

static void BM_FileSystem_Touch(benchmark::State& state)
{
    for (auto _ : state) { FileSystem::Touch("TestFile.mp4"); }
}

static void BM_FileSystem_Delete(benchmark::State& state)
{
    std::vector<uint8_t> data = { 0xF0, 0x0D, 0xBA, 0xBE };
    for (auto _ : state) { FileSystem::Delete("TestFile.mp4"); }
}

BENCHMARK(BM_FileSystem_JoinPath);
BENCHMARK(BM_FileSystem_Exists);
BENCHMARK(BM_FileSystem_IsDirectory);
BENCHMARK(BM_FileSystem_ReadDirectory);
BENCHMARK(BM_FileSystem_ResolveFullPath);
// BENCHMARK(BM_FileSystem_ReadBinaryFile);
// BENCHMARK(BM_FileSystem_WriteBinaryFile);
BENCHMARK(BM_FileSystem_HasExtension);
BENCHMARK(BM_FileSystem_GetFileName);
// BENCHMARK(BM_FileSystem_Touch);
// BENCHMARK(BM_FileSystem_Delete);

BENCHMARK_MAIN();
