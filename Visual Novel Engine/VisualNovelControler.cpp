#include "VisualNovelControler.h"

std::wstring VisualNovelEvent::typeToString(Type typ)
{
	switch (typ)
	{
	case VisualNovelEvent::None:
		return L"//";
	case VisualNovelEvent::Wait:
		return L"wait";
	case VisualNovelEvent::Stop:
		return L"stop";
	case VisualNovelEvent::Say:
		return L"say";
	case VisualNovelEvent::Choice:
		return L"choice";
	case VisualNovelEvent::Flag:
		return L"flag";
	case VisualNovelEvent::SetScene:
		return L"setscene";
	case VisualNovelEvent::MoveIntoRow:
		return L"moveintorow";
	case VisualNovelEvent::RemoveFromRow:
		return L"removefromrow";
	case VisualNovelEvent::BgChange:
		return L"changebg";
	case VisualNovelEvent::AddModel:
		return L"addmodel";
	case VisualNovelEvent::TextureModel:
		return L"texturemodel";
	case VisualNovelEvent::DeleteModel:
		return L"deletemodel";
	case VisualNovelEvent::PlaySound:
		return L"playsound";
	case VisualNovelEvent::PlayMusic:
		return L"playmusic";
	case VisualNovelEvent::FX:
		return L"fx";
	case VisualNovelEvent::ChangeFile:
		return L"changefile";
	case VisualNovelEvent::GoTo:
		return L"goto";
	case VisualNovelEvent::Scope:
		return L"scope";
	case VisualNovelEvent::FlagTest:
		return L"flagtest";
	default:
		break;
	}
	return L"ERROR";
}
