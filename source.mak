# $Id$
#
# Shared macros

HEADERS = \
	args.h ctags.h debug.h entry.h general.h get.h keyword.h \
	main.h options.h parse.h parsers.h read.h routines.h sort.h \
	strlist.h vstring.h

SOURCES = \
	args.c \
	ant.c \
	asm.c \
	awk.c \
	basic.c \
	c.c \
	css.c \
	dosbatch.c \
	entry.c \
	flex.c \
	fortran.c \
	get.c \
	html.c \
	jscript.c \
	keyword.c \
	lregex.c \
	lua.c \
	main.c \
	make.c \
	objc.c \
	ocaml.c \
	options.c \
	parse.c \
	pascal.c \
	perl.c \
	php.c \
	python.c \
	read.c \
	routines.c \
	ruby.c \
	sh.c \
	sort.c \
	sql.c \
	strlist.c \
	tcl.c \
	tex.c \
	vim.c \
	yacc.c \
	vstring.c

ENVIRONMENT_HEADERS = \
    e_amiga.h e_djgpp.h e_mac.h e_msoft.h e_os2.h e_qdos.h e_riscos.h e_vms.h

ENVIRONMENT_SOURCES = \
    argproc.c mac.c qdos.c

REGEX_SOURCES = gnu_regex/regex.c

REGEX_HEADERS = gnu_regex/regex.h

OBJECTS = \
	args.$(OBJEXT) \
	ant.$(OBJEXT) \
	asm.$(OBJEXT) \
	awk.$(OBJEXT) \
	basic.$(OBJEXT) \
	c.$(OBJEXT) \
	css.$(OBJEXT) \
	cobol.$(OBJEXT) \
	dosbatch.$(OBJEXT) \
	entry.$(OBJEXT) \
	flex.$(OBJEXT) \
	fortran.$(OBJEXT) \
	get.$(OBJEXT) \
	html.$(OBJEXT) \
	jscript.$(OBJEXT) \
	keyword.$(OBJEXT) \
	lregex.$(OBJEXT) \
	lua.$(OBJEXT) \
	main.$(OBJEXT) \
	make.$(OBJEXT) \
	objc.$(OBJEXT) \
	ocaml.$(OBJEXT) \
	options.$(OBJEXT) \
	parse.$(OBJEXT) \
	pascal.$(OBJEXT) \
	perl.$(OBJEXT) \
	php.$(OBJEXT) \
	python.$(OBJEXT) \
	read.$(OBJEXT) \
	routines.$(OBJEXT) \
	ruby.$(OBJEXT) \
	sh.$(OBJEXT) \
	sort.$(OBJEXT) \
	sql.$(OBJEXT) \
	strlist.$(OBJEXT) \
	tcl.$(OBJEXT) \
	tex.$(OBJEXT) \
	vim.$(OBJEXT) \
	yacc.$(OBJEXT) \
	vstring.$(OBJEXT)
