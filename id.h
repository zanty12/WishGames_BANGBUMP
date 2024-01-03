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
        int id_rand = rand() % 10000;
        id += std::to_string(id_rand);
        int id_int = std::stoi(id);
        while(std::binary_search(id_list_.begin(), id_list_.end(), id_int))
        {
            id_rand++;
            if(id_rand > 9999)
            {
                id_rand = 0;
            }
            id = std::to_string(id_hash) + std::to_string(id_rand);
        }
        id_list_.push_back(id_int);
        id_list_.sort();
        return id_int;
    }
};
