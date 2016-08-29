require("hex")
require("bit")
do
  local RETURN = "\r\000\n\000"
  local SMS_FILE = "\\MPAPI\\MESSAGES"
  local SMS_INBOX = "PIT_MESSAGE_INBOX"
  local SMS_OUTBOX = "PIT_MESSAGE_OUTBOX"
  local SMS_ARCHIVEBOX = "PIT_MESSAGE_ARCHIVE"
  local SMS_INBOX_DEC = "[INBOX] "
  local SMS_OUTBOX_DEC = "[OUTBOX] "
  local SMS_ARCHIVE_DEC = "[ARCHIVE] "
  local BoxType = {
    NON = 0,
    IN = 1,
    OUT = 2,
    ARCHIVE = 3
  }
  local asci_to_uni = function(asci)
    local uni = ""
    for i = 1, string.len(asci) do
      local str = string.format("%c\000", string.byte(asci, i))
      uni = uni .. str
    end
    return uni
  end
  local asci_padding = function(asci, pad)
    local uni = ""
    for i = 1, string.len(asci) do
      local str = string.format("%c", string.byte(asci, i))
      uni = uni .. str .. pad
    end
    return uni
  end
  local uni_to_asci = function(uni)
    local asci = ""
    for i = 1, string.len(uni), 2 do
      asci = asci .. string.sub(uni, i, i)
    end
    return asci
  end
  local reader = function(str)
    local index = 1
    return function(n)
      local sub = string.sub(str, index, index + n - 1)
      index = index + n
      return sub
    end
  end
  local read_number = function(read, n)
    local str = read(n)
    local rslt = 0
    for i = 1, n do
      local v = string.byte(str, i)
      rslt = bit.bor(rslt, bit.blshift(v, (i - 1) * 8))
    end
    return rslt
  end
  local function read_int(read)
    return read_number(read, 4)
  end
  local function read_short(read)
    return read_number(read, 2)
  end
  local function read_nfb_string(read)
    local len = read_int(read)
    local unistr = read(len * 2)
    return unistr
  end
  local function read_nfb_header(read)
    local nfb_header = {
      ver = read_int(read),
      firmware = read_nfb_string(read),
      phone = read_nfb_string(read),
      entries = read_int(read)
    }
    return nfb_header
  end
  local function read_nfb_file(read)
    local nfbf = {}
    nfbf.path = read_nfb_string(read)
    nfbf.nbytes = read_int(read)
    nfbf.bytes = read(nfbf.nbytes)
    local stamp = read_int(read)
    return nfbf
  end
  local function read_nfb_dir(read)
    local nfbd = {
      path = read_nfb_string(read)
    }
    return nfbd
  end
  local function save_entry(fp, tbl)
    for k, v in pairs(tbl) do
      fp:write(v)
      fp:write(RETURN)
    end
  end
  local function save_sms(fp, ctnt)
    local in_box = asci_padding(SMS_INBOX, "%z")
    local out_box = asci_padding(SMS_OUTBOX, "%z")
    local archive_box = asci_padding(SMS_ARCHIVEBOX, "%z")
    local line_s = asci_padding("1020", "%z")
    local head = asci_padding("1033", "%z")
    local tail = asci_padding("1040", "%z")
    local service_center_tail = asci_padding("1080", "%z")
    local phone_nb = "%+%z%d%z%d%z[%d%z]+"
    local phone_nb_129 = string.rep("%d%z", 11)
    local time = "[%d%z]+%-%z%d%z%d%z%-%z%d%z%d%zT%z%d%z%d%z:%z%d%z%d%z"
    local pattern = "([^\n]+)\r%z\n%z"
    local line_end = "\r%z\n%z"
    local lineb, linee = string.find(ctnt, line_end)
    local start = 1
    local line_number = 1
    while lineb and linee do
      local line = string.sub(ctnt, start, lineb - 1)
      local type = BoxType.NON
      if string.find(line, in_box) then
        fp:write(asci_to_uni(SMS_INBOX_DEC))
        type = BoxType.IN
      elseif string.find(line, out_box) then
        fp:write(asci_to_uni(SMS_OUTBOX_DEC))
        type = BoxType.OUT
      elseif string.find(line, archive_box) then
        fp:write(asci_to_uni(SMS_ARCHIVE_DEC))
        type = BoxType.ARCHIVE
      else
        io.close(fp)
        return
      end
      hb, he = string.find(line, head)
      tb, te = string.find(line, tail)
      local first_number = ""
      sb, se = string.find(line, phone_nb, tb)
      if sb and se then
        first_number = string.sub(line, sb, se)
      else
        sb, se = string.find(line, phone_nb_129, tb)
        if not sb or not se then
          first_number = "empty number"
        end
      end
      se_old = se
      sb, se = string.find(line, phone_nb, se)
      local sender_address = ""
      if sb and se then
        sender_address = string.sub(line, sb, se)
      else
        sb, se = string.find(line, phone_nb_129, se_old)
        if not sb or not se then
          sender_address = first_number
        end
      end
      fp:write(sender_address)
      fp:write(" \000")
      if type ~= BoxType.OUT and first_number ~= "empty number" then
        tmb, tme = string.find(line, time, se)
        if tmb and tme then
        else
          io.close(fp)
          error("error reading date time")
          return
        end
        fp:write(string.sub(line, tmb + 1, tme))
      end
      fp:write(RETURN)
      fp:write(string.sub(line, he + 3, tb - 3))
      fp:write(RETURN)
      fp:write(RETURN)
      start = linee + 1
      lineb, linee = string.find(ctnt, line_end, linee)
      line_number = line_number + 1
    end
  end
  local function save_nfx_to(from, too)
    local fp = io.open(too, "wb")
    if not fp then
      error("error opening file " .. too .. " to write")
      return
    end
    v = string.format("%c%c", 255, 254)
    fp:write(v)
    local nokia = io.open(from, "rb")
    if not nokia then
      error("error open file " .. from .. " to read")
    end
    local ctnt = nokia:read("*a")
    io.close(nokia)
    local read = reader(ctnt)
    local header = read_nfb_header(read)
    for i = 1, header.entries do
      local type = read_int(read)
      if type == 1 then
        local fe = read_nfb_file(read)
        if uni_to_asci(fe.path) == SMS_FILE then
          local smsctnt = fe.bytes
          save_sms(fp, smsctnt)
          return
        end
      elseif type == 2 then
        local fd = read_nfb_dir(read)
      else
        io.close(fp)
        error("unknown entry type : " .. hex.to_hex(type))
      end
    end
    io.close(fp)
  end
  noki = {save_sms = save_nfx_to}
end
