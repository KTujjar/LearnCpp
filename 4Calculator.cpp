#include <iostream>

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double mult(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    return a / b;
}


int main() {

    char option;
    double a, b;

    while(option != 'q')
    {
        std::cout << "Pick an option(Add: a, Subtract: s, Multiply: m, Divide: d, Quit: q)" << std::endl;
        std::cin >> option;
        switch(option)
        {
            case 'm':
                std::cout << "Give me 2 numbers to multiply" << std::endl << std::endl;
                std::cin >> a;
                std::cin >> b;
                std::cout << std::endl << "Answer: " << mult(a, b) << std::endl << std::endl;
                break;

            case 'a':
                std::cout << "Give me 2 numbers to add" << std::endl << std::endl;
                std::cin >> a;
                std::cin >> b;
                std::cout << std::endl << "Answer: " << add(a, b) << std::endl << std::endl;
                break;
            
            case 's':
                std::cout << "Give me 2 numbers to subtract" << std::endl << std::endl;
                std::cin >> a;
                std::cin >> b;
                std::cout << std::endl << "Answer: " << subtract(a, b) << std::endl << std::endl;
                break;

            case 'd':
                std::cout << "Give me 2 numbers to divide" << std::endl << std::endl;
                std::cin >> a;
                std::cin >> b;
                std::cout << std::endl << "Answer: " << divide(a, b) << std::endl << std::endl;
                break;
            
            case 'q':
                break;
        }
    }
    return 0;
}