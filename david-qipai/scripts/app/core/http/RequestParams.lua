local RequestParams = class("RequestParams")
function RequestParams:ctor(params)
  self.baseURL = params.baseURL
  self.urlPrefix = params.urlPrefix
  self.getParams = params.getParams
  self.postParams = params.postParams
  self.postData = params.postData
  self.fullURL = params.fullURL
end
return RequestParams
