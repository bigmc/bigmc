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
#ifndef _PREDICATE_H
#define _PREDICATE_H

class query_val;

typedef bool (*predfn_check_t)(node*,list<query_val*>);
typedef int (*predfn_eval_t)(node*,list<query_val*>);

#ifndef _WIN32
typedef void* DLHANDLE;
#else

#include <windows.h>

typedef HINSTANCE DLHANDLE;
#endif

class predicate {
	predfn_check_t pred_check;
	predfn_eval_t pred_eval;
	DLHANDLE handle;
public:
	predicate(string name, char *filename);
	~predicate();
	bool invoke_check(node *n, list<query_val *> params);
	int invoke_eval(node *n, list<query_val *> params);
};

// Make it a bit prettier to define predicates in modules
#ifndef _WIN32
#define PRED_EXPORT extern "C"
#else
#define PRED_EXPORT extern "C" __declspec( dllexport ) 
#endif

#define PRED_FAIL_NO_EVAL(x) do { rerror("predicate") << "Type error: pred_" << x << \
				" is of type 'bool', but is being used as type 'int'" << endl; \
				exit(1); \
				} while(0);


#define PRED_FAIL_NO_CHECK(x) do { rerror("predicate") << "Type error: pred_" << x << \
				" is of type 'int', but is being used as type 'bool'" << endl; \
				exit(1); \
				} while(0);

#define PRED_FAIL_PARAM(x,y,z)  do { rerror("predicate") << "Type error: pred_" << x << \
				"  is of type '" << y << "' but was applied as '" << \
				z << "'" << endl; \
				exit(1); \
				} while(0);


#endif
