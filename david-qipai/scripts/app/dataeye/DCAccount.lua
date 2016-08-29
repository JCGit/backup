DCAccount = {}
function DCAccount.login(...)
  DCLuaAccount:login(...)
end
function DCAccount.logout()
  DCLuaAccount:logout()
end
function DCAccount.getAccountId()
  return DCLuaAccount:getAccountId()
end
function DCAccount.setAccountType(accountType)
  DCLuaAccount:setAccountType(accountType)
end
function DCAccount.setLevel(level)
  DCLuaAccount:setLevel(level)
end
function DCAccount.setGender(gender)
  DCLuaAccount:setGender(gender)
end
function DCAccount.setAge(age)
  DCLuaAccount:setAge(age)
end
function DCAccount.setGameServer(server)
  DCLuaAccount:setGameServer(server)
end
function DCAccount.addTag(tag, subTag)
  DCLuaAccount:addTag(tag, subTag)
end
function DCAccount.removeTag(tag, subTag)
  DCLuaAccount:removeTag(tag, subTag)
end
return DCAccount
