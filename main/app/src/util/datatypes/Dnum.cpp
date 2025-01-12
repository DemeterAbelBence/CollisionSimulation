#include "Dnum.hpp"

Dnum _Sin(Dnum r) { return Dnum(glm::sin(r.val), glm::cos(r.val) * r.der); }
Dnum _Cos(Dnum r) { return Dnum(glm::cos(r.val), -glm::sin(r.val) * r.der); }
Dnum _Log(Dnum r) { return Dnum(glm::log(r.val), r.der / r.val); }
Dnum _Exp(Dnum r) { return Dnum(glm::exp(r.val), glm::exp(r.val) * r.der); }
Dnum _Sqr(Dnum r) { return Dnum(glm::sqrt(r.val), r.der / (2 * glm::sqrt(r.val))); }