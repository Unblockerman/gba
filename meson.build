################################################################################
##
##  This file is part of the Hades GBA Emulator, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2021-2022 - The Hades Authors
##
################################################################################

project(
    'Hades',
    'c',
    'cpp',
    version: '0.0.1',
    license: 'GPL-2.0-only',
    default_options: ['c_std=gnu17', 'cpp_std=c++11', 'buildtype=release'],
)

incdir = include_directories('include', 'source')
cflags = [
    '-fms-extensions',
    '-Wno-microsoft-anon-tag',
    '-DHADES_VERSION="@0@"'.format(meson.project_version()),
]
ldflags = []

if get_option('static_executable')
    cflags += ['-static']
    ldflags += ['-static']
endif

if host_machine.system() == 'windows'
    winrc = import('windows').compile_resources('./resource/hades.rc' )
else
    winrc = []
endif

cc = meson.get_compiler('c')

###############################
##   External Dependencies   ##
###############################

subdir('external')

###############################
##      Common Library       ##
###############################

subdir('source/common')

###############################
## GBA Emulator Core Library ##
###############################

subdir('source/gba')

###############################
## Graphical User Interface  ##
###############################

subdir('source/platform/gui')

########################
## Hades Final Binary
########################

custom_target(
    'hades',
    output : 'hades',
    input : hades,
    command : ['cp', '@INPUT@', '@PLAINNAME@'],
    install : true,
    install_dir : 'subdir',
)