
local ActivityFunction = {}


ActivityFunction[5] = function()
    libOS:getInstance():WeChatInit("wxd930ea5d5a258f4f")
    if libOS:getInstance():WeChatIsInstalled() then
        libOS:getInstance():WeChatOpen()
    else
        libOS:getInstance():WeChatInstall()
    end	
end


return ActivityFunction