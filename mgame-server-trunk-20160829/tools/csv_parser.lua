--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/8/3
-- Time: 16:53
-- Desc: csv转换工具
-- 注意事项: 在工程目录建一个table文件夹 和tools平行 将需要用到的tmx文件放到map下面 生成的文件的在fgamesvr\data下面
--


function split(string, split)
	local list = {}
	local pos = 1
	if string.find("", split, 1) then -- this would result in endless loops
	error("split matches empty string!")
	end
	while true do
		local first, last = string.find(string, split, pos)
		if first then
			table.insert(list, string.sub(string, pos, first - 1))
			pos = last + 1
		else
			table.insert(list, string.sub(string, pos))
			break
		end
	end
	return list
end

local function getRowContent(file)
	local content;
	local check = false
	local count = 0
	while true do
		local t = file:read()
		if not t then
			if count == 0 then
				check = true
			end
			break
		end

        -- 正确处理在Linux平台上， \r\n 换行符的问题
        if t:sub(#t) == '\r' then t = t:sub(1, -2) end

		if not content then
			content = t
		else
			content = content..t
		end

		local i = 1
		while true do
			local index = string.find(t, "\"", i)
			if not index then break end
			i = index + 1
			count = count + 1
		end

		if count % 2 == 0 then
			check = true
			break
		end
	end

    assert(check)

	return content and (string.gsub(content, " ", ""))
end

function loadCsvFile(filePath)
	-- 读取文件
	local alls = {}
	local file = io.open(filePath, "r")
	while true do
		local line = getRowContent(file)
		if not line then
			break
		end
		table.insert(alls, line)
	end
	--[[ 从第3行开始保存（第一行是标题，第二行是后面的行才是内容） 用二维数组保存：arr[ID][属性标题字符串] ]]
	local titles = split(alls[1], ",")
	local ID = 1
	local arrs = {}
	for i = 3, #alls, 1 do
		-- 一行中，每一列的内容,第一位为ID
		local content = split(alls[i], ",")
		ID = tonumber(content[1])
		--保存ID，以便遍历取用，原来遍历可以使用in pairs来执行，所以这个不需要了
		--table.insert(arrs, i-1, ID)
		arrs[ID] = {}
		-- 以标题作为索引，保存每一列的内容，取值的时候这样取：arrs[1].Title
		for j = 1, #titles, 1 do
			local value = content[j]
			if not value then
				value = 0
			end

			if string.match(value, "^[%d]+$") then
				arrs[ID][titles[j]] = tonumber(value)
			else
				arrs[ID][titles[j]] = value
			end
		end
	end
	return arrs
end

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

function utils.write_file(file_name, string)
	local f = assert(io.open(file_name, 'w'))
	f:write(string)
	f:close()
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
					print(v .. ",")
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

-- 解析单个csv文件
function parser_file(filename)
	local alls = loadCsvFile(filename)
	local table_sz = utils.serialize_table(alls)
	table_sz = "return " .. table_sz
	local prefix = string.match(filename, "\\([%w_]+).csv$")
	utils.write_file(".\\..\\fgamesvr\\data\\" .. prefix .. ".lua", table_sz)
end

function parser()
    local lfs = require "lfs"
    local path = lfs.currentdir() .. "\\..\\table"
	for file_name in lfs.dir(path) do
		if file_name ~= "." and file_name ~= ".." then
			local full_file_name = path .. '\\' .. file_name
			local prefix = string.match(full_file_name, "\\([%w_]+).csv$")
			if prefix and prefix ~= "pinyin" and prefix ~= "TextHelper" and prefix ~= "Creeps" then
				print(full_file_name .. " parse start!!!")
				parser_file(full_file_name)
				print(full_file_name .. " parse end!!!")
			end
		end
	end
end

local function windows_main()
    parser()
end

-- '目前看来不会用到，先不实现，需要的时候从上面复制'
local function parse_all()
    print 'parse all'
end

local function parse_file(file)
    local csv_file = '../fgamesvr/data/' .. file .. '.csv'
    print('process file: ', csv_file)
    local csv_data = loadCsvFile(csv_file)
    local data = 'return ' .. utils.serialize_table(csv_data)
    utils.write_file('../fgamesvr/data/' .. file .. '.lua', data)
end

-- Linux 版 lfs库很容易编译，暂时未使用就没有加进来
local function linux_main(...)
    local files = {...}
    if #files == 0 then
        parse_all()
    else
        for _, file in ipairs(files) do
            parse_file(file)
        end
    end
end

local function check_linux()
    if _VERSION == 'Lua 5.1' then
        return os.execute('uname') == 0
    elseif _VERSION == 'Lua 5.3' or _VERSION == 'Lua 5.2' then
        local r, s, c = os.execute('uname')
        return r and s == 'exit' and c == 0
    else
        return false
    end
end

local function main(...)
    if check_linux() then
        linux_main(...)
    else
        windows_main()
    end
end

main(...)
