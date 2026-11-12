-- =============================================================================
-- Wireshark Lua Dissector for RemoteMediaControl v2 (SOME/IP + SOME/IP-SD + SecOC)
--
-- Decodes:
--   1. SOME/IP Service Discovery (UDP port 30490, Service 0xFFFF / Method 0x8100)
--      - FindService, OfferService, StopOfferService, SubscribeEventgroup,
--        StopSubscribeEventgroup, SubscribeEventgroupAck
--      - IPv4 Endpoint & Multicast Options (IP, TCP/UDP protocol, port)
--   2. RemoteMedia Service (TCP/UDP port 30509, Service ID 0x1300)
--      - Method 0x0001: doOperation(op, freshnessValue, mac) -> bool success
--      - Method 0x0002: setVolume(volumeLevel, freshnessValue, mac) -> bool success
--      - Method 0x0BB8: getCurrentMediaInfo() -> MediaInfo
--      - Event  0x80E8: currentMediaInfo notification -> MediaInfo
--
-- Usage:
--   Option A (CLI):
--     wireshark -X lua_script:/path/to/remote_media_someip.lua
--   Option B (Plugin folder):
--     Copy/symlink into ~/.local/lib/wireshark/plugins/ (Linux)
--     or ~/.config/wireshark/plugins/ (macOS) and press Ctrl+Shift+L in Wireshark.
--
-- Display Filters:
--   remotemedia                  -- All RemoteMedia SOME/IP + SOME/IP-SD traffic
--   remotemedia.service == 0x1300 -- Only RemoteMedia service calls & events
--   remotemedia.sd               -- Only SOME/IP-SD packets
--   remotemedia.method == 0x0001 -- Only doOperation calls/responses
--   remotemedia.method == 0x0002 -- Only setVolume calls/responses
--   remotemedia.method == 0x80e8 -- Only currentMediaInfo broadcasts
-- =============================================================================

local p_rm = Proto("remotemedia", "RemoteMedia SOME/IP & SecOC Protocol")

-- Lookup tables
local SERVICE_NAMES = {
    [0x1300] = "RemoteMedia (0x1300)",
    [0xFFFF] = "SOME/IP-SD (0xFFFF)",
}

local METHOD_NAMES = {
    [0x0001] = "doOperation",
    [0x0002] = "setVolume",
    [0x0BB8] = "getCurrentMediaInfo (Getter)",
    [0x80E8] = "currentMediaInfo (Notifier Event)",
    [0x8100] = "ServiceDiscovery",
}

local MSG_TYPES = {
    [0x00] = "REQUEST",
    [0x01] = "REQUEST_NO_RETURN",
    [0x02] = "NOTIFICATION",
    [0x80] = "RESPONSE",
    [0x81] = "ERROR",
}

local RETURN_CODES = {
    [0x00] = "E_OK",
    [0x01] = "E_NOT_OK",
    [0x02] = "E_UNKNOWN_SERVICE",
    [0x03] = "E_UNKNOWN_METHOD",
    [0x04] = "E_NOT_READY",
    [0x05] = "E_NOT_REACHABLE",
    [0x06] = "E_TIMEOUT",
    [0x07] = "E_WRONG_PROTOCOL_VERSION",
    [0x08] = "E_WRONG_INTERFACE_VERSION",
    [0x09] = "E_MALFORMED_MESSAGE",
    [0x0A] = "E_WRONG_MESSAGE_TYPE",
}

local OP_TYPES = {
    [0] = "PLAY",
    [1] = "PAUSE",
    [2] = "NEXT",
    [3] = "PREVIOUS",
    [4] = "VOLUME_UP",
    [5] = "VOLUME_DOWN",
}

local SD_ENTRY_TYPES = {
    [0x00] = "FindService",
    [0x01] = "OfferService / StopOfferService",
    [0x06] = "SubscribeEventgroup / StopSubscribeEventgroup",
    [0x07] = "SubscribeEventgroupAck / Nack",
}

local L4_PROTO_NAMES = {
    [0x06] = "TCP",
    [0x11] = "UDP",
}

