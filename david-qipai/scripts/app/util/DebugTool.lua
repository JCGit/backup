local DebugTool = {}
function DebugTool.printMyLog(a, b, c, d, e, f, g, f, g)
  local info = debug.getinfo(2)
  print("----------------" .. info.name .. "------------------")
  print("FilePosition:" .. info.source)
  print("LineNum  :" .. info.currentline)
  if a then
    print(a)
  end
  if b then
    print(b)
  end
  if c then
    print(c)
  end
  if d then
    print(d)
  end
  if e then
    print(e)
  end
  if f then
    print(f)
  end
  if g then
    print(g)
  end
  print("------------------------------------------------------")
end
return DebugTool
