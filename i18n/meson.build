# SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
# SPDX-License-Identifier: GPL-3.0-or-later

languages = [
    'es_MX',
    'pt_BR'
]

i18n_feat = get_option('translations')
msgfmt = find_program('msgfmt', required: i18n_feat)

if msgfmt.found() and i18n_feat.allowed()
    meson.add_install_script(
        'gettext.post-install',
        get_option('prefix') / get_option('localedir'),
        textdomain, languages)
endif