-- =============================================================================
-- Header Fields
-- =============================================================================
-- SOME/IP Header (16 bytes)
local f_service       = ProtoField.uint16("remotemedia.service",       "Service ID",        base.HEX, SERVICE_NAMES)
local f_method        = ProtoField.uint16("remotemedia.method",        "Method/Event ID",   base.HEX, METHOD_NAMES)
local f_length        = ProtoField.uint32("remotemedia.length",        "Length",            base.DEC)
local f_client        = ProtoField.uint16("remotemedia.client",        "Client ID",         base.HEX)
local f_session       = ProtoField.uint16("remotemedia.session",       "Session ID",        base.DEC)
local f_proto_ver     = ProtoField.uint8 ("remotemedia.proto_ver",     "Protocol Version",  base.DEC)
local f_iface_ver     = ProtoField.uint8 ("remotemedia.iface_ver",     "Interface Version", base.DEC)
local f_msg_type      = ProtoField.uint8 ("remotemedia.msg_type",      "Message Type",      base.HEX, MSG_TYPES)
local f_ret_code      = ProtoField.uint8 ("remotemedia.ret_code",      "Return Code",       base.HEX, RETURN_CODES)

-- SecOC & Method Fields
local f_op            = ProtoField.uint8 ("remotemedia.op",            "Operation",         base.DEC, OP_TYPES)
local f_vol_level     = ProtoField.uint8 ("remotemedia.volume_level",  "Volume Level",      base.DEC)
local f_freshness     = ProtoField.uint64("remotemedia.freshness",     "Freshness Value",   base.DEC)
local f_mac_len       = ProtoField.uint32("remotemedia.mac_len",       "MAC Length",        base.DEC)
local f_mac           = ProtoField.bytes ("remotemedia.mac",           "HMAC-SHA256 Tag")
local f_success       = ProtoField.bool  ("remotemedia.success",       "SecOC Accepted")

-- MediaInfo Broadcast / Getter Fields
local f_title_len     = ProtoField.uint32("remotemedia.title_len",     "Title Raw Length",  base.DEC)
local f_title         = ProtoField.string("remotemedia.title",         "Title")
local f_album_len     = ProtoField.uint32("remotemedia.album_len",     "Album Raw Length",  base.DEC)
local f_album         = ProtoField.string("remotemedia.album",         "Album")
local f_artist_len    = ProtoField.uint32("remotemedia.artist_len",    "Artist Raw Length", base.DEC)
local f_artist        = ProtoField.string("remotemedia.artist",        "Artist")
local f_info_vol      = ProtoField.uint8 ("remotemedia.info_volume",   "Current Volume",    base.DEC)
local f_info_playing  = ProtoField.bool  ("remotemedia.info_playing",  "Playing")

-- SOME/IP-SD Fields
local f_sd_flags      = ProtoField.uint8 ("remotemedia.sd.flags",      "SD Flags",          base.HEX)
local f_sd_entries_len= ProtoField.uint32("remotemedia.sd.entries_len","Entries Length",    base.DEC)
local f_sd_entry_type = ProtoField.uint8 ("remotemedia.sd.entry_type", "Entry Type",        base.HEX, SD_ENTRY_TYPES)
local f_sd_service    = ProtoField.uint16("remotemedia.sd.service",    "SD Service ID",     base.HEX)
local f_sd_instance   = ProtoField.uint16("remotemedia.sd.instance",   "SD Instance ID",    base.HEX)
local f_sd_major      = ProtoField.uint8 ("remotemedia.sd.major",      "Major Version",     base.DEC)
local f_sd_ttl        = ProtoField.uint24("remotemedia.sd.ttl",        "TTL (seconds)",     base.DEC)
local f_sd_minor      = ProtoField.uint32("remotemedia.sd.minor",      "Minor Version",     base.DEC)
local f_sd_eventgroup = ProtoField.uint16("remotemedia.sd.eventgroup", "Eventgroup ID",     base.HEX)
local f_sd_opts_len   = ProtoField.uint32("remotemedia.sd.opts_len",   "Options Length",    base.DEC)
local f_sd_opt_ip     = ProtoField.ipv4  ("remotemedia.sd.opt_ip",     "Endpoint IPv4")
local f_sd_opt_proto  = ProtoField.uint8 ("remotemedia.sd.opt_proto",  "L4 Protocol",       base.HEX, L4_PROTO_NAMES)
local f_sd_opt_port   = ProtoField.uint16("remotemedia.sd.opt_port",   "Endpoint Port",     base.DEC)

