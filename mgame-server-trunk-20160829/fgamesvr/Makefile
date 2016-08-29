###########Makefile#################
CC := gcc
SHARED := -fPIC --shared
LUA_CLIB_PATH ?= luaclib
CSERVICE_PATH ?= cservice

CFLAGS = -g -O2 -Wall

LUA_CLIB = log

CSERVICE = mylogger busilogger
LUA_CLIB = hcrypt pvp_rank_mgr

all : \
	$(foreach v, $(CSERVICE), $(CSERVICE_PATH)/$(v).so) \
	$(foreach v, $(LUA_CLIB), $(LUA_CLIB_PATH)/$(v).so)

$(CSERVICE_PATH)/mylogger.so : service-src/service_mylogger.c | $(CSERVICE_PATH)
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@ -I../skynet/skynet-src

$(CSERVICE_PATH)/busilogger.so : service-src/service_busilogger.c | $(CSERVICE_PATH)
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@ -I../skynet/skynet-src


$(LUA_CLIB_PATH)/hcrypt.so : luaclib-src/lua-hcrypt.c | $(LUA_CLIB_PATH)
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@ -I ../skynet/3rd/lua

$(LUA_CLIB_PATH)/pvp_rank_mgr.so : luaclib-src/pvp_rank/DelaySortArray.cpp \
                                   luaclib-src/pvp_rank/SectionCount.cpp \
                                   luaclib-src/pvp_rank/PvpRankManager.cpp \
                                   luaclib-src/pvp_rank/lua-pvp_rank_mgr.cpp | $(LUA_CLIB_PATH)
	g++ $(CFLAGS) $(SHARED) -std=c++0x $^ -o $@ -I ../skynet/3rd/lua -I ./luaclib-src/pvp_rank/
	
clean:
	rm -rf $(LUA_CLIB_PATH)/hcrypt.so
	rm -rf $(CSERVICE_PATH)/mylogger.so
	rm -rf $(CSERVICE_PATH)/busilogger.so
	rm -rf $(LUA_CLIB_PATH)/pvp_rank_mgr.so
