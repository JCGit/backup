--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/8/3
-- Modified: 2015-12.23
-- Time: 10:08
-- Desc: tmx解析工具（保存为lua文件）

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

local xml = require "slaxdom"
local lfs = require "lfs"

-- 解析单个地图文件
function parse_pvp_map(filename, maps)

    local map = {}
    local t = xml:dom(assert(io.open(filename, 'r')):read('a'))

    -- 寻找名字为name的子节点, 不递归查找
    local function find_element_by_name(node, name)
        for _, v in ipairs(node.el) do
            if v.name == name then
                return v
            end
        end
    end

    local objectgroup = find_element_by_name(t.root, "objectgroup")
    local len = #objectgroup.el
    while len > 0 do
        local object = objectgroup.el[len]
        local p1 = object.el[1].el[1]
        if p1.attr.name == "BuildId" then
            local obj = {
                id      = tonumber(p1.attr.value),
                type    = object.attr.name,
                x       = tonumber(object.attr.x),
                y       = tonumber(object.attr.y),
                width   = tonumber(object.attr.width),
                height  = tonumber(object.attr.height),
            }
            map[obj.id] = obj
        end
        len = len - 1
    end

    local type, index = string.match(filename, "pvp_([%d]+)_([%d]+).tmx$")
    assert(type, index)
    type = tonumber(type)
    index = tonumber(index)
    if not maps[type] then
        maps[type] = {}
    end
    maps[type][index] = map
end

function parse_pvp_maps(path, output_path)
    -- 地图数据
    local maps = {}
    for file_name in lfs.dir(path) do
        if file_name ~= "." and file_name ~= ".." then
            local full_file_name = path .. file_name
            if string.match(full_file_name, "/pvp_([%d]+)_([%d]+).tmx$") then
                print("parse: " .. full_file_name)
                parse_pvp_map(full_file_name, maps)
            end
        end
    end
    utils.write_file(output_path, "return " .. utils.serialize_table(maps))
end


local function main(...)
    print '使用方法：'
    print 'lua53 linux_tmx_parser.lua 地图配置文件路径 输出lua文件路径'

    local map_path, output_path = ...
    if not map_path then
        map_path = '../../config/trunk/res/map/'
        print('process default path: ', map_path)
    else
        if not map_path:find('/$') then
            map_path = map_path .. '/'
        end
        print('process path: ', map_path)
    end
    if not output_path then
        output_path = '../fgamesvr/data/map.lua'
    end
    print('save output to: ', output_path)

    parse_pvp_maps(map_path, output_path)
end

main(...)
