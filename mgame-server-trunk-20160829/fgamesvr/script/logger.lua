local skynet = require "skynet"
local utils  = require 'utils'

local config = {}

config.log_level = 1 -- 1:trace 2:debug 3:log 4:warning 5:error

local logger = {
    LEVEL_TRACE = 1,
    LEVEL_DEBUG = 2,
    LEVEL_LOG = 3,
    LEVEL_WARNING = 4,
    LEVEL_ERROR = 5,
}

local dstr, lstr, wstr, estr
local level

function logger.level(l)
    level = l
end

logger.level(tonumber(config.log_level) or logger.LEVEL_LOG)

local function write(...)
   skynet.error("[" .. utils.time_format("%H:%M:%S",skynet.time())  .. "][" .. skynet.self() .. "]", ...)
end

function logger.trace(...)
    if level <= 1 then
        write(dstr, ...)
    end
end

function logger.tracef(...)
    if level <= 1 then
        write(dstr, string.format(...))
    end
end

function logger.debug(...)
    if level <= 2 then
        write(dstr, ...)
    end
end

function logger.debugf(...)
    if level <= 2 then
        write(dstr, string.format(...))
    end
end

function logger.log(...)
    if level <= 3 then
        write(lstr, ...)
    end
end

function logger.logf(...)
    if level <= 3 then
        write(lstr, string.format(...))
    end
end

function logger.warning(...)
    if level <= 4 then
        write(wstr, ...)
    end
end

function logger.warningf(...)
    if level <= 4 then
        write(wstr, string.format(...))
    end
end

function logger.error(...)
    if level <= 5 then
        write(estr, ...)
    end
end

function logger.errorf(...)
    if level <= 5 then
        write(estr, string.format(...))
    end
end

function logger.name(name)
    dstr = "[" .. name .. "]"
    lstr = "[" .. name .. "]"
    wstr = "[" .. name .. "]"
    estr = "[" .. name .. "]"
end

local table_concat = table.concat
local table_insert = table.insert
local string_rep = string.rep

local type = type
local pairs = pairs
local tostring = tostring
local next = next

function logger.dump(root)
    if type(root) ~= "table" then
        return tostring(root)
    end

    local cache = { [root] = "." }
    local function _dump(t, space, name)
        local temp = {}
        for k, v in pairs(t) do
            local key = tostring(k)
            if cache[v] then
                table_insert(temp, "+" .. key .. " {" .. cache[v] .. "}")
            elseif type(v) == "table" then
                local newkey = name .. "." .. key
                cache[v] = newkey
                table_insert(temp, "+" .. key .. _dump(v, space .. (next(t, k) and "|" or " ") .. string_rep(" ", #key), newkey))
            else
                table_insert(temp, "+" .. key .. " [" .. tostring(v) .. "]")
            end
        end
        return table_concat(temp, "\n" .. space)
    end

    return _dump(root, "", "")
end

logger.name(SERVICE_NAME)

return logger
