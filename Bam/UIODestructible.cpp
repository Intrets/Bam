#include "common.h"

#include "UIODestructible.h"
#include "UIOCallBackParams.h"

UIODestructible::UIODestructible(Handle self) {
	this->selfHandle = self;

	this->addGlobalBind({ CONTROL::KEY::EVERY_TICK }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (static_cast<UIODestructible*>(self_)->destruct) {
			return BIND::RESULT::CLOSE;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	});
}
