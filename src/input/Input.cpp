#include "Input.hpp"

Demo::Input::Input()
:mousepos(0,0)
,mouseleft(false)
,escape(false)
,exit(false)
,down(false)
{
	
}

Demo::Input::~Input()
{
}

Demo::PointI Demo::Input::MousePos()
{
	return mousepos;
}

bool Demo::Input::MouseLeft()
{
	return mouseleft;
}

bool Demo::Input::Escape()
{
	return escape;
}

bool Demo::Input::Exit()
{
	return exit;
}

bool Demo::Input::Down()
{
	return down;
}
