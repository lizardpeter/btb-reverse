# Fireworks original sound lifecycle
Original 00411C80 creates tune.wav at 004FC0AC; its original CD copy
was found in Exe/tune.wav (1,181,726 bytes), while installed copies may
place it next to the executable. 00413F10's preamble starts original
activity music index 4 (grandopening.wav, 00446DB8), plays interruptible
managed ID 861, then ID 898 after physical managed voices stop. State 14
stops the backing track before opening the global Bink; state 8 loops
tune.wav; 00413450 stops it when entering the certificate. Native output
now issues these exact original WAV and managed-ID commands, including
waiting for the second intro voice before pre-show. DirectSound hardware
playback and the real-time audio observation still require Windows tests.
