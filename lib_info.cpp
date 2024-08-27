// Project 1: this program reads a music library file, replaces underscores with spaces, and prints out a sorted list of artists,
// their albums, and songs along with the total number of songs and their durations.
// Date: 8/28/2024
// Team: Vansh Joshi, Om Patel, Sarvvesh Vindokumar

// credit to Dr. Scott for providing Dr.Plank's structs to help get started with the code
// credit to Dr. Scott for providing code to help us understand how to use iterators and the syntax of a map
// credit to stackoverflow for helping us understand how to add 2 strings and format them
// https://stackoverflow.com/questions/21560353/add-two-string-times-in-java
// credit to stackoverflow for helping us converting an underscore to a space
// https://stackoverflow.com/questions/5252612/replace-space-with-an-underscore
// credit to codesignal for helping us learn how to do time calculations
// https://learn.codesignal.com/preview/lessons/2021/time-calculation-and-string-manipulation-in-c

#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

// Dr. Plank's structs
struct Song{
	string title;
	string time;
	int track;
};

struct Album{
	map<int, Song> songs;
	string time;
	string name;
	int nsongs;
};

struct Artist{
	map<string, Album> albums;
	string time;
	string name;
	int nsongs;
};


// add up the the time durations together and return as a formatted string
string CalcTime (string duration1, string duration2) {

	// extracting minutes and seconds from both durations
	int min1 = stoi(duration1.substr(0, duration1.find(":")));
	int min2 = stoi(duration2.substr(0, duration2.find(":")));
	int sec1 = stoi(duration1.substr(duration1.find(":") + 1));
	int sec2 = stoi(duration2.substr(duration2.find(":") + 1));

	// calculating total minutes and seconds
	int totalMin = min1 + min2 + (sec1 + sec2) / 60;
	int totalSec = (sec1 + sec2) % 60;

	// formating the total string
	string totalString = to_string(totalMin) + ":";

	if (totalSec < 10) {
		totalString += "0" + to_string(totalSec);
	} else {
		totalString += to_string(totalSec);
	}
	return totalString;
}

// function to turn the underscores to a space
string changeToSpace(string text) {
	int len = text.length();
	for(int i = 0; i < len; i++){
		if(text[i] == '_')
			text[i] = ' ';
	}
	return text;
}

int main (int argc, char* argv[]) {
	string title, time, name, album, genre;
	int track;
	map<string, Artist> artistsMap;
	Song songName;
	ifstream file;

	file.open(argv[argc - 1]);
	Album albumName;
	file >> title;

	//Gets the file content all the way till the end
	while (file) {
		file >> time >> name >> album >> genre >> track;

		title = changeToSpace(title);
		album = changeToSpace(album);
		name = changeToSpace(name);
		songName.time = time;
		songName.track = track;
		songName.title = title;

		int notHere = -1;

		// Dr. Scott's code to check if the artist exists in the map
		for (map<string, Artist>::iterator it = artistsMap.begin(); it!= artistsMap.end(); it++) {

			//if artist exist
			if (it->second.name == name){
				notHere = 1;
				albumName.name = album;

				// Dr.Scott's code to check to see if the album exists for this artist
				for (map<string, Album>::iterator it2 = it->second.albums.begin(); it2!= it->second.albums.end(); it2++) {
					if (it2->second.name == album){
						notHere = 2;

						// add the song to the existing album
						it2->second.songs.insert(make_pair(track, songName));
					}

					// if the album does not exist notHere will stay a 1
					else {
						notHere = 1;
					}
				}

				// if the album does not exist, this will create an album and add the song to it
				if (notHere == 1) {
					Album temp;
					temp.name = album;
					temp.songs.insert(make_pair(track, songName));
					it->second.albums.insert(make_pair(album, temp));
				}
			}
		}

		// if artist does not exist, it will create a new artist with the album and the song
		if(notHere == -1){
			Artist artist;
			artist.name = name;
			Album temp;
			temp.name = album;
			temp.songs.insert(make_pair(track, songName));
			artist.albums.insert(make_pair(album, temp));
			artistsMap.insert(make_pair(name, artist));
		}

		file >> title;
	}

	// calculating the total duration and song counts for each artist (used Dr.Scott's code to get started)
	for (map<string, Artist>::iterator it = artistsMap.begin(); it!= artistsMap.end(); it++) {
		string minArtist = "00:00";
		int countArtist = 0;

		for (map<string, Album>::iterator it2 = it->second.albums.begin(); it2!= it->second.albums.end(); it2++) {
			string minAlbum = "00:00";
			int countAlbum = 0;

			for (map<int, Song>::iterator it3 = it2->second.songs.begin(); it3!= it2->second.songs.end(); it3++) {
				minAlbum = CalcTime(minAlbum, it3->second.time);
				countArtist++;
				countAlbum++;
			}
			it2->second.time = minAlbum;
			it2->second.nsongs = countAlbum;
			minArtist = CalcTime(minArtist, minAlbum);

		}
		it->second.nsongs = countArtist;
		it->second.time = minArtist;
	}

	// prints the organized music in an hierarchical format (used Dr.Scott's to help print out everything)
	for (map<string, Artist>::iterator it = artistsMap.begin(); it!= artistsMap.end(); it++) {
		cout << it->first << ": " << it->second.nsongs << ", " << it->second.time<< endl;

		// prints the album name, song count, and duration, indented under the artist
		for (map<string, Album>::iterator it2 = it->second.albums.begin(); it2!= it->second.albums.end(); it2++) {
			cout << "        " << it2->first << ": " <<it2->second.nsongs <<", "<< it2->second.time << endl;

			// prints track number, song title, and duration, further indented under the album
			for (map<int, Song>::iterator it3 = it2->second.songs.begin(); it3!= it2->second.songs.end(); it3++) {
				cout << "                " << it3->first << ". " << it3->second.title << ": " <<it3->second.time << endl;
			}
		}
	}
}  

