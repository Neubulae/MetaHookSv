#include <metahook.h>
#include <capstone.h>

#include "enginedef.h"
#include "plugins.h"
#include "privatehook.h"
#include "message.h"
#include "corpse.h"
#include "physics.h"

#define R_NEWMAP_SIG_SVENGINE "\x55\x8B\xEC\x51\xC7\x45\xFC\x00\x00\x00\x00\xEB\x2A\x8B\x45\xFC\x83\xC0\x01\x89\x45\xFC\x81\x7D\xFC\x00\x01\x00\x00"
#define R_NEWMAP_SIG_NEW "\x55\x8B\xEC\x83\xEC\x08\xC7\x45\xFC\x00\x00\x00\x00\x2A\x2A\x8B\x45\xFC\x83\xC0\x01\x89\x45\xFC\x81\x7D\xFC\x00\x01\x00\x00\x2A\x2A\x8B\x4D\xFC"

#define R_RECURSIVEWORLDNODE_SIG_SVENGINE "\x83\xEC\x08\x53\x8B\x5C\x24\x10\x83\x3B\xFE"
#define R_RECURSIVEWORLDNODE_SIG_NEW "\x55\x8B\xEC\x83\xEC\x08\x53\x56\x57\x8B\x7D\x08\x83\x3F\xFE\x0F\x2A\x2A\x2A\x2A\x2A\x8B\x47\x04"

#define R_DRAWTENTITIESONLIST_SIG_NEW "\x55\x8B\xEC\xD9\x05\x2A\x2A\x2A\x2A\xD8\x1D\x2A\x2A\x2A\x2A\xDF\xE0\xF6\xC4\x44\x0F\x8B\x2A\x2A\x2A\x2A\x8B\x45\x08"
#define R_DRAWTENTITIESONLIST_SIG_SVENGINE "\x55\x8B\xEC\x83\xE4\x2A\x81\xEC\x2A\x00\x00\x00\xA1\x2A\x2A\x2A\x2A\x33\xC4\x89\x84\x24\x2A\x00\x00\x00\xD9\x05\x2A\x2A\x2A\x2A\xD9\xEE"

privte_funcs_t gPrivateFuncs;

