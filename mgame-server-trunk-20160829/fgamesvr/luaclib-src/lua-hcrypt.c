#include <lua.h>
#include <lauxlib.h>

#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//sting -> int hash
static int
lstring_hash(lua_State *L){
	size_t sz = 0;
	const char* key = luaL_checklstring(L, 1, &sz);
	uint32_t h;
	uint8_t* p;
	for (h=0, p=(uint8_t*)key; *p; p++){
		h = 31 * h  + *p;
	}
	lua_pushinteger(L, h);
	return 1;
}

int
luaopen_hcrypt(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "string_hash", lstring_hash },
		{ NULL, NULL },
	};
	luaL_newlib(L, l);
	return 1;
}