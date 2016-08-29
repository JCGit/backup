local json = require("framework.json")
local ChineseSize = 3
local RichLabel = class("RichLabel", function()
  return display.newLayer()
end)
function RichLabel:ctor(param, auto)
  if auto and device.platform == "android" then
    param.rowWidth = param.rowWidth - 50
  end
  param.str = param.str or "传入的字符串为nil"
  param.font = param.font or "Microsoft Yahei"
  param.fontSize = param.fontSize or 14
  param.rowWidth = param.rowWidth or 280
  param.rowSpace = param.rowSpace or -4
  self.font = param.font
  local textTab = self:initData(param.str, param.font, param.fontSize, param.rowWidth)
  self:setContentSize(CCSize(1, 1))
  local ptab, copyVar = self:automaticNewline(textTab)
  self.strNum = 0
  for k, v in pairs(ptab) do
    self.strNum = self.strNum + self:accountTextLen(v, param.fontSize)
  end
  local ocWidth = 0
  local ocRow = 1
  local ocHeight = 0
  local btn, useWidth, useHeight = 0, 0, 0
  for k, v in pairs(copyVar) do
    local params = {}
    self:tab_addDataTo(params, v)
    if params.row == ocRow then
      ocWidth = ocWidth + useWidth
    else
      ocRow = params.row
      ocWidth = 0
      ocHeight = ocHeight + useHeight + param.rowSpace
    end
    local maxsize = params.size
    local byteSize = math.floor((maxsize + 2) / ChineseSize)
    params.width = byteSize * params.breadth
    params.height = maxsize
    params.x = ocWidth
    params.y = -ocHeight
    params.scene = self
    btn, useWidth, useHeight = self:creatViewObj(params)
  end
  ocWidth = ocWidth + useWidth
  self.realWidth = ocWidth
  self.realHeight = ocHeight
  self.ptab = ptab
end
function RichLabel:initData(str, font, fontSize, rowWidth)
  local tab = self:parseString(str, {font = font, size = fontSize})
  local var = {}
  var.tab = tab
  var.width = rowWidth
  return var
end
local str_formatToNumber = function(number, num)
  local s = "%." .. num .. "f"
  return tonumber(string.format(s, number))
end
function RichLabel:accountTextLen(str, tsize)
  local font = self.font
  local list = self:comminuteText(str)
  local aLen = 0
  for k, v in pairs(list) do
    local a = string.len(v)
    local m_pobTexture = CCTexture2D:create()
    m_pobTexture:initWithString(v, font, tsize)
    local width = m_pobTexture:getContentSize().width
    a = tsize / width
    local b = str_formatToNumber(ChineseSize / a, 4)
    aLen = aLen + b
    m_pobTexture:release()
  end
  return aLen
end
function RichLabel:addDataToRenderTab(copyVar, tab, text, index, current, strLen)
  local tag = #copyVar + 1
  copyVar[tag] = {}
  self:tab_addDataTo(copyVar[tag], tab)
  copyVar[tag].text = text
  copyVar[tag].index = index
  copyVar[tag].row = current
  copyVar[tag].breadth = strLen
  copyVar[tag].tag = tag
