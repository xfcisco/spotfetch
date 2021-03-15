#include <boost/algorithm/string.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <json.hpp>
#include <fstream>
using namespace std;
using Json = nlohmann::json;
string UserName();
string GetSong(char);
void trim(string);
static const char* playerName;

int main(int argc, char *argv[]) {
   if (argc < 2) return -1;
   playerName = argv[1];
   Json json;
   system((string("playerctl -p ")+playerName+" metadata mpris:trackid > spotfetch.tmp").c_str());
   fstream f("spotfetch.tmp");
   system("rm spotfetch.tmp");
   string track;
   f >> track;
   system((string("curl https://open.spotify.com/oembed?url=")+track+">spotfetch.tmp").c_str());
   fstream out("spotfetch.tmp");
   out >> json;
   string art_url = json["thumbnail_url"];
   system("clear");
   out.close();
   
   system("rm ./spotfetch.tmp");
   system(string("wget -O thumb.png ").append(art_url).c_str());
   system("clear");
   system("chafa -s 30x15 thumb.png >> spotfetch.tmp");
   
   fstream artFile("spotfetch.tmp");
   char art[80000];
   artFile.read(art, 80000);
   vector<string> Art;
   boost::split(Art, string(art), boost::is_any_of("\n"));
   string songInfo[3] = {
      GetSong('r'), GetSong('s'), GetSong('a')
   };
   
   for (string& info : songInfo) {
      for (int i=0; i<=info.length()-1; ++i) {
         if (info[i] == '\n') info[i] = ' ';
      }
   }

   Art.push_back(songInfo[0]);
   Art.push_back(songInfo[1]);
   Art.push_back(songInfo[2]);

   cout << " ────────────────────────────────" << endl;
   for(int i = 0; i<=Art.size()-5; ++i) {
      cout << "│ " << Art[i] << " │" << endl;
   }
   cout << " ────────────────────────────────" << endl;

   for (int i = Art.size()-3; i <= Art.size()-1; ++i) {
      stringstream ss;
      Art[i].resize(35);
      ss << "│ " << Art[i];
      string line = ss.str();
//      line.replace(34, 1, " │");
      line[line.length()-1] = '|';

      cout << line << endl;
   }

   cout << " ────────────────────────────────" << endl;

   artFile.close();
   system("rm -f spotfetch.tmp");

   system("rm thumb.png");
};

string GetSong(char req) {
   char data[sizeof(string)];
   fstream f("spotfetch.tmp");

   switch (req) {
      case 'r': system((string("playerctl -p ")+playerName+" metadata xesam:artist > spotfetch.tmp").c_str()); break;
      case 's': system((string("playerctl -p ")+playerName+" metadata xesam:title > spotfetch.tmp").c_str()); break;
      case 'a': system((string("playerctl -p ")+playerName+" metadata xesam:album > spotfetch.tmp").c_str()); break;
   };

   f.read(data, sizeof(string));
   f.close();
   return string(data);
};
