#include "BattlePawn.hpp"

#include <sstream>

Demo::BattlePawn::BattlePawn()
:toDestroy(false)
,emptySpace(true)
,selfLocation("",PointU(0,0))
,pawnSize(0,0)
,dodge(20)
,pawnCooldown(0.0)
,pawnMaxCooldown(0.0)
,cooldownBar(PointU(0,0))
,pawnCooldownBar(PointU(0,0))
,bar()
{
	position = PointU(0,0);
	SetSprite("hmm");
	
	cooldownBar.SetColor(0,0,0,50);
	cooldownBar.SetRotation(270);
	
	pawnCooldownBar.SetColor(255,255,255,150);
	pawnCooldownBar.SetPercentage(0.0);
	
	UpdateAbilityDisplay();
	UpdateEffectGraphicDisplay();
}

Demo::BattlePawn::~BattlePawn()
{
}

Demo::BattlePawn::BattlePawn(const BattlePawn& pawn)
:healths(pawn.healths)
,toDestroy(false)
,emptySpace(pawn.emptySpace)
,selfLocation(pawn.selfLocation)
,displayAbilities(pawn.displayAbilities)
,pawnSize(pawn.pawnSize)
,dodge(pawn.dodge)
,pawnCooldown(pawn.pawnCooldown)
,pawnMaxCooldown(pawn.pawnMaxCooldown)
,cooldownBar(pawn.cooldownBar)
,pawnCooldownBar(pawn.pawnCooldownBar)
,bar(pawn.bar)
{
	for(auto& i : pawn.abilities)
	{
		std::unique_ptr<Ability> p(i->Clone());
		this->abilities.push_back(std::move(p));
	}
	for(auto& i : pawn.effects)
	{
		std::unique_ptr<BattleEffect> p(i->Clone());
		this->AddEffect(std::move(p));
	}
}

Demo::BattlePawn& Demo::BattlePawn::operator=(const BattlePawn& pawn)
{
	for(auto& i : pawn.abilities)
	{
		std::unique_ptr<Ability> p(i->Clone());
		this->abilities.push_back(std::move(p));
	}
	for(auto& i : pawn.effects)
	{
		std::unique_ptr<BattleEffect> p(i->Clone());
		this->effects.push_back(std::move(p));
	}
	this->toDestroy = false;
	this->emptySpace = pawn.emptySpace;
	this->selfLocation = pawn.selfLocation;
	this->bar = pawn.bar;
	this->cooldownBar = pawn.cooldownBar;
	this->pawnCooldownBar = pawn.pawnCooldownBar;
	this->healths = pawn.healths;
	this->pawnSize = pawn.pawnSize;
	this->dodge = pawn.dodge;
	this->displayAbilities = pawn.displayAbilities;
	return *this;
}

void Demo::BattlePawn::Update(double delta)
{
	if(healths.count("Pain") == 1)
		if(healths["Pain"].GetEnergy() <= 0)
			toDestroy = true;
	
	dodge.SetFrameInviolability(false);
	
	for(auto& i : abilities)
		i->Update(delta);
	for(auto& i : healths)
		i.second.Update(delta);
	for(auto& i : effects)
		i->BattleUpdate(delta,this);
	
	UpdatePawnCooldown(delta);
	
	if(abilities.size() > 0)
	{
		std::unique_ptr<Ability>& abil = abilities[0];
		cooldownBar.SetPercentage(abil->GetCooldown() * 100.0 / abil->GetMaxCooldown());
		
		if(abil->IsCoolingDown() && abil->CooldownStatusChanged())
			bar.SetColorMultiply(200,200,200);
		else if(!abil->IsCoolingDown() && abil->CooldownStatusChanged())
			bar.SetColorMultiply(255,255,255);
	}
	
	bool erased = false;
	for(auto i = effects.begin(); i != effects.end();)
	{
		if((*i)->Effect::GetFlag(Effect::PersistenceType::Destroyed))
		{
			i = effects.erase(i);
			UpdateEffectGraphicPosition();
			erased = true;
		}
		else
			++i;
	}
	if(erased)
		RearangeEffectLinks();
}

void Demo::BattlePawn::Draw(const Canvas& canvas) const
{
	bar.Draw(canvas);
	for(auto& i : healths)
		i.second.Draw(canvas);
	
	if(abilities.size() > 0)
		cooldownBar.Draw(canvas);
	
	if(pawnCooldown > 0.0)
		pawnCooldownBar.Draw(canvas);
	
	if(displayAbilities != Ability::DisplayMode::None)
		for(auto i = abilities.begin()+1; i != abilities.end(); ++i)
			(*i)->Draw(canvas);
	
	for(auto& i : defaultAbilityInfo)
		i.Draw(canvas);
	
	for(auto& i : effects)
		i->Draw(canvas);
}

