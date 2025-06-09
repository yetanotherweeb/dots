function _tide_item_custom_userhost
    set_color CBA6F7
    echo -n (whoami)

    set_color CDD6F4
    echo -n "@"

    set_color F9E2AF
    echo -n (hostname -s)

    set_color FFFFFF
    echo -n ": "
end
