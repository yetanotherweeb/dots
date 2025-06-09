set -g fish_greeting "fastfetch"

source ~/.aliases

function run_with_in_editor_check
    set -l cmd $argv[1]
    set -e argv[1]

    set -l editors nano vim neovim micro tmux

    if contains -- $cmd $editors
        kitty @ set-user-var in_editor=true
        and command $cmd $argv
        and kitty @ set-user-var in_editor=false
    else
        command $cmd $argv
    end
end

function tmux
    run_with_in_editor_check tmux $argv
end

function vim
    run_with_in_editor_check vim $argv
end

function micro
    run_with_in_editor_check micro $argv
end

function nano
    run_with_in_editor_check nano $argv
end

function neovim
    run_with_in_editor_check neovim $argv
end


if status is-interactive
    # Commands to run in interactive sessions can go here
end
