// This file is part of EmuShell
// A multi video game systems emulator.
//
// Copyright(C) 2015 Ala Ibrahim Hadid
// E-mail: mailto:ahdsoftwares@hotmail.com
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.

// Ala Hadid at 3 November 2015:
// Finally got it working !! :D
#include <vector>
#include "Nes.h"
#include "../XMLParser/xmlParser.h"
using namespace std;
namespace NES
{
namespace NesDB
{
// Loaded game infos
NesDBGameCartInfo* GAMES;
std::string DBVersion;
std::string DBConformance;
std::string DBAgent ;
std::string DBAuthor ;
std::string DBTimeStamp;
int GamesCount;
// Initialize the NesDB, read database file if possible.
// dbFilePath: The complete file path
void Initialize(std::string dbFilePath)
{
    XMLNode xMainNode=XMLNode::openFileHelper(dbFilePath.c_str(),"database");

    if (xMainNode.isAttributeSet("version"))
        DBVersion =  (std::string)xMainNode.getAttribute("version");
    std::cout << "NES: ->DB Version = "<<DBVersion<<"\n";

    if (xMainNode.isAttributeSet("conformance"))
        DBConformance =  (std::string)xMainNode.getAttribute("conformance");
    std::cout << "NES: ->DB Conformance = "<<DBConformance<<"\n";

    if (xMainNode.isAttributeSet("agent"))
        DBAgent =  (std::string)xMainNode.getAttribute("agent");
    std::cout << "NES: ->DB Agent = "<<DBAgent<<"\n";

    if (xMainNode.isAttributeSet("author"))
        DBAuthor  =  (std::string)xMainNode.getAttribute("author");
    std::cout << "NES: ->DB Author = "<<DBAuthor<<"\n";

    if (xMainNode.isAttributeSet("timestamp"))
        DBTimeStamp =  (std::string)xMainNode.getAttribute("timestamp");
    std::cout << "NES: ->DB Time Stamp = "<<DBTimeStamp;

    int gNodesCount = xMainNode. nChildNode("game") ;

    // Calculate the actual games count
    GamesCount=0;
    for (int i=0; i<gNodesCount; i++)
    {
        XMLNode gameNode = xMainNode.getChildNode(i);
        GamesCount += gameNode.nChildNode("cartridge") ;
    }
    std::cout << "NES: ->Games count = " << GamesCount <<"\n";

    GAMES =new NesDBGameCartInfo[GamesCount];
    int cartIndex=0;
    // Load the games !
    for (int i=0; i<gNodesCount; i++)
    {
        // Get the child node
        XMLNode gameNode = xMainNode.getChildNode(i);

        // Fill the info of the game
        if (gameNode.isAttributeSet("name"))
            GAMES[cartIndex]. Game_Name = (std::string)gameNode.getAttribute("name");
        if (gameNode.isAttributeSet("altname"))
            GAMES[cartIndex]. Game_AltName = (std::string)gameNode.getAttribute("altname");
        if (gameNode.isAttributeSet("catalog"))
            GAMES[cartIndex]. Game_Catalog = (std::string)gameNode.getAttribute("catalog");
        if (gameNode.isAttributeSet("publisher"))
            GAMES[cartIndex]. Game_Publisher = (std::string)gameNode.getAttribute("publisher");
        if (gameNode.isAttributeSet("developer"))
            GAMES[cartIndex]. Game_Developer = (std::string)gameNode.getAttribute("developer");
        if (gameNode.isAttributeSet("region"))
            GAMES[cartIndex].Game_Region = (std::string)gameNode.getAttribute("region");
        if (gameNode.isAttributeSet("players"))
            GAMES[cartIndex]. Game_Players = (std::string)gameNode.getAttribute("players");
        if (gameNode.isAttributeSet("date"))
            GAMES[cartIndex].Game_ReleaseDate = (std::string)gameNode.getAttribute("date");

        // See how many carts we have for this game
        int nodesCount = gameNode.nChildNode() ;
        for (int j=0; j<nodesCount; j++)
        {
            // Fill up in formation
            XMLNode cartNode = gameNode.getChildNode(j);
            if ((std::string) cartNode.getName() != (std::string)"cartridge")
            {
                continue;
            }

            if (cartNode.isAttributeSet("system"))
                GAMES[cartIndex].System =  (std::string)cartNode.getAttribute("system");
            if (cartNode.isAttributeSet("crc"))
                GAMES[cartIndex].CRC =  (std::string)cartNode.getAttribute("crc");
            if (cartNode.isAttributeSet("sha1"))
                GAMES[cartIndex].SHA1 =  (std::string)cartNode.getAttribute("sha1");
            if (cartNode.isAttributeSet("dump"))
                GAMES[cartIndex].Dump =  (std::string)cartNode.getAttribute("dump");
            if (cartNode.isAttributeSet("dumper"))
                GAMES[cartIndex].Dumper =  (std::string)cartNode.getAttribute("dumper");
            if (cartNode.isAttributeSet("datedumped"))
                GAMES[cartIndex].DateDumped =  (std::string)cartNode.getAttribute("datedumped");
            // Board info
            XMLNode boardNode = cartNode.getChildNode("board");
            if (boardNode.isAttributeSet("type"))
                GAMES[cartIndex].Board_Type =  (std::string)boardNode.getAttribute("type");
            if (boardNode.isAttributeSet("pcb"))
                GAMES[cartIndex].Board_Pcb =  (std::string)boardNode.getAttribute("pcb");
            if (boardNode.isAttributeSet("mapper"))
                GAMES[cartIndex].Board_Mapper =  (std::string)boardNode.getAttribute("mapper");

            // PRG info
            XMLNode prgNode = boardNode.getChildNode("prg");
            if (prgNode.isAttributeSet("name"))
                GAMES[cartIndex].PRG_name = (std::string)prgNode.getAttribute("name");
            if (prgNode.isAttributeSet("size"))
                GAMES[cartIndex].PRG_size = (std::string)prgNode.getAttribute("size");
            if (prgNode.isAttributeSet("sha1"))
                GAMES[cartIndex].PRG_sha1 = (std::string)prgNode.getAttribute("sha1");
            if (prgNode.isAttributeSet("crc"))
                GAMES[cartIndex].PRG_crc = (std::string)prgNode.getAttribute("crc");

            // CHR info
            XMLNode chrNode = boardNode.getChildNode("chr");
            if (chrNode.isAttributeSet("name"))
                GAMES[cartIndex].CHR_name = (std::string)chrNode.getAttribute("name");
            if (chrNode.isAttributeSet("size"))
                GAMES[cartIndex].CHR_size = (std::string)chrNode.getAttribute("size");
            if (chrNode.isAttributeSet("sha1"))
                GAMES[cartIndex].CHR_sha1 = (std::string)chrNode.getAttribute("sha1");
            if (chrNode.isAttributeSet("crc"))
                GAMES[cartIndex].CHR_crc = (std::string)chrNode.getAttribute("crc");

            // PAD
            XMLNode padNode = boardNode.getChildNode("pad");
            if (padNode.isAttributeSet("h"))
                GAMES[cartIndex].PAD_h = (std::string)padNode.getAttribute("h");
            if (padNode.isAttributeSet("v"))
                GAMES[cartIndex].PAD_v = (std::string)padNode.getAttribute("v");

            // CIC
            XMLNode cicNode = boardNode.getChildNode("cic");
            if (cicNode.isAttributeSet("type"))
                GAMES[cartIndex].CIC_type = (std::string)cicNode.getAttribute("type");

            // Chips, VRAMS and WRAMS !!
            // These information are VERY IMPORTANT for setting up boards.
            int childrenCount =  boardNode.nChildNode();
            if (childrenCount>0)
            {
                for (int c=0; c<childrenCount; c++)
                {
                    XMLNode childNode = boardNode.getChildNode(c);
                    if ((std::string) childNode.getName() == (std::string)"chip")
                    {
                        if (childNode.isAttributeSet("type"))
                            GAMES[cartIndex].chip_types += (std::string)childNode.getAttribute("type") + ", ";
                    }
                    if ((std::string) childNode.getName() == (std::string)"vram")
                    {
                        if (childNode.isAttributeSet("size"))
                            GAMES[cartIndex].VRAM_sizes += (std::string)childNode.getAttribute("size")+ ", ";
                    }

                    if ((std::string) childNode.getName() == (std::string)"wram")
                    {
                        if (childNode.isAttributeSet("size"))
                            GAMES[cartIndex].WRAM_sizes += (std::string)childNode.getAttribute("size");
                        if (childNode.isAttributeSet("battery"))
                            GAMES[cartIndex].WRAM_sizes +=" "+ (std::string)childNode.getAttribute("size")+", ";
                        else
                            GAMES[cartIndex].WRAM_sizes +=", ";
                    }
                }
            }

            cartIndex++;
        }
    }
}
int GetGameIndex(std::string gameCRC)
{
    if (GAMES==NULL)
    {
        return -1;
    }

    for (int i=0; i<GamesCount; i++)
    {
        if (GAMES[i].CRC == gameCRC)
        {
            // This is it !
            return i;
        }
    }
    // Reached here means game doesn't found
    return -1;
}
bool ContainChip(int gameIndex,std::string chipName)
{
    std::size_t  found=GAMES[gameIndex].chip_types.find(chipName);
    if (found!=std::string::npos)
    {
        return true;
    }
    return false;
}
bool ConatinSystem(int gameIndex,std::string systemName)
{
    std::size_t  found=GAMES[gameIndex].System.find(systemName);
    if (found!=std::string::npos)
    {
        return true;
    }
    return false;
}
bool ConatinBoardType(int gameIndex,std::string boardType)
{
    std::size_t  found=GAMES[gameIndex].Board_Type.find(boardType);
    if (found!=std::string::npos)
    {
        return true;
    }
    return false;
}
void Split(std::string item,std::string * &items, int &items_count )
{
    vector<std::string> its;
    std::string delimiter = ", ";

    size_t pos = 0;
    std::string token;
    while ((pos = item.find(delimiter)) != std::string::npos)
    {
        token = item.substr(0, pos);
        its.push_back(token);
        item.erase(0, pos + delimiter.length());
    }

    items_count = (int)its.size();

    items=new std::string[items_count];
    for (int i=0; i<items_count; i++)
        items[i] = its[i];
}
}
}
