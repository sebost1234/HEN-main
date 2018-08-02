#include "VisualNovelControler.h"

std::map < std::wstring, VisualNovelEvent::Type, Comparewstring>  VisualNovelEvent::stringtotypemap;
std::map < VisualNovelEvent::Type, std::wstring>  VisualNovelEvent::typetostringmap;


void VisualNovelEvent::loadEvents()
{
	addEvent(VisualNovelEvent::None, L"//");
	addEvent(VisualNovelEvent::Wait, L"wait");
	addEvent(VisualNovelEvent::Stop, L"stop");
	addEvent(VisualNovelEvent::Say, L"say");
	addEvent(VisualNovelEvent::Choice, L"choice");
	addEvent(VisualNovelEvent::Flag, L"flag");
	addEvent(VisualNovelEvent::SetScene, L"setscene");
	addEvent(VisualNovelEvent::SetModelPosition, L"setmodelposition");
	addEvent(VisualNovelEvent::SetModelTargetPosition, L"setmodeltargetposition");
	addEvent(VisualNovelEvent::SetModelFreeEvent, L"setmodelfree");
	addEvent(VisualNovelEvent::MoveIntoRow, L"moveintorow");
	addEvent(VisualNovelEvent::RemoveFromRow, L"removefromrow");
	addEvent(VisualNovelEvent::BgChange, L"changebg");
	addEvent(VisualNovelEvent::AddModel, L"addmodel");
	addEvent(VisualNovelEvent::TextureModel, L"texturemodel");
	addEvent(VisualNovelEvent::DeleteModel, L"deletemodel");
	addEvent(VisualNovelEvent::PlaySound, L"playsound");
	addEvent(VisualNovelEvent::PlayMusic, L"playmusic");
	addEvent(VisualNovelEvent::FX, L"fx");
	addEvent(VisualNovelEvent::ChangeFile, L"changefile");
	addEvent(VisualNovelEvent::GoTo, L"jump");
	addEvent(VisualNovelEvent::Scope, L"label");
	addEvent(VisualNovelEvent::FlagTest, L"flagtest");
	addEvent(VisualNovelEvent::CG, L"cg");
}


void VisualNovelEvent::addEvent(VisualNovelEvent::Type typ, std::wstring string)
{
	stringtotypemap.emplace(std::make_pair(string, typ));
	typetostringmap.emplace(std::make_pair(typ, string));
}


std::wstring VisualNovelEvent::typeToString(VisualNovelEvent::Type typ)
{
	if (typetostringmap.count(typ) == 0)
		return L"ERROR";
	else
		return typetostringmap[typ];
}

VisualNovelEvent::Type VisualNovelEvent::stringToType(std::wstring name)
{
	if (stringtotypemap.count(name) == 0)
		return VisualNovelEvent::None;
	else
		return stringtotypemap[name];
}