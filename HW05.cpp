#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
private:
    tm storedDate = {0};

    static bool valiDate(int y, int m, int d) {
        struct tm start = {0};

        start.tm_mday = d;
        start.tm_mon = m;
        start.tm_year = y;

        time_t rawtime = mktime(&start) - timezone;

        if (rawtime == -1 || y < 100 || y > 130 || d != start.tm_mday || m != start.tm_mon || y != start.tm_year) {
            return false;
        }
        else {
            return true;
        }
    }

public:
    CDate(int y, int m, int d) {
        m--;
        y -= 1900;

        storedDate.tm_mday = d;
        storedDate.tm_mon = m;
        storedDate.tm_year = y;
    }

    CDate operator+ (int rhs) {
        CDate temp(storedDate.tm_year + 1900, storedDate.tm_mon + 1, storedDate.tm_mday);
        temp.storedDate.tm_mday += rhs;
        time_t rawtime = mktime(&(temp.storedDate)) - timezone;
        temp.storedDate = *(gmtime(&rawtime));

        return temp;
    }

    CDate operator- (int rhs) {
        return (*this) + (rhs * (-1));
    }

    int operator- (CDate & rhs) {
        time_t rawtime0 = mktime(&storedDate) - timezone;
        time_t rawtime1 = mktime(&rhs.storedDate) - timezone;
        int time = (rawtime0 - rawtime1) / (60 * 60 *  24);
        return time;
    }

    CDate & operator++ () {
        storedDate.tm_mday++;
        time_t rawtime = mktime(&storedDate) - timezone;
        storedDate = *(gmtime(&rawtime));
        return *this;
    }

    CDate operator++ (int) {
        CDate tmp(this->storedDate.tm_year + 1900, this->storedDate.tm_mon + 1, this->storedDate.tm_mday);
        operator++();
        return tmp;
    }

    CDate & operator-- () {
        storedDate.tm_mday--;
        time_t rawtime = mktime(&storedDate) - timezone;
        storedDate = *(gmtime(&rawtime));
        return *this;
    }

    CDate operator-- (int) {
        CDate tmp(this->storedDate.tm_year + 1900, this->storedDate.tm_mon + 1, this->storedDate.tm_mday);
        operator--();
        return tmp;
    }

    bool operator== (CDate & rhs) {
        time_t rawtime0 = mktime(&storedDate) - timezone;
        time_t rawtime1 = mktime(&(rhs.storedDate)) - timezone;

        return rawtime0 == rawtime1;
    }

    bool operator!= (CDate & rhs) {
        return !(operator==(rhs));
    }

    bool operator< (const CDate & rhs) const {
        if (storedDate.tm_year < rhs.storedDate.tm_year) {
            return true;
        }
        else if ((storedDate.tm_year == rhs.storedDate.tm_year) && (storedDate.tm_mon < rhs.storedDate.tm_mon)) {
            return true;
        }
        else if (((storedDate.tm_year == rhs.storedDate.tm_year) && (storedDate.tm_mon == rhs.storedDate.tm_mon)) && (storedDate.tm_mday < rhs.storedDate.tm_mday)) {
            return true;
        }
        return false;
    }

    bool operator> (CDate & rhs) {
        time_t rawtime0 = mktime(&storedDate) - timezone;
        time_t rawtime1 = mktime(&rhs.storedDate) - timezone;

        return rawtime0 > rawtime1;
    }

    bool operator<= (CDate & rhs) {
        if (!operator<(rhs)) {
            return operator==(rhs);
        }
        return true;
    }

    bool operator>= (CDate & rhs) {
        if (!operator>(rhs)) {
            return operator==(rhs);
        }
        return true;
    }
};

// This class uses the CDate implementation from one of the previous homeworks.
class CSupermarket
{
public:
    CSupermarket & store( string name, CDate date, int count ) {
        auto pos = db.find(name);
        
        // If the product record doesnt exist, create it.
        if (pos == db.end()) {
            vector<pair<CDate, int>> temp;
            temp.emplace_back(pair<CDate, int>(date, count));

            db.insert(pair<string,vector<pair<CDate,int>>>(name, temp));
        }
        else {
            // Insert the product into the correct place based on expiry date.
            auto vecPos = lower_bound(db[name].begin(), db[name].end(), pair<CDate, int>(date, count), [](pair<CDate, int> lhs, pair<CDate, int> rhs) {
                return rhs.first > lhs.first;
            });

            pos->second.insert(vecPos, pair<CDate, int>(date, count));
        }
        return *this;
    }
    
