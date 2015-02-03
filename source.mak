# Shared macros

HEADERS = \
	args.h ctags.h debug.h entry.h flags.h general.h get.h htable.h keyword.h \
	main.h options.h parse.h parsers.h pcoproc.h read.h routines.h sort.h \
	strlist.h trashbox.h vstring.h

SOURCES = \
	ada.c \
	args.c \
	ant.c \
	asm.c \
	awk.c \
	basic.c \
	c.c \
	css.c \
	dosbatch.c \
	entry.c \
	erlang.c \
	falcon.c \
	flags.c \
	flex.c \
	fortran.c \
	get.c \
	html.c \
	htable.c \
	jscript.c \
	json.c \
	keyword.c \
	lregex.c \
	lua.c \
	lxcmd.c \
	main.c \
	make.c \
	matlab.c \
	objc.c \
	ocaml.c \
	options.c \
	parse.c \
	pascal.c \
	pcoproc.c \
	perl.c \
	php.c \
	python.c \
	read.c \
	routines.c \
	ruby.c \
	rust.c \
	scheme.c \
	sh.c \
	sort.c \
	sql.c \
	strlist.c \
	tcl.c \
	tex.c \
	tg.c \
	trashbox.c \
	verilog.c \
	vhdl.c \
	vim.c \
	windres.c \
	yacc.c \
	vstring.c

ENVIRONMENT_HEADERS = e_msoft.h

ENVIRONMENT_SOURCES = 

REGEX_SOURCES = gnu_regex/regex.c

REGEX_HEADERS = gnu_regex/regex.h

FNMATCH_SOURCES = fnmatch/fnmatch.c

FNMATCH_HEADERS = fnmatch/fnmatch.h

OBJECTS = \
	$(SOURCES:.c=.$(OBJEXT)) \
	$(LIBOBJS)
