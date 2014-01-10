#ifndef WARNINGS_H
#define WARNINGS_H

//Warnings against normal constructions. This file should be included as the last one.

#define WARNING(USED, INSTEAD) T::instance().report.createMacroWarning(#USED " used. Use " INSTEAD " instead."); USED

#define if WARNING(if, "IF")
#define while WARNING(while, "WHILE")

//Not defined yet
#define for WARNING(for, "WHILE")
#define switch WARNING(switch, "IF")

#define return WARNING(return, "RETURN();")
#define break WARNING(break, "BREAK();")
#define continue WARNING(continue, "CONTINUE();")

//TODO - the same thing for common data types?

#endif // WARNINGS_H
