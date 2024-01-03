#pragma once
#include <algorithm>
#include <list>
#include <string>

class ID
{
private:
    static std::list<int> id_list_;
public:
    static int Hash(int seed)
    {
        return seed % 59;
    }

    /**
         * \brief return a unique id in format of aabbbb
         * aa is a hash of seed
         * bbbb is a random number generated from seed
         * \param seed seed based on the position of the object
         * \return id
         */
    static int GenerateID(int seed)
    {
        std::string id;
        int id_hash = Hash(seed);
        id += std::to_string(id_hash);
        srand(seed);
        id += std::to_string(rand() % 10000);
        int id_int = std::stoi(id);
        while(std::binary_search(id_list_.begin(), id_list_.end(), id_int))
        {
            id_int++;
        }
        id_list_.push_back(id_int);
        id_list_.sort();
        return id_int;
    }
};
