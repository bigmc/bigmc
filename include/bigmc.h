/*******************************************************************************
*
* Copyright (C) 2011 Gian Perrone (http://itu.dk/~gdpe)
* 
* BigMC - A bigraphical model checker (http://bigraph.org/bigmc).
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
* USA.
*********************************************************************************/
#ifndef _BIGMC_H
#define _BIGMC_H

using namespace std;
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <stdlib.h>

#include "config.h"

#include "globals.h"
#include "report.h"
#include "match.h"
#include "term.h"
#include "reactionrule.h"
#include "bigraph.h"
#include "node.h"
#include "graph.h"
#include "predicate.h"
#include "query.h"
#include "parsenode.h"
#include "subtree.h"
#include "matcher.h"
#include "analyser.h"
#include "mc.h"
#include "driver.h"
#include "parser.h"

void print_version();

#endif
