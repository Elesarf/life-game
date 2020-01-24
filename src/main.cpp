#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <unistd.h>

static std::vector<bool> field;
static std::vector<bool> next_generation;

static constexpr size_t width = 160;
static constexpr size_t height = 50;
static constexpr size_t field_size = width * height;

static size_t generation = 0;

struct Neighbors
{
    size_t left_up{0};
    size_t up{0};
    size_t right_up{0};
    size_t left{0};
    size_t right{0};
    size_t left_down{0};
    size_t down{0};
    size_t right_down{0};
};

Neighbors get_neighbors(const size_t index)
{
    Neighbors n;

    size_t row = index / width;

    n.right = index + 1;
    if (index / width != n.right / width)
        n.right -= width;

    n.left = index - 1;
    if (index / width != n.left / width)
        n.left += width;

    n.up = index - width;
    if (row == 0)
        n.up = width * (height - 1) + index;

    n.down = index + width;
    if (row == (height - 1))
        n.down = index - width * (height - 1);

    n.left_up = index - width - 1;

    if (index % width == 0)
        n.left_up += width;

    if (row == 0)
    {
        n.left_up = width * (height - 1) + index - 1;
    }

    n.right_up = index - width + 1;
    if (row == 0)
    {
        n.right_up = width * (height - 1) + index + 1;

        if (index == width - 1)
            n.right_up -= width;
    }

    n.right_down = index + width + 1;
    if (row == (height - 1))
    {
        n.right_down = index - width * (height - 1) + 1;

        if (index == field_size - 1)
            n.right_down -= width;
    }

    if (n.right_down >= field_size)
        n.right_down = field_size - n.right_down;

    n.left_down = index + width - 1;
    if (row == (height - 1))
    {
        n.left_down = index % width - 1;

        if (index == field_size - width)
            n.left_down += width;
    }

    return n;
}

bool original_rule(const size_t index)
{
    size_t neighbors = 0;
    Neighbors n = get_neighbors(index);

    if (field.at(n.left_up))
        ++neighbors;
    if (field.at(n.up))
        ++neighbors;
    if (field.at(n.right_up))
        ++neighbors;
    if (field.at(n.right))
        ++neighbors;
    if (field.at(n.left))
        ++neighbors;
    if (field.at(n.left_down))
        ++neighbors;
    if (field.at(n.right_down))
        ++neighbors;
    if (field.at(n.down))
        ++neighbors;

    if (field.at(index))
    {
        if (neighbors == 2 || neighbors == 3)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (neighbors == 3)
        {
            return true;
        }
    }
    return false;
}

bool rule_110(const size_t index)
{
    size_t left = index - 1;
    size_t right = index + 1;

    if (left == std::numeric_limits<size_t>::max())
        left = field_size - 1;

    if (right >= field_size)
        right = 0;

    if (index && field.at(left) && field.at(right))
        return false;

    if (field.at(left) && index && (!field.at(right)))
        return true;

    if (field.at(left) && (!index) && (field.at(right)))
        return true;

    if (field.at(left) && (!index) && (!field.at(right)))
        return false;

    if ((!field.at(left)) && (index) && (field.at(right)))
        return true;

    if ((!field.at(left)) && (index) && (!field.at(right)))
        return true;

    if ((!field.at(left)) && (!index) && (field.at(right)))
        return true;

    if ((!field.at(left)) && (!index) && (!field.at(right)))
        return false;

    return false;
}

void tick()
{
    for(size_t i = 0; i < field_size; ++i)
    {
        next_generation.push_back(original_rule(i));
    }

    field = next_generation;
    next_generation.clear();
}

void print_field()
{
    for(size_t i = 0; i < field_size; ++i)
    {
        std::cout << (field.at(i)?"\e[41m@\e[0m":"\e[46m*\e[0m");

        if (i % width == width - 1)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Generation " << generation << std::endl;
}

void init_field()
{
    auto timestamp = std::chrono::seconds(time(nullptr));
    auto mill = std::chrono::milliseconds(timestamp).count();
    std::mt19937 generator(static_cast<unsigned>(mill));
    std::uniform_int_distribution<int> distribution(0,1);

    for(size_t j = 0; j < field_size; ++j)
    {
        field.push_back((static_cast<bool>(distribution(generator))));
    }

    next_generation.reserve(field_size);
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    init_field();
    while (1)
    {
        tick();
        std::cout << "\033c";
        print_field();
        usleep(100000);
        ++generation;
    }
}
