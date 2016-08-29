local cache_num = 10
local fname = "dump.rdb"

local t = os.date("*t")

local index = 1
local indexF = io.open("_index", "r")
if indexF then
    index = indexF:read("n")
    indexF:close()
end

local name = string.format(
    "%s.bak_%d-%d%02d%02d",
    fname, index, t.year, t.month, t.day)

local cp_sc = string.format("cp -rf %s %s", fname, name)
os.execute(cp_sc)


if index > cache_num then
    local delindex = index - cache_num
    local del_sc = string.format(
        "rm -rf %s.bak_%d-*", fname, delindex)
    os.execute(del_sc)
end


index = index + 1
indexF = io.open("_index", "w")
indexF:write(index)
indexF:close()