end
function RichLabel:automaticNewline(var)
  local allTab = {}
  local copyVar = {}
  local useLen = 0
  local str = ""
  local current = 1
  for ktb, tab in ipairs(var.tab) do
    local txtTab, member = self:comminuteText(tab.text)
    local num = math.floor(var.width / math.ceil((tab.size + 2) / ChineseSize))
    if useLen > 0 then
      local remain = num - useLen
      local txtLen = self:accountTextLen(tab.text, tab.size)
      if remain >= txtLen then
        allTab[current] = allTab[current] .. tab.text
        self:addDataToRenderTab(copyVar, tab, tab.text, useLen + 1, current, txtLen)
        useLen = useLen + txtLen
        txtTab = {}
      else
        local cTag = 0
        local mstr = ""
        local sIndex = useLen + 1
        local s_Len = 0
        for k, element in pairs(txtTab) do
          local sLen = self:accountTextLen(element, tab.size)
          if num >= useLen + sLen then
            useLen = useLen + sLen
            s_Len = s_Len + sLen
            cTag = k
            mstr = mstr .. element
          else
            if 0 < string.len(mstr) then
              allTab[current] = allTab[current] .. mstr
              self:addDataToRenderTab(copyVar, tab, mstr, sIndex, current, s_Len)
            end
            current = current + 1
            useLen = 0
            str = ""
            s_Len = 0
            break
          end
        end
        for i = 1, cTag do
          table.remove(txtTab, 1)
        end
      end
    end
    for k, element in pairs(txtTab) do
      local sLen = self:accountTextLen(element, tab.size)
      if num >= useLen + sLen then
        useLen = useLen + sLen
        str = str .. element
      else
        allTab[current] = str
        self:addDataToRenderTab(copyVar, tab, str, 1, current, useLen)
        current = current + 1
        useLen = sLen
        str = element
      end
      if k == #txtTab and num >= useLen then
        allTab[current] = str
        self:addDataToRenderTab(copyVar, tab, str, 1, current, useLen)
      end
    end
  end
  return allTab, copyVar
end
function RichLabel:comminuteText(str)
  local list = {}
  local len = string.len(str)
  local i = 1
  while len >= i do
    local c = string.byte(str, i)
    local shift = 1
    if c > 0 and c <= 127 then
      shift = 1
    elseif c >= 192 and c <= 223 then
      shift = 2
    elseif c >= 224 and c <= 239 then
      shift = 3
    elseif c >= 240 and c <= 247 then
      shift = 4
    end
    local char = string.sub(str, i, i + shift - 1)
    i = i + shift
    table.insert(list, char)
  end
  return list, len
end
function RichLabel:creatViewObj(params)
  if not params.type then
    local font = params.font
    local lab = CCLabelTTF:create(params.text, font, params.size)
    if params.color then
      lab:setColor(params.color)
    end
    lab:setAnchorPoint(ccp(0, 1))
    lab:setPosition(ccp(params.x, params.y))
    lab:setTouchSwallowEnabled(false)
    params.scene:addChild(lab)
    local useWidth = lab:getBoundingBox().size.width
    local useHeight = lab:getBoundingBox().size.height
    return lab, useWidth, useHeight
  else
    local btn = cc.ui.UIPushButton.new("ui/texture/wsk.png", {scale9 = true}):setButtonSize(params.width, params.height):setButtonLabel("normal", ui.newTTFLabel({
      text = params.text,
      size = params.size,
      color = params.color,
      font = params.font
    })):onButtonPressed(function(event)
      event.target:getButtonLabel("normal"):setPosition(ccp(0, 0))
    end):onButtonClicked(function(event)
      event.target:getButtonLabel("normal"):setPosition(ccp(0, 0))
      if self.listener then
        self.listener(event.target, params)
      end
    end):onButtonRelease(function(event)
      event.target:getButtonLabel("normal"):setPosition(ccp(0, 0))
    end):align(display.LEFT_TOP, params.x, params.y):addTo(params.scene)
    btn:setButtonLabelAlignment(display.TOP_LEFT)
    btn:setTouchSwallowEnabled(false)
    local normalLab = btn:getButtonLabel("normal")
    normalLab:setPosition(ccp(0, 0))
    local useWidth = normalLab:getBoundingBox().size.width
    local useHeight = normalLab:getBoundingBox().size.height
    if params.image then
      self:imageManage(btn, params, useWidth)
    end
    return btn, useWidth, useHeight
  end
end
function RichLabel:imageManage(object, params, useWidth)
  local g = display.newSprite(params.image, 0, -4)
  g:setScaleX(useWidth / g:getContentSize().width)
  g:setScaleY(params.size / g:getContentSize().height)
  g:setAnchorPoint(ccp(0, 1))
  object:addChild(g, 1)
  object:setButtonLabelString("normal", "")
  local move1 = CCMoveBy:create(0.5, ccp(0, 2))
  local move2 = CCMoveBy:create(0.5, ccp(0, -2))
  g:runAction(CCRepeatForever:create(CCSequence:createWithTwoActions(move1, move2)))
  object.imageSprite = g
