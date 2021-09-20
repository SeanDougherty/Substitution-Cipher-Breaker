#include <iostream>
#include <ios>
#include <string>
#include <fstream>
#include <utility>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

map<char, int> getCharFrequency(ifstream& cipherFile);
void printMap(map<string, int>& myMap);
void printMap(map<char, int>& myMap);
void printMap(map<char, char>& myMap);
void printMapNoLines(map<char, char>& myMap);
int scoreText(string& pText, map<string, int> dictionary);
map<string, int> generateDictionary(ifstream& dictionaryFile);
map<char, char> generateSubMapFromFrequencyMap(map<char, int>& frequencyMap);
map<char, char> generateSubMapFromKey(string& key);
map<char, char> replaceKeyCharacters(map<char, char>& subMap);
map<char, char> autoReplaceKeyCharacters(map<char, char>& subMap);
string decrypt(map<char, char>& subMap, ifstream& cipherFile);

map<char, int> scoredFrequency;
struct comp
{
	template<typename T>
	bool operator()(const T& left, const T& right) const
	{
		if(left.second != right.second)
			return left.second > right.second;

		return left.first < right.first;
	}
};

void autoAttemptDecryption(const string&  dictionaryFileName, const string& cipherFileName)
{
	int currentScore = 0;
	int previousScore = 0;
	ifstream cipherFileForCharFreq (cipherFileName, ifstream::in);
	ifstream cipherFileForSub (cipherFileName, ifstream::in);
	map<char, int> charFrequencyMap = getCharFrequency(cipherFileForCharFreq);
	map<char, char> subMap = generateSubMapFromFrequencyMap(charFrequencyMap);
	cout << "Your key: " << '\n';
	cout << "PLAIN: ABCDEFGHIJKLMNOPQRSTUVWXYZ" << '\n';
	cout << "KEY:   ";
	printMapNoLines(subMap);
	cout << '\n';
	string decryptedText = decrypt(subMap, cipherFileForSub);
	ifstream dictionaryFile (dictionaryFileName, ifstream::in);
	map<string,int> dictionary = generateDictionary(dictionaryFile);
	currentScore = scoreText(decryptedText, dictionary);

	string option;
	cout << "Your decryption:" << '\n';
	cout << decryptedText << '\n';
	cout << '\n' << "What would you like to do next? type h and then Enter for options." << '\n';
	bool isValidInput;
	while(option[0] != 'q')
	{	
		option = "";
		isValidInput = false;
		while(!isValidInput)
		{
			cin >> option;
			if(option.length() != 1)
			{
				cout << "Incorrect length of input, try again." << '\n';
				isValidInput = false;
			}else if(!(option[0] == 'h' || option[0] == 'r' || option[0] == 'a' || option[0] == 'k'  || option[0] == 'p' || option[0] == 'n' || option[0] == 'q'))
			{
				cout << "Unrecognized input, try again." << '\n';
				isValidInput = false;
			} else
			{
				isValidInput = true;
			}
		}
	

		switch(option[0])
		{
			case 'h':
				{
				cout << "p : print the original ciphertext" << '\n';
				cout << "n : supply a new key" << '\n';
				cout << "r : replace a character in the current decrypted text with a different character" << '\n';
				cout << "k : print the key that was used" << '\n';
				cout << "a : execute one round of automatic key guessing" << '\n';
				cout << "q : quit analysis and return to command line" << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'k':
				{
				cout << "The key that was used:" << '\n';
				cout << "PLAIN: ABCDEFGHIJKLMNOPQRSTUVWXYZ" << '\n';
				cout << "KEY:   ";
				printMapNoLines(subMap);
				cout << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'r':
				{
				replaceKeyCharacters(subMap);
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;	
				}
			case 'p':
				{
				char oneChar;
				ifstream cipherFileToPrint (cipherFileName, ifstream::in);
				while(cipherFileToPrint.eof() == false)
				{
					oneChar = cipherFileToPrint.get();
					cout << oneChar;
				}
				cout << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'n':
				{
				cout << "Your new Key:" << '\n';
				string newKey;
				cin >> newKey;

				if(newKey.length() != 26)
				{
					cout << "Incorrect key length, expecting key of size: 26" << '\n';
					cout << "Key length found: " << newKey.length() << '\n';
				}else
				{
					subMap = generateSubMapFromKey(newKey);
					ifstream newStream (cipherFileName, ifstream::in);
					decryptedText = decrypt(subMap, newStream);
					scoreText(decryptedText, dictionary);
					cout << "Your decryption:" << '\n';
					cout << decryptedText << '\n';
				}
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'a':
				{
				previousScore = currentScore;
				subMap = autoReplaceKeyCharacters(subMap);
				ifstream newStream (cipherFileName, ifstream::in);
				decryptedText = decrypt(subMap, newStream);
				currentScore = scoreText(decryptedText, dictionary);
				cout << "Your previous score: " << previousScore << '\n';
				cout << "Your decryption:" << '\n';
				cout << decryptedText << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'q':
				{
				cout << "Thank you! Have a nice day!" << '\n';
				break;
				}
			default:
				{ break; }
		}
	}

	return;
}

void customAttemptDecryption(const string& cipherFileName, const string& dictionaryFileName)
{
	string customKey;
	int currentScore = 0;
	int previousScore = 0;

	cout << "Key:" << '\n';
	cin >> customKey;

	if(customKey.length() != 26)
	{
		cout << "Incorrect key length, expecting key of size: 26" << '\n';
		return;
	}

	map<char, char> subMap = generateSubMapFromKey(customKey);
	
	ifstream cipherFile (cipherFileName, ifstream::in);
	cout << "Your Key: " << '\n';
	cout << "PLAIN: ABCDEFGHIJKLMNOPQRSTUVWXYZ" << '\n';
	cout << "KEY:   ";	
	printMapNoLines(subMap);
	cout << '\n';
	string decryptedText = decrypt(subMap, cipherFile);

	ifstream dictionaryFile (dictionaryFileName, istream::in);
	map<string,int> dictionary = generateDictionary(dictionaryFile);
	currentScore = scoreText(decryptedText, dictionary);
	
	string option;
	cout << "Your decryption:" << '\n';
	cout << decryptedText << '\n';
	cout << '\n' << "What would you like to do next? type h and then Enter for options." << '\n';
	bool isValidInput;
	while(option[0] != 'q')
	{
		option = "";
		isValidInput = false;
		while(!isValidInput)
		{
			cin >> option;
			if(option.length() != 1)
			{
				cout << "Incorrect length of input, try again." << '\n';
				isValidInput = false;
			}else if(!(option[0] == 'h' || option[0] == 'r' || option[0] == 'a' || option[0] == 'k' || option[0] == 'p' || option[0] == 'n' || option[0] == 'q'))
			{
				cout << "Unrecognized input, try again." << '\n';
				isValidInput = false;
			} else
			{
				isValidInput = true;
			}
		}
		switch(option[0])
		{
			case 'h':
				{
				cout << "p : print the original ciphertext" << '\n';
				cout << "n : supply a new key" << '\n';
				cout << "r : replace a character in the current decrypted text with a different character" << '\n';
				cout << "k : print the key that was used" << '\n';
				cout << "a : execute one round of automatic key guessing" << '\n';
				cout << "q : quit analysis and return to command line" << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'k':
				{
				cout << "The key that was used:" << '\n';
				cout << "PLAIN: ABCDEFGHIJKLMNOPQRSTUVWXYZ" << '\n';
				cout << "KEY:   ";
				printMapNoLines(subMap);
				cout << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'r':
				{
				replaceKeyCharacters(subMap);
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'p':
				{
				char oneChar;
				ifstream cipherFileToPrint (cipherFileName, ifstream::in);
				while(cipherFileToPrint.eof() == false)
				{
					oneChar = cipherFileToPrint.get();
					cout << oneChar;
				}
				cout << '\n';
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'n':
				{
				cout << "Your new Key:" << '\n';
				string newKey;
				cin >> newKey;

				if(newKey.length() != 26)
				{
					cout << "Incorrect key length, expecting key of size: 26" << '\n';
					cout << "Key length found: " << newKey.length() << '\n';	
				}else
				{
					subMap = generateSubMapFromKey(newKey);
					ifstream newStream (cipherFileName, ifstream::in);
					decryptedText = decrypt(subMap, newStream);
					scoreText(decryptedText, dictionary);
					cout << "Your decryption:" << '\n';
					cout << decryptedText << '\n';
				}
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'a':
				{
				previousScore = currentScore;
				subMap = autoReplaceKeyCharacters(subMap);
				ifstream newStream (cipherFileName, ifstream::in);
				decryptedText = decrypt(subMap, newStream);
				currentScore = scoreText(decryptedText, dictionary);
				cout << "Your previous score: " << previousScore << '\n';
				cout << "Your decryption:" << '\n';
				cout << decryptedText << '\n';		
				cout << "What would you like to do next? type h and then enter for options." << '\n';
				break;
				}
			case 'q':
				{
				cout << "Thank you! Have a nice day!" << '\n';
				break;
				}
			default:
				{ break; }
		}

	}

	return;
}

void noAttemptDecryption(const string& cipherFileName)
{
	ifstream cipherFile (cipherFileName, ifstream::in);
	if(cipherFile.fail())
		cout << "error loading file: " << cipherFileName << '\n';
	map<char, int> frequencyMap = getCharFrequency(cipherFile);
	cout << "Frequency of characters in cipher:" << '\n';
	printMap(frequencyMap);
	return;
}

map<char,char> replaceKeyCharacters(map<char,char>& subMap)
{
	string cCharS;
	string pCharS;
	cout << "Where (CHAR1) is in the current decrypted text, it should be (CHAR2) instead" << '\n';
	cout << "CHAR1: ";
	cin >> cCharS;
	cout << "CHAR2: ";
	cin >> pCharS;

	char keyCharOne;
	char keyCharTwo;
	for (auto elem : subMap)
	{
		if (elem.second == cCharS[0])
			keyCharOne = elem.first;
		
		if (elem.second == pCharS[0])
			keyCharTwo = elem.first;
	}

	subMap.find(keyCharOne)->second = pCharS[0];
	subMap.find(keyCharTwo)->second = cCharS[0];
	printMapNoLines(subMap);
	return subMap;
}

map<char,char> autoReplaceKeyCharacters(map<char,char>& subMap)
{
	char lowestFreqChar;
	int lowestFreq = 65535;
	char keyCharOne;
	char secondLowestFreqChar;
	int secondLowestFreq = 65535;
	char keyCharTwo;

	for(auto elem : scoredFrequency)
	{
		if ((rand() % 10 + 1) < 8)
		{
		if (elem.second < lowestFreq)
		{
			lowestFreq = elem.second;
			lowestFreqChar = elem.first;			
		} else if (elem.second < secondLowestFreq)
		{
			secondLowestFreq = elem.second;
			secondLowestFreqChar = elem.first;
		}
		}
	}

	cout << "lowest Freq: " << lowestFreqChar << '\n';
	cout << "second lows: " << secondLowestFreqChar <<'\n';

	for(auto elem : subMap)
	{
		if(elem.second == lowestFreqChar)
			keyCharOne = elem.first;

		if(elem.second == secondLowestFreqChar)
			keyCharTwo = elem.first;
	}
	
	subMap.find(keyCharOne)->second = secondLowestFreqChar;
	subMap.find(keyCharTwo)->second = lowestFreqChar;
	printMapNoLines(subMap);
	return subMap;
}

int scoreText(string& pText, map<string, int> dictionary)
{
	scoredFrequency.clear();
	int score = 0;
	int longestWordLength = 3;
	int shortestWordLength = 3;
	for(auto elem : dictionary)
	{
		if (elem.second > longestWordLength)
			longestWordLength = elem.second;
	}
	
	string testString;
	int endOfSubStr;
	for(int i=0; i<pText.length()-2; i++)
	{
		for(int j=shortestWordLength; j<longestWordLength; j++)
		{
			endOfSubStr = i+j;
			if (endOfSubStr > pText.length()-1)
			{ 
	
			}else
			{
			testString = pText.substr(i,j);
			
			for(auto elem : dictionary)
			{
				if (elem.second == j)
				{
					if(testString.compare(elem.first) == 0)
					{
						score++;
						for(int k=0; k<testString.length()-1; k++)
						{
							
							scoredFrequency[testString[k]]++;
						}
					}			
				}					
			}
			}
		}
	
	}
	cout << "How many times each character appeared in a meaningful word:" << '\n';
	cout << "(If the character has a low frequency, consider switching it with another character of low frequency)" << '\n';
	printMap(scoredFrequency);
	cout << "Your Score: " << score << '\n';
	return score;
}

map<string, int> generateDictionary(ifstream& dictionaryFile)
{
	map<string, int> myDictionary;
	string word;
	while(dictionaryFile.eof() == false)
	{
		getline(dictionaryFile,word);
		word.erase(std::remove(word.begin(), word.end(), '\r'), word.end());
		myDictionary.insert(pair<string, int>(word, word.length()));
	}
	
	return myDictionary;
}

void printMap(map<char, int>& myMap)
{
	for(auto elem : myMap)
	{
		cout << elem.first << " : " << elem.second << '\n';
	}
	
	return;
}

void printMap(map<char, char>& myMap)
{
	map<char, char> newMap;
	for(auto elem : myMap)
	{
		newMap.insert(pair<char,char>(elem.second, elem.first));
	}

	for(auto elem : newMap)
	{
		cout << elem.first << " : " << elem.second << '\n';
	}
	
	return;
}

void printMapNoLines(map<char, char>& myMap)
{
	map<char, char> newMap;
	for(auto elem : myMap)
	{
		newMap.insert(pair<char,char>(elem.second, elem.first));
	}

	for(auto elem : newMap)
	{
		cout << elem.second;
	}
	cout << '\n';

	return;
}

void printMap(map<string, int>& myMap)
{
	for(auto &elem : myMap)
	{
		cout << elem.first;
		cout << " : " << elem.second << '\n';
	}
}

map<char, int> getCharFrequency(ifstream& cipherFile)
{
	char oneChar;
	map<char, int> frequencyMap;
		
	oneChar = cipherFile.get();
	
	while(cipherFile.eof() == false)
	{
		if (oneChar == '\n' || oneChar == '\0' || oneChar == '\r')
		{
			//Improper Character found
		} else
		{
			frequencyMap[oneChar]++;
		}
		oneChar = cipherFile.get();
	}

	return frequencyMap;
}

string decrypt(map<char, char>& subMap, ifstream& cipherFile)
{
	char oneChar;
	string decryptedText; 

	oneChar = cipherFile.get();

	while(cipherFile.eof() == false)
	{
		if ( oneChar != '\n' && oneChar != '\r' )
		{
			decryptedText.append(1, subMap.find(oneChar)->second);
		}
		oneChar = cipherFile.get();
	}

	cipherFile.close();
	
	return decryptedText;
}

map<char, char> generateSubMapFromFrequencyMap(map<char, int>& frequencyMap)
{
	int counter = 0;
	printMap(frequencyMap);
	
	string freqAlphabet = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
	string cipherBet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string plainBet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	set< pair<char,int>, comp> frequencySet(frequencyMap.begin(), frequencyMap.end());
	map<char, char> subMap;

	for (auto &freqPair : frequencySet)
	{
		subMap.insert(pair<char,char>(freqPair.first, freqAlphabet.at(counter)));	
		cipherBet.erase(remove(cipherBet.begin(), cipherBet.end(), freqPair.first), cipherBet.end());
		plainBet.erase(remove(plainBet.begin(), plainBet.end(), freqAlphabet.at(counter)), plainBet.end());
		counter++;
	}

	for (int i=0; i<plainBet.length(); i++)
	{
		subMap.insert(pair<char,char>(cipherBet[i],plainBet[i]));
	}
	
	return subMap;
}

map<char, char> generateSubMapFromKey(string& key)
{
	map<char, char> subMap;
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i<key.length(); i++)
	{
		subMap.insert(pair<char,char>(key.at(i), alphabet.at(i)));
	}

	return subMap;
}

int main(int argc, char** argv ) {
	if(argc != 4)
	{
		cout << "Incorrect number of input parameters, see README for details" << '\n';
		return 0;
	}

	string analysisMethod = argv[3];
	const string dictionaryFileName = argv[2];
	const string cipherFileName = argv[1];
	
	char analysisFlag = analysisMethod.at(1);

	switch(analysisFlag)
	{
		case 'a':
			autoAttemptDecryption(dictionaryFileName, cipherFileName);
			break;
		case 'c':
			customAttemptDecryption(cipherFileName, dictionaryFileName);
			break;
		case 'n':
			noAttemptDecryption(cipherFileName);
			break;
		default:
			cout << "Analysis Method not recognized, see README for details" << '\n';
			return 0;
	}


	return 0;
}
