from typing import Any
from kitty.boss import Boss  # type: ignore

def on_load(boss: Boss, data: dict[str, Any]) -> None:
    with open("/tmp/kitty_watcher_log.txt", "a") as f:
        f.write("on_load çalıştı!\n")
