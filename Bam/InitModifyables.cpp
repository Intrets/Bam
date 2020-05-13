#include "common.h"

#include "InitModifyables.h"
#include "Piston.h"
#include "Modifyables.h"
#include "Platform.h"

void initModifyables() {
	{
		auto temp = new Modifyer<Piston>();
		Piston tempObject;
		tempObject.fillModifyingMap(*temp);
		Locator<Modifyer<Piston>>::provide(temp);

	}
	{
		auto temp = new Modifyer<Platform>();
		Platform tempObject;
		tempObject.fillModifyingMap(*temp);
		Locator<Modifyer<Platform>>::provide(temp);
	}
	{
		auto temp = new Modifyer<Anchor>();
		Anchor tempObject;
		tempObject.fillModifyingMap(*temp);
		Locator<Modifyer<Anchor>>::provide(temp);
	}
}
