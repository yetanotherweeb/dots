from typing import Any
from kitty.boss import Boss  # type: ignore
from kitty.window import Window  # type: ignore

def set_padding(boss: Boss, window: Window, padding: int):
    try:
        boss.call_remote_control(
            window,
            ("set-spacing", f"--match=id:{window.id}", f"{padding}", f"{padding}")
        )
    except Exception as e:
        with open("/tmp/kitty_padding_debug.txt", "a") as f:
            f.write(f"HATA call_remote_control: {e}\n")

def on_set_user_var(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    with open("/tmp/kitty_padding_debug.txt", "a") as f:
        f.write(f"on_set_user_var: {data}\n")

    if data["key"] == "in_editor":
        value = data.get("value", "").lower()
        padding = 0 if value == "true" else 8
        set_padding(boss, window, padding)