p_rm.fields = {
    f_service, f_method, f_length, f_client, f_session,
    f_proto_ver, f_iface_ver, f_msg_type, f_ret_code,
    f_op, f_vol_level, f_freshness, f_mac_len, f_mac, f_success,
    f_title_len, f_title, f_album_len, f_album, f_artist_len, f_artist,
    f_info_vol, f_info_playing,
    f_sd_flags, f_sd_entries_len, f_sd_entry_type, f_sd_service,
    f_sd_instance, f_sd_major, f_sd_ttl, f_sd_minor, f_sd_eventgroup,
    f_sd_opts_len, f_sd_opt_ip, f_sd_opt_proto, f_sd_opt_port
}

-- =============================================================================
-- Helper: Decode a CommonAPI SOME/IP UTF-8 String (4B len + 3B UTF-8 BOM + str + 1B null)
-- =============================================================================
local function decode_someip_string(tvb, offset, tree, len_field, str_field, label)
    if tvb:len() < offset + 4 then
        return offset, ""
    end
    local str_len = tvb(offset, 4):uint()
    tree:add(len_field, tvb(offset, 4))
    offset = offset + 4

    if str_len == 0 or tvb:len() < offset + str_len then
        return offset, ""
    end

    -- Skip 3-byte UTF-8 BOM (0xEF 0xBB 0xBF) if present
    local str_start = offset
    local char_len = str_len
    if char_len >= 3 and tvb(offset, 1):uint() == 0xEF
                     and tvb(offset + 1, 1):uint() == 0xBB
                     and tvb(offset + 2, 1):uint() == 0xBF then
        str_start = offset + 3
        char_len = char_len - 3
    end

    -- Exclude trailing null byte (0x00)
    if char_len >= 1 and tvb(str_start + char_len - 1, 1):uint() == 0x00 then
        char_len = char_len - 1
    end

    local val = ""
    if char_len > 0 then
        val = tvb(str_start, char_len):string()
    end
    tree:add(str_field, tvb(offset, str_len), val)
    return offset + str_len, val
end

-- =============================================================================
-- Helper: Dissect SOME/IP-SD Payload (Service 0xFFFF, Method 0x8100)
-- =============================================================================
local function dissect_someip_sd(tvb, offset, max_offset, pinfo, tree)
    if max_offset < offset + 12 then return end

    local sd_tree = tree:add(p_rm, tvb(offset, max_offset - offset), "SOME/IP Service Discovery")
    sd_tree:add(f_sd_flags, tvb(offset, 1))
    offset = offset + 4 -- 1B flags + 3B reserved

    local entries_len = tvb(offset, 4):uint()
    sd_tree:add(f_sd_entries_len, tvb(offset, 4))
    offset = offset + 4

    local entries_end = offset + entries_len
    local summary_parts = {}

    while offset + 16 <= entries_end and offset + 16 <= max_offset do
        local etype = tvb(offset, 1):uint()
        local svc   = tvb(offset + 4, 2):uint()
        local inst  = tvb(offset + 6, 2):uint()
        local major = tvb(offset + 8, 1):uint()
        local ttl   = tvb(offset + 9, 3):uint()

        local entry_label = SD_ENTRY_TYPES[etype] or string.format("Entry(0x%02x)", etype)
        if etype == 0x01 then
            entry_label = (ttl > 0) and "OfferService" or "StopOfferService"
        elseif etype == 0x06 then
            entry_label = (ttl > 0) and "SubscribeEventgroup" or "StopSubscribeEventgroup"
        elseif etype == 0x07 then
            entry_label = (ttl > 0) and "SubscribeEventgroupAck" or "SubscribeEventgroupNack"
        end

        local etree = sd_tree:add(p_rm, tvb(offset, 16),
            string.format("%s [Service=0x%04x, Instance=0x%04x, TTL=%d]", entry_label, svc, inst, ttl))
        etree:add(f_sd_entry_type, tvb(offset, 1))
        etree:add(f_sd_service,    tvb(offset + 4, 2))
        etree:add(f_sd_instance,   tvb(offset + 6, 2))
        etree:add(f_sd_major,      tvb(offset + 8, 1))
        etree:add(f_sd_ttl,        tvb(offset + 9, 3))

        if etype == 0x00 or etype == 0x01 then
            etree:add(f_sd_minor, tvb(offset + 12, 4))
            table.insert(summary_parts, string.format("%s(0x%04x:0x%04x)", entry_label, svc, inst))
        else
            local eg = tvb(offset + 14, 2):uint()
            etree:add(f_sd_eventgroup, tvb(offset + 14, 2))
            table.insert(summary_parts, string.format("%s(0x%04x:0x%04x, EG=0x%04x)", entry_label, svc, inst, eg))
        end

        offset = offset + 16
    end

    offset = entries_end
    if offset + 4 <= max_offset then
        local opts_len = tvb(offset, 4):uint()
        sd_tree:add(f_sd_opts_len, tvb(offset, 4))
        offset = offset + 4

        local opts_end = offset + opts_len
        while offset + 4 <= opts_end and offset + 4 <= max_offset do
            local opt_len  = tvb(offset, 2):uint()
            local opt_type = tvb(offset + 2, 1):uint()
            local total_opt = 3 + opt_len
            if offset + total_opt > max_offset then break end

            if (opt_type == 0x04 or opt_type == 0x14) and total_opt >= 12 then
                local ip_str = tostring(tvb(offset + 4, 4):ipv4())
                local proto  = tvb(offset + 9, 1):uint()
                local port   = tvb(offset + 10, 2):uint()
                local pname  = L4_PROTO_NAMES[proto] or string.format("0x%02x", proto)
                local otree  = sd_tree:add(p_rm, tvb(offset, total_opt),
                    string.format("IPv4 Endpoint Option: %s:%d (%s)", ip_str, port, pname))
                otree:add(f_sd_opt_ip,    tvb(offset + 4, 4))
                otree:add(f_sd_opt_proto, tvb(offset + 9, 1))
                otree:add(f_sd_opt_port,  tvb(offset + 10, 2))
            end
            offset = offset + total_opt
        end
    end

    if #summary_parts > 0 then
        pinfo.cols.info:set("SOME/IP-SD: " .. table.concat(summary_parts, ", "))
    end
