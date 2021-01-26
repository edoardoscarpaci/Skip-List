#include <vector>
#include <random>
#include <iostream>
/*
	Scarpaci Edoardo Alessandro M-Z 1000001781
	
*/

template <typename T,int MAXLVL = 32>
class Node_Skip {
	T _Key;
	Node_Skip* _NextNodes[MAXLVL+1];
	//std::vector<Node_Skip*> _NextNodes; 
public:
	
	Node_Skip() {
		for (int i = 0; i < MAXLVL; i++) {
			_NextNodes[i] = nullptr;
		}

	}
	Node_Skip(T& key) :_Key(key){
		for (int i = 0; i < MAXLVL; i++) {
			_NextNodes[i] = nullptr;
		}
	}

	void setNext(Node_Skip* next , int level) {
		
		if (level + 1 > _NextNodes.size()) {
			_NextNodes.resize(level + 1);
		}

		_NextNodes[level] = next;
	}


	Node_Skip* getNext(int level) {
		if (level > MAXLVL) {
			std::cout << "Accessing illegal Zone " << level << " " <<_NextNodes.size(); 
		}

		return _NextNodes[level];
	}

	T getKey() const {
		return _Key;
	}
};



template <typename T,int MAXLVL = 32>
class SkipList {
private:
	using Node = Node_Skip<T, MAXLVL>;

	void _inizialize() { //Collega la sentinella testa in ogni livello con la sentinella coda NIL
		_Head = new Node();
		for (int i = 0; i < MAXLVL; i++) 
			_Head->setNext(_NIL, i);
	}

	
	int _randomLevel() {
		int level = 0;
		float value = _Distribution(_Generator);
		while (value <= _Probability && level < MAXLVL) {
			value = _Distribution(_Generator);
			
			level++;
		}
		
		return level;
	}

	Node* _search(T key) {
		memset(_PathSearched, 0, sizeof(Node*) * MAXLVL);

		Node* currentNode = _Head;
		for (int level = _CurrentLvL; level >= 0; level--) {
			//Node* nextNode = currentNode->getNext(level);

			while (currentNode->getNext(level) != _NIL && currentNode->getNext(level)->getKey() < key) {
				currentNode = currentNode->getNext(level);
			}

			_PathSearched[level] = currentNode;
		}
		currentNode = currentNode->getNext(0);
		return currentNode;
	}



public:
	
	
	SkipList():_Probability(0.5f), _CurrentLvL(0) {
		_inizialize();
		_Distribution = std::uniform_real_distribution<float>(0.0, 1);
		
	}

	SkipList& insert(T key) {

		Node* x = _search(key); 									
		
		int levelOfNode = _randomLevel();


		if (levelOfNode > _CurrentLvL) {							
			for (int i = _CurrentLvL + 1; i <=levelOfNode; i++) {   
				_PathSearched[i] = _Head;			
				}
			}
			_CurrentLvL = levelOfNode;
		}
		
		x = new Node(key);

		for (int i = 0; i <= levelOfNode; i++) {
			x->setNext(_PathSearched[i]->getNext(i), i);
			_PathSearched[i]->setNext(x,i);
		}
		

		return *this;
	}

	SkipList& erase(T key) {

		Node* nodeToErase = _search(key);
		
		if (nodeToErase->getKey() == key) {
			for (int i = 0; i <= _CurrentLvL; i++) {
				if (_PathSearched[i]->getNext(i) != nodeToErase)
					break;
				_PathSearched[i]->setNext(nodeToErase->getNext(i),i);
			}
		}

		delete nodeToErase;


		while (_CurrentLvL > 0 && _Head->getNext(_CurrentLvL) == _NIL)
			_CurrentLvL--;
		
		return* this;
		
	}

	bool search(T key) {
		Node* x = _search(key);
		return !(x == _NIL || key != x->getKey());
	}

	
	T min() {
		Node* firstNode = _Head->getNext(0);

		if (firstNode != _NIL) return T(firstNode->getKey());
		return T();
	}



	T max() {
		Node* currentNode = _Head;

		for (int level = _CurrentLvL;level >= 0; level--) {
			while (currentNode->getNext(level) != _NIL) {
				currentNode = currentNode->getNext(level);
			}
		}

		if (currentNode != _Head) return T(currentNode->getKey());
		return T();
	}


	void printLevels() {
		Node* currentNode = nullptr;
		for (int i = 0; i <= _CurrentLvL; i++) {
			
			currentNode = _Head->getNext(i); 
			std::cout << "L" << i;

			while (currentNode != _NIL) {
				std::cout << " " << currentNode->getKey();
				currentNode = currentNode->getNext(i);
			}

			std::cout << std::endl;
		}
	}

	int currentLevel() const { return _CurrentLvL; }
	int maxLvL ()const{ return MAXLVL; }

private:
	Node* _Head;
	Node* _NIL;
	Node* _PathSearched[MAXLVL];

	const float _Probability;
	int _CurrentLvL;

	std::default_random_engine _Generator;
	std::uniform_real_distribution<float> _Distribution;
};