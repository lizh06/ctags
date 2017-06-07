/*
 *
 *  Copyright (c) 2015, Red Hat, Inc.
 *  Copyright (c) 2015, Masatake YAMATO
 *
 *  Author: Masatake YAMATO <yamato@redhat.com>
 *
 *   This source code is released for free distribution under the terms of the
 *   GNU General Public License version 2 or (at your option) any later version.
 *
 */

#include "general.h"  /* must always come first */
#include "debug.h"
#include "main.h"
#include "options.h"
#include "routines.h"
#include "trashbox.h"
#include "xtag.h"

#include <string.h>
#include <ctype.h>

typedef struct sXtagObject {
	xtagDefinition *def;
	langType language;
	xtagType sibling;
} xtagObject;

static bool isPseudoTagsEnabled (xtagDefinition *pdef CTAGS_ATTR_UNUSED)
{
	return ! isDestinationStdout ();
}

static xtagDefinition xtagDefinitions [] = {
	{ true, 'F',  "fileScope",
	  "Include tags of file scope" },
	{ false, 'f', "inputFile",
	  "Include an entry for the base file name of every input file"},
	{ false, 'p', "pseudo",
	  "Include pseudo tags",
	  isPseudoTagsEnabled},
	{ false, 'q', "qualified",
	  "Include an extra class-qualified tag entry for each tag"},
	{ false, 'r', "reference",
	  "Include reference tags"},
	{ false, 'g', "guest",
	  "Include tags generated by guest parsers"},
	{ true, 's', "subparser",
	  "Include tags generated by subparsers"},
};

static unsigned int       xtagObjectUsed;
static unsigned int       xtagObjectAllocated;
static xtagObject* xtagObjects;

static xtagObject* getXtagObject (xtagType type)
{
	Assert ((0 <= type) && ((unsigned int)type < xtagObjectUsed));
	return (xtagObjects + type);
}

extern xtagDefinition* getXtagDefinition (xtagType type)
{
	Assert ((0 <= type) && ((unsigned int)type < xtagObjectUsed));

	return getXtagObject (type)->def;
}

typedef bool (* xtagPredicate) (xtagObject *pobj, langType language, const void *user_data);
static xtagType  getXtagTypeGeneric (xtagPredicate predicate, langType language, const void *user_data)
{
	static bool initialized = false;
	unsigned int i;

	if (language == LANG_AUTO && (initialized == false))
	{
		initialized = true;
		initializeParser (LANG_AUTO);
	}
	else if (language != LANG_IGNORE && (initialized == false))
		initializeParser (language);

	for (i = 0; i < xtagObjectUsed; i++)
	{
		if (predicate (xtagObjects + i, language, user_data))
			return i;
	}
	return XTAG_UNKNOWN;
}

static bool xtagEqualByLetter (xtagObject *pobj, langType language CTAGS_ATTR_UNUSED,
							   const void *user_data)
{
	return (pobj->def->letter == *((char *)user_data))? true: false;
}

extern xtagType  getXtagTypeForLetter (char letter)
{
	return getXtagTypeGeneric (xtagEqualByLetter, LANG_IGNORE, &letter);
}

static bool xtagEqualByNameAndLanguage (xtagObject *pobj, langType language, const void *user_data)
{
	const char* name = user_data;

	if ((language == LANG_AUTO || pobj->language == language)
		&& (strcmp (pobj->def->name, name) == 0))
		return true;
	else
		return false;
}

extern xtagType  getXtagTypeForNameAndLanguage (const char *name, langType language)
{
	return getXtagTypeGeneric (xtagEqualByNameAndLanguage, language, name);
}


#define PR_XTAG_WIDTH_LETTER     7
#define PR_XTAG_WIDTH_NAME      22
#define PR_XTAG_WIDTH_ENABLED   7
#define PR_XTAG_WIDTH_LANGUAGE  16
#define PR_XTAG_WIDTH_DESC      30

#define PR_XTAG_STR(X) PR_XTAG_WIDTH_##X
#define PR_XTAG_FMT(X,T) "%-" STRINGIFY(PR_XTAG_STR(X)) STRINGIFY(T)
#define MAKE_XTAG_FMT(LETTER_SPEC)		\
	PR_XTAG_FMT (LETTER,LETTER_SPEC)	\
	" "					\
	PR_XTAG_FMT (NAME,s)			\
	" "					\
	PR_XTAG_FMT (ENABLED,s)			\
	" "					\
	PR_XTAG_FMT (LANGUAGE,s)		\
	" "					\
	PR_XTAG_FMT (DESC,s)			\
	"\n"

