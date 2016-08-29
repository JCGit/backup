require("bit")
local to_hex = function(n)
  if type(n) ~= "number" then
    error("non-number type passed in.")
  end
  if n - math.floor(n) > 0 then
    error("trying to apply bitwise operation on non-integer!")
  end
  if n < 0 then
    n = bit.tobits(bit.bnot(math.abs(n)) + 1)
    n = bit.tonumb(n)
  end
  hex_tbl = {
    "A",
    "B",
    "C",
    "D",
    "E",
    "F"
  }
  hex_str = ""
  while n ~= 0 do
    last = math.mod(n, 16)
    if last < 10 then
      hex_str = tostring(last) .. hex_str
    else
      hex_str = hex_tbl[last - 10 + 1] .. hex_str
    end
    n = math.floor(n / 16)
  end
  if hex_str == "" then
    hex_str = "0"
  end
  return "0x" .. hex_str
end
local to_dec = function(hex)
  if type(hex) ~= "string" then
    error("non-string type passed in.")
  end
  head = string.sub(hex, 1, 2)
  if head ~= "0x" and head ~= "0X" then
    error("wrong hex format, should lead by 0x or 0X.")
  end
  v = tonumber(string.sub(hex, 3), 16)
  return v
end
hex = {to_dec = to_dec, to_hex = to_hex}
