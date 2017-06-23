/*
 * Generated by ./misc/optlib2c from optlib/gdbinit.ctags, Don't edit this manually.
 */
#include "general.h"
#include "parse.h"
#include "routines.h"


static void initializeGdbinitParser (const langType language)
{
	{
		kindDefinition *kdef = getLanguageKindForLetter (language, 'D');
		enableKind (kdef, false);
	}
	{
		kindDefinition *kdef = getLanguageKindForLetter (language, 'l');
		enableKind (kdef, false);
	}
}

extern parserDefinition* GdbinitParser (void)
{
	static const char *const extensions [] = {
		"gdb",
		NULL
	};

	static const char *const aliases [] = {
		NULL
	};

	static const char *const patterns [] = {
		".gdbinit",
		NULL
	};

	static kindDefinition GdbinitKindTable [] = {
		{ true, 'd', "definition", "definitions" },
		{ true, 'D', "document", "documents" },
		{ true, 't', "toplevelVariable", "toplevel variables" },
		{ true, 'l', "localVariable", "local variables" },
	};
	static tagRegexTable GdbinitTagRegexTable [] = {
		{"^#.*", "",
		"", "{exclusive}"},
		{"^define[[:space:]]+([^[:space:]]+)$", "\\1",
		"d", NULL},
		{"^document[[:space:]]+([^[:space:]]+)$", "\\1",
		"D", NULL},
		{"^set[[:space:]]+\\$([a-zA-Z0-9_]+)[[:space:]]*=", "\\1",
		"t", NULL},
		{"^[[:space:]]+set[[:space:]]+\\$([a-zA-Z0-9_]+)[[:space:]]*=", "\\1",
		"l", NULL},
	};


	parserDefinition* const def = parserNew ("gdbinit");

	def->enabled       = true;
	def->extensions    = extensions;
	def->patterns      = patterns;
	def->aliases       = aliases;
	def->method        = METHOD_NOT_CRAFTED|METHOD_REGEX;
	def->kindTable = GdbinitKindTable;
	def->kindCount = ARRAY_SIZE(GdbinitKindTable);
	def->tagRegexTable = GdbinitTagRegexTable;
	def->tagRegexCount = ARRAY_SIZE(GdbinitTagRegexTable);
	def->initialize    = initializeGdbinitParser;

	return def;
}
