#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool Convert(const char* InputFilename, const char* OutputFilename)
{
    std::ifstream InFile(InputFilename);
    if(!InFile)
    {
        std::cout << "Could not open  input file '" << InputFilename << "'" << std::endl;
        return false;
    }

    struct PositionData
    {
        double time;
        double X;
        double Y;
        double Z;
    };

    typedef std::vector<PositionData> PositionVec;

    PositionVec Positions;
    PositionData Pos;
    while(InFile)
    {
        InFile >> Pos.time >> Pos.X >> Pos.Y >> Pos.Z;
        Positions.push_back(Pos);
    }

    std::ofstream OutFile(OutputFilename);
    if(!OutFile)
    {
        std::cout << "Could not open output file '" << OutputFilename << "'" << std::endl;
        return false;
    }

    OutFile << "Text here" << std::endl;
    for(PositionVec::iterator it = Positions.begin(); it != Positions.end(); ++it)
    {
        const PositionData& p(*it);
        OutFile << "\t" << p.time << "\t" << p.X << "\t" << p.Y << "\t" << p.Z << std::endl;
    }
    OutFile << "End of Keyframe Data" << std::endl;
    return true;
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::cout << "Usage: convert <input filename> <output filename>" << std::endl;
        return 0;
    }
    bool Success = Convert(argv[1], argv[2]);

    return Success;
}