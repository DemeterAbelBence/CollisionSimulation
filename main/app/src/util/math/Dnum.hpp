#pragma once

#include <glm.hpp>

struct Dnum {
	float val;
	glm::vec3 der;

	Dnum(float _val, glm::vec3 _der = glm::vec3(0)) { val = _val, der = _der; }
	Dnum operator+(Dnum r) { return Dnum(val + r.val, der + r.der); }
	Dnum operator*(Dnum r) { return Dnum(val * r.val, val * r.der + der * r.val); }
	Dnum operator/(Dnum r) { return Dnum(val / r.val, (der * r.val - val * r.der) / r.val / r.val); }
};

Dnum _Sin(Dnum r);
Dnum _Cos(Dnum r);
Dnum _Log(Dnum r);
Dnum _Exp(Dnum r);
Dnum _Sqr(Dnum r);