local function concat_path(args)
    local r = ''
    for i = 1, # args do
        r = r .. ';' .. args[i]
    end
    return r
end

thread      = 4
logger      = nil
harbor      = 0
start       = "main"
bootstrap   = "snlua bootstrap"

skynet_root = "../skynet/"
root        = "./"

lua_path   = concat_path {
    root.."client/?.lua",
    root.."script/common/?.lua",
    root.."lualib/?.lua",
    skynet_root.."lualib/?.lua",
}

luaservice = concat_path {
    skynet_root.."service/?.lua",
    root.."client/?.lua",
}

lua_cpath  = skynet_root.."luaclib/?.so;"..root.."luaclib/?.so"

cpath      = skynet_root.."cservice/?.so;"..root.."cservice/?.so"
lualoader  = skynet_root.."lualib/loader.lua"
snax       = skynet_root.."?.lua"
