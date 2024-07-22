# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no      # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no       # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
KEY_LOCK_ENABLE = yes
TAP_DANCE_ENABLE = yes
UNICODE_ENABLE = yes

LAYOUTS = 60_ansi 60_ansi_arrow 60_ansi_split_bs_rshift 60_hhkb 60_iso 60_abnt2 60_tsangan_hhkb

SRC += events/event_dispatcher.c

SRC += features/select_word.c
SRC += features/open_brackets.c
SRC += features/taunt_mode.c
SRC += features/alternate_text_mode.c
SRC += features/game_mode.c