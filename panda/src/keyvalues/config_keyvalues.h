#ifndef __CONFIG_VIFPARSER_H__
#define __CONFIG_VIFPARSER_H__

#include "pandabase.h"

#ifdef BUILDING_VIFPARSER
#define EXPCL_VIF EXPORT_CLASS
#define EXPTP_VIF EXPORT_TEMPL
#else
#define EXPCL_VIF IMPORT_CLASS
#define EXPTP_VIF IMPORT_TEMPL
#endif // BUILDING_VIFPARSER

extern EXPCL_VIF void init_libkeyvalues();

#endif // __CONFIG_VIFPARSER_H__