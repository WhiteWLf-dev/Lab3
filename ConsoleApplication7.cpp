#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <map>
#include <set>
std::vector<long long> p_pow(10);
size_t capacity = 100000;
size_t simpleHash(std::string a) {
    size_t result = 1;

    for (int i = 0; i < a.length(); ++i)
        result = (result * (a[i] - '/' + 1)) ;

    return result % capacity;
}
size_t hardHash(std::string a) {
    size_t hash = 0;
    for (size_t j = 0; j < a.length(); ++j)
        hash = (hash + (a[j] - '/' + 1) * p_pow[j]) ;

    return hash % capacity;
}
class Lab3 {
public:
    struct Elements {
        std::string date;
        size_t win;
        size_t number;
        size_t cost;
        size_t (*typeHash)(std::string);
        size_t hash;
        friend bool operator== (const Elements& c1, const Elements& c2) {
            if (c1.date == c2.date) {
                if (c1.win == c2.win) {
                    if (c1.number == c1.number) {
                        return true;
                    }
                }
            }
            return false;
        }
        friend bool operator >(const Elements& c1, const Elements& c2) {
            if (std::strcmp(c1.date.c_str(), c2.date.c_str()) == -1) {
                return false;
            }
            if (std::strcmp(c1.date.c_str(), c2.date.c_str()) == 1) {
                return true;
            }
            if (std::strcmp(c1.date.c_str(), c2.date.c_str()) == 0) {
                if (c1.win < c2.win) {
                    return true;
                }
                if (c1.win > c2.win) {
                    return false;
                }
                if (c1.win == c2.win) {
                    if (c1.number > c2.number) {
                        return true;
                    }
                    if (c1.number < c2.number) {
                        return false;
                    }
                    if (c1.number == c2.number) {
                        return false;
                    }
                }
            }
        }
    };
    friend bool operator <(const Elements& c1, const Elements& c2) {
        if (c1 == c2) {
            return false;
        }
        return !(c1 > c2);
    }
    friend bool operator >=(const Elements& c1, const Elements& c2) {
        if (c1 == c2) {
            return true;
        }
        return c1 > c2;

    }
    friend bool operator <=(const Elements& c1, const Elements& c2) {
        if (c1 == c2) {
            return true;
        }
        return c1 < c2;
    }

    std::vector<Lab3::Elements> data;

    const int maxLen = 10;
    
    std::string filename;
    Lab3() = default;
    Lab3(std::string namefile, size_t(*typeHash)(std::string));
    ~Lab3() = default;
    size_t simpleHash(std::string a);
    size_t hardHash(std::string a);
};

Lab3::Lab3(std::string namefile, size_t (*typeHash)(std::string))
{
    std::ifstream inf(namefile);
    filename = namefile;

    if (!inf.is_open())
    {
        std::cerr << "The file could not be opened for reading!\n";
    }
    std::string s;
    while (std::getline(inf, s)) {
        Lab3::Elements obj;
        size_t i = 0;
        bool flag = 1;
        std::string num;
        while (flag) {
            if (s[i] == ' ') {
                flag = 0;
                i++;
                break;
            }
            else {
                num += s[i];
                i++;
            }
        }
        obj.number = std::stoi(num.c_str());
        std::string cost;
        flag = 1;
        while (flag) {
            if (s[i] == ' ') {
                flag = 0;
                i++;
                break;
            }
            else {
                cost += s[i];
                i++;
            }
        }
        obj.cost = std::stoi(cost.c_str());
        std::string date;
        flag = 1;
        while (flag) {
            if (s[i] == ' ') {
                flag = 0;
                i++;
                break;
            }
            else {
                date += s[i];
                i++;
            }
        }
        obj.date = date;
        flag = 1;
        std::string sum;
        while (flag) {
            if (i == s.size()) {
                flag = 0;
                break;
            }
            else {
                sum += s[i];
                i++;
            }
        }

        obj.win = std::stoi(sum.c_str());
        obj.typeHash = typeHash;
        obj.hash = typeHash(obj.date);
        data.push_back(obj);
    }
    inf.close();
    
}
struct Item {
    std::vector<Lab3::Elements> values;
};

class HashTable {
    std::vector<Item> table;
    size_t(*typeHash)(std::string);

public:
    HashTable(std::vector<Lab3::Elements> &elements) {
        table.resize(capacity);
        for (auto i : elements) {
            this->typeHash = i.typeHash;
            this->insert(i);
        }
    }
    void insert(Lab3::Elements& item) {

        Item& currentItem = this->table[item.hash];

        for (auto i : currentItem.values) {
            if (i == item) {
                return;
            }
        }

        currentItem.values.push_back(item);
    }

    std::vector<Lab3::Elements> find(std::string key) {

        Item& currentItem = this->table[typeHash(key)];
        std::vector<Lab3::Elements> out;
        for (auto i = currentItem.values.begin(); i != currentItem.values.end(); ++i) {
            if ((*i).date == key) {
                out.push_back((*i));
            }
        }
        return out;
    }

    size_t getCollisions() {

        size_t res = 0;
        for (auto z = this->table.begin(); z != this->table.end(); ++z) {
            
            std::set<std::string> tmp;
            for (auto j = z->values.begin(); j != z->values.end(); ++j) {
                tmp.insert(j->date);
            }
            
            if (tmp.size()) {
                res += tmp.size() - 1;
            }
        }
        return res;
    }
};
int main() {
    p_pow[0] = 1;
    size_t p = 7;
    for (size_t i = 1; i < p_pow.size(); ++i)
        p_pow[i] = p_pow[i - 1] * p;
    std::string m[7] = { "100.txt","1000.txt","5000.txt","10000.txt","20000.txt","40000.txt","100000.txt" };

    for (int i = 0; i < 7; ++i) {
        Lab3 v(m[i % 7],simpleHash);
        HashTable t(v.data);
        auto begin = std::chrono::steady_clock::now();
        t.find("1999/04/14");
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "The time of search Hash Table with simpleHash() " << m[i % 7] << " : " << elapsed_ms.count() << " mc\n";
        std::cout << "The count of collisions: " << t.getCollisions()<<"\n";
        Lab3 v1(m[i % 7], hardHash);
        HashTable t1(v1.data);
        begin = std::chrono::steady_clock::now();
        t1.find("1999/04/14");
        end = std::chrono::steady_clock::now();
        elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "The time of search Hash Table with hardHash() " << m[i % 7] << " : " << elapsed_ms.count() << " mc\n";
        std::cout << "The count of collisions: " << t1.getCollisions() << "\n";
    }
}