void Engine_FillAddreess(void)
{
	if (g_iEngineType == ENGINE_SVENGINE)
	{
		gPrivateFuncs.R_DrawTEntitiesOnList = (void(*)(int))Search_Pattern(R_DRAWTENTITIESONLIST_SIG_SVENGINE);
		Sig_FuncNotFound(R_DrawTEntitiesOnList);
	}
	else
	{
		gPrivateFuncs.R_DrawTEntitiesOnList = (void(*)(int))Search_Pattern(R_DRAWTENTITIESONLIST_SIG_NEW);
		Sig_FuncNotFound(R_DrawTEntitiesOnList);
	}

	if (g_iEngineType == ENGINE_SVENGINE)
	{
		gPrivateFuncs.R_RecursiveWorldNode = (decltype(gPrivateFuncs.R_RecursiveWorldNode))Search_Pattern(R_RECURSIVEWORLDNODE_SIG_SVENGINE);
		Sig_FuncNotFound(R_RecursiveWorldNode);

		//mov     eax, [edi+4]
		//mov     ecx, r_visframecount
#define R_VISFRAMECOUNT_SIG_SVENGINE "\x8B\x43\x04\x3B\x05"
		if (1)
		{
			DWORD addr = (DWORD)Search_Pattern_From_Length(gPrivateFuncs.R_RecursiveWorldNode, 0x100, R_VISFRAMECOUNT_SIG_SVENGINE);
			Sig_AddrNotFound(r_visframecount);
			r_visframecount = *(int **)(addr + 5);
		}

#define GTEMPENTS_SIG_SVENGINE "\x68\x00\xE0\x5F\x00\x6A\x00\x68\x2A\x2A\x2A\x2A\xA3"
		if (1)
		{
			DWORD addr = (DWORD)Search_Pattern(GTEMPENTS_SIG_SVENGINE);
			Sig_AddrNotFound(gTempEnts);
			gTempEnts = *(decltype(gTempEnts)*)(addr + 8);
		}

#define CL_VIEWENTITY_SIG_SVENGINE "\x68\x2A\x2A\x2A\x2A\x50\x6A\x06\xFF\x35\x2A\x2A\x2A\x2A\xE8"
		if (1)
		{
			DWORD addr = (DWORD)Search_Pattern(CL_VIEWENTITY_SIG_SVENGINE);
			Sig_AddrNotFound(cl_viewentity);
			cl_viewentity = *(decltype(cl_viewentity)*)(addr + 10);
		}

		gPrivateFuncs.R_NewMap = (decltype(gPrivateFuncs.R_NewMap))Search_Pattern(R_NEWMAP_SIG_SVENGINE);
		Sig_FuncNotFound(R_NewMap);

	}
	else
	{
		gPrivateFuncs.R_RecursiveWorldNode = (decltype(gPrivateFuncs.R_RecursiveWorldNode))Search_Pattern(R_RECURSIVEWORLDNODE_SIG_NEW);
		Sig_FuncNotFound(R_RecursiveWorldNode);

		//mov     eax, [edi+4]
		//mov     ecx, r_visframecount
#define R_VISFRAMECOUNT_SIG_NEW "\x8B\x47\x04\x8B\x0D"
		if(1)
		{
			DWORD addr = (DWORD)Search_Pattern_From_Length(gPrivateFuncs.R_RecursiveWorldNode, 0x100, R_VISFRAMECOUNT_SIG_NEW);
			Sig_AddrNotFound(r_visframecount);
			r_visframecount = *(int **)(addr + 5);
		}

#define GTEMPENTS_SIG_NEW "\x68\x30\x68\x17\x00\x6A\x00\x68\x2A\x2A\x2A\x2A\xE8"
		if (1)
		{
			DWORD addr = (DWORD)Search_Pattern(GTEMPENTS_SIG_NEW);
			Sig_AddrNotFound(gTempEnts);
			gTempEnts = *(decltype(gTempEnts)*)(addr + 8);
		}

#define CL_VIEWENTITY_SIG_NEW "\x8B\x0D\x2A\x2A\x2A\x2A\x6A\x64\x6A\x00\x68\x00\x00\x80\x3F\x68\x00\x00\x80\x3F\x68\x2A\x2A\x2A\x2A\x50"
		if (1)
		{
			DWORD addr = (DWORD)Search_Pattern(CL_VIEWENTITY_SIG_NEW);
			Sig_AddrNotFound(gTempEnts);
			cl_viewentity = *(decltype(cl_viewentity)*)(addr + 2);
		}

		gPrivateFuncs.R_NewMap = (decltype(gPrivateFuncs.R_NewMap))Search_Pattern(R_NEWMAP_SIG_NEW);
		Sig_FuncNotFound(R_NewMap);
	}

#define MOD_KNOWN_SIG "\xB8\x9D\x82\x97\x53\x81\xE9"
	if (1)
	{
		
		DWORD addr = (DWORD)Search_Pattern(MOD_KNOWN_SIG);
		Sig_AddrNotFound(mod_known);
		mod_known = *(void **)(addr + 7);
	}

	if (1)
	{
		const char sigs1[] = "Cached models:\n";
		auto Mod_Print_String = Search_Pattern_Data(sigs1);
		if (!Mod_Print_String)
			Mod_Print_String = Search_Pattern_Rdata(sigs1);
		Sig_VarNotFound(Mod_Print_String);
		char pattern[] = "\x57\x68\x2A\x2A\x2A\x2A\xE8";
		*(DWORD *)(pattern + 2) = (DWORD)Mod_Print_String;
		auto Mod_Print_Call = Search_Pattern(pattern);
		Sig_VarNotFound(Mod_Print_Call);

		g_pMetaHookAPI->DisasmRanges(Mod_Print_Call, 0x50, [](void *inst, PUCHAR address, size_t instLen, int instCount, int depth, PVOID context)
			{
				auto pinst = (cs_insn *)inst;

				if (pinst->id == X86_INS_MOV &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					pinst->detail->x86.operands[1].type == X86_OP_MEM &&
					pinst->detail->x86.operands[1].mem.base == 0)
				{//A1 84 5C 32 02 mov     eax, mod_numknown
					DWORD imm = pinst->detail->x86.operands[1].mem.disp;

					mod_numknown = (decltype(mod_numknown))imm;
				}
				else if (pinst->id == X86_INS_CMP &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_MEM &&
					pinst->detail->x86.operands[0].mem.base == 0 &&
					pinst->detail->x86.operands[1].type == X86_OP_REG)
				{//39 3D 44 32 90 03 cmp     mod_numknown, edi
					DWORD imm = pinst->detail->x86.operands[0].mem.disp;

					mod_numknown = (decltype(mod_numknown))imm;
				}

				if (mod_numknown)
					return TRUE;

				if (address[0] == 0xCC)
					return TRUE;

				if (pinst->id == X86_INS_RET)
					return TRUE;

				return FALSE;
			}, 0, NULL);
	}

	if (1)
	{
		typedef struct
		{
			int disableFog_instcount;
			int parsemod_instcount;
			int getskin_instcount;
		}R_DrawTEntitiesOnList_ctx;

		R_DrawTEntitiesOnList_ctx ctx = { 0 };

		g_pMetaHookAPI->DisasmRanges(gPrivateFuncs.R_DrawTEntitiesOnList, 0x250, [](void *inst, PUCHAR address, size_t instLen, int instCount, int depth, PVOID context)
			{
				auto pinst = (cs_insn *)inst;
				auto ctx = (R_DrawTEntitiesOnList_ctx *)context;

				if (pinst->id == X86_INS_MOV &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					pinst->detail->x86.operands[0].reg == X86_REG_EAX &&
					pinst->detail->x86.operands[1].type == X86_OP_MEM &&
					pinst->detail->x86.operands[1].mem.base == 0 &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp > (PUCHAR)g_dwEngineDataBase &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp < (PUCHAR)g_dwEngineDataBase + g_dwEngineDataSize)
				{
					//.text:01D923D9 A1 DC 72 ED 01                                      mov     eax, cl_parsemod
					//.text:01D88CBB A1 CC AF E3 01                                      mov     eax, cl_parsemod
					DWORD value = *(DWORD *)pinst->detail->x86.operands[1].mem.disp;
					if (value == 63)
					{
						ctx->parsemod_instcount = instCount;
					}
				}
				else if (!cl_parsecount && ctx->parsemod_instcount &&
					instCount < ctx->parsemod_instcount + 3 &&
					(pinst->id == X86_INS_MOV || pinst->id == X86_INS_AND) &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					pinst->detail->x86.operands[1].type == X86_OP_MEM &&
					pinst->detail->x86.operands[1].mem.base == 0 &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp >(PUCHAR)g_dwEngineDataBase &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp < (PUCHAR)g_dwEngineDataBase + g_dwEngineDataSize)
				{
					//.text:01D923DE 23 05 AC D2 30 02                                   and     eax, cl_parsecount
					//.text:01D88CC0 8B 0D 04 AE D8 02                                   mov     ecx, cl_parsecount
					cl_parsecount = (decltype(cl_parsecount))pinst->detail->x86.operands[1].mem.disp;
				}
				else if (!cl_frames && ctx->parsemod_instcount &&
					instCount < ctx->parsemod_instcount + 20 &&
					pinst->id == X86_INS_LEA &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					pinst->detail->x86.operands[0].reg == X86_REG_EAX &&
					pinst->detail->x86.operands[1].type == X86_OP_MEM &&
					pinst->detail->x86.operands[1].mem.base != 0 &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp >(PUCHAR)g_dwEngineDataBase &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp < (PUCHAR)g_dwEngineDataBase + g_dwEngineDataSize)
				{
					//.text:01D923F0 8D 80 F4 D5 30 02                                   lea     eax, cl_frames[eax]
					//.text:01D88CE8 8D 84 CA 4C B1 D8 02                                lea     eax, cl_frames_1[edx+ecx*8]
					cl_frames = (decltype(cl_frames))pinst->detail->x86.operands[1].mem.disp;
				}
				else if (ctx->parsemod_instcount &&
					instCount < ctx->parsemod_instcount + 5 &&
					pinst->id == X86_INS_IMUL &&
					pinst->detail->x86.op_count == 3 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					pinst->detail->x86.operands[1].type == X86_OP_REG &&
					pinst->detail->x86.operands[2].type == X86_OP_IMM &&
					pinst->detail->x86.operands[2].imm > 0x4000 &&
					pinst->detail->x86.operands[2].imm < 0xF000)
				{
					//.text:01D923E4 69 C8 D8 84 00 00                                   imul    ecx, eax, 84D8h
					size_of_frame = pinst->detail->x86.operands[2].imm;
				}
				else if (
					pinst->id == X86_INS_MOVSX &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					pinst->detail->x86.operands[0].size == 4 &&
					pinst->detail->x86.operands[1].type == X86_OP_MEM &&
					pinst->detail->x86.operands[1].size == 2 &&
					pinst->detail->x86.operands[1].mem.base != 0 &&
					pinst->detail->x86.operands[1].mem.disp == 0x2E8)
				{
					//.text:01D924D9 0F BF 83 E8 02 00 00                                movsx   eax, word ptr [ebx+2E8h]
					ctx->getskin_instcount = instCount;
				}

				if (cl_parsecount && cl_frames)
					return TRUE;

				if (address[0] == 0xCC)
					return TRUE;

				if (pinst->id == X86_INS_RET)
					return TRUE;

				return FALSE;
			}, 0, &ctx);

		Sig_VarNotFound(cl_frames);
		Sig_VarNotFound(cl_parsecount);
	}

}

