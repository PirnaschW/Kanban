// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "MFC/framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include <string>
#include <vector>
#include <memory>

#include "Classes/Card.h"
#include "Classes/Column.h"
#include "Classes/Board.h"

#include "MFC/Kanban.h"
#include "MFC/MainFrm.h"
#include "MFC/ChildFrm.h"
#include "MFC/KanbanDoc.h"
#include "MFC/KanbanView.h"

#include <propkey.h>


#endif //PCH_H
