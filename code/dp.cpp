#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <algorithm>

std::vector<std::string> Database, Query_2, Query_3;
std::ofstream log, log3, outfile;
typedef std::pair<std::string, std::pair<std::string, std::string>> string3;

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

void load(){
    load_database();
    load_query_2();
    load_query_3();
}

int min_3(int a, int b, int c) { return std::min(std::min(a, b), c);}

int judge(char a, char b) { return (a == b) ? 0 : 3;}

int judge3(char a, char b, char c, int mode)
{
    switch(mode){
        case 0:
            if(a == b && a == c) return 0;
            else if(a != b && a != c && b != c) return 9; 
            else return 6;
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

std::pair<int, int> min_7(int a[])
{
    int MIN = a[0];
    int MIN_IND = 0;
    for(int i = 1; i <= 6; ++i){
        if(MIN > a[i]){
            MIN = a[i];
            MIN_IND = i;
        }
    }
    return std::pair<int, int>(MIN, MIN_IND);
}

std::pair<std::string, std::string> search_2_out(std::string S, std::string T){
    int n = S.size();
    int m = T.size();
    std::vector<std::vector<int>> path(n+1);
    std::vector<std::vector<int>> DP(n+1);
    for(int i = 0; i <= n; ++i) path[i].resize(m+1);
    for(int i = 0; i <= n; ++i) DP[i].resize(m+1);
    for(int i = 0; i <= n; ++i) {DP[i][0] = 2 * i; path[i][0] = 3;}
    for(int i = 0; i <= m; ++i) {DP[0][i] = 2 * i; path[0][i] = 1;}
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j){
            if(S[i-1] == T[j-1]) {
                int M = min_3(DP[i-1][j-1], DP[i][j-1]+2, DP[i-1][j]+2);
                DP[i][j] = M;
                if(M == DP[i-1][j-1]) path[i][j] = 2;
                else if(M == DP[i][j-1] + 2) path[i][j] = 1;
                else if(M == DP[i-1][j] + 2) path[i][j] = 3;
            }
            else{
                int M = min_3(DP[i-1][j-1]+3, DP[i][j-1]+2, DP[i-1][j]+2);
                DP[i][j] = M;
                if(M == DP[i-1][j-1] + 3) path[i][j] = 2;
                else if(M == DP[i][j-1] + 2) path[i][j] = 1;
                else if(M == DP[i-1][j] + 2) path[i][j] = 3;
            }
        }
    std::string s1 = "";
    std::string s2 = "";
    int ptr1 = n, ptr2 = m;
    while(ptr1 > 0 || ptr2 > 0){
        if(path[ptr1][ptr2] == 2) {s1 += S[--ptr1]; s2 += T[--ptr2];}
        else if(path[ptr1][ptr2] == 1) {s1 += "-"; s2 += T[--ptr2];}
        else {s1 += S[--ptr1]; s2 += "-";}
    }
    reverse(s1.begin(), s1.end());
    reverse(s2.begin(), s2.end());
    std::pair<std::string, std::string> P(s1, s2);
    return P;
}

void search_2(std::string s, int pos)
{
    int MIN = 1e6;
    int MIN_IND = -1;
    for(int t = 0; t < Database.size(); ++t){
        int n = s.size();
        int m = Database[t].size();
        std::vector<std::vector<int>> dp(n+1);
        for(int i = 0; i <= n; ++i) dp[i].resize(m+1);
        for(int i = 0; i <= n; ++i) dp[i][0] = 2 * i;
        for(int i = 0; i <= m; ++i) dp[0][i] = 2 * i;
        for(int i = 1; i <= n; ++i)
            for(int j = 1; j <= m; ++j){
                if(s[i-1] == Database[t][j-1])
                    dp[i][j] = min_3(dp[i-1][j-1], dp[i][j-1]+2, dp[i-1][j]+2);
                else dp[i][j] = min_3(dp[i-1][j-1]+3, dp[i][j-1]+2, dp[i-1][j]+2);
            }
        if(MIN > dp[n][m]) { MIN = dp[n][m]; MIN_IND = t;}
        log << "Test (Query2: " << pos << " and Database: " << t+1 << ") cost = " << dp[n][m] <<std::endl;
    }
    std::pair<std::string, std::string> P = search_2_out(s, Database[MIN_IND]);
    outfile << "Query " << pos << " : " << "cost = " << MIN << std::endl;
    outfile << "          alignment with: " << "number (" << MIN_IND + 1 << ") " << "in Database" << std::endl;
    outfile << "          alignment result: " << P.first << std::endl;;
    outfile << "                            " << P.second << std::endl;
}

string3 search_3_out(std::string s1, std::string s2, std::string s3)
{
    int n = s1.size();
    int m = s2.size();
    int p = s3.size();
    std::vector<std::vector<std::vector<int>>> dp3(n+1);
    std::vector<std::vector<std::vector<int>>> path(n+1);
    for(int i = 0; i <= n; ++i) dp3[i].resize(m+1);
    for(int i = 0; i <= n; ++i) path[i].resize(m+1);
    for(int i = 0; i <= n; ++i)
        for(int j = 0; j <=m; ++j) dp3[i][j].resize(p+1);
    for(int i = 0; i <= n; ++i)
        for(int j = 0; j <=m; ++j) path[i][j].resize(p+1);
    for(int i = 0; i <= n; ++i) {dp3[i][0][0] = 4 * i; path[i][0][0] = 4;}
    for(int i = 0; i <= m; ++i) {dp3[0][i][0] = 4 * i; path[0][i][0] = 5;}
    for(int i = 0; i <= p; ++i) {dp3[0][0][i] = 4 * i; path[0][0][i] = 6;}
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j){
            dp3[i][j][0] = min_3(dp3[i-1][j-1][0] + 4 + judge(s1[i-1], s2[j-1]), dp3[i][j-1][0] + 4, dp3[i-1][j][0] + 4);
            if(dp3[i][j][0] == dp3[i-1][j-1][0] + 4 + judge(s1[i-1], s2[j-1])) path[i][j][0] = 1;
            else if(dp3[i][j][0] == dp3[i][j-1][0] + 4) path[i][j][0] = 5;
            else if(dp3[i][j][0] == dp3[i-1][j][0] + 4) path[i][j][0] = 4;
        }
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= p; ++j){
            dp3[i][0][j] = min_3(dp3[i-1][0][j-1] + 4 + judge(s1[i-1], s3[j-1]), dp3[i][0][j-1] + 4, dp3[i-1][0][j] + 4);
            if(dp3[i][0][j] == dp3[i-1][0][j-1] + 4 + judge(s1[i-1], s3[j-1])) path[i][0][j] =  2;
            else if(dp3[i][0][j] == dp3[i][0][j-1] + 4) path[i][0][j] = 6;
            else if(dp3[i][0][j] == dp3[i-1][0][j] + 4) path[i][0][j] = 4;
        }
    for(int i = 1; i <= m; ++i)
        for(int j = 1; j <= p; ++j){
            dp3[0][i][j] = min_3(dp3[0][i-1][j-1] + 4 + judge(s2[i-1], s3[j-1]), dp3[0][i][j-1] + 4, dp3[0][i-1][j] + 4);
            if(dp3[0][i][j] == dp3[0][i-1][j-1] + 4 + judge(s2[i-1], s3[j-1])) path[0][i][j] =  3;
            else if(dp3[0][i][j] == dp3[0][i][j-1] + 4) path[0][i][j] = 6;
            else if(dp3[0][i][j] == dp3[0][i-1][j] + 4) path[0][i][j] = 5;
        }
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j)
            for(int r = 1; r <= p; ++r){
                char a = s1[i-1], b = s2[j-1], c = s3[r-1];
                int A[7] = {dp3[i-1][j-1][r-1]+judge3(a, b, c, 0), dp3[i-1][j-1][r]+judge3(a, b, c, 1), dp3[i-1][j][r-1]+judge3(a, b, c, 2),
                        dp3[i][j-1][r-1]+judge3(a, b, c, 3), dp3[i-1][j][r]+4, dp3[i][j-1][r]+4, dp3[i][j][r-1]+4};
                std::pair<int, int> pa = min_7(A);
                dp3[i][j][r] = pa.first;
                path[i][j][r] = pa.second;
            }
    std::string S1 = "";
    std::string S2 = "";
    std::string S3 = "";
    int ptr1 = n, ptr2 = m, ptr3 = p;
    while(ptr1 > 0 || ptr2 > 0 || ptr3 > 0){
        if(path[ptr1][ptr2][ptr3] == 0) {S1 += s1[--ptr1]; S2 += s2[--ptr2]; S3 += s3[--ptr3];}
        else if(path[ptr1][ptr2][ptr3] == 1) {S1 += s1[--ptr1]; S2 += s2[--ptr2]; S3 += "-";}
        else if(path[ptr1][ptr2][ptr3] == 2) {S1 += s1[--ptr1]; S2 += "-"; S3 += s3[--ptr3];}
        else if(path[ptr1][ptr2][ptr3] == 3) {S1 += "-"; S2 += s2[--ptr2]; S3 += s3[--ptr3];}
        else if(path[ptr1][ptr2][ptr3] == 4) {S1 += s1[--ptr1]; S2 += "-"; S3 += "-";}
        else if(path[ptr1][ptr2][ptr3] == 5) {S1 += "-"; S2 += s2[--ptr2]; S3 += "-";}
        else if(path[ptr1][ptr2][ptr3] == 6) {S1 += "-"; S2 += "-"; S3 += s3[--ptr3];}
    }
    reverse(S1.begin(), S1.end());
    reverse(S2.begin(), S2.end());
    reverse(S3.begin(), S3.end());
    string3 re(S1, std::pair<std::string, std::string>(S2, S3));
    return re;
}

