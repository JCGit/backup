require("hex")
require("bit")
do
  local BYTE_1_HEAD = hex.to_dec("0x00")
  local BYTE_2_HEAD = hex.to_dec("0xC0")
  local BYTE_3_HEAD = hex.to_dec("0xE0")
  local BYTE_1_MASK = hex.to_dec("0x80")
  local BYTE_2_MASK = hex.to_dec("0xE0")
  local BYTE_3_MASK = hex.to_dec("0xF0")
  local TAIL_MASK = hex.to_dec("0x3F")
  local mask_tbl = {
    BYTE_3_MASK,
    BYTE_2_MASK,
    BYTE_1_MASK
  }
  local head_tbl = {
    BYTE_3_HEAD,
    BYTE_2_HEAD,
    BYTE_1_HEAD
  }
  local len_tbl = {
    [BYTE_1_HEAD] = 1,
    [BYTE_2_HEAD] = 2,
    [BYTE_3_HEAD] = 3
  }
  local function utf_read_char(utf, start)
    local head_byte = string.byte(utf, start)
    for m = 1, table.getn(mask_tbl) do
      local mask = mask_tbl[m]
      local head = bit.band(head_byte, mask)
      if head == head_tbl[m] then
        local len = len_tbl[head_tbl[m]]
        local tail_idx = start + len - 1
        local char = 0
        for i = tail_idx, start + 1, -1 do
          local tail_byte = string.byte(utf, i)
          local byte = bit.band(tail_byte, TAIL_MASK)
          if tail_idx - i > 0 then
            local sft = bit.blshift(byte, (tail_idx - i) * 6)
            char = bit.bor(char, sft)
          else
            char = byte
          end
        end
        local head_val = bit.band(head_byte, bit.bnot(mask))
        head_val = bit.blshift(head_val, (len - 1) * 6)
        char = bit.bor(head_val, char)
        return char, len
      end
    end
    error("not find proper head mask")
  end
  local print_hex = function(str)
    local cat = ""
    for i = 1, string.len(str) do
      cat = cat .. " " .. hex.to_hex(string.byte(str, i))
    end
    print(cat)
  end
  local HI_MASK = hex.to_dec("0xF0")
  local LO_MASK = hex.to_dec("0xFF")
  local function char_to_str(char)
    local hi, lo = bit.brshift(char, 8), bit.band(char, LO_MASK)
    if hi == 0 then
      return string.format("%c\000", lo)
    elseif lo == 0 then
      return string.format("\000%c", hi)
    else
      return string.format("%c%c", lo, hi)
    end
  end
  local function utf_to_uni(utf)
    local n = string.len(utf)
    local i = 1
    local uni = ""
    while n >= i do
      char, len = utf_read_char(utf, i)
      i = i + len
      uni = uni .. char_to_str(char)
    end
    return uni
  end
  utf8 = {utf_to_uni = utf_to_uni, print_hex = print_hex}
end
