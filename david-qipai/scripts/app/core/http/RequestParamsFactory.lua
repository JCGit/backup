local RequestParams = import(".RequestParams")
local HttpConst = import(".HttpConst")
local RequestParamsFactory = class("RequestParamsFactory")
function RequestParamsFactory:ctor()
end
function RequestParamsFactory:createRequest(bean, action, requestData)
  local url = HttpConst.REQUEST_BASE .. "/" .. bean .. "/" .. action
  local params = {}
  params.fullURL = url
  params.postParams = requestData
  if g.vars.user ~= nil then
    requestData.uid = g.vars.user.uid
    requestData.loginKey = g.vars.user.loginKey
  end
  local requestParams = RequestParams.new(params)
  requestParams.dataAnalysisApiKey = bean .. "/" .. action
  return requestParams
end
function RequestParamsFactory:assembleRequestURL(bean, action)
  local url = HttpConst.REQUEST_BASE .. "/" .. bean .. "/" .. action
  return url
end
function RequestParamsFactory:createGETRequest(url, requestData, dataAnalysisApiKey)
  local params = {}
  params.fullURL = url
  params.getParams = requestData
  local requestParams = RequestParams.new(params)
  requestParams.dataAnalysisApiKey = dataAnalysisApiKey
  return requestParams
end
return RequestParamsFactory
