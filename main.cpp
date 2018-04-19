#include<iostream>
#include<queue>
#include<fstream>

using namespace std;

int freq[256];

class HuffmanNode{
public:
	char c;
	int freq;
	HuffmanNode *left, *right;

	HuffmanNode(char c, int freq){
		this->c = c;
		this->freq = freq;
		left = right = NULL;
	}
};

class Compare{
public:
	bool operator()(HuffmanNode* a, HuffmanNode *b){
		return a->freq > b->freq;
	}
};

priority_queue<HuffmanNode*, 
				vector<HuffmanNode*>,
				Compare > pq; //node asli

queue<HuffmanNode*> q; //internal node

void makeTree(){
	while( !(pq.empty() && q.size() == 1) ){
		HuffmanNode *left, *right, *top;

		if(q.empty()){
			left = pq.top();
			pq.pop();

			right = pq.top();
			pq.pop();
		} else if(pq.empty()){
			left = q.front();
			q.pop();

			right = q.front();
			q.pop();
		} else {
			if(pq.top()->freq <= q.front()->freq){
				left = pq.top();
				pq.pop();
			} else {
				left = q.front();
				q.pop();
			}

			if( q.empty() && !pq.empty() ){
				right = pq.top();
				pq.pop();
			} else if( pq.empty() && !q.empty() ){
				right = q.front();
				q.pop();
			} else {
				if(pq.top()->freq <= q.front()->freq){
					right = pq.top();
					pq.pop();
				} else {
					right = q.front();
					q.pop();
				}
			}
		}

		top = new HuffmanNode('\0', left->freq + right->freq);
		top->left = left;
		top->right = right;
		q.push(top);
	} //end while
} //end function

int total_bit = 0; //keseluruhan teks setelah translate
int total_code_bit = 0; //total huffman code
string huffman_code[256];
void generateCode(HuffmanNode* curr = q.front(),
					string code = ""){
	if(!curr) return;

	if(curr->c != '\0'){
		// cout << curr->c << '\t' 
		// 	 << curr->freq << '\t' 
		// 	 << code << '\t'
		// 	 << code.length() << '\t'
		// 	 << code.length() * curr->freq << '\t'
		// 	 << endl;
			
		huffman_code[curr->c] = code;

		total_bit += code.length() * curr->freq;
		total_code_bit += code.length();

		return;
	}

	generateCode(curr->left, code + "0");
	generateCode(curr->right, code + "1");
}

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("argumen harus ada nama file");
		return 1;
	}

	fstream fin(argv[1], fstream::in);
	char c;
	while(fin >> noskipws >> c){
		freq[c]++;
	}
	fin.close();

	for(int i = 1; i < 256; i++){
		if(!freq[i]) continue;
		pq.push( new HuffmanNode((char) i, freq[i]) );
	}

	makeTree();
	generateCode();

	cout << total_bit << '\t' << total_code_bit << endl;
	cout << total_bit / 8 << endl;

	ofstream out((string(argv[1]) + ".out").c_str());

	fin.open(argv[1], fstream::in);
	
	int i = 0;
	c = '\0';
	char c1;
	while(fin >> noskipws >> c1){
		string code = huffman_code[c1];

		for(int j = 0; j < code.length(); j++){
			if(code[j] == '1') {
				c |= (1 << (7 - i));
			}
			i++;

			if(i == 8){
				cout << "test" << endl;
				out.write((char*)&c, sizeof(char));
				c = '\0';
				i = 0;
			}
		}
	}
	if(i != 0) out.write((char*)&c, sizeof(char));
	return 0;
}