/*
*   Copyright (c) 2008, David Fishburn
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License version 2 or (at your option) any later version.
*
*   This module contains functions for generating tags for Ant language files.
*/

/*
*   INCLUDE FILES
*/
#include "general.h"  /* must always come first */

#include <string.h>
#include "parse.h"
#include "routines.h"
#ifdef HAVE_LIBXML
#include "options.h"
#include "read.h"
#include "selectors.h"
#endif

#ifdef HAVE_LIBXML
/*
*   FUNCTION PROTOTYPES
*/
static void antFindTagsUnderProject (xmlNode *node,
				     const struct sTagXpathRecurSpec *spec,
				     xmlXPathContext *ctx,
				     void *userData);
static void antFindTagsUnderTask (xmlNode *node,
				  const struct sTagXpathRecurSpec *spec,
				  xmlXPathContext *ctx,
				  void *userData);
static void makeTagForProjectName (xmlNode *node,
				   const struct sTagXpathMakeTagSpec *spec,
				   struct sTagEntryInfo *tag,
				   void *userData);
static void makeTagForTargetName (xmlNode *node,
				  const struct sTagXpathMakeTagSpec *spec,
				  struct sTagEntryInfo *tag,
				  void *userData);
static void makeTagWithScope (xmlNode *node,
			      const struct sTagXpathMakeTagSpec *spec,
			      struct sTagEntryInfo *tag,
			      void *userData);
#endif

#ifdef HAVE_LIBXML
typedef enum {
	K_PROJECT, K_TARGET, K_PROPERTY, K_IMPORT,
} antKind;

typedef enum {
	R_IMPORT_GENERIC,
} antAntfileRole;

static roleDesc AntAntfileRoles [] = {
        { TRUE, "imported", "imported" },
};

static kindOption AntKinds [] = {
	{ TRUE,  'p', "project",  "projects"   },
	{ TRUE,  't', "target",   "targets"    },
	{ TRUE,  'P', "property", "properties(global)" },
	{ TRUE,  'i', "antfile",  "antfiles",
	  .referenceOnly = TRUE, ATTACH_ROLES(AntAntfileRoles)},
};

enum antXpathTable {
	TABLE_MAIN, TABLE_PROJECT, TABLE_MAIN_NAME, TABLE_TARGET_NAME,
};

static tagXpathTable antXpathMainTable [] = {
	{ "///project",
	  LXPATH_TABLE_DO_RECUR,
	  { .recurSpec= { antFindTagsUnderProject } }
	},
};

static tagXpathTable antXpathProjectTable [] = {
	{ "target",
	  LXPATH_TABLE_DO_RECUR,
	  { .recurSpec= { antFindTagsUnderTask } }
	},
	{ "property/@name",
	  LXPATH_TABLE_DO_MAKE,
	  { .makeTagSpec = { K_PROPERTY, ROLE_INDEX_DEFINITION,
			     makeTagWithScope } }
	},
	{ "import/@file",
	  LXPATH_TABLE_DO_MAKE,
	  { .makeTagSpec = { K_IMPORT, R_IMPORT_GENERIC,
			     makeTagWithScope } }
	},
};

static tagXpathTable antXpathMainNameTable [] = {
	{ "@name",
	  LXPATH_TABLE_DO_MAKE,
	  { .makeTagSpec = { K_PROJECT, ROLE_INDEX_DEFINITION,
			     makeTagForProjectName } }
	},
};

static tagXpathTable antXpathTargetNameTable [] = {
	{ "@name",
	  LXPATH_TABLE_DO_MAKE,
	  { .makeTagSpec = { K_TARGET, ROLE_INDEX_DEFINITION,
			     makeTagForTargetName} }
	},
};

static tagXpathTableTable antXpathTableTable[] = {
	[TABLE_MAIN]        = { ARRAY_AND_SIZE (antXpathMainTable)       },
	[TABLE_PROJECT]     = { ARRAY_AND_SIZE (antXpathProjectTable)    },
	[TABLE_MAIN_NAME]   = { ARRAY_AND_SIZE (antXpathMainNameTable)   },
	[TABLE_TARGET_NAME] = { ARRAY_AND_SIZE (antXpathTargetNameTable) },
};

