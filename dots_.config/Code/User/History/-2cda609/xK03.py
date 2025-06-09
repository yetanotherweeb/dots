from typing import Any
from kitty.boss import Boss  # type: ignore
from kitty.window import Window  # type: ignore

def set_padding(boss: Boss, window: Window, in_editor: str) -> None:
    padding = "0" if in_editor.lower() == "true" else "8"
    boss.call_remote_control(
        window,
        (
            "set-spacing",
            f"--match=id:{window.id}",
            f"padding={padding}",
        )
    )

def on_set_user_var(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    if data["key"] == "in_editor":
        set_padding(boss, window, data["value"])

def on_focus_change(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    in_editor = window.user_vars.get("in_editor", "false")
    set_padding(boss, window, in_editor)
