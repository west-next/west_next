#include "LevelData.h"

OpenLevelData* OpenLevelData::_instance = nullptr;

OpenLevelData* OpenLevelData::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)OpenLevelData;
	}
	return _instance;
}

void OpenLevelData::Decrypt(char* cSrc, char* cDest)
{
	int   i, h, l, m, n, j = 0;
	for (i = 0; i < (int)strlen(cSrc); i = i + 2)
	{
		h = (cSrc[i] - 'x');
		l = (cSrc[i + 1] - 'z');
		m = (h << 4);
		n = (l & 0xf);
		cDest[j] = m + n;
		j++;
	}
	cDest[j] = '\0';
}

void OpenLevelData::OpenLevelsData(const std::string& worlddata)
{
	std::string str = FileUtils::getInstance()->getStringFromFile(worlddata);

	char *Encryption,*Password;
	Encryption = (char*)malloc(sizeof(char) * str.size());
	Password = (char*)malloc(sizeof(char) * str.size());
	strcpy(Encryption, str.c_str());

	/* Ω‚√‹ */
	Decrypt(Encryption, Password);
	
	document.Parse<rapidjson::kParseDefaultFlags>(Password);

	if (document.HasParseError())
	{
		return;
	}
}

void OpenLevelData::SetInit()
{
	LevelData.GameType.clear();
	LevelData.ZombiesType.clear();
	LevelData.MunchZombiesFrequency.clear();
	LevelData.ZombiesNumbers.clear();
	LevelData.ZombiesTypeProbabilityFrequency.clear();
	LevelData.NoPlantsPosition.clear();
	LevelData.FlowerPosition = -1;
	LevelData.AtLeastSunNumbers = 0;
	LevelData.CarNumbers = 0;
	LevelData.UsePlantsNumbers = 0;
	LevelData.ZombiesIsVisible = false;
	LevelData.ZombiesIsSmall = false;
	LevelData.ZombiesIsBig = false;
	LevelData.IsNoPlants = false;
}

void OpenLevelData::ReadLevelData(char* LevelName)
{	
	SetInit(); /* ≥ı ºªØ */

	if (document.HasMember(LevelName))
	{
		LevelData.IsEncryption = document[LevelName]["IsEncryption"].GetBool();
		LevelData.CoinNumbers = document[LevelName]["CoinNumbers"].GetInt();
		LevelData.ZombiesFrequency = document[LevelName]["Frequency"].GetInt();
		LevelData.FirstFrequencyTime = document[LevelName]["FirstFrequencyTime"].GetInt();
		LevelData.UserLose = document[LevelName]["UserLose"].GetFloat();

		for (unsigned int i = 0; i < document[LevelName]["GameType"].Size(); i++)
		{
			LevelData.GameType.push_back(document[LevelName]["GameType"][i].GetInt());
		}
		for (unsigned int i = 0; i < document[LevelName]["ZombiesType"].Size(); i++)
		{
			LevelData.ZombiesType.push_back(document[LevelName]["ZombiesType"][i].GetInt());
		}
		for (unsigned int i = 0; i < document[LevelName]["MunchZombiesFrequency"].Size(); i++)
		{
			LevelData.MunchZombiesFrequency.push_back(document[LevelName]["MunchZombiesFrequency"][i].GetInt());
		}
		for (unsigned int i = 0; i < document[LevelName]["ZombiesNumbers"].Size(); i++)
		{
			LevelData.ZombiesNumbers.push_back(document[LevelName]["ZombiesNumbers"][i].GetInt());
		}

		std::vector<int>v;
		for (unsigned int i = 0; i < document[LevelName]["ZombiesTypeProbability"].Size(); i++)
		{
			v.clear();
			for (unsigned int j = 0; j < document[LevelName]["ZombiesTypeProbability"][i].Size(); j++)
			{
				v.push_back(document[LevelName]["ZombiesTypeProbability"][i][j].GetInt());
			}
			LevelData.ZombiesTypeProbabilityFrequency.push_back(v);
		}

		for (unsigned int i = 0; i < LevelData.GameType.size(); i++)
		{
			switch (LevelData.GameType.at(i))
			{
			case AtLeastSunNumbers:
				LevelData.AtLeastSunNumbers = document[LevelName]["AtLeastSunNumbers"].GetInt();
				break;
			case FlowerPosition:
				LevelData.FlowerPosition = 570 + 122 * document[LevelName]["FlowerPosition"].GetInt();
				break;
			case CarNumbers:
				LevelData.CarNumbers = document[LevelName]["CarNumbers"].GetInt();
				break;
			case UserPlantsNumbers:
				LevelData.UsePlantsNumbers = document[LevelName]["UserPlantsNumbers"].GetInt();
				break;
			case ZombiesInvisible:
				LevelData.ZombiesIsVisible = true;
				break;
			case SmallZombies:
				LevelData.ZombiesIsSmall = true;
				break;
			case BigZombies:
				LevelData.ZombiesIsBig = true;
				break;
			case NoPlants:
			{
				LevelData.IsNoPlants = true;
				_MyPoint MyPoint = { 0 };
				for (unsigned int i = 0; i < document[LevelName]["NoPlants"].Size(); i++)
				{
					MyPoint.x = document[LevelName]["NoPlants"][i][0].GetInt();
					MyPoint.y = document[LevelName]["NoPlants"][i][1].GetInt();
					LevelData.NoPlantsPosition.push_back(MyPoint);
				}
			}
				break;
			default:
				break;
			}
		}
	}
}
