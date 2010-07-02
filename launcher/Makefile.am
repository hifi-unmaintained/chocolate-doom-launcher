
gamesdir = $(prefix)/games

AM_CFLAGS = -DLAUNCHER -I../setup -I../textscreen -I../src `pkg-config gtk+-2.0 --cflags`

games_PROGRAMS = chocolate-launcher

SOURCE_FILES =                                  \
    ../setup/m_argv.c ../setup/m_argv.h         \
    ../setup/execute.c ../setup/execute.h       \
    launcher.c        launcher.h                \
    gtk.c             gtk.h

if HAVE_WINDRES
chocolate_launcher_SOURCES=$(SOURCE_FILES) launcher-res.rc
else
chocolate_launcher_SOURCES=$(SOURCE_FILES)
endif

chocolate_launcher_LDFLAGS =                     \
                       `pkg-config gtk+-2.0 --libs`

.rc.o:
	$(WINDRES) $^ -o $@
%.o : %.rc
	$(WINDRES) $^ -o $@

