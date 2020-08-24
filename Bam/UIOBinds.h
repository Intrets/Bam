#pragma once

class UIOBase;
class UIOTextDisplay;

namespace UIOBinds
{
	namespace Base
	{
		void activatable(UIOBase* ptr);
		void focusable(UIOBase* ptr);
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
	}
}
