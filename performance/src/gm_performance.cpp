#include "GarrysMod/Lua/Interface.h"
#include "CSVWriter.h"
#include <stdio.h>
#include <chrono>
#include <sstream>

using namespace GarrysMod::Lua;

void print(lua_State* state, char* str)
{
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
        LUA->GetField(-1, "print");
        LUA->PushString(str);
        LUA->Call(1, 0);
    LUA->Pop();
}

void print(lua_State* state, double number)
{
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-1, "print");
    LUA->PushNumber(number);
    LUA->Call(1, 0);
    LUA->Pop();
}

std::chrono::time_point<std::chrono::system_clock> oldTime;
std::chrono::microseconds lastDuration;

double getFPS()
{
    long long lastDurationCount = lastDuration.count();
    double fps = 1.0 / (double)lastDurationCount * 1000000.0;
    return fps;
}

double getDiffus()
{
    long long lastDurationCount = lastDuration.count();
    return (double)lastDurationCount;
}

int lua_getFPS(lua_State* state)
{
    long long lastDurationF = lastDuration.count();
    LUA->PushNumber(getFPS());
    return 1;
}

int lua_getDiffus(lua_State* state)
{
    LUA->PushNumber(getDiffus());
    return 1;
}

int lua_getDiffms(lua_State* state)
{
    LUA->PushNumber(getDiffus()/1000.0);
    return 1;
}

CSVWriter* csvWriter = nullptr;
long long benchmarkTime;
bool benchmark = false;
long long benchmarkTimer;

void benchmarkStart()
{
    csvWriter = new CSVWriter();
    benchmarkTime = 0;
    benchmarkTimer = 0;
    benchmark = true;
}

void benchmarkUpdate(long long diff)
{
    benchmarkTime += diff;
    benchmarkTimer += diff;

    if (benchmarkTimer > 200000) // after every 200ms
    {
        std::stringstream sstream;
        sstream << benchmarkTime << ";" << getFPS() << ";" << getDiffus() / 1000.0 << ";" << getDiffus();
        csvWriter->writeLine(sstream.str());

        benchmarkTimer = 0;
    }
}

void benchmarkFinish()
{
    benchmark = false;
    delete csvWriter;
    csvWriter = nullptr;
}

int lua_startbenchmark(lua_State* state)
{
    benchmarkStart();
    return 0;
}

int lua_stopbenchmark(lua_State* state)
{
    benchmarkFinish();
    return 0;
}

int perf_think(lua_State* state)
{
    lastDuration = std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::system_clock::now() - oldTime);
    if (benchmark)
        benchmarkUpdate(lastDuration.count());

    oldTime = std::chrono::system_clock::now();
    return 0;
}

GMOD_MODULE_OPEN()
{
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
        LUA->GetField(-1, "hook");
            LUA->GetField(-1, "Add");
            LUA->PushString("Think");
            LUA->PushString("PerforThink");
            LUA->PushCFunction(perf_think);
            LUA->Call(3, 0);
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
        LUA->CreateTable();
            LUA->PushCFunction(lua_getFPS); LUA->SetField(-2, "getFPS");
            LUA->PushCFunction(lua_getDiffus); LUA->SetField(-2, "getDiff");
            LUA->PushCFunction(lua_getDiffms); LUA->SetField(-2, "getDiffMS");
            LUA->PushCFunction(lua_startbenchmark); LUA->SetField(-2, "startBenchmark");
            LUA->PushCFunction(lua_stopbenchmark); LUA->SetField(-2, "stopBenchmark");
        LUA->SetField(-2, "performance");
    LUA->Pop();

    print(state, "[Performance] Performance Monitor by Sapd loaded");

    return 0;
}

GMOD_MODULE_CLOSE()
{
    if (csvWriter)
        benchmarkFinish();
    return 0;
}