#else
static const tagRegexTable const antTagRegexTable [] = {
	{"^[ \t]*<[ \t]*project[^>]+name=\"([^\"]+)\".*", "\\1",
	 "p,project,projects", NULL},
	{"^[ \t]*<[ \t]*target[^>]+name=\"([^\"]+)\".*", "\\1",
	 "t,target,targets", NULL},
	{"^[ \t]*<[ \t]*property[^>]+name=\"([^\"]+)\".*", "\\1",
	 "P,property,property", NULL},
};
#endif

/*
*   FUNCTION DEFINITIONS
*/
#ifdef HAVE_LIBXML

static void
antFindTagsUnderProject (xmlNode *node,
			 const struct sTagXpathRecurSpec *spec __unused__,
			 xmlXPathContext *ctx,
			 void *userData __unused__)
{
	int corkIndex = SCOPE_NIL;

	findXMLTags (ctx, node,
		     antXpathTableTable + TABLE_MAIN_NAME,
		     AntKinds,
		     &corkIndex);
	findXMLTags (ctx, node,
		     antXpathTableTable + TABLE_PROJECT,
		     AntKinds,
		     &corkIndex);
}

static void antFindTagsUnderTask (xmlNode *node,
				  const struct sTagXpathRecurSpec *spec __unused__,
				  xmlXPathContext *ctx,
				  void *userData)
{
	int corkIndex = *(int *)userData;

	findXMLTags (ctx, node,
		     antXpathTableTable + TABLE_TARGET_NAME,
		     AntKinds,
		     &corkIndex);
}

static void makeTagForProjectName (xmlNode *node __unused__,
				   const struct sTagXpathMakeTagSpec *spec __unused__,
				   struct sTagEntryInfo *tag,
				   void *userData)
{
	int *corkIndex = userData;

	*corkIndex = makeTagEntry (tag);
}

static void makeTagForTargetName (xmlNode *node __unused__,
				  const struct sTagXpathMakeTagSpec *spec __unused__,
				  struct sTagEntryInfo *tag,
				  void *userData)
{
	int *corkIndex = (int *)userData;
	int parentIndex = *corkIndex;

	tag->extensionFields.scopeKind  = NULL;
	tag->extensionFields.scopeName  = NULL;
	tag->extensionFields.scopeIndex = parentIndex;

	*corkIndex = makeTagEntry (tag);
}

static void makeTagWithScope (xmlNode *node __unused__,
			      const struct sTagXpathMakeTagSpec *spec __unused__,
			      struct sTagEntryInfo *tag,
			      void *userData)
{
	tag->extensionFields.scopeKind  = NULL;
	tag->extensionFields.scopeName  = NULL;
	tag->extensionFields.scopeIndex = *(int *)userData;

	makeTagEntry (tag);
}

static void
findAntTags (void)
{
	findXMLTags (NULL, NULL, antXpathTableTable + TABLE_MAIN,
		     AntKinds,
		     NULL);
}
#endif

extern parserDefinition* AntParser (void)
{
	static const char *const extensions [] = { "build.xml", "ant",
#ifdef HAVE_LIBXML
				/* libxml based selector is needed to select a
				 * proper concrete xml parser.*/
						   "xml",
#endif
						   NULL };
	static const char *const patterns [] = { "build.xml", NULL };
	parserDefinition* const def = parserNew ("Ant");
#ifdef HAVE_LIBXML
	static selectLanguage selectors[] = { selectByDTD, NULL };
#endif
	def->extensions = extensions;
	def->patterns = patterns;
#ifdef HAVE_LIBXML
	def->kinds = AntKinds;
	def->kindCount = ARRAY_SIZE (AntKinds);
	def->parser = findAntTags;
	def->tagXpathTableTable = antXpathTableTable;
	def->tagXpathTableCount = ARRAY_SIZE (antXpathTableTable);
	def->useCork = TRUE;
	def->selectLanguage = selectors;
#else
	def->tagRegexTable = antTagRegexTable;
	def->tagRegexCount = ARRAY_SIZE (antTagRegexTable);
	def->method     = METHOD_NOT_CRAFTED|METHOD_REGEX;
#endif
	return def;
}

/* vi:set tabstop=4 shiftwidth=4: */