void Demo::BattlePawn::Destroy()
{
	toDestroy = false;
	emptySpace = true;
	abilities.clear();
	defaultAbilityInfo.clear();
	healths.clear();
	actionBuffer.clear();
}
bool Demo::BattlePawn::ToDestroy()
{
	return toDestroy;
}

void Demo::BattlePawn::OrderAction(unsigned int abilityIndex, Location actionDestination, bool targetingEnemy)
{
	if(abilityIndex < abilities.size())
	{
		auto vec = abilities[abilityIndex].get()->Use(actionDestination,targetingEnemy);
		for(auto& i : vec)
			actionBuffer.push_back(i);
	}
}

std::vector<Demo::Action> Demo::BattlePawn::GetActions()
{
	SetActionsOwnership();
	ModifyActionBuffer();
	std::vector<Demo::Action> result;
	if(actionBuffer.size() > 0)
	{
		result = std::move(actionBuffer);
		actionBuffer.clear();
	}
	return result;
}

void Demo::BattlePawn::AddAbility(std::unique_ptr<Ability> ab, unsigned int index)
{
	ab->SetDefault(index == 1 || (abilities.size() == 0 && index == 0));
	if(!index)
		abilities.push_back(std::move(ab));
	else
	{
		if(index > abilities.size()) 
			abilities.push_back(std::move(ab));
		else
			abilities[index - 1] = std::move(ab);
	}
	UpdateDefaultAbilityInfo();
}

void Demo::BattlePawn::RemoveAbility(unsigned int index)
{
	if(index < abilities.size())
		abilities.erase(abilities.begin() + index);
	UpdateDefaultAbilityInfo();
}

bool Demo::BattlePawn::IsAbilityDraggable(unsigned int index)
{
	if(index < abilities.size())
		return abilities[index]->IsAbilityDraggable();
	return false;
}

void Demo::BattlePawn::SendAbilityCallBack(const CallBack& what,const std::string& actionData)
{
	for(auto& i : abilities)
	{
		if(i->WasItYourAction(what,actionData))
			return;
	}
}

Demo::ActionRange Demo::BattlePawn::GetAbilityRange(unsigned int index)
{
	if(index < abilities.size())
		return abilities[index]->GetAbilityRange();
	return ActionRange::None;
}

void Demo::BattlePawn::AddEffect(std::unique_ptr<BattleEffect> be, const bool& avoidable)
{
	if(dodge.IsInviolable() && avoidable)
		return;
	
	std::string effectName = be->GetName();
	bool existInEffects = false;
	for(auto& i : effects)
	{
		if(i->GetName() == effectName)
		{
			existInEffects = true;
			i->MergeEffect(&*be);
			break;
		}
	}
	if(!existInEffects)
	{
		unsigned int index = effects.size();
		if(be->GetFlag(BattleEffect::EffectType::AttackInputModifier)) 
			attackRecivingActionModyfier.push_back(std::make_pair(index++,effectName));
			
		if(be->GetFlag(BattleEffect::EffectType::AttackOutputDefaultModifier) 
		|| be->GetFlag(BattleEffect::EffectType::AttackOutputAbilityModifier) ) 
			attackDealingActionModyfier.push_back(std::make_pair(index++,effectName));
		
		effects.push_back(std::move(be));
		UpdateEffectGraphicDisplay();
	}
}

void Demo::BattlePawn::OrderToMove(Location moveDestination, bool forced)
{
	if(!IsEmptySpace())
	{
		Action a(moveDestination);
		a.aType = Action::ActionType::Move;
		actionBuffer.push_back(std::move(a));
	}
}