end
function RichLabel:tab_addDataTo(tab, src)
  for k, v in pairs(src) do
    tab[k] = v
  end
end
function RichLabel:strSplit(str, flag)
  local tab = {}
  while true do
    local n = string.find(str, flag)
    if n then
      local first = string.sub(str, 1, n - 1)
      str = string.sub(str, n + 1, #str)
      table.insert(tab, first)
    else
      table.insert(tab, str)
      break
    end
  end
  return tab
end
function RichLabel:parseString(str, param)
  local tb
  local s = string.find(str, "%[color")
  if not s then
    tb = json.decode(str)
  end
  if tb then
    for k, v in pairs(tb) do
      if not v.number then
        v.number = k
      end
      self:tab_addDataTo(v, param)
      if v.color then
        v.color = self:GetTextColor(v.color)
      end
    end
  else
    tb = self:parseString1(str, param)
  end
  return tb
end
function RichLabel:parseString1(str, param)
  local clumpheadTab = {}
  for w in string.gfind(str, "%b[]") do
    if string.sub(w, 2, 2) ~= "/" then
      table.insert(clumpheadTab, w)
    end
  end
  local totalTab = {}
  for k, ns in pairs(clumpheadTab) do
    do
      local tab = {}
      local tStr
      string.gsub(ns, string.sub(ns, 2, #ns - 1), function(w)
        local n = string.find(w, "=")
        if n then
          local temTab = self:strSplit(w, " ")
          for k, pstr in pairs(temTab) do
            local temtab1 = self:strSplit(pstr, "=")
            local pname = temtab1[1]
            if k == 1 then
              tStr = pname
            end
            local js = temtab1[2]
            local p = string.find(js, "[^%d.]")
            if not p then
              js = tonumber(js)
            end
            if pname == "color" then
              tab[pname] = self:GetTextColor(js)
            else
              tab[pname] = js
            end
          end
        end
      end)
      if tStr then
        local beginFind, endFind = string.find(str, "%[%/" .. tStr .. "%]")
        local endNumber = beginFind - 1
        local gs = string.sub(str, #ns + 1, endNumber)
        tab.text = gs
        str = string.sub(str, endFind + 1, #str)
        if param then
          if not tab.number then
            tab.number = k
          end
          tab.type = tab.type or 0
          self:tab_addDataTo(tab, param)
        end
        table.insert(totalTab, tab)
      end
    end
  end
  if table.nums(clumpheadTab) == 0 then
    local ptab = {}
    ptab.text = str
    if param then
      param.number = 1
      self:tab_addDataTo(ptab, param)
    end
    table.insert(totalTab, ptab)
  end
  return totalTab
end
function RichLabel:GetTextColor(xStr)
  if string.len(xStr) == 6 then
    local tmp = {}
    for i = 0, 5 do
      local str = string.sub(xStr, i + 1, i + 1)
      if str >= "0" and str <= "9" then
        tmp[6 - i] = str - "0"
      elseif str == "A" or str == "a" then
        tmp[6 - i] = 10
      elseif str == "B" or str == "b" then
        tmp[6 - i] = 11
      elseif str == "C" or str == "c" then
        tmp[6 - i] = 12
      elseif str == "D" or str == "d" then
        tmp[6 - i] = 13
      elseif str == "E" or str == "e" then
        tmp[6 - i] = 14
      elseif str == "F" or str == "f" then
        tmp[6 - i] = 15
      else
        print("Wrong color value.")
        tmp[6 - i] = 0
      end
    end
    local r = tmp[6] * 16 + tmp[5]
    local g = tmp[4] * 16 + tmp[3]
    local b = tmp[2] * 16 + tmp[1]
    return ccc3(r, g, b)
  end
  return ccc3(255, 255, 255)
end
function RichLabel:setClickEventListener(eventName)
  self.listener = eventName
end
return RichLabel
