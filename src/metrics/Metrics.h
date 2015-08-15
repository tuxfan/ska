/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Metrics function definitions.
 *----------------------------------------------------------------------------*/

#ifndef Metrics_h
#define Metrics_h

#include <Statistics.hh>

namespace ska {

namespace metric {

/*----------------------------------------------------------------------------*
 * Standard balance metric.
 *
 * Return the ratio of loads to flops.
 *----------------------------------------------------------------------------*/

double balance(double flops, double loads) {
	return flops/loads;
} // balance_metric

/*----------------------------------------------------------------------------*
 * Machine balance metric.
 *
 * Return the ratio of theoretical peak throughput to burst memory bandwidth.
 *----------------------------------------------------------------------------*/

double machine_balance(double throughput, double bandwidth) {
	return throughput/bandwidth;
} // balance_metric

/*----------------------------------------------------------------------------*
 * Strahler number.
 *
 * Compute the strahler number of the given directed graph.
 *----------------------------------------------------------------------------*/

double strahler_number(/* need data type */) {
	return 0.0;
} // strahler_number

} // namespace metric

} // namespace ska

#endif // Metrics_h

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/
