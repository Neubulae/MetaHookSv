#include <metahook.h>
#include <com_model.h>
#include "exportfuncs.h"
#include "privatehook.h"
#include "plugins.h"
#include "command.h"
#include "message.h"
#include "qgl.h"

cl_exportfuncs_t gExportfuncs;
mh_interface_t *g_pInterface;
metahook_api_t *g_pMetaHookAPI;
mh_enginesave_t *g_pMetaSave;
IFileSystem *g_pFileSystem;

HINSTANCE g_hInstance, g_hThisModule, g_hEngineModule;
PVOID g_dwEngineBase;
DWORD g_dwEngineSize;
PVOID g_dwEngineTextBase;
DWORD g_dwEngineTextSize;
PVOID g_dwEngineDataBase;
DWORD g_dwEngineDataSize;
PVOID g_dwEngineRdataBase;
DWORD g_dwEngineRdataSize;
DWORD g_dwEngineBuildnum;
int g_iEngineType;
PVOID g_dwClientBase;
DWORD g_dwClientSize;

hook_t *g_phook_R_NewMap = NULL;

void IPluginsV4::Init(metahook_api_t *pAPI, mh_interface_t *pInterface, mh_enginesave_t *pSave)
{
	g_pInterface = pInterface;
	g_pMetaHookAPI = pAPI;
	g_pMetaSave = pSave;
	g_hInstance = GetModuleHandle(NULL);
}

void IPluginsV4::Shutdown(void)
{
}

void IPluginsV4::LoadEngine(cl_enginefunc_t *pEngfuncs)
{
	g_pFileSystem = g_pInterface->FileSystem;
	g_iEngineType = g_pMetaHookAPI->GetEngineType();
	g_dwEngineBuildnum = g_pMetaHookAPI->GetEngineBuildnum();
	g_hEngineModule = g_pMetaHookAPI->GetEngineModule();
	g_dwEngineBase = g_pMetaHookAPI->GetEngineBase();
	g_dwEngineSize = g_pMetaHookAPI->GetEngineSize();
	g_dwEngineTextBase = g_pMetaHookAPI->GetSectionByName(g_dwEngineBase, ".text\x0\x0\x0", &g_dwEngineTextSize);
	g_dwEngineDataBase = g_pMetaHookAPI->GetSectionByName(g_dwEngineBase, ".data\x0\x0\x0", &g_dwEngineDataSize);
	g_dwEngineRdataBase = g_pMetaHookAPI->GetSectionByName(g_dwEngineBase, ".rdata\x0\x0", &g_dwEngineRdataSize);

	memcpy(&gEngfuncs, pEngfuncs, sizeof(gEngfuncs));

	if (g_iEngineType != ENGINE_SVENGINE && g_iEngineType != ENGINE_GOLDSRC)
	{
		g_pMetaHookAPI->SysError("Unsupported engine: %s, buildnum %d", g_pMetaHookAPI->GetEngineTypeName(), g_dwEngineBuildnum);
	}

	Engine_FillAddreess();

	Install_InlineHook(R_NewMap);
}

void IPluginsV4::LoadClient(cl_exportfuncs_t *pExportFunc)
{
	memcpy(&gExportfuncs, pExportFunc, sizeof(gExportfuncs));

	g_dwClientBase = g_pMetaHookAPI->GetClientBase();
	g_dwClientSize = g_pMetaHookAPI->GetClientSize();

	pExportFunc->HUD_Init = HUD_Init;
	pExportFunc->HUD_GetStudioModelInterface = HUD_GetStudioModelInterface;
	pExportFunc->HUD_CreateEntities = HUD_CreateEntities;
	pExportFunc->HUD_TempEntUpdate = HUD_TempEntUpdate;
	pExportFunc->HUD_AddEntity = HUD_AddEntity;
	pExportFunc->HUD_DrawNormalTriangles = HUD_DrawNormalTriangles;
	pExportFunc->HUD_Frame = HUD_Frame;
	pExportFunc->HUD_Shutdown = HUD_Shutdown;
	pExportFunc->V_CalcRefdef = V_CalcRefdef;

	Client_FillAddress();

	auto err = glewInit();
	if (GLEW_OK != err)
	{
		g_pMetaHookAPI->SysError("glewInit failed, %s", glewGetErrorString(err));
		return;
	}
}

void IPluginsV4::ExitGame(int iResult)
{
	Uninstall_Hook(R_NewMap);
}

const char completeVersion[] =
{
	BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
	'-',
	BUILD_MONTH_CH0, BUILD_MONTH_CH1,
	'-',
	BUILD_DAY_CH0, BUILD_DAY_CH1,
	'T',
	BUILD_HOUR_CH0, BUILD_HOUR_CH1,
	':',
	BUILD_MIN_CH0, BUILD_MIN_CH1,
	':',
	BUILD_SEC_CH0, BUILD_SEC_CH1,
	'\0'
};

const char *IPluginsV4::GetVersion(void)
{
	return completeVersion;
}

EXPOSE_SINGLE_INTERFACE(IPluginsV4, IPluginsV4, METAHOOK_PLUGIN_API_VERSION_V4);