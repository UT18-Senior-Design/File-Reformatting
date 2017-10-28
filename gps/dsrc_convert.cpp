#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <boost/algorithm/string/erase.hpp>
//#include <gmpxx.h>

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
        double rcv_time;
        double rcv;
        double msg_id;
        double tmd_id;
        double snd_time;
        double lat;
        double lon;
        double elevation;
        double accuracy;
        double X;
        double Y;
        double Z;
    };

    enum{
        RCV_TIME,
        RCV,
        MSG_ID,
        TMD_ID,
        SND_TIME,
        LAT,
        LON,
        ELEVATION,
        ACCURACY
    };

    typedef std::vector<PositionData> PositionVec;

    PositionVec Positions;
    PositionData Pos;

    char str[255];
    int item_counter = 0;
    while(InFile)
    {
        InFile.getline(str, 255);
        int line_counter = 0;

        if(InFile){
            std::stringstream ss(str);
            std::string token;
            std::string::size_type sz;
            int count = 0;
            while(getline(ss, token, ',')){
                //std::cout << "item number " << item_counter << "\ttoken " << token << "\tcount " << count << std::endl;
                boost::algorithm::erase_all(token," ");
                    if(item_counter > 8){
                        switch (count%9){
                            case RCV_TIME:
                                Pos.rcv_time = std::stod(token, &sz);
                                //std::cout << "rcv_time " << Pos.rcv_time << std::endl;
                                break;
                            case RCV:
                                Pos.rcv = std::stod(token, &sz);
                                //std::cout << "rcv "<< Pos.rcv << std::endl;
                                break;
                            case MSG_ID:
                                Pos.msg_id = std::stod(token, &sz);
                                //std::cout << "msg_id " << Pos.msg_id << std::endl;
                                break;
                            case TMD_ID:
                                Pos.tmd_id = std::stod(token, &sz);
                                //std::cout << "tmd_id "<< Pos.tmd_id << std::endl;
                                break;
                            case SND_TIME:
                                Pos.snd_time = std::stod(token, &sz);
                                //std::cout << "snd_time " << Pos.snd_time << std::endl;
                                break;
                            case LAT:
                                Pos.lat = std::stod(token, &sz);
                                Pos.lat = Pos.lat / 10000000;
                                //std::cout << "lat " << Pos.lat << std::endl;
                                break;
                            case LON:
                            {
                                //TODO: fix loss of precision
                                Pos.lon = std::stod(token, &sz);
                                //mpz_class x(token);
                                //mpz_class y("100000000");
                                ////std::cout << "lon " << x/y << std::endl;
                                Pos.lon = Pos.lon/10000000;
                                //std::cout << "lon " << Pos.lon << std::endl;
                                break;
                            }
                            case ELEVATION:
                                Pos.elevation = std::stod(token, &sz);
                                Pos.elevation = Pos.elevation /100;
                                //std::cout << "elevation " << Pos.elevation << std::endl;
                                break;
                            case ACCURACY:
                                Pos.accuracy= std::stod(token, &sz);
                                Pos.accuracy = Pos.accuracy / 100000000;
                                //std::cout << "accuracy " << Pos.accuracy << std::endl;
                                break;
                            }
                        count++;
                        //placeholder for x y z conversion (lat and long to spherical?)
                        double r = 6378000;
                        Pos.X = -r*cos(Pos.lat)*cos(Pos.lon);
                        Pos.Y = r*sin(Pos.lat);
                        Pos.Z = r*cos(Pos.lat)*cos(Pos.lon);
                        
                        //std::cout << "X: " << Pos.X << "\tY: " << Pos.Y << "\tZ: " << Pos.Z << std::endl; 
                    }
                    item_counter++;
                }
        }

        Positions.push_back(Pos);
    }

    std::ofstream OutFile(OutputFilename);
    if(!OutFile)
    {
        std::cout << "Could not open output file '" << OutputFilename << "'" << std::endl;
        return false;
    }

    PositionData point = Positions.at(2);

    OutFile << "header {\n" << std::endl;
    OutFile << "\ttimestamp_sec: "<< point.snd_time << std::endl;
    OutFile << "}" << std::endl; 
    OutFile << "\tlocalization {" << std::endl;
    OutFile << "\t\tposition {" << std::endl;

    OutFile << "\t\t\tx: " << point.X << std::endl;
    OutFile << "\t\t\ty: " << point.Y<<  std::endl;
    OutFile << "\t\t\tz: " << point.Z << std::endl;
    OutFile << "\t}" << std::endl; 
    OutFile << "}" << std::endl;

    for(PositionVec::iterator it = Positions.begin(); it != Positions.end(); ++it)
    {
        const PositionData& p(*it);
        
        //OutFile << "\ttimestamp_sec:" << p.time << "\t" << p.X << "\t" << p.Y << "\t" << p.Z << "\n} << std::endl;
    }
    //OutFile << "End of Keyframe Data" << std::endl;
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