#include <iostream>
#include <fstream>
#include <vector>
#include <string>	
#include <map> 		//map
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include "Ngram.h"
#include "Vocab.h"

using namespace std;

#define	MAXWORD	3000
#define	MAXLEN	100
#define INF 1e9

Vocab voc;
Ngram lm( voc, 2);
map <string, vector<string> > mapping;


double Bigram_Prob(const char *w1, const char *w2, Ngram &lm, Vocab &voc){
	VocabIndex wid1 = voc.getIndex(w1);
	VocabIndex wid2 = voc.getIndex(w2);

	//OOV
	if( wid1 == Vocab_None )  wid1 = voc.getIndex(Vocab_Unknown);
	if( wid2 == Vocab_None )  wid2 = voc.getIndex(Vocab_Unknown);

	VocabIndex context[] = { wid1, Vocab_None };
	return lm.wordProb( wid2, context);
}

void Decode( ofstream &output_file, string &line){	//Viterbi algorithm
	double delta[MAXLEN][MAXWORD] = {{0.0}};
	int backtrac_table[MAXLEN][MAXWORD] = {{0}};

	string zhuyin;

	//initialize
	zhuyin.assign( line.begin(), line.begin()+2);	
	for( int i = 0 ; i < mapping[zhuyin].size() ; i++)
		delta[0][i] = Bigram_Prob("<s>", mapping[zhuyin][i].c_str(), lm, voc);
	int previous_state = mapping[zhuyin].size(); string prev_zhuyin = zhuyin;
	//init success
	
	//cal delta and record back tracking
	for( int t = 1 ; t < line.size()/2 ; t++){
		zhuyin.assign( line.begin()+2*t, line.begin()+2*t+2);
		for( int j = 0 ; j < mapping[zhuyin].size() ; j++){
			 //if zhuyin is char -> won't enter for loop
			double max_prob = -INF; int max_index = 0;

			for( int i = 0 ; i < previous_state ; i++){
					double current_prob = Bigram_Prob( mapping[prev_zhuyin][i].c_str(), mapping[zhuyin][j].c_str(), lm, voc) + delta[t-1][i];
					if( current_prob > max_prob ){ max_prob = current_prob; max_index = i;}
				}
				delta[t][j] = max_prob; backtrac_table[t][j] = max_index;
			}
			previous_state = mapping[zhuyin].size(); prev_zhuyin = zhuyin;
		}


		//last word
		double max_prob = -INF; int max_index = 0;
		for( int i = 0 ; i < previous_state ; i++){
			double current_prob = Bigram_Prob( mapping[prev_zhuyin][i].c_str(), "</s>", lm, voc) + delta[line.size()/2-1][i];
			if( current_prob > max_prob ){ max_prob = current_prob; max_index = i;}
		}

		// backtracking
		stack<string> decoded;
		for( int t = line.size()/2-1 ; t >= 0 ; t --){
			zhuyin.assign( line.begin()+2*t, line.begin()+2*t+2);
			if( mapping[zhuyin].size() != 0 ) decoded.push( mapping[zhuyin][max_index]);
			else decoded.push( zhuyin);

			if( t > 0 ) max_index = backtrac_table[t][max_index];
		}

		// output sentence
		output_file << "<s>";
		while( !decoded.empty()){
			output_file << " " << decoded.top();
			decoded.pop();
		}
		output_file << " </s>" << endl;

	return;
}

int main( int argc, char* argv[]){

	//mapping: ZhuYin-Big5: argv[2]
	ifstream mapfile; 	//mapping input file stream
	mapfile.open( argv[2]);  //file open success
	string read_line;
	while( getline( mapfile, read_line) ){
		string zhuyin, word;

		//Big5 2 byte
		zhuyin.assign( read_line.begin(), read_line.begin()+2 );
		//cout << zhuyin;
		int len = read_line.size();
		for( int i = 3 ; i < len ; i+=3 ){ //and one space
			word.assign( read_line.begin()+i, read_line.begin()+i+2);
			mapping[zhuyin].push_back( word);
		}
	}
	mapfile.close();
	//mapping success

	//language model: argv[3]
	File lanmodel_File( argv[3], "r");
	lm.read(lanmodel_File);
	lanmodel_File.close();
	//language model success

	string line;	
	ifstream decode_file; decode_file.open( argv[1]); //input file to be decoded
	ofstream output_file; output_file.open( argv[4]); //output file

	while( getline( decode_file, line) ){

		line.erase( remove( line.begin(), line.end(), ' '), line.end() );	// erase space
		//cout << line << endl;

		//can done this
		//decode ZhuYin
		Decode( output_file, line);
		//cout << "after decode: where is segmentation fault" << endl;
	}

	decode_file.close(); output_file.close();

	return 0;
}
