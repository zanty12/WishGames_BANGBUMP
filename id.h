#pragma once
#include <algorithm>
#include <ctime>
#include <list>
#include <string>

class ID
{
private:
    static std::list<std::string> id_list_;
public:
    static int Hash(int seed)
    {
        //the prime number could be better
        if(seed < 0)
        {
            seed *= -1;
        }
        if(seed > 1000)
        {
            //suppress the number to a reasonable range
            seed = seed % 1000;
        }
        return seed / 10;
    }

    /**
         * \brief return a unique id in format of aabbbbb
         * aa is a hash of seed
         * bbbbb is a random number generated from seed
         * \param seed seed based on the position of the object
         * \return id
         */
    static std::string GenerateID(int seed)
    {
        std::string id;
        int id_hash = Hash(seed);
        if(id_hash < 10)
        {
            id += "0";
        }
        id += std::to_string(id_hash);
        //srand(time(NULL));
        int id_rand = rand() % 100000;
        if(id_rand < 10)
        {
            id += "0000";
        }
        else if(id_rand < 100)
        {
            id += "000";
        }
        else if(id_rand < 1000)
        {
            id += "00";
        }
        else if(id_rand < 10000)
        {
            id += "0";
        }
        id += std::to_string(id_rand);
        while(std::binary_search(id_list_.begin(), id_list_.end(), id))
        {
            id_rand++;
            if(id_rand > 9999)
            {
                id_rand = 0;
            }
            if(id_hash < 10)
            {
                id += "0";
            }
            id = std::to_string(id_hash);
            if(id_rand < 10)
            {
                id += "0000";
            }
            else if(id_rand < 100)
            {
                id += "000";
            }
            else if(id_rand < 1000)
            {
                id += "00";
            }
            else if(id_rand < 10000)
            {
                id += "0";
            }
            id += std::to_string(id_rand);
        }
        id_list_.push_back(id);
        //sort the list according to the numeric value of the id
        id_list_.sort([](std::string a, std::string b) { return std::stoi(a) < std::stoi(b); });
        return id;
    }
};
