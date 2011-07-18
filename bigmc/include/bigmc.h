#ifndef _BIGMC_H
#define _BIGMC_H

#define VERSION "0.1-dev"

typedef unsigned short control;
typedef unsigned short name;

using namespace std;
#include <set>
#include <vector>
#include <map>
#include <string>

#include "edge.h"
#include "match.h"
#include "term.h"
#include "reactionrule.h"
#include "bigraph.h"
#include "parsenode.h"
#include "mc.h"
#include "driver.h"
#include "parser.h"

void print_version();

#endif
