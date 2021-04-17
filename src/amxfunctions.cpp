#include <string>
#include "amxfunctions.h"

// From "amx.c", part of the PAWN language runtime:
// http://code.google.com/p/pawnscript/source/browse/trunk/amx/amx.c
/*
#define USENAMETABLE(hdr) \
	((hdr)->defsize==sizeof(AMX_FUNCSTUBNT))

#define NUMENTRIES(hdr,field,nextfield) \
	(unsigned)(((hdr)->nextfield - (hdr)->field) / (hdr)->defsize)

#define GETENTRY(hdr,table,index) \
	(AMX_FUNCSTUB *)((unsigned char*)(hdr) + (unsigned)(hdr)->table + (unsigned)index*(hdr)->defsize)

#define GETENTRYNAME(hdr,entry) \
	(USENAMETABLE(hdr) ? \
		(char *)((unsigned char*)(hdr) + (unsigned)((AMX_FUNCSTUBNT*)(entry))->nameofs) : \
		((AMX_FUNCSTUB*)(entry))->name)

void Redirect(AMX * amx, char const * const from, ucell to, AMX_NATIVE * store)
{
	int
		num,
		idx;
	// Operate on the raw AMX file, don't use the amx_ functions to avoid issues
	// with the fact that we've not actually finished initialisation yet.  Based
	// VERY heavilly on code from "amx.c" in the PAWN runtime library.
	AMX_HEADER *
		hdr = (AMX_HEADER *)amx->base;
	AMX_FUNCSTUB *
		func;
	num = NUMENTRIES(hdr, natives, libraries);
	//logprintf("Redirect 1");
	for (idx = 0; idx != num; ++idx)
	{
		func = GETENTRY(hdr, natives, idx);
		//logprintf("Redirect 2 \"%s\" \"%s\"", from, GETENTRYNAME(hdr, func));
		if (!strcmp(from, GETENTRYNAME(hdr, func)))
		{
			//logprintf("Redirect 3");
			// Intercept the call!
			if (store)
			{
				*store = (AMX_NATIVE)func->address;
			}
			func->address = to;
			break;
		}
	}
}
*/

cell* get_amxaddr(AMX *amx, cell amx_addr)
{
	cell *addr;
	if(amx_GetAddr(amx, amx_addr, &addr) != AMX_ERR_NONE) return nullptr;
	return addr;
}

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max)
{
	if (max <= 0) return 0;

	cell *dest = get_amxaddr(amx, amx_addr);
	if(!dest) return 0;
	cell *start = dest;
	while (--max && *source)
	{
		*dest++ = (unsigned char)*source++;
	}
	*dest = 0;
	return dest - start;
}

int set_amxstring(AMX *amx, cell amx_addr, const std::string &source, int max)
{
	if (max <= 0) return 0;

	cell *dest = get_amxaddr(amx, amx_addr);
	if(!dest) return 0;
	cell *start = dest;
	auto it = source.begin();
	while (--max && it != source.end())
	{
		*dest++ = *it != '\0' ? (unsigned char)*it : 0x00FFFF00;
		it++;
	}
	*dest = 0;
	return dest - start;
}
