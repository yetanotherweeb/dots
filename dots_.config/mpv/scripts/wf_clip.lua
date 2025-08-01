local is_recording = false

local function get_output_path()
    return os.getenv("HOME") .. "/clip_" .. os.date("%Y%m%d_%H%M%S") .. ".mp4"
end

mp.add_key_binding("Alt+d", "toggle-recording", function()
    if not is_recording then
        local output_path = get_output_path()
        -- Tam ekran kaydı için wf-recorder'a geometry verilmiyor, tüm ekran kaydedilir
        local args = {
            "wf-recorder",
            "--audio",
            "-f", output_path
        }

        mp.command_native_async({
            name = "subprocess",
            playback_only = false,
            detach = true,
            args = args
        }, function(success)
            if success then
                is_recording = true
            else
                mp.osd_message("Failed to start wf-recorder", 3)
            end
        end)

    else
        -- wf-recorder prosesini durdur
        mp.command_native({
            name = "subprocess",
            args = {"pkill", "-INT", "wf-recorder"}
        })
        is_recording = false
    end
end)
