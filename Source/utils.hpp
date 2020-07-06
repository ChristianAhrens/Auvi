#pragma once

#include <JuceHeader.h>

namespace Auvi
{
namespace utils
{

enum dBRange
{
    min = -90,
    max = 0,
};

static int getGlobalMindB()
{
    return dBRange::min;
}

static int getGlobalMaxdB()
{
    return dBRange::max;
}

constexpr int const max_input_channels = 512;
     
}; // namespace utils
}; // namespace Auvi