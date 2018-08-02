#pragma once

#define timestep 0.01f

struct Row;

struct Model
{
	Model(std::wstring id) :
		id(id), currenttexture("none"), currentrow(nullptr), free(false)
	{
	}
	std::wstring id;
	std::string currenttexture;
	sf::Sprite sprite;
	Row*currentrow;


	void setPosition(sf::Vector2f position)
	{
		sprite.setPosition(sf::Vector2f(sf::Vector2i(position)));
		smoothedpos = position;
	}

	void move(sf::Vector2f move)
	{
		smoothedpos += move;
		sprite.setPosition(sf::Vector2f(sf::Vector2i(smoothedpos)));
	}

	sf::Vector2f getPosition()
	{
		return smoothedpos;
	}

	sf::Vector2f getTruePosition()
	{
		return sprite.getPosition();
	}

	sf::Vector2f speed;
	sf::Vector2f targetpos;
	sf::Vector2f smoothedpos;
	bool free;
};

struct Row
{
	Row(std::wstring id, sf::Vector2f startingposition, float width) : id(id), startingposition(startingposition), width(width)
	{

	}

	void calculatePositions()
	{
		float size = 0;
		for (unsigned int i = 0; i<models.size(); i++)
		{
			if (!models[i]->free)
			{
				size += models[i]->sprite.getGlobalBounds().width;
				if (i != models.size() - 1)
					size += 100;
			}
		}

		float center = startingposition.x + width*0.5f;

		float pos = 0;
		for (unsigned int i = 0; i<models.size(); i++)
		{
			if (!models[i]->free)
			{
				models[i]->targetpos = sf::Vector2f(center - size*0.5f + pos, startingposition.y - models[i]->sprite.getGlobalBounds().height);
				pos += models[i]->sprite.getGlobalBounds().width;
				pos += 100;
			}
		}
	}


	std::wstring id;
	sf::Vector2f startingposition;
	float width;
	std::vector<Model*>models;
};

class Scene : public sf::Drawable
{
public:
	Scene(bool defaultsetup = true)
	{
		timer = 0;
		gamerectangle.setSize(gamesize);
		if (defaultsetup)
		{
			rows.push_back(new Row(L"front", sf::Vector2f(0, gamesize.y), gamesize.x));
			rows.push_back(new Row(L"frontleft", sf::Vector2f(0, gamesize.y), gamesize.x*0.5f));
			rows.push_back(new Row(L"frontright", sf::Vector2f(gamesize.x*0.5f, gamesize.y), gamesize.x*0.5f));

			rows.push_back(new Row(L"back", sf::Vector2f(0, (gamesize.y*3)/4.0f), gamesize.x));
			rows.push_back(new Row(L"backleft", sf::Vector2f(0, (gamesize.y * 3) / 4.0f), gamesize.x*0.5f));
			rows.push_back(new Row(L"backright", sf::Vector2f(gamesize.x*0.5f, (gamesize.y * 3) / 4.0f), gamesize.x*0.5f));
		}
	}
	~Scene()
	{
		for (unsigned int i = 0; i < models.size(); i++)
			delete models[i];
		models.clear();
		for (unsigned int i = 0; i < rows.size(); i++)
			delete rows[i];
		rows.clear(); 
	}

	bool processEvent(VisualNovelEvent event)
	{
		switch (event.getType())
		{
			case VisualNovelEvent::BgChange:
				gamerectangle.setTexture(ResourceManager::getTexture("Data\\Game\\" + event.getArgumentS(BgchangeEventpath)));
				currenttexture = event.getArgument(BgchangeEventpath);
				return true;
			case VisualNovelEvent::CG:
			{
				std::vector<std::string> seencglist;

				{
					std::fstream file;
					file.open("Data\\Save\\seencgs.czpal");
					if (file.is_open())
						while (!file.eof())
						{
							std::string path;
							std::getline(file, path, '\n');
							seencglist.push_back(path);
						}

					file.close();
				}


				bool found = false;
				for (unsigned int i = 0; i < seencglist.size(); i++)
					if (seencglist[i] == event.getArgumentS(CGEventpath))
					{
						found = true;
						break;
					}

				if (!found)
				{
					seencglist.push_back(event.getArgumentS(CGEventpath));
					std::fstream file;
					file.open("Data\\Save\\seencgs.czpal", std::ios::out | std::ios::trunc);
					for (unsigned int i = 0; i < seencglist.size(); i++)
						file << seencglist[i] << std::endl;

				}
				auto tmp = ResourceManager::getTexture("Data\\Game\\" + event.getArgumentS(CGEventpath));
				tmp->setSmooth(false);
				gamerectangle.setTexture(tmp);
				currenttexture = event.getArgument(CGEventpath);

				return true;
			}

			case VisualNovelEvent::AddModel:
			{
				std::wstring id = event.getArgument(ModelEventid);

				models.push_back(new Model(id));
				return true;
			}
			case VisualNovelEvent::SetModelPosition:
			{
				Model*model = findModelbyId(event.getArgument(SetModelPositionEventid));
				if(model!=nullptr)
					model->setPosition(sf::Vector2f(stof(event.getArgument(SetModelPositionEventx)), stof(event.getArgument(SetModelPositionEventy))));
				return true;
			}
			case VisualNovelEvent::SetModelTargetPosition:
			{
				Model*model = findModelbyId(event.getArgument(SetModelPositionEventid));
				if (model != nullptr)
					model->targetpos = sf::Vector2f(stof(event.getArgument(SetModelPositionEventx)), stof(event.getArgument(SetModelPositionEventy)));
				return true;
			}
			case VisualNovelEvent::SetModelFreeEvent:
			{
				Model*model = findModelbyId(event.getArgument(SetModelFreeEventid));
				if (model != nullptr)
					model->free = !!stoi(event.getArgument(SetModelFreeEventbool));
				return true;
			}
			case VisualNovelEvent::TextureModel:
			{
				std::wstring id = event.getArgument(TextureModelEventid);
				std::string path = event.getArgumentS(TextureModelEventpath);

				for (unsigned int i = 0; i < models.size(); i++)
					if (models[i]->id == id)
					{
						models[i]->currenttexture = path;
						models[i]->sprite.setTexture(*ResourceManager::getCharacterTexture("Data\\Game\\" + path));
						syncModel(models[i]);
					}
				return true;
			}
			case VisualNovelEvent::MoveIntoRow:
				putModelIntoRow(event.getArgument(MoveIntoRowEventmodelid), event.getArgument(MoveIntoRowEventrowid), stoi(event.getArgument(MoveIntoRowEventpos)), !!stoi(event.getArgument(MoveIntoRowEventteleport)));
				return true;
			case VisualNovelEvent::RemoveFromRow:
			{
				removeModelfromRow(findModelbyId(event.getArgument(RemoveFromRowEventmodelid)));
				return true;
			}
			case VisualNovelEvent::DeleteModel:
			{
				std::wstring id = event.getArgument(ModelEventid);
				for (int i = models.size() - 1; i >= 0; i--)
					if (models[i]->id == id)
					{
						removeModelfromRow(models[i]);
						delete models[i];
						models.erase(models.begin() + i);
					}
				return true;
			}
		}
		return true;
	}

