/*
*   Copyright (c) 2000-2003, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License version 2 or (at your option) any later version.
*
*   External interface to all language parsing modules.
*
*   To add a new language parser, you need only modify this single input
*   file to add the name of the parser definition function.
*/
#ifndef CTAGS_MAIN_PARSERS_H
#define CTAGS_MAIN_PARSERS_H

#ifdef HAVE_LIBXML
#define XML_PARSER_LIST \
	DbusIntrospectParser, \
	GladeParser, \
	Maven2Parser
#else
#define XML_PARSER_LIST
#endif

/* Add the name of any new parser definition function here */
#define PARSER_LIST \
	AntParser, \
	AsmParser, \
	AspParser, \
	AwkParser, \
	BasicParser, \
	CoffeeScriptParser, \
	CParser, \
	CppParser, \
	CssParser, \
	CsharpParser, \
	CtagsParser, \
	CobolParser, \
	DParser, \
	DiffParser, \
	DosBatchParser, \
	ErlangParser, \
	FortranParser, \
	GdbinitParser, \
	GoParser, \
	HtmlParser, \
	JavaParser, \
	JavaScriptParser, \
	JsonParser, \
	LuaParser, \
	M4Parser, \
	MakefileParser, \
	MatLabParser, \
	ObjcParser, \
	OcamlParser, \
	PascalParser, \
	PerlParser, \
	Perl6Parser, \
	PhpParser, \
	PythonParser, \
	RParser, \
	RstParser, \
	RubyParser, \
	RustParser, \
	SchemeParser, \
	ShParser, \
	SmlParser, \
	SqlParser, \
	TclParser, \
	TexParser, \
	TTCNParser, \
	VeraParser, \
	VerilogParser, \
	SystemVerilogParser, \
	VhdlParser, \
	VimParser, \
	WindResParser, \
	YaccParser, \
	ZephirParser

#endif  /* CTAGS_MAIN_PARSERS_H */

/* vi:set tabstop=4 shiftwidth=4: */
