local FacebookSimulator = class("FacebookSimulator")
function FacebookSimulator:ctor()
end
function FacebookSimulator:login(callback)
end
function FacebookSimulator:logout()
  self:interfaceCall("logout", {}, "()V")
end
function FacebookSimulator:getUnauthorizedFriends(callback)
  if callback then
    callback(true, {
      {name = "a1", url = ""},
      {name = "a2", url = ""},
      {name = "a3", url = ""},
      {name = "a4", url = ""},
      {name = "a5", url = ""},
      {name = "a6", url = ""},
      {name = "a7", url = ""},
      {name = "a8", url = ""},
      {name = "a9", url = ""},
      {name = "a10", url = ""},
      {name = "a11", url = ""},
      {name = "a12", url = ""},
      {name = "a13", url = ""},
      {name = "a14", url = ""},
      {name = "a15", url = ""},
      {name = "a16", url = ""},
      {name = "a17", url = ""},
      {name = "a18", url = ""},
      {name = "a19", url = ""},
      {name = "a20", url = ""},
      {name = "a21", url = ""},
      {name = "a22", url = ""},
      {name = "a23", url = ""},
      {name = "a24", url = ""},
      {name = "a25", url = ""},
      {name = "a26", url = ""},
      {name = "a27", url = ""},
      {name = "a28", url = ""},
      {name = "a29", url = ""},
      {name = "a30", url = ""},
      {name = "a31", url = ""},
      {name = "a32", url = ""},
      {name = "a33", url = ""},
      {name = "a34", url = ""},
      {name = "a35", url = ""},
      {name = "a36", url = ""},
      {name = "a37", url = ""},
      {name = "a38", url = ""},
      {name = "a39", url = ""},
      {name = "a1=40", url = ""},
      {name = "a41", url = ""},
      {name = "a42", url = ""},
      {name = "a43", url = ""},
      {name = "a44", url = ""},
      {name = "a45", url = ""},
      {name = "a46", url = ""},
      {name = "a47", url = ""},
      {name = "a48", url = ""},
      {name = "a49", url = ""},
      {name = "a50", url = ""},
      {name = "a51", url = ""},
      {name = "a52", url = ""},
      {name = "a53", url = ""},
      {name = "a54", url = ""},
      {name = "a55", url = ""}
    })
  end
end
function FacebookSimulator:sendInvites(data, toID, title, message, callback)
  if callback then
    callback(true, nil)
  end
end
function FacebookSimulator:shareFeed(params, callback)
  self._shareFeedCallback = callback
  self:interfaceCall("shareFeed", {
    json.encode(params)
  }, "(Ljava/lang/String;)V")
end
function FacebookSimulator:updateAppRequest()
  self._updateInviteRetryTimes = 3
  self:interfaceCall("getRequestId", {}, "()V")
end
return FacebookSimulator
