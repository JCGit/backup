--版本信息
--
--auth:diandian
--date:20160215

local version = {}

version.version_map = {
    VERSION_TRUNK = 1, --主干开发版
    VERSION_CHINA = 2, --国内正式版
    VERSION_MPLUS = 3, --台湾m+版
}

function version.get_version()
    return version.version_map.VERSION_TRUNK 
end


return version
