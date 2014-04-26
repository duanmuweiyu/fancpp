#include "cfan/Math.h"
#include <math.h>

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
