#include "common.h"

#include "UIOBase.h"

void UIOBase::addElement(UniqueReference<UIOBase, UIOBase> element) {
	elements.push_back(std::move(element));
}

int32_t UIOBase::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	int32_t maxDepth = 0;
	for (auto& element : elements) {
		maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(renderInfo, depth));
	}
	return maxDepth;
}
