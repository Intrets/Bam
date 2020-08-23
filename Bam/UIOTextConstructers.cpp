#include "common.h"

#include "UIOTextConstructers.h"

#include "UIOCallBackParams.h"
#include "UIOConstructer.h"
#include "UIOBinds.h"

namespace TextConstructer
{
	UIOConstructer<UIOTextDisplay> constructTextEdit(std::string text) {
		auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
		auto ptr = res.get();
		ptr->text.addLine(text);

		UIOBinds::TextEdit::clickSelect(ptr);
		UIOBinds::Base::activatable(ptr);

		UIOBinds::TextEdit::up(ptr);
		UIOBinds::TextEdit::down(ptr);
		UIOBinds::TextEdit::left(ptr);
		UIOBinds::TextEdit::right(ptr);

		UIOBinds::TextEdit::input(ptr);

		UIOBinds::TextEdit::backspace(ptr);
		UIOBinds::TextEdit::del(ptr);
		UIOBinds::TextEdit::tab(ptr);

		UIOBinds::TextEdit::viewDown(ptr);
		UIOBinds::TextEdit::viewUp(ptr);

		return UIOConstructer<UIOTextDisplay>(std::move(res));
	}

	UIOConstructer<UIOTextDisplay> constructSingleLineTextEdit(std::string text) {
		auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
		auto ptr = res.get();
		ptr->text.addLine(text);

		UIOBinds::TextEdit::clickSelect(ptr);
		UIOBinds::Base::activatable(ptr);

		UIOBinds::TextEdit::left(ptr);
		UIOBinds::TextEdit::right(ptr);

		UIOBinds::TextEdit::inputNoLineBreaks(ptr);

		UIOBinds::TextEdit::backspace(ptr);
		UIOBinds::TextEdit::del(ptr);
		UIOBinds::TextEdit::tab(ptr);

		return UIOConstructer<UIOTextDisplay>(std::move(res));
	}

	UIOConstructer<UIOTextDisplay> constructDisplayText(std::string text) {
		auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(true);
		auto ptr = res.get();
		ptr->text.addLine(text);

		UIOBinds::TextEdit::viewDown(ptr);
		UIOBinds::TextEdit::viewUp(ptr);

		return UIOConstructer<UIOTextDisplay>(std::move(res));
	}

	UIOConstructer<UIOTextDisplay> constructSingleLineDisplayText(std::string text, bool shrink) {
		auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(true);
		auto ptr = res.get();
		ptr->text.setString(text);
		ptr->text.hideCursor();
		ptr->shrinkToFit = shrink;

		return UIOConstructer<UIOTextDisplay>(std::move(res));
	}
}
