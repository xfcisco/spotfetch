#include <boost/algorithm/string.hpp>
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

int main() {
   Json json;
   system("playerctl -p spotify metadata mpris:trackid > spotfetch.tmp");
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
      while (!(Art[i].length()==30)) {
         if (Art[i].length()>=30) {
            while (!(Art[i].length()==30)) Art[i].pop_back();
         } else { 
            Art[i].append(" ");
         }
      }

      cout << "│ " << Art[i] << " │" << endl;
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
      case 'r': system("playerctl -p spotify metadata xesam:artist > spotfetch.tmp"); break;
      case 's': system("playerctl -p spotify metadata xesam:title > spotfetch.tmp"); break;
      case 'a': system("playerctl -p spotify metadata xesam:album > spotfetch.tmp"); break;
   };

   f.read(data, sizeof(string));
   f.close();
   return string(data);
};
