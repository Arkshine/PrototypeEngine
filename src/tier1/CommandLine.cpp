#include "CCommandLine.h"
#include "CommandLine.h"

namespace
{
static CCommandLine g_CommandLine;
}

ICommandLine* GetCommandLine()
{
	return &g_CommandLine;
}