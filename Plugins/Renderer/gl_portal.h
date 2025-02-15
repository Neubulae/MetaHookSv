#pragma once

typedef struct portal_vbo_hash_s
{
	struct portal_vbo_hash_s(void *a1, GLuint a2, GLuint a3)
	{
		ClientPortal = a1;
		overlayTextureId = a2;
		textureId = a3;
	}
	void *ClientPortal;
	GLuint overlayTextureId;
	GLuint textureId;

	bool operator==(const struct portal_vbo_hash_s& p) const
	{
		return ClientPortal == p.ClientPortal && overlayTextureId == p.overlayTextureId && textureId == p.textureId;
	}
}portal_vbo_hash_t;

typedef struct portal_vbo_hasher_t
{
	size_t operator()(const struct portal_vbo_hash_s& p) const {
		return ((ULONG_PTR)p.ClientPortal ^ (ULONG_PTR)p.overlayTextureId) + p.textureId;
	}
}portal_vbo_hasher;

typedef struct portal_vbo_s
{
	portal_vbo_s()
	{
		hEBO = 0;
		texinfo = NULL;
		iPolyCount = 0;
	}

	GLuint hEBO;
	mtexinfo_t *texinfo;
	std::vector<GLuint> vIndicesBuffer;
	std::set<int> PolySet;
	int iPolyCount;
}portal_vbo_t;

typedef struct
{
	int program;
	int u_entityMatrix;
}portal_program_t;

typedef struct portal_texture_s
{
	struct portal_texture_s *next;
	struct portal_texture_s *prev;
	GLuint gl_texturenum1;
	GLuint gl_texturenum2;
}portal_texture_t;

void __fastcall ClientPortalManager_ResetAll(void * pthis, int);
mtexinfo_t * __fastcall ClientPortalManager_GetOriginalSurfaceTexture(void * pthis, int dummy, msurface_t *surf);
void __fastcall ClientPortalManager_DrawPortalSurface(void * pthis, int dummy, void * ClientPortal, msurface_t *surf, GLuint texture);
void __fastcall ClientPortalManager_EnableClipPlane(void * pthis, int dummy, int index, vec3_t a1, vec3_t a2, vec3_t a3);

void R_LoadPortalProgramStates(void);
void R_SavePortalProgramStates(void);
void R_NewMapPortal(void);
void R_ShutdownPortal(void);
void R_InitPortal(void);

#define OVERLAY_TEXTURE_ENABLED				1
#define PORTAL_TEXCOORD_ENABLED				2
#define REVERSE_PORTAL_TEXCOORD_ENABLED		4