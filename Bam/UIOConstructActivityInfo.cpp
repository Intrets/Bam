#include "common.h"

#include "UIOConstructActivityInfo.h"

#include "UIOListSelection.h"
#include "Activity.h"
#include "UIOCursor.h"
#include "UIOSizeType.h"
#include "UIOButton.h"

void UIO2::constructActivityInfo(UIOCursor* cursor) {
	using PairType = std::pair<int32_t, ManagedReference<Activity, Activity>>;

	UIO2::startList(UIO::DIR::DOWN_REVERSE);

	auto listSelection = UIO2::makeEnd<UIOListSelection<PairType>>(
		[](PairType const& e)
	{
		if (e.second.isValid()) {
			std::stringstream out;
			out << std::string(e.first, ' ') << "id " << e.second.get()->selfHandle << " type: " << e.second.get()->getTypeName() << " label: " << e.second.get()->getLabel();
			return out.str();
		}
		else {
			return std::string(e.first, ' ') + "invalid";
		}
	});

	std::vector<PairType> membersManaged;
	std::vector<std::pair<int32_t, Activity*>> members;

	cursor->getTarget().get()->getRootPtr()->impl_getTreeMembersDepths(members, 0);

	int32_t i = 0;
	int32_t index = 0;
	for (auto [depth, activity] : members) {
		if (activity->selfHandle == cursor->getTarget().getHandle()) {
			index = i;
		}
		membersManaged.push_back({ depth, ManagedReference<Activity, Activity>(*activity) });
		i++;
	}

	listSelection->setList(membersManaged);
	listSelection->setSelected(index);

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::startList(UIO::DIR::LEFT);

	UIO2::constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f });
	auto labelName = UIO2::textEditSingle("");

	auto setLabel = UIO2::textButton("Set Label");
	setLabel->setOnRelease([labelName, listSelection](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto name = labelName->text.getLines()[0];
		name = name.substr(0, name.size() - 1);
		listSelection->getSelected().value()->second.get()->setLabel(name);
		listSelection->invalidateView();
		return BIND::RESULT::CONTINUE;
	});

	UIO2::endList();
	UIO2::endList();
}
