#include "Effect.hpp"


Demo::Effect::Effect(const std::string& effectName)
:name(effectName),persistenceFlags(0)
{
	
}

Demo::Effect::~Effect()
{
}

void Demo::Effect::Draw(const Canvas& canvas)
{
	if(effectGraphic)
		effectGraphic->Draw(canvas);
}

bool Demo::Effect::IsGraphicSet()
{
	if(effectGraphic)
		return true;
	return false;
}

void Demo::Effect::SetGraphicPosition(const PointU& pos)
{
	if(effectGraphic)
		effectGraphic->SetPosition(pos);
}

void Demo::Effect::SetGraphicSize(const PointU& siz)
{
	if(effectGraphic)
		effectGraphic->SetSize(siz);
}

bool Demo::Effect::GetFlag(PersistenceType flag)
{
	return persistenceFlags[flag];
}

void Demo::Effect::SetFlag(PersistenceType flag, bool value)
{
	persistenceFlags[flag] = value;
}

std::string Demo::Effect::GetName()
{
	return name;
}

void Demo::Effect::SetToDestroy()
{
	SetFlag(Effect::PersistenceType::Destroyed,true);
}