void Demo::BattlePawn::DealDamage(const std::string& healthType, const int& amount,const unsigned int& accuracy, const bool& ignoreResistances)
{
	if(dodge.IsInviolable() && accuracy > 0)
	{
		DamageDisplay d("miss",false);
		d.SetTextColor(DamageNameToColor::ToColor(healthType));
		damageToDisplay.push_back(std::move(d));
		return;
	}
	
	DamageModificationInfo info((double)amount,healthType);
	
	if(!ignoreResistances)
		for(auto& i : attackRecivingActionModyfier)
			effects[i.first]->ModifyInDamage(info);
	
	int damage = info.GetModifiedDamage();
	
	if(damage < 0)
		damage = 0;

	dodge.IncreaseDodge(accuracy);
	if(dodge.CanDodge())
	{
		dodge.ResetDodge();
		dodge.SetFrameInviolability(true);
		DamageDisplay d("miss",false);
		d.SetTextColor(DamageNameToColor::ToColor(healthType));
		damageToDisplay.push_back(std::move(d));
		return;
	}

	try{
		healths.at(healthType).Increase(-damage);
		DamageDisplay d(std::to_string(-damage),false);
		d.SetTextColor(DamageNameToColor::ToColor(healthType));
		damageToDisplay.push_back(std::move(d));
	}
	catch(const std::out_of_range& e){}
}

void Demo::BattlePawn::GiveSupport(const std::string& healthType, const int& amount)
{
	DamageModificationInfo info(-amount,healthType);
	
	for(auto& i : attackRecivingActionModyfier)
		effects[i.first]->ModifyInDamage(info);

	try{
		int damage = info.GetModifiedDamage();
		healths.at(healthType).Increase(-damage);
		damageToDisplay.push_back(DamageDisplay(std::to_string(-damage)));
	}
	catch(const std::out_of_range& e){}
}

void Demo::BattlePawn::SetHP(const std::string& healthType, int value)
{
	try{
		EnergyBar& hp = healths.at(healthType);
		if(hp.GetMaxEnergy() < value)
			value = hp.GetMaxEnergy();
		if(value < 0)
			value = 0;
		hp.SetEnergy(value);
	}
	catch(const std::out_of_range& e){}
}

std::vector<Demo::DamageDisplay>& Demo::BattlePawn::GetDamageToDisplay()
{
	return damageToDisplay;
}

void Demo::BattlePawn::SetPawnDoodge(const unsigned int& newDodge)
{
	dodge.SetNewDodge(newDodge);
}

void Demo::BattlePawn::SetPawnInitialDoodge(const unsigned int& initialDodge)
{
	dodge.SetInitialDodge(initialDodge);
}

void Demo::BattlePawn::RandomizeDodge()
{
	dodge.RandomizeDodge();
}


void Demo::BattlePawn::UpdatePawnCooldown(const double& delta)
{
	if(pawnCooldown > 0.0)
	{
		pawnCooldown -= delta;
		
		if(pawnCooldown <= 0.0)
			pawnCooldown = 0.0;
			
		if(pawnMaxCooldown == 0.0)
			pawnMaxCooldown = 1.0;
			
		double percentage = pawnCooldown*100.0/pawnMaxCooldown;
		pawnCooldownBar.SetPercentage(percentage);
		for(auto& i : abilities)
			i->SetPawnCooldownPercentage(percentage);
	}
}

void Demo::BattlePawn::IncreasePawnCooldown(const double& cooldown)
{
	if(pawnCooldown == 0.0)
	{
		pawnCooldown = cooldown;
		pawnMaxCooldown = cooldown;
		return;
	}
	if(pawnCooldown + cooldown < pawnMaxCooldown)
	{
		pawnCooldown += cooldown;
		return;
	}
	else
	{
		pawnCooldown += cooldown;
		pawnMaxCooldown += cooldown;
	}
}

void Demo::BattlePawn::SetPosition(const PointU& pos)
{
	GameObject::SetPosition(pos);
	bar.SetPosition(PointU(pos.x,pos.y));
	cooldownBar.SetPosition(PointU(pos.x,pos.y));
	pawnCooldownBar.SetPosition(PointU(pos.x,pos.y));
	
	unsigned int i = 1;
	PointU size = bar.GetSize();
	for(auto& j : healths)
	{
		j.second.SetPosition(PointU(pos.x,pos.y+size.y/2-i*size.y*0.1));
		i++;
	}
	UpdateAbilityDisplay();
	UpdateEffectGraphicPosition();
}

Demo::PointU Demo::BattlePawn::GetPosition(const unsigned int abilityIndex)
{
	if(abilityIndex == 0)
		return GameObject::GetPosition();
	unsigned int x = GetPosition().x - pawnSize.x/2 + pawnSize.x/5 * (abilityIndex-1) + pawnSize.x/10;
	unsigned int y = GetPosition().y + pawnSize.y/2;
	return PointU(x,y);
}

bool Demo::BattlePawn::IsEmptySpace() const
{
	return emptySpace;
}

