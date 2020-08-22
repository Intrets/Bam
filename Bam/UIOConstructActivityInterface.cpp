#include "common.h"

#include "UIOConstructActivityInterface.h"

#include "UIOList.h"
#include "UIOConstructer.h"
#include "UIOActivityInterface.h"
#include "UIOCallBackParams.h"

UIOConstructer<UIOList> Constructer::constructActivityInteractor() {
	auto outerList = UIOConstructer<UIOList>::makeConstructer(UIOList::DIR::DOWN);
	UIOList* outerListPtr;
	outerList.setPtr(outerListPtr);

	UIOActivityInterface* interfacePtr;
	outerListPtr->addElement(
		UIOConstructer<UIOActivityInterface>::makeConstructer()
		.setPtr(interfacePtr)
		.get()
	);

	outerListPtr->addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [interfacePtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto target = params.gameState.staticWorld.getActivity(params.uiState.getCursorPositionWorld());
		if (target.has_value()) {
			interfacePtr->setBase(target.value());
		}
		return BIND_RESULT::CONTINUE;
	});

	return std::move(outerList);
}
