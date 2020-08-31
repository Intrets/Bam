#include "common.h"

#include "UIOConstructActivityInterface.h"

#include "UIOList.h"
#include "UIOConstructer.h"
#include "UIOActivityInterface.h"
#include "UIOCallBackParams.h"
#include "UIOTextConstructers.h"
#include "UIOListSelection.h"

UIOConstructer<UIOList> CONSTRUCTER::constructActivityInteractor(UIOActivityInterface*& interfacePtr) {
	// Resulting outermost container
	UIOList* outerListPtr;

	auto outerList = UIOConstructer<UIOList>::makeConstructer(UIOList::DIR::UP);
	outerList.setPtr(outerListPtr);

	// Hidden functionality
	outerListPtr->addElement(
		UIOConstructer<UIOActivityInterface>::makeConstructer()
		.setPtr(interfacePtr)
		.get()
	);

	// Display of the selected activity in the UIOActivityInterface
	using PairType = std::pair<int32_t, ManagedReference<Activity, Activity>>;

	auto treeDisplay =
		UIOConstructer<UIOListSelection<PairType>>::makeConstructer(
			[](PairType const& e)
	{
		if (e.second.isValid()) {
			std::stringstream out;
			out << std::string(e.first, ' ') << "id " << e.second.get()->selfHandle << " type " << e.second.get()->getTypeName();
			return out.str();
		}
		else {
			return std::string(e.first, ' ') + "invalid";
		}
	})
		.addGameWorldBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [interfacePtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		// interact?
		switch (interfacePtr->getType()) {
			case UIOActivityInterface::USER_ACTION_TYPE::HOVERING:
				{
					interfacePtr->interact(params.gameState, params.uiState.getCursorPositionWorld());
				}
				break;
			case UIOActivityInterface::USER_ACTION_TYPE::NOTHING:
				{
					if (auto const& maybeTarget = params.gameState.staticWorld.getActivity(params.uiState.getCursorPositionWorld())) {
						auto self = static_cast<UIOListSelection<PairType>*>(self_);

						auto const& target = maybeTarget.value();
						auto const& base = maybeTarget.value().get()->getRoot();

						interfacePtr->setBase(base);
						interfacePtr->setTarget(target);

						std::vector<std::pair<int32_t, Activity*>> members;
						base.get()->getTreeMembersDepths(members, 0);
						std::vector<std::pair<int32_t, ManagedReference<Activity, Activity>>> membersManaged;

						int32_t i = 0;
						int32_t index = 0;
						for (auto [depth, ptr] : members) {
							if (ptr->selfHandle == target.handle) {
								index = i;
							}
							membersManaged.push_back({ depth, ManagedReference<Activity, Activity>(ptr->selfHandle) });
							i++;
						}

						self->setList(membersManaged);
						self->setSelected(index);
					}
					else {
						interfacePtr->cancel(true);
					}
				}
				break;
			default:
				break;
		}

		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	})
		.addOnHoverBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [interfacePtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOListSelection<PairType>*>(self_);

		if (auto const& maybeSelected = self->getSelected()) {
			auto const& [depth, selected] = *maybeSelected.value();

			if (selected.isValid()) {
				interfacePtr->setTarget(selected);
			}
		}

		return BIND_RESULT::CONTINUE;
	})
		.get();

	auto disconnectButton =
		TextConstructer::constructSingleLineDisplayText(" disconnect")
		.alignCenter()
		.button()
		.onRelease([interfacePtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		interfacePtr->splitTarget();
		return BIND_RESULT::CONTINUE;
	})
		.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
		.constrainHeight(UIOSizeType(UIOSizeType::FH, 1.2f))
		.get();

	outerListPtr->addElement(std::move(disconnectButton));
	outerListPtr->addElement(std::move(treeDisplay));

	return std::move(outerList);
}
