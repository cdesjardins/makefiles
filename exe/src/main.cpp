#include <iostream>

void sharedLib();
void staticLib();

int main()
{
    std::cout<<"hello world"<<std::endl;
    sharedLib();
    staticLib();
    return 0;
}