void Demo::BattlePawn::SetEmptySpace(bool empty)
{
	emptySpace = empty;
}


bool Demo::BattlePawn::IsPointOnAbility(const PointU& point, unsigned int& index)
{
	index = 0;
	if(abilities.size() < 2)
		return false;
	
	unsigned int beginx = GetPosition().x - pawnSize.x/2;
	unsigned int beginy = GetPosition().y + pawnSize.y/2 - pawnSize.y/10;
	
	unsigned int endx = GetPosition().x + pawnSize.x/2;
	unsigned int endy = GetPosition().y + pawnSize.y/2 + pawnSize.y/10;
	
	if(point.x >= beginx && point.x < endx && point.y >= beginy && point.y < endy)
	{
		endx = beginx + pawnSize.y/5;
		for(unsigned int i = 1; i < abilities.size(); ++i)
		{
			unsigned int abegin = beginx + (i-1) * pawnSize.y/5;
			unsigned int aend = endx + (i-1) * pawnSize.y/5;
			
			if( point.x >= abegin && point.x < aend )
			{
				index = i;
				return true;
			}
		}
	}
	return false;
	
}

void Demo::BattlePawn::SetLocation(const Location& l)
{
	selfLocation = l;
}

Demo::Location Demo::BattlePawn::GetLocation()
{
	return selfLocation;
}

void Demo::BattlePawn::SetSize(const PointU& size)
{
	pawnSize = size;
	bar.SetSize(size);
	cooldownBar.SetSize(size);
	pawnCooldownBar.SetSize(size);
	for(auto& i : healths)
	{
		i.second.SetSize(PointU((unsigned int)(size.x*0.8),(unsigned int)(size.y*0.05)));
	}
	UpdateAbilityDisplay();
	UpdateEffectGraphicDisplay();
}

void Demo::BattlePawn::SetSprite(const std::string& spriteName, unsigned int index)
{
	Demo::TextureHolder::GetInstance().SetSpriteTexture(bar,spriteName);
	bar.SetPosition(PointU(position.x,position.y));
	bar.SetSize(pawnSize);
}

void Demo::BattlePawn::AddEnergyBar(const std::string index, EnergyBar energy)
{
	if(healths.count(index) == 0)
		healths.insert(std::pair<std::string,EnergyBar>(index,std::move(energy)));
	else
		healths[index] = std::move(energy);
	
	SetSize(bar.GetSize());
	SetPosition(GetPosition());
}

void Demo::BattlePawn::ModifyActionBuffer()
{
	if(attackDealingActionModyfier.size() == 0)
		return;
	for(auto& i : actionBuffer)
	{
		for(auto j = i.begin(); j != i.end(); ++j)
		{
			(*j).creatorIndex = GetID();
			switch((*j).aType)
			{
				case Action::ActionType::Default:
				{
					if((*j).aIntention == ActionIntentions::Support)
						break;
						
					std::istringstream stream;
					stream.str((*j).actionData);
					
					std::ostringstream outstream;
					
					std::string hpType;
					int amount;
					char separator = '|';
					while(stream.good() && !stream.eof() && separator == '|')
					{
						stream >> hpType >> amount >> separator;
						DamageModificationInfo info(amount,hpType);
						for(auto& k : attackDealingActionModyfier)
						{
							using BeT = BattleEffect::EffectType;
							if((*j).defaultAction && effects[k.first]->GetFlag(BeT::AttackOutputDefaultModifier))
								effects[k.first]->ModifyOutDamage(info);
							else if( !(*j).defaultAction && effects[k.first]->GetFlag(BeT::AttackOutputAbilityModifier) )
								effects[k.first]->ModifyOutDamage(info);
						}
						
						outstream << info.damageType << " " << info.GetModifiedDamage();
						if(!stream.eof() && separator == '|')
							outstream << " | ";
						// TODO check -215643
					}
					
					(*j).actionData = outstream.str();
					break;
				}
				default: 
					break;
				
			}
		}
	}
}

void Demo::BattlePawn::SetActionsOwnership()
{
	for(auto& i : actionBuffer)
	{
		for(auto j = i.begin(); j != i.end(); ++j)
		{
			(*j).creatorLocation = GetLocation();
			(*j).creatorIndex = GetID();
		}
	}
}

