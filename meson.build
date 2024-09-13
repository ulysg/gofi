project('launcher', 'c')

sources = ['main.c', 'app_list.c', 'search_bar.c']
gtkdep = [dependency('gtk4-layer-shell-0'), dependency('libadwaita-1')]
executable('launcher', sources, dependencies: gtkdep)
