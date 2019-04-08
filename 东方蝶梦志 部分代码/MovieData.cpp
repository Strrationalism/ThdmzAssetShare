#include "Precompiled.h"
#include "MovieData.h"

constexpr auto size = sizeof(::MovieData::Head);
static_assert(sizeof(::MovieData::Head) == 64, "MovieData size not matched!");

void MovieData::SaveMovie(const MovieData & movieData, std::ofstream & out)
{
	out.write((char*)&movieData.head, sizeof(movieData.head));

	const uint32_t mapSize = static_cast<uint32_t>(movieData.stages.size());
	out.write((char*)&mapSize, sizeof(mapSize));

	for (const auto& p : movieData.stages)
	{
		WriteStringToStream(p.first, out);
		p.second.first.WriteToStream(out);
		p.second.second.SaveToStream(out);
	}
}

void MovieData::ReadMovie(MovieData & movieData, std::ifstream & in)
{
	in.read((char*)&movieData.head, sizeof(movieData.head));

	uint32_t mapSize;
	in.read((char*)&mapSize, sizeof(mapSize));

	for (uint32_t i = 0; i < mapSize; ++i)
	{
		std::string first;
		LoadStringFromStream(first, in);

		auto& p = movieData.stages[std::move(first)];
		p.first.ReadFromStream(in);
		p.second.LoadFromStream(in);
	}
}
