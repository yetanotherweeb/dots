from typing import Any
from kitty.boss import Boss # type: ignore
from kitty.window import Window # type: ignore

def set_padding(window: Window, in_editor: str):
    """in_editor değerine göre padding ayarla"""
    padding = 0 if in_editor.lower() == "true" else 8
    window.set_padding(padding, padding, padding, padding)

def on_set_user_var(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    if data["key"] == "in_editor":
        set_padding(window, data["value"])

def on_focus_change(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    # Yeni açılan pencere, fokus alırken de padding kontrolü yap
    in_editor = window.user_vars.get("in_editor", "false")
    set_padding(window, in_editor)
