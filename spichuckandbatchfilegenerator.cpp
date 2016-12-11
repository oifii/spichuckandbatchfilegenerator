////////////////////////////////////////////////////////////////
//nakedsoftware.org, spi@oifii.org or stephane.poirier@oifii.org
//
//
//2012dec09, creation for generating chuck files and batch files
//			 for each propellerhead recycle output .wav files.
//
//nakedsoftware.org, spi@oifii.org or stephane.poirier@oifii.org
////////////////////////////////////////////////////////////////

#include <string>
#include <fstream>
#include <vector>

#include <iostream>
#include <sstream>
using namespace std;
//#include <stdlib.h>

#include <ctime>
#include <windows.h>
 
string backtoforwardslash(string backslashstring)
{
	string forwardslashstring=backslashstring;
	for(int i=0; i<forwardslashstring.size(); i++)
	{
		if(forwardslashstring[i]=='\\')
			forwardslashstring[i]='/';
	}
	return forwardslashstring;
}

int main(int argc, char *argv[]);
int main(int argc, char *argv[])
{
	///////////////////
	//read in arguments
	///////////////////
	char wavfoldername[2048] = {"."};
	char wavfilename[2048] = {""};
	double period_s = 1.0f;
	if(argc>1)
	{
		//first argument is the wav filename
		strcpy_s(wavfilename, 2048-1, argv[1]);
	}
	else
	{
		printf("usage: \n");
		printf("       \n");
		printf("spichuckandbatchfilegenerator <sample generic wavfilename> <period in seconds>\n");
	}
	if(argc>2)
	{
		//second argument is the period length in seconds
		period_s = atof(argv[2]);
	}
	ostringstream periodstring;
	periodstring << period_s;

	////////////////////////////////////////////
	//execute cmd line to get all .wav filenames
	////////////////////////////////////////////
	string path = wavfoldername;
	string quote;
	quote = "\"";
	path = path + "\\" + wavfilename + "*.wav";
	string command = "DIR " + quote + path + quote + " /S /B /O:N > filenames.txt";
	//system("DIR *.wav /S /B /O:N > filenames.txt");
	system(command.c_str());

	///////////////////////////////////////
	//load filenames.txt into string vector
	///////////////////////////////////////
	vector<string> filenames;
	ifstream ifs("filenames.txt");
	string temp;
	while(getline(ifs,temp))
		filenames.push_back(temp);

	///////////////////////////////////////
	//load header.txt + bodies + footer.txt
	///////////////////////////////////////
	char header_filename[2048] = {"spichuckandbatchfilegenerator_header.txt"};
	char body1_filename[2048] = {"spichuckandbatchfilegenerator_body1.txt"};
	char body2_filename[2048] = {"spichuckandbatchfilegenerator_body2.txt"};
	char body3_filename[2048] = {"spichuckandbatchfilegenerator_body3.txt"};
	char footer_filename[2048] = {"spichuckandbatchfilegenerator_footer.txt"};
	vector<string>::iterator it;
	vector<string> header_stringvector;
	vector<string> body1_stringvector;
	vector<string> body2_stringvector;
	vector<string> body3_stringvector;
	vector<string> footer_stringvector;
	//string temp;
	//header
	ifstream header_ifs(header_filename);
	while( getline( header_ifs, temp ) )
		header_stringvector.push_back( temp );
	//body1
	ifstream body1_ifs(body1_filename);
	while( getline( body1_ifs, temp ) )
		body1_stringvector.push_back( temp );
	//body2
	ifstream body2_ifs(body2_filename);
	while( getline( body2_ifs, temp ) )
		body2_stringvector.push_back( temp );
	//body3
	ifstream body3_ifs(body3_filename);
	while( getline( body3_ifs, temp ) )
		body3_stringvector.push_back( temp );
	//footer
	ifstream footer_ifs(footer_filename);
	while( getline( footer_ifs, temp ) )
		footer_stringvector.push_back( temp );

	vector<string>::iterator it2;
	for ( it2=filenames.begin() ; it2 < filenames.end(); it2++ )
	{
		////////////////////////////////////////////
		//recombine header.txt + bodies + footer.txt
		////////////////////////////////////////////
		//string chuck_filename = "test.ck";
		string chuck_filename = *it2;
		int lastindex = chuck_filename.find_last_of("."); 
		chuck_filename = chuck_filename.substr(0, lastindex);
		chuck_filename = chuck_filename + "(" + periodstring.str() + ")" + ".ck";
		//batchfile
		/*
		string batch_filename = chuck_filename;
		lastindex = batch_filename.find_last_of("."); 
		batch_filename = batch_filename.substr(0, lastindex);
		batch_filename += ".bat";
		ofstream batchfile_ofs;
		batchfile_ofs.open(batch_filename);
		batchfile_ofs << "echo on\n";
		batchfile_ofs << "start chuck.exe " << quote << chuck_filename << quote << "\n";
		batchfile_ofs.close();
		*/
		//chuckfile
		ofstream chuckfile_ofs;
		chuckfile_ofs.open(chuck_filename);
		//header
		for(it = header_stringvector.begin(); it != header_stringvector.end(); it++)
		{   
			string str = *it;   
			chuckfile_ofs << str << "\n";
		}
		//body1
		/*
		for(it = body1_stringvector.begin(); it != body1_stringvector.end(); it++)
		{   
			string str = *it;   
			chuckfile_ofs << str << "\n";
		}
		*/
		chuckfile_ofs << period_s << "::second => dur T;" << "\n";
		//body2
		for(it = body2_stringvector.begin(); it != body2_stringvector.end(); it++)
		{   
			string str = *it;   
			chuckfile_ofs << str << "\n";
		}	
		//body3
		/*
		for(it = body3_stringvector.begin(); it != body3_stringvector.end(); it++)
		{   
			string str = *it;   
			chuckfile_ofs << str << "\n";
		}
		*/
		string wavfilename2 = *it2;
		wavfilename2 = backtoforwardslash(wavfilename2);
		//chuckfile_ofs << quote << wavfilename2 << quote << " => buf.read;" << "\n"; //*it2 is a wavfilename
		chuckfile_ofs << quote << wavfilename2 << quote << " => filename;" << "\n"; //*it2 is a wavfilename
		//footer
		for(it = footer_stringvector.begin(); it != footer_stringvector.end(); it++)
		{   
			string str = *it;   
			chuckfile_ofs << str << "\n";
		}
		chuckfile_ofs.close();
	}
}

