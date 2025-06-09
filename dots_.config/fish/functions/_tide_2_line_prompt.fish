function _tide_2_line_prompt
    set -g add_prefix
    _tide_side=left for item in $_tide_left_items
        _tide_item_$item
    end
    if not set -e add_prefix
        set_color $prev_bg_color -b normal
        echo $tide_left_prompt_suffix
    end

    echo

    set -g add_prefix
    _tide_side=right for item in $_tide_right_items
        _tide_item_$item
    end
    if not set -e add_prefix
        set_color $prev_bg_color -b normal
        echo $tide_right_prompt_suffix
    end
end

function _tide_item_pwd
    _tide_print_item pwd @PWD@
end

function _tide_item_newline
    set_color $prev_bg_color -b normal
    v=tide_"$_tide_side"_prompt_suffix echo $$v
    set -g add_prefix
end

function _tide_item_context
    if set -q SSH_TTY
        set -fx tide_context_color $tide_context_color_ssh
    else if test "$EUID" = 0
        set -fx tide_context_color $tide_context_color_root
    else if test "$tide_context_always_display" = true
        set -fx tide_context_color $tide_context_color_default
    else
        return
    end

    string match -qr "^(?<h>(\.?[^\.]*){0,$tide_context_hostname_parts})" @$hostname
    _tide_print_item context $USER$h
end
