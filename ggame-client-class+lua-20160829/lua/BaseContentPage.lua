local BasePage = require("BasePage")

local BaseContentPage = {}
setmetatable(BaseContentPage,BasePage)

function BaseContentPage:new(new)
    new = new or {}
    self.__index = self
    setmetatable(new,self)
    new.super = self
    return new
end

function BaseContentPage:onFunction(eventName, container)
    if eventName == "luaRefreshItemView" then
		self:onRefreshItemView(container);
		return
	end	
	
	if self[eventName]~=nil then
	    self[eventName](container)
	end
end

function BaseContentPage:onRefreshItemView(container)
end


return BaseContentPage