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
#ifndef _GLOBALS_H
#define _GLOBALS_H

struct global_config {
	bool debug;
	int verbose_level;
	unsigned long max_steps;
	unsigned long report_interval;
	char *graph_out;
	bool check_local;
	char *model_file;
	char *bigmc_home;
	int threads;
	bool print_mode;
	bool analyse_only;
	bool calculation;
};

extern global_config global_cfg; 
typedef unsigned short control;
typedef unsigned short name;

#endif
