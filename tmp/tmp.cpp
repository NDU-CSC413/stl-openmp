#include <iostream>

int main(){
#ifdef DIRECTIVE
	std::cout << " DIRECTIVE =" << DIRECTIVE << "\n";
#else
	std::cout << "NOT defined \n";
#endif 
}