static void printXtag (xtagType i)
{
	unsigned char letter = getXtagDefinition (i)->letter;
	langType lang;
	const char *language;
	const char *desc;

	if (letter == NUL_XTAG_LETTER)
		letter = '-';


	lang = getXtagObject (i)->language;

	if (lang == LANG_IGNORE)
		language = "NONE";
	else
		language = getLanguageName (lang);

	desc = getXtagDefinition (i)->description;
	if (!desc)
		desc = "NONE";

	printf((Option.machinable? "%c\t%s\t%s\t%s\t%s\n": MAKE_XTAG_FMT(c)),
	       letter,
		   getXtagName (i),
	       getXtagDefinition (i)->enabled? "TRUE": "FALSE",
		   language,
	       desc);
}

extern void printXtags (int language)
{
	unsigned int i;

	if (Option.withListHeader)
		printf ((Option.machinable? "%s\t%s\t%s\t%s\t%s\n": MAKE_XTAG_FMT(s)),
				"#LETTER", "NAME", "ENABLED", "LANGUAGE", "DESCRIPTION");

	for (i = 0; i < xtagObjectUsed; i++)
	{
		if (language == LANG_AUTO || getXtagOwner (i) == language)
			printXtag (i);
	}

}

extern bool isXtagEnabled (xtagType type)
{
	xtagDefinition* def = getXtagDefinition (type);

	Assert (def);

	if (def->isEnabled)
		return def->isEnabled (def);
	else
		return def->enabled;
}

extern bool enableXtag (xtagType type, bool state)
{
	bool old;
	xtagDefinition* def = getXtagDefinition (type);

	Assert (def);

	old = isXtagEnabled (type);
	def->enabled = state;
	def->isEnabled = NULL;

	return old;
}

extern bool isCommonXtag (xtagType type)
{
	return (type < XTAG_COUNT)? true: false;
}

extern int     getXtagOwner (xtagType type)
{
	return getXtagObject (type)->language;
}

const char* getXtagName (xtagType type)
{
	xtagDefinition* def = getXtagDefinition (type);
	if (def)
		return def->name;
	else
		return NULL;
}

extern void initXtagObjects (void)
{
	xtagObject *xobj;

	xtagObjectAllocated = ARRAY_SIZE (xtagDefinitions);
	xtagObjects = xMalloc (xtagObjectAllocated, xtagObject);
	DEFAULT_TRASH_BOX(&xtagObjects, eFreeIndirect);

	for (unsigned int i = 0; i < ARRAY_SIZE (xtagDefinitions); i++)
	{
		xobj = xtagObjects + i;
		xobj->def = xtagDefinitions + i;
		xobj->language = LANG_IGNORE;
		xobj->sibling = XTAG_UNKNOWN;
		xtagObjectUsed++;
	}
}

extern int countXtags (void)
{
	return xtagObjectUsed;
}

static void updateSiblingXtag (xtagType type, const char* name)
{
	int i;
	xtagObject *xobj;

	for (i = type; i > 0; i--)
	{
		xobj = xtagObjects + i - 1;
		if (xobj->def->name && (strcmp (xobj->def->name, name) == 0))
		{
			Assert (xobj->sibling == XTAG_UNKNOWN);
			xobj->sibling = type;
			break;
		}
	}
}

extern int defineXtag (xtagDefinition *def, langType language)
{
	xtagObject *xobj;
	size_t i;

	Assert (def);
	Assert (def->name);
	for (i = 0; i < strlen (def->name); i++)
	{
		Assert ( isalnum (def->name [i]) );
	}
	def->letter = NUL_XTAG_LETTER;

	if (xtagObjectUsed == xtagObjectAllocated)
	{
		xtagObjectAllocated *= 2;
		xtagObjects = xRealloc (xtagObjects, xtagObjectAllocated, xtagObject);
	}
	xobj = xtagObjects + (xtagObjectUsed);
	def->xtype = xtagObjectUsed++;
	xobj->def = def;
	xobj->language = language;
	xobj->sibling  = XTAG_UNKNOWN;

	updateSiblingXtag (def->xtype, def->name);

	verbose ("Add extra[%d]: %s,%s in %s\n",
			 def->xtype,
			 def->name, def->description,
			 getLanguageName (language));

	return def->xtype;
}

extern xtagType nextSiblingXtag (xtagType type)
{
	xtagObject *xobj;

	xobj = xtagObjects + type;
	return xobj->sibling;
}