	void syncModel(Model*model)
	{
		if (model->currentrow != nullptr)
			model->currentrow->calculatePositions();
	}

	void deleteRow(std::wstring id)
	{
		for (int i = rows.size() - 1; i >= 0; i--)
			if (rows[i]->id == id)
			{
				delete rows[i];
				rows.erase(rows.begin() + i);
			}
	}

	void putModelIntoRow(std::wstring modelid, std::wstring rowid, int pos, bool teleport)
	{
		Model*model = findModelbyId(modelid);
		Row*row = findRowbyId(rowid);

		if (model != nullptr&&row != nullptr)
		{
			removeModelfromRow(model);

			model->currentrow = row;

			if (pos >= 0 && pos <= int(row->models.size()))
				row->models.insert(row->models.begin() + pos, model);
			else
				row->models.push_back(model);



			row->calculatePositions();
			if (teleport)
				model->setPosition(model->targetpos);
		}
	}

	void save(SaveData&data)
	{
		for (unsigned int i = 0; i < models.size(); i++)
		{
			data.toLoad.push_back(L"addmodel;" + models[i]->id);
			if (models[i]->sprite.getTexture() != nullptr)
				data.toLoad.push_back(L"texturemodel;" + models[i]->id + L';' + sf::String(models[i]->currenttexture).toWideString());
		}
		if (gamerectangle.getTexture() != nullptr)
			data.toLoad.push_back(L"changebg;" + currenttexture);

		for (unsigned int i = 0; i < rows.size(); i++)
			for (unsigned int o = 0; o < rows[i]->models.size(); o++)
			{
				data.toLoad.push_back(L"moveintorow;" + rows[i]->models[o]->id + L';' + rows[i]->id + L';' + std::to_wstring(o) + L";0");
				data.toLoad.push_back(L"setmodelposition;" + rows[i]->models[o]->id + L';' + std::to_wstring(rows[i]->models[o]->getPosition().x) + L";" + std::to_wstring(rows[i]->models[o]->getPosition().y));
			}
	}

	void sync(float time)
	{
		timer += time;

		if (timer > timestep)
		{
			timer -= timestep;

			for (unsigned int i = 0; i < models.size(); i++)
			{
				sf::Vector2f diff = models[i]->targetpos - models[i]->getPosition();
				float lengthsqr = diff.x*diff.x + diff.y*diff.y;

				if (lengthsqr > 10000)//length>100
				{
					lengthsqr /= timestep;
					lengthsqr /= 100000;

					diff /= sqrtf(lengthsqr);
					models[i]->speed += diff;
				}
				else
				{
					models[i]->speed *= 0.9f;
					models[i]->move(diff*timestep*10.0f);

					if (lengthsqr < 1)//length<1
						models[i]->setPosition(models[i]->targetpos);
				}

				models[i]->move(models[i]->speed*timestep);
			}
		}
	}

	void removeModelfromRow(Model*model)
	{
		if(model!=nullptr)
		if (model->currentrow != nullptr)
		{
			for (int i = model->currentrow->models.size() - 1; i >= 0; i--)
				if (model->currentrow->models[i] == model)
				{
					model->currentrow->models.erase(model->currentrow->models.begin() + i);
				}
			model->currentrow->calculatePositions();
			model->currentrow = nullptr;
		}
	}

	Model*findModelbyId(std::wstring id)
	{
		for (unsigned int i = 0; i < models.size(); i++)
			if (models[i]->id == id)
				return models[i];
		return nullptr;
	}

	Row*findRowbyId(std::wstring id)
	{
		for (unsigned int i = 0; i < rows.size(); i++)
			if (rows[i]->id == id)
				return rows[i];
		return nullptr;
	}
private:
	sf::RectangleShape gamerectangle;
	std::wstring currenttexture;



	std::vector <Model*> models;
	std::vector<Row*> rows;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
	{
		target.draw(gamerectangle, states);
		for (int i = rows.size() - 1; i >= 0; i--)
		{
			for (unsigned int o = 0; o < rows[i]->models.size(); o++)
				target.draw(rows[i]->models[o]->sprite, states);
		}
	}


	float timer;
};
