#include <iostream>
#include <fstream>
#include <vector>
#include <string>	
#include <map> 		//map
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include "Ngram.h"
#include "Vocab.h"

using namespace std;

#define	WORD_MAX	5000
#define	SEN_MAXLEN	500


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
	cout << "where is segmentation fault" << endl;
	double delta[SEN_MAXLEN][WORD_MAX] = {{0.0}};		
	int backtrac_table[SEN_MAXLEN][WORD_MAX] = {{0}};	

	cout << "where is segmentation fault" << endl;

	// initialize
	string zhuyin; zhuyin.assign( line.begin(), line.begin()+2);
	int N = mapping[zhuyin].size(); //if zhuyin is Chinese char -> won't enter for loop
	for( int i = 0 ; i < N; i++)
		delta[0][i] = Bigram_Prob( "<s>", mapping[zhuyin][i].c_str(), lm, voc);
	int preN = N; string prev_zhuyin = zhuyin; 

	cout << "before cal: where is segmentation fault" << endl;
	//cal delta & record back track
	for( int t = 1 ; t < line.size()/2 ; t++){ //at word t (2t)
		zhuyin.assign( line.begin()+2*t, line.begin()+2*t+2);
		int N = mapping[zhuyin].size(); //if zhuyin is Chinese char -> won't enter for loop
		for( int j = 0 ; j < N ; j++){
			int max_idx = 0; double max_prob = 0.0;
 			for( int i = 0 ; i < preN ; i++){
				double current_prob = Bigram_Prob( mapping[prev_zhuyin][i].c_str(), mapping[zhuyin][j].c_str(), lm, voc) + delta[t-1][i];

				if( current_prob > max_prob ){ max_prob = current_prob; max_idx = i;}
			}
			delta[t][j] = max_prob; backtrac_table[t][j] = max_idx;
		}
		preN = N; prev_zhuyin = zhuyin;
	}

	//last word
	int max_idx; double max_prob = 0.0;
	for( int i = 0 ; i < preN ; i++){
		double current_prob = Bigram_Prob( mapping[prev_zhuyin][i].c_str(), "</s>", lm, voc) + delta[line.size()/2-1][i];
		if( current_prob > max_prob ){ max_prob = current_prob; max_idx = i;}
	}

	
	//backtracking & output
	vector<string> decoded;
	for( int t = line.size()/2-1 ; t >= 0; t--){
		zhuyin.assign( line.begin()+2*t, line.begin()+2*t+2);
		decoded.push_back( mapping[zhuyin][max_idx]);
		if( t > 0 ) max_idx = backtrac_table[t][max_idx];
	}
	//reverse( decoded.begin(), decoded.end());
	output_file << "where is segmentation fault" << endl;
	output_file << "<s>";
	for (int i = decoded.size()-1 ; i >= 0 ; i--){
		output_file << " " << decoded[i];
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

		//line.assign(line.begin() + 1, line.begin() - 1);

		//can done this
		//decode ZhuYin
		cout << line << endl;
		Decode( output_file, line);
		cout << "after decode: where is segmentation fault" << endl;
	}

	decode_file.close(); output_file.close();

	return 0;
}
