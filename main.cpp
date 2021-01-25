#include "List.h"
#include <cstdlib>
int main() {
	srand(2);
	SkipList<int> list;

	for (int i = 0; i < 20; i++) {
		int value = rand();
		list.insert(value);
		std::cout << std::endl;
	}
	list.erase(24406);
	list.printLevels();

	std::cout << "Min: " << list.min() << " Max: " << list.max() << std::endl;
}