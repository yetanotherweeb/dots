#!/bin/bash

# timedatectl komutunun çıktısını al
output=$(timedatectl)

# 'Local time' satırını bul ve saat bilgisini çıkar
local_time=$(echo "$output" | grep "Local time" | awk '{print $5}')

# JSON formatında çıktı oluştur
json_output=$(printf '{"text": "%s"}' "$local_time")

# JSON çıktıyı ekrana yazdır
echo "$json_output"

