#include "NuzPrecompiled.h"
#include "SeManager.h"
#include "CSVReader.h"
#include "Asset.h"
#include "LowerSound.h"
#include "Globals.h"
using namespace std;
struct Chunk {
	uint8_t* buf;
	uint32_t size;
};

static map<string, Chunk> SEs;

void Basic::Sound::SeManager::Init()
{
	Basic::CSVReader csv;
	csv.Load("SEs/list.txt");

	while (!csv.IsLastLine()) {
		while (!csv.LineEnd()) {
			string fileName = csv.PopString();
			string name = fileName.substr(3, fileName.length() - 7);

			SDL_AudioSpec fmt;
			Chunk p;
			if (SDL_LoadWAV(("../assets/SEs/" + fileName).c_str(), &fmt, &p.buf, &p.size) == nullptr)
				throw exception("Unable to load SE");
			//LowerSound::ConvertU8_22050(p->buf, p->size);

			SEs[name] = p;
		}
		csv.NextLine();
	}

}

void Basic::Sound::SeManager::Quit()
{
	for (auto& p : SEs) {
		SDL_FreeWAV((Uint8*)p.second.buf);
	}
	SEs.clear();
}

static std::map<std::string,std::pair<uint8_t,int16_t>> tasks;
static std::mutex tlock;
void Basic::Sound::SeManager::Play(const std::string & name, int16_t pan, uint8_t volume)
{
	if (name == "gun00" || name == "tan02")
		Crash(L"无效音效");
	if (SEs.count(name) == 0)
		Crash(L"未找到音效:" + Str2WStr(name));
	//LowerSound::PlaySound(SEs[name]->buf, SEs[name]->size);
	//std::lock_guard<std::mutex> g(tlock);
	auto& t = tasks[name];
	t.first = volume > t.first ? volume : t.first;

	t.second = pan;
}

void Basic::Sound::SeManager::PlayHigh(const std::string & name, int16_t pan,uint8_t volume)
{
	if (SEs.count(name) == 0)
		Crash(L"未找到音效");
	LowerSound::PlaySound(SEs[name].buf, SEs[name].size,volume,pan);
}

static int s = 0;
void Basic::Sound::SeManager::Update() {
	++s;
	if (s >= 5) {
		for (auto& task : tasks) {
			LowerSound::PlaySound(SEs[task.first].buf, SEs[task.first].size, task.second.first,task.second.second);
		}
		tasks.clear();
		s = 0;
	}
}
