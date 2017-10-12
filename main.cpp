/* Code written by Kliment Serafimov */

#include <fstream>
#include <iomanip>
#include <iostream>

#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <stack>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#define P push
#define f first
#define s second
#define pb push_back
#define mp make_pair
#define DEC 0.00000001
#define MAX 2139062143
#define MAX_63  1061109567
#define MAXll 9187201950435737471
#define bp(a) __builtin_popcount(a)
#define rand(a, b) ((rand()%(b-a+1))+a)
#define MEM(a, b) memset(a, b, sizeof(a))
#define sort_v(a) sort(a.begin(), a.end())
#define rev_v(a)  reverse(a.begin(), a.end())

//#define fin cin
//#define fout cout
using namespace std;
//ifstream fin(".in");
//ofstream fout(".out");

string int_to_string(int n)
{
    if(n == 0)
    {
        return "0";
    }
    string ret;
    while(n>0)
    {
        ret+=(n%10)+'0';
        n/=10;
    }
    rev_v(ret);
    return ret;
}

class element
{
public:
    int is_gap;
    int value;
    element(int _value)
    {
        is_gap = false;
        value = _value;
    }
    element()
    {
        is_gap = true;
    }
    element(element *to_copy)
    {
        is_gap = to_copy->is_gap;
        value = to_copy->value;
    }
    string print()
    {
        if(is_gap)
        {
            return "_ ";
        }
        else
        {
            return int_to_string(value)+" ";
        }
    }
};
bool print_tree_search = false;
class ordered_file
{
    public:
    vector<element> file;
    ordered_file(vector<element> _file)
    {
        file = _file;
    }
    void insert(int pos, element val)
    {
        if(pos >= file.size())
        {
            cout << "Position out of bounds" <<endl;
            assert(0);
        }
        for(int level = 0; !insert_value_in_level_at_position(level, pos, val); level++);
    }
    bool insert_value_in_level_at_position(int level, int pos, element val)
    {
        int n = file.size();
        int interval_size = min((int)((1<<(level))*(log2(n))), n);
        int interval_id = pos/interval_size;
        int interval_end = min((interval_id+1)*(interval_size), n);
        int interval_start = interval_end - interval_size; //interval_id*interval_size;
        int count = !val.is_gap;
        for(int i = interval_start; i<interval_end; i++)
        {
            count+=(1-file[i].is_gap);
        }
        int tree_height = (log2(n) - log2(log2(n))+1.0);
        double high_bound = 0.75 + (tree_height-level)/(4.0*tree_height);
        double low_bound = 0.5 - (tree_height-level)/(4.0*tree_height);
        double current_density = (double)count/interval_size;
        if(print_tree_search)
        {
            cout << "level = " << level << " size = " << interval_size <<" id = "<< interval_id <<" start = " << interval_start <<" end = " << interval_end
                <<" | high_treshold = " << high_bound <<" low_treshold = "<<low_bound << " | current density =" <<  current_density <<endl;
        }
        if((low_bound < current_density && current_density < high_bound) || tree_height == level)
        {
            insert_and_rebalance(interval_size, count, current_density, interval_start, interval_end, pos, val, tree_height == level);
            return true;
        }
        else
        {
            return false;
        }
    }
    void insert_and_rebalance(int size, int count, double density, int start, int end, int pos, element val, bool is_root)
    {
        if(is_root)
        {
            assert(start == 0);
            assert(end == file.size());
            assert(size == file.size());
        }

        bool unbalanced_root = is_root && density > 0.75;

        vector<element> tmp_file;

        for(int i = start; i<end; i++)
        {
            if(unbalanced_root)
            {
                file.pb(element());
            }
            if(i == pos)
            {
                tmp_file.pb(val);
            }
            if(!file[i].is_gap)
            {
                tmp_file.pb(file[i]);
            }
        }

        if(unbalanced_root)
        {
            end+=size;
            size+=size;
        }
        double gap_size = (double)size/count;

        double current_gap_size = 0.5*gap_size;

        for(int i = start, j = 0; i < end; i++)
        {
            if(current_gap_size+1 > gap_size)
            {
                assert(j<tmp_file.size());
                file[i] = tmp_file[j];
                j++;
                current_gap_size-=gap_size;
            }
            else
            {
                file[i].is_gap = true;
            }
            current_gap_size+=1;
        }
    }
    void print()
    {
        for(int i = 0;i<file.size();i++)
        {
            cout << file[i].print();
        }
        cout << endl;
    }
};

int main()
{
    ///set to true if you want to print the tree search:
    print_tree_search = false;
    int n;
    cin>> n;
    vector<element> niz;
    for(int i = 0;i<n;i++)
    {
        int a;
        cin>> a;
        niz.pb(element());
        niz.pb(element(a));
    }
    ordered_file file = ordered_file(niz);
    file.print();
    while(true)
    {
        int pos, val;
        cin >> pos >> val;
        file.insert(pos, element(val));
        ///prints state of ordered file
        file.print();
    }
    return 0;
}
