----------------------------------------------------------------------------------
--[[
	FILE:			AddCountPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	增减数量页面,如转盘商城页面用来选择兑换数量
	AUTHOR:			hgs
	CREATED:		2013-12-16
--]]
----------------------------------------------------------------------------------

require "CommonPage"

local option = {
	ccbiFile = "SlotsExchange.ccbi",
	handlerMap = {
		onClose = "onClose",
		onConfirm = "onConfirm",
		onAddNum = "onAddNum",
		onAddNum10 = "onAddNumTen",
		onReduceNum = "onReduceNum",
		onReduceNum10 = "onReduceNumTen"
	}
}
local PageName = "AddCountPage"
local AddCountPage = CommonPage.new(PageName, option)

--页面参数
local pageInfo = {
	curNum 	 = 0,
	minNum 	 = 0,
	maxNum 	 = 0,
	step	 = 10,
	title 	 = "",
	notice   = "",
	callback = nil
}

function AddCountPage.onEnter(container)
	common:setStringForLabel(container, {
		["mlmjsjuanxiantitle"] = pageInfo.title,
		["mjuanxianNote"]	   = pageInfo.notice
	})

	AddCountPage.changeNumStatus(container)
end

--刷新当前数量，刷新4个按钮点击状态
function AddCountPage.changeNumStatus(container)
	common:setStringForLabel(container, {mNum = pageInfo.curNum})

	container:getVarMenuItem("mAddButton1w"):setEnabled(pageInfo.curNum < pageInfo.maxNum)
	container:getVarMenuItem("mAddButton10w"):setEnabled(pageInfo.curNum + pageInfo.step <= pageInfo.maxNum)
	container:getVarMenuItem("mReduceButton1w"):setEnabled(pageInfo.curNum > pageInfo.minNum)
	container:getVarMenuItem("mReduceButton10w"):setEnabled(pageInfo.curNum - pageInfo.step >= pageInfo.minNum)
end

function AddCountPage.onAddNum(container)
	if pageInfo.curNum < pageInfo.maxNum then
		pageInfo.curNum = pageInfo.curNum + 1
		AddCountPage.changeNumStatus(container)
	end
end

function AddCountPage.onAddNumTen(container)
	if pageInfo.curNum + pageInfo.step <= pageInfo.maxNum then
		pageInfo.curNum = pageInfo.curNum + pageInfo.step
		AddCountPage.changeNumStatus(container)
	end
end

function AddCountPage.onReduceNum(container)
	if pageInfo.curNum > pageInfo.minNum then
		pageInfo.curNum = pageInfo.curNum - 1
		AddCountPage.changeNumStatus(container)
	end
end

function AddCountPage.onReduceNumTen(container)
	if pageInfo.curNum - pageInfo.step <= pageInfo.maxNum then
		pageInfo.curNum = pageInfo.curNum - pageInfo.step
		AddCountPage.changeNumStatus(container)
	end
end

function AddCountPage.onConfirm()
	if pageInfo.callback then
		pageInfo.callback(pageInfo.curNum)
	end
	PageManager.popPage(PageName)
end

function AddCountPage.onClose()
	PageManager.popPage(PageName)
end

--对外唯一接口
--maxNum: 数量上限, callback:回调, title:标题,
--notice: 简单提示语, minNum:数量下限, step: "++"的增幅
function PopupAddCountPage(maxNum, callback, title, notice, minNum, step)
	maxNum = tonumber(maxNum) or 0
	pageInfo = {
		curNum   = maxNum > 0 and 1 or 0,
		minNum 	 = tonumber(minNum) or 0,
		maxNum	 = maxNum,
		step 	 = tonumber(step) or 10,
		title 	 = title or "",
		notice 	 = notice or "",
		callback = callback or nil
	}
	PageManager.pushPage(PageName)
end
