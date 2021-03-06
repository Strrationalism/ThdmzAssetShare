#include "NuzPrecompiled.h"
#include "CSVReader.h"
#include "Asset.h"
using namespace Basic;

static std::string Trim(const std::string& s)
{
    std::string t;
    for(auto p = s.begin();p != s.end();++p)
        if(*p != ' ' && *p != '\t' && *p != '\0')
            t+=*p;
    return t;
}

/*void CSVReader::loadFromBin(const std::shared_ptr<std::vector<uint8_t> >& buf)
{
    uint32_t ptr = 4;
    uint32_t lineCount;
    for(uint32_t i = 0;i < sizeof(lineCount);++i)
        ((uint8_t*)(&lineCount))[i] = (*buf)[ptr++];
    for(uint32_t lineNum = 0;lineNum < lineCount;++lineNum){
        //ReadLine
        //Line Size
        uint32_t lineSize;
        for(uint32_t i = 0;i < sizeof(lineSize);++i)
            ((uint8_t*)(&lineSize))[i] = (*buf)[ptr++];
            m_csvCache.push_back(std::vector<std::string>());
            auto p = m_csvCache.end();
            p--;
            //Read Units
        for(uint32_t unitNum = 0;unitNum < lineSize;++unitNum){
            //Read string Size
            uint32_t length;
            std::string unit;
            for(uint32_t i = 0;i < sizeof(length);++i)
                ((uint8_t*)&length)[i] = (*buf)[ptr++];

            //GetUnit
            for(uint32_t i = 0;i < length;++i)
                unit += (char)(*buf)[ptr++];

            p -> push_back(unit);
        }

    }
}*/

void CSVReader::loadFromText(const std::shared_ptr<std::vector<uint8_t> >& buf)
{
    uint32_t num = 0;
    bool bRun = true;
    while(bRun){
        //GetLine
        std::string line;
        bool bGetLine_Run = true;
        while(bGetLine_Run){
            if(num >= buf -> size()){
                bRun = false;
                bGetLine_Run = false;
            }
            char c = (*buf)[num++];
            if(num >= buf -> size()){
                bRun = false;
                bGetLine_Run = false;
            }
            if(c == '\n') break;
            else if(c == '\r') break;
            else line += c;
        }
        if (Trim(line).empty()) continue;
		if (Trim(line)[0] == '#') continue;
        m_csvCache.push_back(std::vector<std::string>());
        auto p = m_csvCache.end();
        p--;


        //Read Units
        bool bGetUnitRun = true;
        while(bGetUnitRun){
            std::string thisUnit;
            auto pNextComma = line.find(',');
            if(pNextComma == std::string::npos){
                bGetUnitRun = false;
                thisUnit = line;
            }
            else{
                thisUnit = line.substr(0,pNextComma);
                line = line.substr(pNextComma+1,line.length() - pNextComma - 1);
            }
            p -> push_back(thisUnit);
        }
    }
}



void CSVReader::Load(const std::string& csv){

	Asset asset(csv);
	auto buf = asset.Ptr();
    m_csvCache.clear();

    //If it is an empty file.
    if(buf -> size() == 0) throw InvalidCSV("It is an empty csv file.");
    //Read binary CSV
    //if((*buf)[0] == 0xFF)
        //loadFromBin(buf);
    //else 
	loadFromText(buf);

    Reset();
}

std::string CSVReader::PopString(){
    if(LineEnd()) throw ValueNotFound("This line is end.");
    return m_csvCache[m_y][m_x++];
}

double CSVReader::PopDouble(){
    return atof(PopString().c_str());
}


int CSVReader::PopInt(){
    return atoi(PopString().c_str());
}

bool CSVReader::LineEnd() const{
    return m_x >= m_csvCache[m_y].size();
}

bool CSVReader::NextLine(){
	if (IsLastLine()) return false;
    m_y++;
    m_x = 0;
    return !IsLastLine();
}

bool CSVReader::IsLastLine() const{
    return m_y >= m_csvCache.size();
}

void CSVReader::Reset(){
    m_x = m_y = 0;
}
