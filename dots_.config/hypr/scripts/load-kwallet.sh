#!/usr/bin/env bash

sleep 3 && /usr/lib/pam_kwallet_init --no-startup-id

notify-send 'Kwallet has been loaded.'
