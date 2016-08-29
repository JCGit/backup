--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/8/3
-- Time: 10:08
-- Desc: tmx解析工具（保存为lua文件）
-- 注意事项: 在工程目录建一个map文件夹 和tools平行 将需要用到的tmx文件放到map下面 生成的文件的在fgamesvr\data下面
--

-- tmx路径
require "LuaXml"
local lfs = require "lfs"

-- 地图数据
local maps = {}

-- 姓名数据
local names = {
	man = {
		first = {},
		last = {},
	},
	woman = {
		first = {},
		last = {},
	}
}

local map_path = lfs.currentdir() .. "\\..\\map"

local utils = {}
function utils.serialize_table(obj, lvl)
	local lua = ""
	local t = type(obj)
	if t == "number" then
		lua = lua .. obj
	elseif t == "boolean" then
		lua = lua .. tostring(obj)
	elseif t == "string" then
		lua = lua .. string.format("%q", obj)
	elseif t == "table" then
		lvl = lvl or 0
		local lvls = ('  '):rep(lvl)
		local lvls2 = ('  '):rep(lvl+1)
		lua = lua .. "{\n"
		for k, v in pairs(obj) do
			lua = lua .. lvls2
			lua = lua .. "[" .. utils.serialize_table(k,lvl+1) .. "]=" .. utils.serialize_table(v,lvl+1) .. ",\n"
		end
		local metatable = getmetatable(obj)
		if metatable ~= nil and type(metatable.__index) == "table" then
			for k, v in pairs(metatable.__index) do
				lua = lua .. "[" .. utils.serialize_table(k,lvl+1) .. "]=" .. utils.serialize_table(v,lvl+1) .. ",\n"
			end
		end
		lua = lua .. lvls
		lua = lua .. "}"
	elseif t == "nil" then
		return nil
	else
		print("can not serialize a " .. t .. " type.")
	end
	return lua
end

function utils.var_dump(data, max_level, prefix)
	if type(prefix) ~= "string" then
		prefix = ""
	end
	if type(data) ~= "table" then
		print(prefix .. tostring(data))
	else
		print(data)
		if max_level ~= 0 then
			local prefix_next = prefix .. "    "
			print(prefix .. "{")
			for k,v in pairs(data) do
				io.stdout:write(prefix_next .. k .. " = ")
				if type(v) ~= "table" or (type(max_level) == "number" and max_level <= 1) then
					print(v, ",")
				else
					if max_level == nil then
						utils.var_dump(v, nil, prefix_next)
					else
						utils.var_dump(v, max_level - 1, prefix_next)
					end
				end
			end
			print(prefix .. "}")
		end
	end
end

function utils.write_file(file_name, string)
	local f = assert(io.open(file_name, 'w'))
	f:write(string)
	f:close()
end

-- 解析单个地图文件
function parser_map(filename)
	local xfile = xml.load(filename)
	local map = {}
	local objectgroup = xfile:find("objectgroup")
	local len = #objectgroup
	while len > 0
	do
		for i = 1, 1, 1 do
			local object = objectgroup[len]

			if object[1][1].name ~= "BuildId" then
				len = len - 1
				break
			end

			local obj = {
				id		= tonumber(object[1][1].value),
				type	= object.name,
				x 		= tonumber(object.x),
				y 		= tonumber(object.y),
				width	= tonumber(object.width),
				height  = tonumber(object.height),
			}

			map[obj.id] = obj

			len = len - 1
		end
	end

	local type, index = string.match(filename, "\\pvp_([%d]+)_([%d]+).tmx$")
	if not type then
		print("error filename " .. filename)
		return
	end
	if not index then
		print("error filename " .. filename)
		return
	end

	type = tonumber(type)
	index = tonumber(index)
	if not maps[type] then
		maps[type] = {}
	end
	maps[type][index] = map
end

function parser_plist(filename)
	local xfile = xml.load(filename)
	local sex, index = string.match(filename, "\\([%w]+)_([%w]+)_name.plist$")

	if not sex then
		print("error filename " .. filename)
		return
	end
	if not index then
		print("error filename " .. filename)
		return
	end

	--local csv_str = "key,string\n索引,名字\n"

	local dict = xfile:find("dict")
	local len = #dict
	while len > 0
	do
		local order = dict[len-1][1]
		local name	= dict[len][1]
		table.insert(names[sex][index], name)
		len = len - 2
		--csv_str = csv_str .. order .. "," .. name .. "\n"
	end

	--utils.write_file(".\\..\\fgamesvr\\data\\" .. sex .. "_" .. index .. "_" .. "name.csv", csv_str)
end

function parser(path)
	for file_name in lfs.dir(path) do
		if file_name ~= "." and file_name ~= ".." then
			local full_file_name = path .. '\\' .. file_name
			print(full_file_name .. " parse start!!!")
			if string.match(full_file_name, "\\pvp_([%d]+)_([%d]+).tmx$") then
				parser_map(full_file_name)
			elseif string.match(full_file_name, ".+.plist$") then
				parser_plist(full_file_name)
			end
			print(full_file_name .. " parse end!!!")
		end
	end

	-- 写文件
	local map_sz = utils.serialize_table(maps)
	map_sz = "return " .. map_sz
	utils.write_file(".\\..\\fgamesvr\\data\\map.lua", map_sz)

	--注掉这段代码是为了不再修改names.lua文件 因为names.lua不会变
--	local names_sz = utils.serialize_table(names)
--	names_sz = "return " .. names_sz
--	utils.write_file(".\\..\\fgamesvr\\data\\names.lua", names_sz)
end

parser(map_path)

