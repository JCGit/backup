local TestDataModule = class("TestDataModule")
function TestDataModule:readFile(fileName)
  local path = device.writablePath .. "TestData/" .. fileName
  local file = io.open(path, "rb")
  if file then
    local content = file:read("*all")
    io.close(file)
    logger:debug("Fake Json Data" .. fileName)
    logger:debug(content)
    return content
  end
  return nil
end
return TestDataModule