void Demo::BattlePawn::RELIteration(unsigned int& index, unsigned int& linkIndex, std::vector<std::pair<unsigned int,std::string>>& effectLinkVec)
{
	if(effectLinkVec[linkIndex].second == effects[index]->GetName())
	{
		if(effectLinkVec[linkIndex].first != index)
			effectLinkVec[linkIndex].first = index;
		++linkIndex;
	}
	else
	{
		if(effectLinkVec[linkIndex].first <= index)
			effectLinkVec.erase(effectLinkVec.begin() + linkIndex);
	}
}

void Demo::BattlePawn::RemoveLinksOnEnd(unsigned int& linkIndex, std::vector<std::pair<unsigned int, std::string> >& effectLinkVec)
{
	while(linkIndex < effectLinkVec.size())
		effectLinkVec.erase(effectLinkVec.begin() + linkIndex);
}

void Demo::BattlePawn::RearangeEffectLinks()
{
	unsigned int ADAMindex = 0, ARAMindex = 0;
	
	for(unsigned int i = 0; i < effects.size(); ++i)
	{
		if(ADAMindex < attackDealingActionModyfier.size())
			RELIteration(i,ADAMindex,attackDealingActionModyfier);
			
		if(ARAMindex < attackRecivingActionModyfier.size())
			RELIteration(i,ARAMindex,attackRecivingActionModyfier);
	}
	RemoveLinksOnEnd(ADAMindex,attackDealingActionModyfier);
	RemoveLinksOnEnd(ARAMindex,attackRecivingActionModyfier);
}

void Demo::BattlePawn::SetAbilityDisplayMode(Ability::DisplayMode newMode)
{
	displayAbilities = newMode;
}

Demo::Ability::DisplayMode Demo::BattlePawn::GetAbilityDisplayMode()
{
	return displayAbilities;
}

void Demo::BattlePawn::UpdateAbilityDisplay()
{
	for(unsigned int i = 1; i < abilities.size(); ++i)
	{
		abilities[i]->SetDisplaySize(PointU(pawnSize.x/5,pawnSize.x/5));
		unsigned int x = GetPosition().x - pawnSize.x/2 + pawnSize.x/5 * (i-1) + pawnSize.x/10;
		unsigned int y = GetPosition().y + pawnSize.y/2;
		abilities[i]->SetDisplayPosition(PointU(x,y));
	}
	UpdateDefaultAbilityInfo();
}

void Demo::BattlePawn::UpdateEffectGraphicPosition()
{
	unsigned int i = 0;
	for(auto& e : effects)
	{
		if(e->IsGraphicSet())
		{
			unsigned int x = GetPosition().x - pawnSize.x/2 + pawnSize.x/5 * i + pawnSize.x/10;
			unsigned int y = GetPosition().y - pawnSize.y/2;
			e->SetGraphicPosition(PointU(x,y));
			++i;
		}
	}
}

void Demo::BattlePawn::UpdateEffectGraphicDisplay()
{
	for(auto& e : effects)
	{
		e->SetGraphicSize(PointU(pawnSize.x/5,pawnSize.x/5));
	}
	UpdateEffectGraphicPosition();
}

void Demo::BattlePawn::UpdateDefaultAbilityInfo()
{
	defaultAbilityInfo.clear();
	if(abilities.size() > 0)
	{
		auto vec = abilities[0]->GetAbilityDamageData(false);
		for(unsigned int i = 0; i < vec.size(); ++i)
		{
			DefaultAbilityDamageDisplay spr(vec[i].first,vec[i].second);
			spr.SetTextAllign(false);
			spr.SetSize(pawnSize.x/5);
			
			unsigned int x = GetPosition().x - pawnSize.x/2;
			unsigned int y = GetPosition().y - pawnSize.y/2 + pawnSize.y/5 * i + pawnSize.y/5;
			spr.SetPosition(PointU(x,y));
			
			defaultAbilityInfo.push_back(std::move(spr));
		}
		vec.clear();
		
		vec = abilities[0]->GetAbilityDamageData(true);
		for(unsigned int i = 0; i < vec.size(); ++i)
		{
			DefaultAbilityDamageDisplay spr(vec[i].first,vec[i].second);
			spr.SetTextAllign(true);
			spr.SetSize(pawnSize.x/5);
			
			unsigned int x = GetPosition().x + pawnSize.x/2;
			unsigned int y = GetPosition().y - pawnSize.y/2 + pawnSize.y/5 * i + pawnSize.y/5;
			spr.SetPosition(PointU(x,y));
			defaultAbilityInfo.push_back(std::move(spr));
		}
	}
}