void search_3(std::string s, int pos)
{
    int MIN = 1e6, MIN_IND1 = -1, MIN_IND2 = -1;
    int n = s.size();
    for(int t = 0; t < Database.size(); ++t)
        for(int k = t + 1; k < Database.size(); ++k){
            int m = Database[t].size();
            int p = Database[k].size();
            std::vector<std::vector<std::vector<int>>> dp3(n+1);
            for(int i = 0; i <= n; ++i) dp3[i].resize(m+1);
            for(int i = 0; i <= n; ++i)
                for(int j = 0; j <=m; ++j) dp3[i][j].resize(p+1);
            for(int i = 0; i <= n; ++i) dp3[i][0][0] = 4 * i;
            for(int i = 0; i <= m; ++i) dp3[0][i][0] = 4 * i;
            for(int i = 0; i <= p; ++i) dp3[0][0][i] = 4 * i;
            for(int i = 1; i <= n; ++i)
                for(int j = 1; j <= m; ++j)
                    dp3[i][j][0] = min_3(dp3[i-1][j-1][0] + 4 + judge(s[i-1], Database[t][j-1]), dp3[i][j-1][0] + 4, dp3[i-1][j][0] + 4);
            for(int i = 1; i <= n; ++i)
                for(int j = 1; j <= p; ++j)
                    dp3[i][0][j] = min_3(dp3[i-1][0][j-1] + 4 + judge(s[i-1], Database[k][j-1]), dp3[i][0][j-1] + 4, dp3[i-1][0][j] + 4);
            for(int i = 1; i <= m; ++i)
                for(int j = 1; j <= p; ++j)
                    dp3[0][i][j] = min_3(dp3[0][i-1][j-1] + 4 + judge(Database[t][i-1], Database[k][j-1]), dp3[0][i][j-1] + 4, dp3[0][i-1][j] + 4);
            for(int i = 1; i <= n; ++i)
                for(int j = 1; j <= m; ++j)
                    for(int r = 1; r <= p; ++r){
                        char a = s[i-1], b = Database[t][j-1], c = Database[k][r-1];
                        int A[7] = {dp3[i-1][j-1][r-1]+judge3(a, b, c, 0), dp3[i-1][j-1][r]+judge3(a, b, c, 1), dp3[i-1][j][r-1]+judge3(a, b, c, 2),
                                dp3[i][j-1][r-1]+judge3(a, b, c, 3), dp3[i-1][j][r]+4, dp3[i][j-1][r]+4, dp3[i][j][r-1]+4};
                        dp3[i][j][r] = min_7(A).first;
                    }
            log3 << "Test (Query3: " << pos << " , Database: " << t+1 << " and Database: " << k+1 << ") cost = " << dp3[n][m][p] <<std::endl;
            if(MIN > dp3[n][m][p]){
                MIN = dp3[n][m][p];
                MIN_IND1 = t;
                MIN_IND2 = k;
            }
        }
    string3 P = search_3_out(s, Database[MIN_IND1], Database[MIN_IND2]);
    outfile << "Query " << pos << " : " << "cost = " << MIN << std::endl;
    outfile << "          alignment with: " << "number (" << MIN_IND1 + 1 << ") " << "and number (" << MIN_IND2 + 1 << ")" << " in Database" << std::endl;
    outfile << "          alignment result: " << P.first << std::endl;
    outfile << "                            " << P.second.first << std::endl;
    outfile << "                            " << P.second.second << std::endl;
}

void Search_2()
{
    outfile << "Pairwise Alignment: " << std::endl;
    clock_t start_time, end_time;
    for(int i = 0; i < Query_2.size(); ++i){
        start_time = clock();
        search_2(Query_2[i], i+1);
        end_time = clock();
        outfile << "Running Time is " << (double)(end_time - start_time)/CLOCKS_PER_SEC << "s" << std::endl << std::endl;
    }
}

void Search_3()
{
    outfile << "Three-sequence Alignment: " << std::endl;
    clock_t start_time, end_time;
    for(int i = 0; i < Query_3.size(); ++i) {
        start_time = clock();
        search_3(Query_3[i], i+1);
        end_time = clock();
        outfile << "Running Time is " << (double)(end_time - start_time)/CLOCKS_PER_SEC << "s" << std::endl << std::endl;
    }
}

int main()
{
    log.open("../logfiles/log_dp_2.txt"); 
    log3.open("../logfiles/log_dp_3.txt");
    outfile.open("../output/output_dp.txt");
    load();
    Search_2();
    Search_3();
    log.close();
    log3.close();
    outfile.close();
    return 0;
}