void Client_FillAddress(void)
{
	if ((void *)g_pMetaSave->pExportFuncs->CL_IsThirdPerson > g_dwClientBase && (void *)g_pMetaSave->pExportFuncs->CL_IsThirdPerson < (PUCHAR)g_dwClientBase + g_dwClientSize)
	{
		typedef struct
		{
			ULONG_PTR Candidates[16];
			int iNumCandidates;
		}CL_IsThirdPerson_ctx;

		CL_IsThirdPerson_ctx ctx = { 0 };

		g_pMetaHookAPI->DisasmRanges((void *)g_pMetaSave->pExportFuncs->CL_IsThirdPerson, 0x100, [](void *inst, PUCHAR address, size_t instLen, int instCount, int depth, PVOID context) {

			auto ctx = (CL_IsThirdPerson_ctx *)context;
			auto pinst = (cs_insn *)inst;

			if (ctx->iNumCandidates < 16)
			{
				if (pinst->id == X86_INS_MOV &&
					pinst->detail->x86.op_count == 2 &&
					pinst->detail->x86.operands[0].type == X86_OP_REG &&
					(
						pinst->detail->x86.operands[0].reg == X86_REG_EAX ||
						pinst->detail->x86.operands[0].reg == X86_REG_EBX ||
						pinst->detail->x86.operands[0].reg == X86_REG_ECX ||
						pinst->detail->x86.operands[0].reg == X86_REG_EDX ||
						pinst->detail->x86.operands[0].reg == X86_REG_ESI ||
						pinst->detail->x86.operands[0].reg == X86_REG_EDI
						) &&
					pinst->detail->x86.operands[1].type == X86_OP_MEM &&
					pinst->detail->x86.operands[1].mem.base == 0 &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp > (PUCHAR)g_dwClientBase &&
					(PUCHAR)pinst->detail->x86.operands[1].mem.disp < (PUCHAR)g_dwClientBase + g_dwClientSize)
				{
					ctx->Candidates[ctx->iNumCandidates] = (ULONG_PTR)pinst->detail->x86.operands[1].mem.disp;
					ctx->iNumCandidates++;
				}
			}

			if (address[0] == 0xCC)
				return TRUE;

			if (pinst->id == X86_INS_RET)
				return TRUE;

			return FALSE;
			}, 0, &ctx);

		if (ctx.iNumCandidates >= 3 && ctx.Candidates[ctx.iNumCandidates - 1] == ctx.Candidates[ctx.iNumCandidates - 2] + sizeof(int))
		{
			g_iUser1 = (decltype(g_iUser1))ctx.Candidates[ctx.iNumCandidates - 2];
			g_iUser2 = (decltype(g_iUser2))ctx.Candidates[ctx.iNumCandidates - 1];
		}
	}
}

void R_NewMap(void)
{
	gPrivateFuncs.R_NewMap();
	gPhysicsManager.NewMap();
	gCorpseManager.NewMap();
}

TEMPENTITY *efxapi_R_TempModel(float *pos, float *dir, float *angles, float life, int modelIndex, int soundtype)
{
	auto r = gPrivateFuncs.efxapi_R_TempModel(pos, dir, angles, life, modelIndex, soundtype);
	if (r && g_bIsCreatingClCorpse && g_iCreatingClCorpsePlayerIndex > 0)
	{
		int tentindex = r - gTempEnts;

		gPhysicsManager.ChangeRagdollEntIndex(g_iCreatingClCorpsePlayerIndex, ENTINDEX_TEMPENTITY + tentindex);

		r->entity.curstate.iuser4 = PhyCorpseFlag;
		r->entity.curstate.iuser3 = ENTINDEX_TEMPENTITY + tentindex;
		r->entity.curstate.owner = g_iCreatingClCorpsePlayerIndex;
	}

	return r;
}