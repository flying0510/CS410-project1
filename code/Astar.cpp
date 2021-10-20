#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

bool** flag;
int N, M;
int v[3][2] = {{1,1},{1,0},{0,1}};
int w[7][3] = {{1,1,1},{1,1,0},{1,0,1},{0,1,1},{1,0,0},{0,1,0},{0,0,1}};
std::vector<std::vector<std::vector<int>>> dp1;
std::vector<std::vector<std::vector<int>>> dp2;
std::ofstream d2, log3, outfile;

struct node{
    int g;
    int h;
    int x;
    int y;
    int fatherx;
    int fathery;
    node(int a, int b, int c, int d, int e, int f):g(a),h(b),x(c),y(d),fatherx(e),fathery(f){}
    bool operator < (const node &a) const
    {
        return (this->g + this->h) < (a.g+a.h);        
    }
    bool operator > (const node &a) const
    {
        return (this->g + this->h) > (a.g+a.h);        
    }
    node& operator = (const node &a)
    {
        g = a.g; h = a.h; x = a.x; y = a.y; fatherx = a.fatherx; fathery = a.fathery;
        return *this;
    }
};

struct node_3{
    int g; int h; int x; int y; int z; int fatherx; int fathery; int fatherz;
    node_3(int a, int b, int c, int d, int e, int fax, int fay, int faz):g(a),h(b),x(c),y(d),z(e),fatherx(fax),fathery(fay),fatherz(faz){}
    bool operator < (const node_3 & a) const
    {
        return (this->g + this->h) < (a.g + a.h);
    }
    bool operator > (const node_3 &a) const
    {
        return (this->g + this->h) > (a.g + a.h);
    }
    node_3& operator = (const node_3 &a)
    {
        g = a.g; h = a.h; x = a.x; y = a.y; z = a.z; fatherx = a.fatherx; fathery = a.fathery; fatherz = a.fatherz;
        return *this;
    }
};

std::vector<std::string> Database, Query_2, Query_3;

void load_database()
{
    std::ifstream ifs("../data/MSA_database.txt");
    if(!ifs.good()){
        std::cerr << "open file error!";
        return;
    }
    std::string line;
    while(getline(ifs, line))
        Database.push_back(line);
    ifs.close();
}

void load_query_2()
{
    std::ifstream ifs("../data/MSA2_query.txt");
    if(!ifs.good()){
        std::cerr << "open file error!";
        return;
    }
    std::string line;
    while(getline(ifs, line))
        Query_2.push_back(line);
    ifs.close();
}

void load_query_3()
{
    std::ifstream ifs("../data/MSA3_query.txt");
    if(!ifs.good()){
        std::cerr << "open file error!";
        return;
    }
    std::string line;
    while(getline(ifs, line))
        Query_3.push_back(line);
    ifs.close();
}

void load()
{
    load_database();
    load_query_2();
    load_query_3();
}

int H_func(int X, int Y)
{
    int tmp1 = N - X;
    int tmp2 = M - Y;
    return abs(tmp1-tmp2) * 2;
}

int judge(char a, char b, int mode = 0)
{
    if(mode == 0) return (a == b)? 0 : 3;
    else return 2;
}

int min_3(int a, int b, int c)
{
    return std::min(std::min(a, b), c);
}