end

-- =============================================================================
-- Helper: Dissect a Single SOME/IP PDU
-- =============================================================================
local function dissect_one_pdu(tvb, offset, pdu_len, pinfo, root)
    local subtree = root:add(p_rm, tvb(offset, pdu_len), "RemoteMedia SOME/IP PDU")

    local service  = tvb(offset,     2):uint()
    local method   = tvb(offset + 2, 2):uint()
    local length   = tvb(offset + 4, 4):uint()
    local client   = tvb(offset + 8, 2):uint()
    local session  = tvb(offset + 10,2):uint()
    local msg_type = tvb(offset + 14,1):uint()
    local ret_code = tvb(offset + 15,1):uint()

    local hdr = subtree:add(p_rm, tvb(offset, 16),
        string.format("Header [Service=0x%04x, Method=0x%04x, Session=%d, Type=%s]",
            service, method, session, MSG_TYPES[msg_type] or "UNKNOWN"))
    hdr:add(f_service,   tvb(offset,      2))
    hdr:add(f_method,    tvb(offset + 2,  2))
    hdr:add(f_length,    tvb(offset + 4,  4))
    hdr:add(f_client,    tvb(offset + 8,  2))
    hdr:add(f_session,   tvb(offset + 10, 2))
    hdr:add(f_proto_ver, tvb(offset + 12, 1))
    hdr:add(f_iface_ver, tvb(offset + 13, 1))
    hdr:add(f_msg_type,  tvb(offset + 14, 1))
    hdr:add(f_ret_code,  tvb(offset + 15, 1))

    local pay_offset = offset + 16
    local pay_end    = offset + pdu_len

    -- 1. SOME/IP-SD
    if service == 0xFFFF and method == 0x8100 then
        pinfo.cols.protocol:set("SOME/IP-SD")
        dissect_someip_sd(tvb, pay_offset, pay_end, pinfo, subtree)
        return
    end

    pinfo.cols.protocol:set("RemoteMedia")

    -- 2. RemoteMedia Service (0x1300)
    if service == 0x1300 then
        local payload_tree = subtree:add(p_rm, tvb(pay_offset, pay_end - pay_offset), "Payload")

        -- Method 0x0001: doOperation
        if method == 0x0001 then
            if msg_type == 0x00 and pay_end >= pay_offset + 13 then
                local op_val    = tvb(pay_offset, 1):uint()
                local op_name   = OP_TYPES[op_val] or tostring(op_val)
                local fresh_val = tvb(pay_offset + 1, 8):uint64()
                local mac_len   = tvb(pay_offset + 9, 4):uint()

                payload_tree:add(f_op,        tvb(pay_offset,     1))
                payload_tree:add(f_freshness, tvb(pay_offset + 1, 8))
                payload_tree:add(f_mac_len,   tvb(pay_offset + 9, 4))
                if pay_end >= pay_offset + 13 + mac_len then
                    payload_tree:add(f_mac, tvb(pay_offset + 13, mac_len))
                end
                pinfo.cols.info:set(string.format("[REQ] doOperation(%s) | Freshness=%s", op_name, tostring(fresh_val)))

            elseif msg_type == 0x80 and pay_end >= pay_offset + 1 then
                local ok = tvb(pay_offset, 1):uint() ~= 0
                payload_tree:add(f_success, tvb(pay_offset, 1))
                pinfo.cols.info:set(string.format("[RSP] doOperation -> %s", ok and "ACCEPTED (true)" or "REJECTED (false)"))
            end

        -- Method 0x0002: setVolume
        elseif method == 0x0002 then
            if msg_type == 0x00 and pay_end >= pay_offset + 13 then
                local vol_val   = tvb(pay_offset, 1):uint()
                local fresh_val = tvb(pay_offset + 1, 8):uint64()
                local mac_len   = tvb(pay_offset + 9, 4):uint()

                payload_tree:add(f_vol_level, tvb(pay_offset,     1))
                payload_tree:add(f_freshness, tvb(pay_offset + 1, 8))
                payload_tree:add(f_mac_len,   tvb(pay_offset + 9, 4))
                if pay_end >= pay_offset + 13 + mac_len then
                    payload_tree:add(f_mac, tvb(pay_offset + 13, mac_len))
                end
                pinfo.cols.info:set(string.format("[REQ] setVolume(%d) | Freshness=%s", vol_val, tostring(fresh_val)))

            elseif msg_type == 0x80 and pay_end >= pay_offset + 1 then
                local ok = tvb(pay_offset, 1):uint() ~= 0
                payload_tree:add(f_success, tvb(pay_offset, 1))
                pinfo.cols.info:set(string.format("[RSP] setVolume -> %s", ok and "ACCEPTED (true)" or "REJECTED (false)"))
            end

        -- Event 0x80E8 (currentMediaInfo Notifier) or Method 0x0BB8 (Getter Response)
        elseif method == 0x80E8 or method == 0x0BB8 then
            if method == 0x0BB8 and msg_type == 0x00 then
                pinfo.cols.info:set("[REQ] getCurrentMediaInfo()")
            elseif pay_end > pay_offset then
                local cur = pay_offset
                local title, album, artist
                cur, title  = decode_someip_string(tvb, cur, payload_tree, f_title_len,  f_title,  "Title")
                cur, album  = decode_someip_string(tvb, cur, payload_tree, f_album_len,  f_album,  "Album")
                cur, artist = decode_someip_string(tvb, cur, payload_tree, f_artist_len, f_artist, "Artist")

                local vol = 0
                local playing = false
                if pay_end >= cur + 2 then
                    vol     = tvb(cur,     1):uint()
                    playing = tvb(cur + 1, 1):uint() ~= 0
                    payload_tree:add(f_info_vol,     tvb(cur,     1))
                    payload_tree:add(f_info_playing, tvb(cur + 1, 1))
                end

                local prefix = (method == 0x80E8) and "[EVENT] currentMediaInfo" or "[RSP] getCurrentMediaInfo"
                pinfo.cols.info:set(string.format('%s: "%s" by %s [%s] | Vol=%d | Playing=%s',
                    prefix, title, artist, album, vol, playing and "true" or "false"))
            end
        end
    end
end

-- =============================================================================
-- Main Dissector Entry Point (supports multiple SOME/IP PDUs in one TCP segment)
-- =============================================================================
function p_rm.dissector(tvb, pinfo, root)
    local total_len = tvb:len()
    if total_len < 16 then return 0 end

    local offset = 0
    while offset + 16 <= total_len do
        local length = tvb(offset + 4, 4):uint()
        local pdu_len = 8 + length

        -- Handle TCP segmentation if a SOME/IP PDU spans multiple TCP packets
        if offset + pdu_len > total_len then
            pinfo.desegment_offset = offset
            pinfo.desegment_len = (offset + pdu_len) - total_len
            return total_len
        end

        dissect_one_pdu(tvb, offset, pdu_len, pinfo, root)
        offset = offset + pdu_len
    end

    return offset
end

-- Register on UDP 30490 (SOME/IP-SD) and TCP/UDP 30509 (RemoteMedia Service)
local udp_port_table = DissectorTable.get("udp.port")
udp_port_table:add(30490, p_rm)
udp_port_table:add(30509, p_rm)

local tcp_port_table = DissectorTable.get("tcp.port")
tcp_port_table:add(30509, p_rm)
