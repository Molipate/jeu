#pragma once
#include "UniteArmee.h"

class UniteArmeeInfanterie : public UniteArmee {
public:
	UniteArmeeInfanterie(std::string _nom, std::string _desc, int _ressMax, int _deplacementMax, int _champVision,
		int _munMax, int _rangeMax, int _rangeMin, Arme principale, Arme secondaire);
	bool isInfanterie();
};