int search_2(std::string S, std::string T)
{
    N = S.size();
    M = T.size();
    std::vector<node> open_list, closed_list;
    std::vector<std::vector<bool>> open(N+1);
    std::vector<std::vector<bool>> closed(N+1);
    for(int i = 0; i <= N; ++i) {open[i].resize(M+1, false); closed[i].resize(M+1, false);}
    open_list.push_back(node(0, H_func(0,0), 0, 0, -1, -1));
    open[0][0] = true;
    while(open_list.size()){
        node optimal_node = open_list[0];
        int pos = 0;
        for(int i = 1; i < open_list.size(); ++i){
            if(optimal_node > open_list[i]){
                optimal_node = open_list[i];
                pos = i;
            }
        }
        open_list.erase(std::begin(open_list) + pos);
        closed_list.push_back(optimal_node);
        closed[optimal_node.x][optimal_node.y] = true;
        if((optimal_node.x == N) && (optimal_node.y == M)) return optimal_node.g;
        //judge (1,1), (1,0), (0,1)
        for(int t = 0; t < 3; ++t){
            node cur_node(optimal_node.g+judge(S[optimal_node.x], T[optimal_node.y], t), H_func(optimal_node.x+v[t][0],optimal_node.y+v[t][1]), optimal_node.x+v[t][0], optimal_node.y+v[t][1], optimal_node.x, optimal_node.y);
            if(cur_node.x <= N && cur_node.y <= M){
                if(!open[cur_node.x][cur_node.y]){
                    open[cur_node.x][cur_node.y] = true;
                    open_list.push_back(cur_node);
                }
                else if(!closed[cur_node.x][cur_node.y]){
                    for(int i = 0; i < open_list.size(); ++i){
                        if(open_list[i].x == cur_node.x && open_list[i].y == cur_node.y){
                            open_list[i] = (open_list[i].g > cur_node.g) ? cur_node : open_list[i]; break;}
                    }
                }
                else{
                    for(int i = 0; i < closed_list.size(); ++i){
                        if(closed_list[i].x == cur_node.x && closed_list[i].y == cur_node.y && closed_list[i].g > cur_node.g){
                            closed[cur_node.x][cur_node.y] = false;
                            closed_list.erase(std::begin(closed_list)+i);
                            open_list.push_back(cur_node);
                            break;
                        }
                    }
                }
            }
        }
    }
}

