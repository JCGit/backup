local FeedbackInfo = class("FeedbackInfo")
function FeedbackInfo:ctor()
end
function FeedbackInfo:requestFeedbackListData(successCallback, failCallback)
  if not self._isRequstingFeedbackList then
    self._isRequstingFeedbackList = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("feedback", "list", requestData), function(data)
      print(data)
      self._isRequstingFeedbackList = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if successCallback ~= nil then
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      logger:debug("requestFeedbackListData failCallback")
      self._isRequstingFeedbackList = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function FeedbackInfo:getPicitureFromAlbum(successCallback, failCallback)
  g.native:getHeadPic(function(success, result)
    if success then
      if g.util.functions.isFileExist(result) then
        if successCallback ~= nil then
          successCallback(result)
        end
      else
        g.ui.manager.tip:top(g.lang:getContent("feedback.uploadImageFail"))
        if failCallback ~= nil then
          failCallback()
        end
      end
    else
      if result == "nosdcard" then
        g.ui.manager.tip:top(g.lang:getContent("feedback.uploadNoSDCard"))
      else
        g.ui.manager.tip:top(g.lang:getContent("feedback.uploadImageFail"))
      end
      if failCallback ~= nil then
        failCallback()
      end
    end
  end)
end
function FeedbackInfo:uploadPic(result, successCallback, failCallback)
  if g.util.functions.isFileExist(result) then
    g.ui.manager.tip:top(g.lang:getContent("language.feedback.uploading"))
    local uploadURL = g.requestParamsFactory:assembleRequestURL("users", "upload")
    network.uploadFile(function(evt)
      if evt.name == "completed" then
        local request = evt.request
        local code = request:getResponseStatusCode()
        local dataString = request:getResponseString()
        print(dataString)
        local jsonData = json.decode(dataString)
        if jsonData.ret == 0 and jsonData.info.imgURL then
          local imgURL = jsonData.info.imgURL
          if successCallback ~= nil then
            successCallback(imgURL)
          end
          g.ui.manager.tip:top(g.lang:getContent("feedback.uploadSuccess"))
        else
          if failCallback ~= nil then
            failCallback()
          end
          g.ui.manager.tip:top(g.lang:getContent("feedback.uploadFail"))
        end
      end
    end, uploadURL, {
      fileFieldName = "upload",
      filePath = result,
      contentType = "Image/jpeg",
      extra = {
        {
          "loginKey",
          g.vars.user.loginKey
        },
        {
          "uid",
          g.vars.user.uid
        },
        {"path", "feedback"}
      }
    })
  else
    g.ui.manager.tip:top(g.lang:getContent("feedback.uploadImageFail"))
    failCallback()
  end
end
function FeedbackInfo:submitFeedbackContent(subject, content, pic, successCallback, failCallback)
  if not self._isSubmitFeedback then
    self._isSubmitFeedback = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.subject = subject
    requestData.content = content
    requestData.pic = pic
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("feedback", "add", requestData), function(data)
      print(data)
      self._isSubmitFeedback = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if successCallback ~= nil then
          successCallback()
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isSubmitFeedback = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
return FeedbackInfo
