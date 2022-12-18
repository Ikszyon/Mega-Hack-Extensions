#include <Windows.h>
#include <iostream>

#include "extensions2.h"

void MH_CALL OnButton(MegaHackExt::Button *obj)
{
	std::cout << "Clicked\n";
}

DWORD MainThread(LPVOID lpParam)
{
	using namespace MegaHackExt;

	Window *window = Window::Create("Window");

	Button *button = Button::Create("Right");
	button->setCallback(OnButton);

	CheckBox *checkbox = CheckBox::Create("Left");
	checkbox->setCallback([](CheckBox *obj, bool b) { std::cout << b << '\n'; });

	window->add(HorizontalLayout::Create(checkbox, button));

	Label *label = Label::Create("Label");
	window->add(label);

	Client::commit(window);

	return S_OK;
}

DWORD DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0x1000, reinterpret_cast<LPTHREAD_START_ROUTINE>(&MainThread), NULL, 0, NULL);
		break;

	default:
		break;
	}

	return TRUE;
}