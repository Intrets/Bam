#pragma once

class UIOBase;
class UIOTextDisplay;
class UIOButton;

namespace UIOBinds
{
	namespace Base
	{
		void activatable(UIOBase* ptr);
		void focusable(UIOBase* ptr);

		// Mouse is over UI, dont run world binds
		void blockWorldBinds(UIOBase* ptr);
	}

	namespace TextEdit
	{
		void clickSelect(UIOTextDisplay* ptr);
		void input(UIOTextDisplay* ptr);
		void inputNoLineBreaks(UIOTextDisplay* ptr);
		void backspace(UIOTextDisplay* ptr);
		void tab(UIOTextDisplay* ptr);
		void del(UIOTextDisplay* ptr);
		void down(UIOTextDisplay* ptr);
		void up(UIOTextDisplay* ptr);
		void right(UIOTextDisplay* ptr);
		void left(UIOTextDisplay* ptr);
		void viewUp(UIOTextDisplay* ptr);
		void viewDown(UIOTextDisplay* ptr);
		void wrap(UIOTextDisplay* ptr);
		void nowrap(UIOTextDisplay* ptr);
	}

	namespace Button
	{
		void close(UIOButton* ptr);
		void hide(UIOButton* ptr);
	}
}
