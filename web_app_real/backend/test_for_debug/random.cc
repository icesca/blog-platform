#include <iostream>
#include <random>

int main()
{
    // Create a random device to seed the random number generator
    std::random_device rd;

    // Initialize Mersenne Twister with the random device seed
    std::mt19937 gen(rd());

    // Define a uniform integer distribution range (e.g., 1 to 100)
    std::uniform_int_distribution<> distr(1, 100);

    // Generate and print 10 random numbers
    for (int i = 0; i < 10; ++i)
    {
        std::cout << distr(gen) << " ";
    }
    std::cout << std::endl;

    return 0;
}
