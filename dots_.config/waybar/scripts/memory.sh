#!/usr/bin/env bash

mem_info=$(free -h | awk 'NR==2{printf "{\"total\": \"%s\", \"used\": \"%s\", \"free\": \"%s\", \"shared\": \"%s\", \"buff/cache\": \"%s\", \"available\": \"%s\"}", $2, $3, $4, $5, $6, $7}')

mem_usage=$(echo $mem_info | jq -r '.used')
mem_total=$(echo $mem_info | jq -r '.total')
mem_free=$(echo $mem_info | jq -r '.free')
mem_shared=$(echo $mem_info | jq -r '.shared')
mem_cache=$(echo $mem_info | jq -r '."buff/cache"')
mem_available=$(echo $mem_info | jq -r '.available')

echo "{\"text\":\"  ${mem_usage}\", \"tooltip\":\"Total: ${mem_total}\nFree: ${mem_free}\nAvailable: ${mem_available}\nShared: ${mem_shared}\nBuffer/Cache: ${mem_cache}\"}"