    // Should sell the products that expire earlier first.
    void sell(list<pair<string, int>> & ls) {
        string product;
        auto i = ls.begin();
        long unsigned int idx = 0;

        while (i != ls.end()) {
            int counter = 0;
            bool match = true;
            auto pos = db.find(i->first);

            if (pos == db.end()) {
                for (auto j : db) {
                    if (errCmp(i->first, j.first) == 1) {
                        product = j.first;
                        counter++;
                    }
                    if (counter > 1) {
                        match = false;
                        break;
                    }
                }
                if (counter == 0 || counter > 1) {
                    ++i;
                    match = false;
                    continue;
                }
            }
            else {
                product = pos->first;
            }

            auto found = db.find(product);

            while (idx < found->second.size() && match) {
                if (i->second >= found->second[idx].second) {
                    i->second -= found->second[idx].second;
                    found->second[idx].second = 0;

                    found->second.erase(found->second.begin() + idx);
                    if (found->second.empty()) {
                        db.erase(found);
                        i++;
                        break;
                    }
                }
                else {
                    found->second[idx].second -= i->second;
                    i->second = 0;
                    idx++;
                }

                if (i->second <= 0) {
                    auto temp = i;
                    --i;
                    ls.erase(temp);
                    break;
                }
            }
            idx = 0;
            ++i;
        }
    }

    list<pair<string, int>> expired(CDate date) const{
        list<pair<string, int>> result;

        for (auto & i : db) {
            int expiredCount = 0;
            for (auto & j : i.second) {
                if (j.first < date) {
                    expiredCount += j.second;
                }
            }
            if (expiredCount > 0) {
                result.emplace_back(pair<string, int>(i.first, expiredCount));
            }
        }
        result.sort([](pair<string, int> & lhs, pair<string, int> & rhs){
            return lhs.second > rhs.second;
        });

        return result;
    }

private:
    map<string, vector<pair<CDate, int>>> db;

    // Allow string matching where there is at most 1 mistake/discrepancy
    static bool errCmp(const string & x, const string & y) {
        int discrepancies = 0;

        if (x.size() != y.size()) {
            return false;
        }

        auto iterX = x.begin();
        auto iterY = y.begin();

        while (iterX < x.end() && iterY < y.end()) {
            if (*iterX != *iterY) {
                discrepancies++;
            }
            if (discrepancies > 1)
                return false;
            ++iterX;
            ++iterY;
        }

        return true;
    }
};
#ifndef __PROGTEST__
int main ( void )
{
    CSupermarket s;
    s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
            . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
            . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
            . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
            . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

    list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
    assert ( l0 . size () == 4 );
    assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

    list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
    s . sell ( l1 );
    assert ( l1 . size () == 2 );
    assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

    list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
    assert ( l2 . size () == 1 );
    assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

    list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
    assert ( l3 . size () == 1 );
    assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

    list<pair<string,int> > l4 { { "bread", 105 } };
    s . sell ( l4 );
    assert ( l4 . size () == 0 );
    assert ( ( l4 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l5 . size () == 3 );
    assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

    s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

    list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
    s . sell ( l6 );
    assert ( l6 . size () == 3 );
    assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

    list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l7 . size () == 4 );
    assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

    s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

    list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
    s . sell ( l8 );
    assert ( l8 . size () == 2 );
    assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

    list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l9 . size () == 5 );
    assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

    list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
    s . sell ( l10 );
    assert ( l10 . size () == 2 );
    assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

    list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l11 . size () == 4 );
    assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

    list<pair<string,int> > l12 { { "Cake", 4 } };
    s . sell ( l12 );
    assert ( l12 . size () == 0 );
    assert ( ( l12 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l13 . size () == 4 );
    assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

    list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
    s . sell ( l14 );
    assert ( l14 . size () == 1 );
    assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

    s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
            . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
            . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

    list<pair<string,int> > l15 { { "ccccc", 10 } };
    s . sell ( l15 );
    assert ( l15 . size () == 1 );
    assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

    cout << "SUCCESS" << endl;
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
