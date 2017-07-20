#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <cstdint>
#include <random>


class RandomGenerator
{
public:
    RandomGenerator();

    static RandomGenerator *get();
    static uint8_t byte();

private:
    static RandomGenerator *m_instance;
    static std::mt19937 m_gen;
};

#endif // RANDOMGENERATOR_H
