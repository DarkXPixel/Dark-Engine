#include "Application.h"
#include <CommandLine/CommandLine.h>

Application* Application::_inst;

Application app;

Application::Application()
{
	_inst = this;
}

Application::~Application()
{
}

void Application::Initialize(HINSTANCE inst, HINSTANCE prevInst, FString cmdLine, INT32 cmdshow)
{
	hInsance = inst;
	nPrevInstance = prevInst;
	lpCmdLine = cmdLine;
	nCmdShow = cmdshow;

	ParseCmdLine();
}

void Application::ParseCmdLine()
{
	//for (size_t i = 0; i < lpCmdLine.Len(); i++)
	//{
	//	if (lpCmdLine[i] == '-')
	//	{
	//		++i;
	//		FCMDParameter parameter;
	//		char buf[64] = { '\0' };
	//		while (i < lpCmdLine.Len() && (lpCmdLine[i] != ' ' && lpCmdLine[i] != '-'))
	//		{
	//			strncat_s(buf, &lpCmdLine[i], 1);
	//			++i;
	//		}
	//		parameter.Parameter = buf;
	//		memset(buf, '\0', strlen(buf));
	//		if (i >= lpCmdLine.Len())
	//		{
	//			parameters.Add(parameter);
	//			return;
	//		}

	//		if (lpCmdLine[i] == '-')
	//		{
	//			--i;
	//			parameters.Add(parameter);
	//			continue;
	//		}
	//		++i;
	//		if (lpCmdLine[i] == '-')
	//		{
	//			--i;
	//			parameters.Add(parameter);
	//			continue;
	//		}
	//		while (i < lpCmdLine.Len() && (lpCmdLine[i] != ' ' && lpCmdLine[i] != '-'))
	//		{
	//			strncat_s(buf, &lpCmdLine[i], 1);
	//			++i;
	//		}

	//		parameter.Value = buf;
	//		parameters.Add(parameter);
	//	}
	//}
}

bool Application::CheckParameterCmdLine(FString Parametr, FString& data)
{
	return false;
}