std::pair<std::string, std::string> search_2_out(std::string S, std::string T)
{
    N = S.size();
    M = T.size();
    std::string s1 = "";
    std::string s2 = "";
    int ptr1 = N-1;
    int ptr2 = M-1;
    std::vector<node> open_list, closed_list;
    std::vector<std::vector<bool>> open(N+1);
    std::vector<std::vector<bool>> closed(N+1);
    for(int i = 0; i <= N; ++i) {open[i].resize(M+1, false); closed[i].resize(M+1, false);}
    open_list.push_back(node(0, H_func(0,0), 0, 0, -1, -1));
    open[0][0] = true;
    while(open_list.size()){
        node optimal_node = open_list[0];
        int pos = 0;
        for(int i = 1; i < open_list.size(); ++i){
            if(optimal_node > open_list[i]){
                optimal_node = open_list[i];
                pos = i;
            }
        }
        open_list.erase(std::begin(open_list) + pos);
        closed_list.push_back(optimal_node);
        closed[optimal_node.x][optimal_node.y] = true;
        if((optimal_node.x == N) && (optimal_node.y == M)) {
            int X = N;
            int Y = M;
            node tmp = optimal_node;
            while(X > 0 || Y > 0){
                X = tmp.fatherx;
                Y = tmp.fathery;
                if(X == tmp.x - 1) s1 += S[ptr1--];
                else s1 += "-";
                if(Y == tmp.y - 1) s2 += T[ptr2--];
                else s2 += "-";
                if(!closed[X][Y]){
                    for(int i = 0; i < open_list.size(); ++i)
                        if(open_list[i].x == X && open_list[i].y == Y) tmp = open_list[i];
                }
                else{
                    for(int i = 0; i < closed_list.size(); ++i)
                        if(closed_list[i].x == X && closed_list[i].y == Y) tmp = closed_list[i];
                }
            }
            reverse(s1.begin(),s1.end());
            reverse(s2.begin(),s2.end());
            std::pair<std::string, std::string> P(s1, s2);
            return P;
        }
        //judge (1,1), (1,0), (0,1)
        for(int t = 0; t < 3; ++t){
            node cur_node(optimal_node.g+judge(S[optimal_node.x], T[optimal_node.y], t), H_func(optimal_node.x+v[t][0],optimal_node.y+v[t][1]), optimal_node.x+v[t][0], optimal_node.y+v[t][1], optimal_node.x, optimal_node.y);
            if(cur_node.x <= N && cur_node.y <= M){
                if(!open[cur_node.x][cur_node.y]){
                    open[cur_node.x][cur_node.y] = true;
                    open_list.push_back(cur_node);
                }
                else if(!closed[cur_node.x][cur_node.y]){
                    for(int i = 0; i < open_list.size(); ++i){
                        if(open_list[i].x == cur_node.x && open_list[i].y == cur_node.y){
                            open_list[i] = (open_list[i].g > cur_node.g) ? cur_node : open_list[i]; break;}
                    }
                }
                else{
                    for(int i = 0; i < closed_list.size(); ++i){
                        if(closed_list[i].x == cur_node.x && closed_list[i].y == cur_node.y && closed_list[i].g > cur_node.g){
                            closed[cur_node.x][cur_node.y] = false;
                            closed_list.erase(std::begin(closed_list)+i);
                            open_list.push_back(cur_node);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Search_2()
{
    outfile << "Pairwise Alignment: " << std::endl;
    clock_t start_time, end_time;
    int num_2 = Query_2.size();
    for(int i = 0; i < num_2; ++i){
        start_time = clock();
        int MIN = 1e9;
        int MIN_IND = -1;
        for(int t = 0; t < Database.size(); ++t){
            int cost = search_2(Query_2[i], Database[t]);
            d2 << "Test (Query2: " << i+1 << " and Database: " << t+1 << ") cost = " << cost <<std::endl;
            if(MIN > cost) {MIN = cost; MIN_IND = t;}
        } 
        std::pair<std::string, std::string> pa = search_2_out(Query_2[i], Database[MIN_IND]);
        end_time = clock();
        outfile << "Query " << i+1 << " : " << "cost = " << MIN << std::endl;
        outfile << "          alignment with: " << "number (" << MIN_IND + 1 << ") " << "in Database" << std::endl;
        outfile << "          alignment result: " << pa.first << std::endl;
        outfile << "                            " << pa.second << std::endl;
        outfile << "Running Time is " << (double)(end_time - start_time)/CLOCKS_PER_SEC << "s" << std::endl << std::endl;
    }
}

void DP(std::string S, std::string T)
{
    int n = S.size();
    int m = T.size();
    std::vector<std::vector<int>> dp(n+1);
    for(int i = 0; i <= n; ++i) dp[i].resize(m+1);
    for(int i = 0; i <= n; ++i) dp[i][0] = i * 2;
    for(int i = 0; i <= m; ++i) dp[0][i] = i * 2;
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j)
            dp[i][j] = min_3(dp[i-1][j-1]+judge(S[i-1],T[j-1],0), dp[i][j-1]+2, dp[i-1][j]+2);
}

void init_dp1()
{
    int ind = 0;
    dp1.resize(Database.size() * (Database.size()-1) / 2);
    for(int i = 0; i < Database.size(); ++i)
        for(int j = i+1; j < Database.size(); ++j){
            int n = Database[i].size();
            int m = Database[j].size();
            std::string S = Database[i];
            std::string T = Database[j];
            reverse(S.begin(), S.end());
            reverse(T.begin(), T.end());
            dp1[ind].resize(n+1);
            for(int k = 0; k <= n; ++k) dp1[ind][k].resize(m+1);
            for(int k = 0; k <= n; ++k) dp1[ind][k][0] = 2 * k;
            for(int k = 0; k <= m; ++k) dp1[ind][0][k] = 2 * k;
            for(int s = 1; s <= n; ++s)
                for(int t = 1; t <= m; ++t)
                    dp1[ind][s][t] = min_3(dp1[ind][s-1][t-1]+judge(S[s-1],T[t-1],0),dp1[ind][s][t-1]+2,dp1[ind][s-1][t]+2);
            ind += 1;
        }
}

void init_dp2()
{
    int ind = 0;
    dp2.resize(Database.size() * Query_3.size());
    for(int i = 0; i < Query_3.size(); ++i)
        for(int j = 0; j < Database.size(); ++j){
            int n = Query_3[i].size();
            int m = Database[j].size();
            std::string S = Query_3[i];
            std::string T = Database[j];
            reverse(S.begin(), S.end());
            reverse(T.begin(), T.end());
            dp2[ind].resize(n+1);
            for(int k = 0; k <= n; ++k) dp2[ind][k].resize(m+1);
            for(int k = 0; k <= n; ++k) dp2[ind][k][0] = 2 * k;
            for(int k = 0; k <= m; ++k) dp2[ind][0][k] = 2 * k;
            for(int s = 1; s <= n; ++s)
                for(int t = 1; t <= m; ++t)
                    dp2[ind][s][t] = min_3(dp2[ind][s-1][t-1]+judge(S[s-1],T[t-1],0),dp2[ind][s][t-1]+2,dp2[ind][s-1][t]+2);
            ind += 1;
        }
}

int map_to_dp1(int a, int b)
{
    int tmp_a = std::min(a,b);
    int tmp_b = std::max(a,b);
    int ans = 0;
    for(int i = 0; i < tmp_a; ++i) ans += (Database.size()-1-i);
    ans += (tmp_b - tmp_a - 1);
    return ans;
}

int map_to_dp2(int a, int b)
{
    return Database.size() * a + b;
}

int H_func3(int Q_ind, int seq1, int seq2, int x, int y, int z)
{
    int tmpind;
    tmpind = map_to_dp2(Q_ind, seq1);
    int QS1 = dp2[tmpind][Query_3[Q_ind].size() - x][Database[seq1].size() - y];
    tmpind = map_to_dp2(Q_ind, seq2);
    int QS2 = dp2[tmpind][Query_3[Q_ind].size() - x][Database[seq2].size() - z];
    tmpind = map_to_dp1(seq1, seq2);
    int SS = dp1[tmpind][Database[seq1].size() - y][Database[seq2].size() - z];
    return (QS1 + QS2 + SS);
}

int judge_3(char a, char b, char c, int mode)
{
    switch(mode){
    case 0:
        if(a == b && b == c) return 0;
        else if(a == b) return 6;
        else if(b == c) return 6;
        else if(a == c) return 6;
        else return 9;
        break;
    case 1:
        if(a == b) return 4;
        else return 7;
        break;
    case 2:
        if(a == c) return 4;
        else return 7;
        break;
    case 3:
        if(b == c) return 4;
        else return 7;
        break;
    case 4: return 4; break;
    case 5: return 4; break;
    case 6: return 4; break;
    }
}

int search_3(int Q_ind, int seq1, int seq2)
{
    int K = Query_3[Q_ind].size();
    int N1 = Database[seq1].size();
    int M1 = Database[seq2].size();
    std::vector<node_3> open_list, closed_list;
    std::vector<std::vector<std::vector<bool>>> open(K+1);
    std::vector<std::vector<std::vector<bool>>> closed(K+1);
    for(int i = 0; i <= K; ++i) {open[i].resize(N1+1); closed[i].resize(N1+1);}
    for(int i = 0; i <= K; ++i) 
        for(int j = 0; j <= N1; ++j) {open[i][j].resize(M1+1, false); closed[i][j].resize(M1+1, false);}
    open_list.push_back(node_3(0, H_func3(Q_ind, seq1, seq2, 0, 0, 0), 0, 0, 0, -1, -1, -1));
    open[0][0][0] = true;
    while(open_list.size()){
        node_3 optimal_node = open_list[0];
        int pos = 0;
        for(int i = 1; i < open_list.size(); ++i){
            if(optimal_node > open_list[i]){
                optimal_node = open_list[i];
                pos = i;
            }
        }
        open_list.erase(std::begin(open_list) + pos);
        closed_list.push_back(optimal_node);
        closed[optimal_node.x][optimal_node.y][optimal_node.z] = true;
        if((optimal_node.x == K) && (optimal_node.y == N1) && (optimal_node.z == M1)) return optimal_node.g;
        for(int t = 0; t < 7; ++t){
            if(optimal_node.x+w[t][0] <= K && optimal_node.y+w[t][1] <= N1 && optimal_node.z+w[t][2] <= M1){
                node_3 cur_node(optimal_node.g+judge_3(Query_3[Q_ind][optimal_node.x],Database[seq1][optimal_node.y],Database[seq2][optimal_node.z], t), H_func3(Q_ind,seq1,seq2,optimal_node.x+w[t][0],optimal_node.y+w[t][1],optimal_node.z+w[t][2]), optimal_node.x+w[t][0], optimal_node.y+w[t][1], optimal_node.z+w[t][2], optimal_node.x, optimal_node.y, optimal_node.z);
                if(!open[cur_node.x][cur_node.y][cur_node.z]){
                    open[cur_node.x][cur_node.y][cur_node.z] = true;
                    open_list.push_back(cur_node);
                }
                else if(!closed[cur_node.x][cur_node.y][cur_node.z]){
                    for(int i = 0; i < open_list.size(); ++i){
                        if(open_list[i].x == cur_node.x && open_list[i].y == cur_node.y && open_list[i].z == cur_node.z){
                            open_list[i] = (open_list[i].g > cur_node.g)? cur_node : open_list[i]; break;}
                    }
                }
                else{
                    for(int i = 0; i < closed_list.size(); ++i){
                        if(closed_list[i].x == cur_node.x && closed_list[i].y == cur_node.y && closed_list[i].z == cur_node.z && closed_list[i].g > cur_node.g){
                            closed_list.erase(std::begin(closed_list) + i);
                            open_list.push_back(cur_node);
                            break;
                        }
                    }
                }
            }   
        }
    }
}

std::pair<std::string, std::pair<std::string, std::string>> search_3_out(int Q_ind, int seq1, int seq2)
{
    int K = Query_3[Q_ind].size();
    int N1 = Database[seq1].size();
    int M1 = Database[seq2].size();
    std::string s1 = "";
    std::string s2 = "";
    std::string s3 = "";
    int ptr1 = K - 1;
    int ptr2 = N1 - 1;
    int ptr3 = M1 - 1;
    std::vector<node_3> open_list, closed_list;
    std::vector<std::vector<std::vector<bool>>> open(K+1);
    std::vector<std::vector<std::vector<bool>>> closed(K+1);
    for(int i = 0; i <= K; ++i) {open[i].resize(N1+1); closed[i].resize(N1+1);}
    for(int i = 0; i <= K; ++i) 
        for(int j = 0; j <= N1; ++j) {open[i][j].resize(M1+1, false); closed[i][j].resize(M1+1, false);}
    open_list.push_back(node_3(0, H_func3(Q_ind, seq1, seq2, 0, 0, 0), 0, 0, 0, -1, -1, -1));
    open[0][0][0] = true;
    while(open_list.size()){
        node_3 optimal_node = open_list[0];
        int pos = 0;
        for(int i = 1; i < open_list.size(); ++i){
            if(optimal_node > open_list[i]){
                optimal_node = open_list[i];
                pos = i;
            }
        }
        open_list.erase(std::begin(open_list) + pos);
        closed_list.push_back(optimal_node);
        closed[optimal_node.x][optimal_node.y][optimal_node.z] = true;
        if((optimal_node.x == K) && (optimal_node.y == N1) && (optimal_node.z == M1)) {
            int X = K;
            int Y = N1;
            int Z = M1;
            node_3 tmp = optimal_node;
            while(X > 0 || Y > 0 || Z > 0){
                X = tmp.fatherx;
                Y = tmp.fathery;
                Z = tmp.fatherz;
                if(X == tmp.x - 1) s1 += Query_3[Q_ind][ptr1--];
                else s1 += "-";
                if(Y == tmp.y - 1) s2 += Database[seq1][ptr2--];
                else s2 += "-";
                if(Z == tmp.z - 1) s3 += Database[seq2][ptr3--];
                else s3 += "-";
                if(!closed[X][Y][Z]){
                    for(int i = 0; i < open_list.size(); ++i)
                        if(open_list[i].x == X && open_list[i].y == Y && open_list[i].z == Z) tmp = open_list[i];
                }
                else{
                    for(int i = 0; i < closed_list.size(); ++i)
                        if(closed_list[i].x == X && closed_list[i].y == Y && closed_list[i].z == Z) tmp = closed_list[i];
                }
            }
            reverse(s1.begin(),s1.end());
            reverse(s2.begin(),s2.end());
            reverse(s3.begin(),s3.end());
            std::pair<std::string, std::string> pa(s2,s3);
            std::pair<std::string, std::pair<std::string, std::string>> P(s1, pa);
            return P;
        }
        for(int t = 0; t < 7; ++t){
            if(optimal_node.x+w[t][0] <= K && optimal_node.y+w[t][1] <= N1 && optimal_node.z+w[t][2] <= M1){
                node_3 cur_node(optimal_node.g+judge_3(Query_3[Q_ind][optimal_node.x],Database[seq1][optimal_node.y],Database[seq2][optimal_node.z], t), H_func3(Q_ind,seq1,seq2,optimal_node.x+w[t][0],optimal_node.y+w[t][1],optimal_node.z+w[t][2]), optimal_node.x+w[t][0], optimal_node.y+w[t][1], optimal_node.z+w[t][2], optimal_node.x, optimal_node.y, optimal_node.z);
                if(!open[cur_node.x][cur_node.y][cur_node.z]){
                    open[cur_node.x][cur_node.y][cur_node.z] = true;
                    open_list.push_back(cur_node);
                }
                else if(!closed[cur_node.x][cur_node.y][cur_node.z]){
                    for(int i = 0; i < open_list.size(); ++i){
                        if(open_list[i].x == cur_node.x && open_list[i].y == cur_node.y && open_list[i].z == cur_node.z){
                            open_list[i] = (open_list[i].g > cur_node.g)? cur_node : open_list[i]; break;}
                    }
                }
                else{
                    for(int i = 0; i < closed_list.size(); ++i){
                        if(closed_list[i].x == cur_node.x && closed_list[i].y == cur_node.y && closed_list[i].z == cur_node.z && closed_list[i].g > cur_node.g){
                            closed_list[i].g = cur_node.g;
                            closed[cur_node.x][cur_node.y][cur_node.z] = false;
                            closed_list.erase(std::begin(closed_list) + i);
                            open_list.push_back(cur_node);
                            break;
                        }
                    }
                }
            }   
        }
    }
}

void Search_3()
{
    outfile << "Three-sequence Alignment: " << std::endl;
    clock_t start_time, end_time;
    int num_3 = Query_3.size();
    for(int i = 0; i < num_3; ++i){
        start_time = clock();
        int MIN = 1e9;
        int MIN_IND_1 = -1, MIN_IND_2 = -1;
        for(int t = 0; t < Database.size(); ++t){
            for(int k = t+1; k < Database.size(); ++k){
                int cost = search_3(i, t, k);
                log3 << "Test (Query3: " << i+1 << " , Database: " << t+1 << " and Database: " << k+1 << ") cost = " << cost <<std::endl;
                if(MIN > cost) {MIN = cost; MIN_IND_1 = t; MIN_IND_2 = k;}
            }
        }
        std::pair<std::string, std::pair<std::string, std::string>> pa = search_3_out(i, MIN_IND_1, MIN_IND_2);
        end_time = clock();
        outfile << "Query " << i+1 << " : " << "cost = " << MIN << std::endl;
        outfile << "          alignment with: " << "number (" << MIN_IND_1 + 1 << ") and number (" << MIN_IND_2 + 1 << ") in Database" << std::endl;
        outfile << "          alignment result: " << pa.first << std::endl;
        outfile << "                            " << pa.second.first << std::endl;
        outfile << "                            " << pa.second.second << std::endl;
        outfile << "Running Time is " << (double)(end_time - start_time)/CLOCKS_PER_SEC << "s" << std::endl << std::endl;
    }
}

int main()
{
    d2.open("../logfiles/log_astar_2.txt"); 
    log3.open("../logfiles/log_astar_3.txt");
    outfile.open("../output/output_astar.txt");
    load();
    Search_2();
    init_dp1();
    init_dp2();
    Search_3();
    d2.close();
    log3.close();
    outfile.close();
    return 0;
}