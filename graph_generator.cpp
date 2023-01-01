#define NDEBUG //using this ignores all assert statements

#include<bits/stdc++.h>
using namespace std;

#define ve vector
#define nl cout << "\n"

#define MIN_TIME 20
#define MAX_TIME 1000
#define MIN_DIST 20
#define MAX_DIST 2000


mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rand(int lo, int hi){ //returns in the range [lo, hi)
    return lo + rng() % (hi - lo);
}

#define N 40 //NUMBER OF EDGES TO MAKE
#define M 20 // cities

void solve(){
    int n;
    cin >> n;

    ve<string> cities;

    ofstream out;
    out.open("cities.txt");
    out << M << "\n";

    string s, ex1, ex2;
    int m = M;
    while(m--){
        cin >> s >> ex1 >> ex2;
        out << s << " " << ex1 << " " << ex2 << "\n";
        cities.push_back(s);
    } out.close();

    for(int i = 0; i < N; i++){
        int x = rand(0,M);
        int y = rand(0,M);
        if(x == y) continue;

        int t = rand(MIN_TIME, MAX_TIME);
        int d = rand(MIN_TIME, MAX_TIME);
        cout << cities[x] << " " << cities[y] << " " << t << " " << d << "\n";
    }
}

/*

*/

int32_t main(){
    freopen("Cities_DataBase.txt","r",stdin);
    freopen("Input_Graph.txt","w+",stdout);

    solve();
    return 0;
}

/*

*/
