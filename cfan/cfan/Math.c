#include "cfan/Math.h"
#include <math.h>
#include <stdlib.h>
#include <stddef.h>

bool cf_Math_approx(double a, double b, double tolerance) {
  double af;
  double bf;
  if (tolerance == -1) {
    af = fabs(a/1e6);
    bf = fabs(b/1e6);
    tolerance = cf_Math_min(af, bf);
  }
  return fabs(a - b) < tolerance;
}

double cf_Math_log2(double x) {
    return  log(x) * cf_Math_log2e;
}