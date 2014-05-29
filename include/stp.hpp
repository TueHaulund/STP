#ifndef STP_HPP
#define STP_HPP

//Transformations
#include "stp/core.hpp"

//Boolean reductions
#include "stp/bool_reductions/all.hpp"
#include "stp/bool_reductions/any.hpp"
#include "stp/bool_reductions/contains.hpp"
#include "stp/bool_reductions/equal.hpp"

//Filters
#include "stp/filters/drop.hpp"
#include "stp/filters/take.hpp"
#include "stp/filters/where.hpp"

//Generators
#include "stp/generators/range.hpp"
#include "stp/generators/repeat.hpp"

//Miscellaneous
#include "stp/misc/map.hpp"
#include "stp/misc/stl.hpp"
#include "stp/misc/unique.hpp"
#include "stp/misc/zip.hpp"

//Numerical reductions
#include "stp/num_reductions/avg.hpp"
#include "stp/num_reductions/count.hpp"
#include "stp/num_reductions/fold.hpp"
#include "stp/num_reductions/max.hpp"
#include "stp/num_reductions/min.hpp"
#include "stp/num_reductions/size.hpp"
#include "stp/num_reductions/sum.hpp"

//Order operations
#include "stp/order/reverse.hpp"
#include "stp/order/sort.hpp"

//Set operations
#include "stp/set/difference.hpp"
#include "stp/set/intersect.hpp"
#include "stp/set/join.hpp"

#endif