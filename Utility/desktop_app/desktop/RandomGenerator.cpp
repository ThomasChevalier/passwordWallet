#include "RandomGenerator.h"

#include <chrono>
#include <random>

RandomGenerator* RandomGenerator::m_instance = nullptr;
std::mt19937 RandomGenerator::m_gen;

RandomGenerator::RandomGenerator()
{
    m_gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

RandomGenerator *RandomGenerator::get()
{
    if(m_instance==nullptr)
    {
        m_instance = new RandomGenerator();
    }
    return m_instance;
}

uint8_t RandomGenerator::byte()
{
    std::uniform_int_distribution<uint8_t> dis(0,255);
    return dis(m_gen);
}
