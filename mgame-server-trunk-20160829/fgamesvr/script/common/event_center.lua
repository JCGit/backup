local string_upper = string.upper
local tostring, pairs, type = tostring, pairs, type

local EventCenter = {}

-- EventCenter使用说明：
-- 在一个服务（虚拟机）内使用
-- 不同的事件通过 事件名 来唯一区分
--
-- 产生事件源的位置：
-- EventCenter.dispatchEvent('level up', ...)
--
-- 处理事件的模块使用下面的方法来添加监听事件:
-- EventCenter.addEventListener('level up', function (...) end, tag)
--
-- 不需要监听时使用3个remove方法来取消事件监听
-- removeEventListener 参数是handle， 添加时的返回值
-- removeEventListenersByTag
-- removeEventListenersByEvent
--
-- 判断是否已经监听某事件使用
-- hasEventListener
-- 回调函数格式
-- func callback(event_name, param)  --第一个参数为事件名

local listeners_ = {}
local nextListenerHandleIndex_ = 0

function EventCenter.addEventListener(eventName, listener, tag)
    assert(type(eventName) == "string" and eventName ~= "", "bad argument")
    assert(type(listener) == 'function', 'bad argument!')

    eventName = string_upper(eventName)
    if listeners_[eventName] == nil then
        listeners_[eventName] = {}
    end

    nextListenerHandleIndex_ = nextListenerHandleIndex_ + 1
    local handle = tostring(nextListenerHandleIndex_)
    listeners_[eventName][handle] = { listener, tag }

    return handle
end

function EventCenter.dispatchEvent(eventName, ...)
    local t = listeners_[string_upper(eventName)]
    if t then
        for _, listener in pairs(t) do
            listener[1](eventName, ...) -- 这里可以使用pcall，容错性高一些
        end
    end
end

function EventCenter.removeEventListener(handleToRemove, eventName)
    if eventName then
        assert(type(eventName) == "string" and eventName ~= "", 'bad argument')
        local listenersForEvent = listeners_[string_upper(eventName)]
        if listenersForEvent[handleToRemove] then
            listenersForEvent[handleToRemove] = nil
            return true
        end
    end
    for _, listenersForEvent in pairs(listeners_) do
        if listenersForEvent[handleToRemove] then
            listenersForEvent[handleToRemove] = nil
            return true
        end
    end
    return false
end

function EventCenter.removeEventListenersByTag(tagToRemove)
    assert(tagToRemove, 'nil tag argument')
    for eventName, listenersForEvent in pairs(listeners_) do
        for handle, listener in pairs(listenersForEvent) do
            -- listener = { listener, tag }
            if listener[2] == tagToRemove then
                listenersForEvent[handle] = nil
            end
        end
    end
end

function EventCenter.removeEventListenersByEvent(eventName)
    assert(type(eventName) == "string" and eventName ~= "", "bad argument")
    listeners_[string_upper(eventName)] = nil
end

function EventCenter.removeAllEventListeners()
    listeners_ = {} -- nextListenerHandleIndex_ handle不重置
end

function EventCenter.hasEventListener(eventName)
    assert(type(eventName) == "string" and eventName ~= "", 'bad argument')

    local t = listeners_[string_upper(eventName)]
    if t == nil then
        return false
    end
    return next(t) and true or false
end

function EventCenter.dumpAllEventListeners()
    for name, listeners in pairs(listeners_) do
        print("-- event: ", name)
        for handle, listener in pairs(listeners) do
            print("--     listener, handle: ", listener[1], handle)
        end
    end
end

return EventCenter
