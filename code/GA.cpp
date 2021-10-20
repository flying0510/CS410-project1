#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <algorithm>
#include <utility>

const double alpha = 0.25;
const int population_size = 50;
const double breed_rate = 2;
const double inherit_rate = 0.4;
const double mutation_percent1 = 0.4;
const double mutation_percent2 = 0.4;
const double mutation_percent3 = 0.6;
const double mutation_rate1 = 0.04;
const double mutation_rate2 = 0.04;
const double mutation_rate3 = 0.03;
const int Cycle = 15;
std::vector<std::string> Database, Query_2, Query_3;
std::ofstream log, log3, outfile;

struct gene{
    std::vector<bool> v1;
    std::vector<bool> v2;
    int len;
    int cost;
    gene(){len = 0; cost = 0;}
    gene(int length){
        v1.resize(length,0);
        v2.resize(length,0);
        len = length;
        cost = 0;
    }
    gene& operator = (const gene &g){
        this->v1 = g.v1;
        this->v2 = g.v2;
        this->len = g.len;
        this->cost = g.cost;
        return *this;
    }
};

struct gene3{
    std::vector<bool> v1;
    std::vector<bool> v2;
    std::vector<bool> v3;
    int len;
    int cost;
    gene3(){len = 0; cost = 0;}
    gene3(int length){
        v1.resize(length,0);
        v2.resize(length,0);
        v3.resize(length,0);
        len = length;
        cost = 0;
    }
    gene3& operator = (const gene3 &g){
        this->v1 = g.v1;
        this->v2 = g.v2;
        this->v3 = g.v3;
        this->len = g.len;
        this->cost = g.cost;
        return *this;
    }
};

typedef std::vector<gene> population;
typedef std::vector<gene3> population3;

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

gene init_2(int length, int l1, int l2)
{
    gene g(length);
    for(int i = 0; i < length - l1 ; ++i){
        int r = rand() % length;
        if(!g.v1[r]) g.v1[r] = 1; 
        else --i;
    }
    for(int i = 0; i < length - l2 ; ++i){
        int r = rand() % length;
        if(!g.v2[r]) g.v2[r] = 1; 
        else --i;
    }
    g.len = length;
    return g;
}

gene3 init_3(int length, int l1, int l2, int l3)
{
    gene3 g(length);
    for(int i = 0; i < length - l1; ++i){
        int r = rand() % length;
        if(!g.v1[r]) g.v1[r] = 1;
        else --i;
    }
    for(int i = 0; i < length - l2; ++i){
        int r = rand() % length;
        if(!g.v2[r]) g.v2[r] = 1;
        else --i;
    }
    for(int i = 0; i < length - l3; ++i){
        int r = rand() % length;
        if(!g.v3[r]) g.v3[r] = 1;
        else --i;
    }
    g.len = length;
    return g;
}

int judge(char a, char b, int mode = 0){
    if(mode == 0) return (a == b) ? 0 : 3;
    else return 2;
}

int judge3(char a, char b, char c, int mode = 0){
    switch(mode){
        case 0:
            if(a == b && b == c) return 0;
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

int evaluate_2_gene(int que, int seq, gene g)
{
    int cost = 0;
    std::string s1 = Query_2[que];
    std::string s2 = Database[seq];
    int ptr1 = 0;
    int ptr2 = 0;
    for(int i = 0; i < g.len; ++i){
        if(g.v1[i] == 0 && g.v2[i] == 0) {cost += judge(s1[ptr1++], s2[ptr2++], 0);}
        else if(g.v1[i] == 0 && g.v2[i] == 1) {cost += judge(s1[ptr1++], s2[ptr2], 1);}
        else if(g.v1[i] == 1 && g.v2[i] == 0) {cost += judge(s1[ptr1], s2[ptr2++], 2);}
    }
    return cost;
}

int evaluate_3_gene(int que, int seq1, int seq2, gene3 g)
{
    int cost = 0;
    std::string s1 = Query_3[que];
    std::string s2 = Database[seq1];
    std::string s3 = Database[seq2];
    int ptr1 = 0;
    int ptr2 = 0;
    int ptr3 = 0;
    for(int i = 0; i < g.len; ++i){
        if(g.v1[i] == 0 && g.v2[i] == 0 && g.v3[i] == 0) cost += judge3(s1[ptr1++], s2[ptr2++], s3[ptr3++], 0);
        else if(g.v1[i] == 0 && g.v2[i] == 0 && g.v3[i] == 1) cost += judge3(s1[ptr1++], s2[ptr2++], s3[ptr3], 1);
        else if(g.v1[i] == 0 && g.v2[i] == 1 && g.v3[i] == 0) cost += judge3(s1[ptr1++], s2[ptr2], s3[ptr3++], 2);
        else if(g.v1[i] == 1 && g.v2[i] == 0 && g.v3[i] == 0) cost += judge3(s1[ptr1], s2[ptr2++], s3[ptr3++], 3);
        else if(g.v1[i] == 1 && g.v2[i] == 1 && g.v3[i] == 0) cost += judge3(s1[ptr1], s2[ptr2], s3[ptr3++], 4);
        else if(g.v1[i] == 1 && g.v2[i] == 0 && g.v3[i] == 1) cost += judge3(s1[ptr1], s2[ptr2++], s3[ptr3], 5);
        else if(g.v1[i] == 0 && g.v2[i] == 1 && g.v3[i] == 1) cost += judge3(s1[ptr1++], s2[ptr2], s3[ptr3], 6);
    }
    return cost;
}

population init_2_pop(int que, int seq, int size)
{
    population P;
    srand(time(NULL));
    int len1 = Query_2[que].size();
    int len2 = Database[seq].size();
    int len = std::max(len1, len2);
    int extralen = (int)(len * alpha);
    P.resize(0);
    for(int i = 0; i < size; ++i){
        int L = len + rand() % (extralen + 1);
        gene g = init_2(L,len1, len2);
        P.push_back(g);
        //P.push_back(init_2(L, len1, len2));
    }
    for(int i = 0; i < P.size(); ++i)
        P[i].cost = evaluate_2_gene(que, seq, P[i]);
    return P;
}

population3 init_3_pop(int que, int seq1, int seq2, int size)
{
    population3 P;
    srand(time(NULL));
    int len1 = Query_3[que].size();
    int len2 = Database[seq1].size();
    int len3 = Database[seq2].size();
    int len = std::max(std::max(len1, len2), len3);
    int extralen = (int)(len * alpha);
    P.resize(0);
    for(int i = 0; i < size; ++i){
        int L = len + rand() % (extralen + 1);
        gene3 g = init_3(L, len1, len2, len3);
        P.push_back(g);
        //P.push_back(init_3(L, len1, len2, len3));
    }
    for(int i = 0; i < P.size(); ++i)
        P[i].cost = evaluate_3_gene(que, seq1, seq2, P[i]);
    return P;
        
}

void evaluate_2_pop(int que, int seq, population& P)
{
    for(int i = 0; i < P.size(); ++i)
        P[i].cost = evaluate_2_gene(que, seq, P[i]);
}

void evaluate_3_pop(int que, int seq1, int seq2, population3& P)
{
    for(int i = 0; i < P.size(); ++i)
        P[i].cost = evaluate_3_gene(que, seq1, seq2, P[i]);
}

bool cmp(const gene& g1, const gene& g2){
    return g1.cost < g2.cost;
}

bool cmp3(const gene3& g1, const gene3& g2){
    return g1.cost < g2.cost;
}

void sort_population(population & P){
    sort(P.begin(), P.end(), cmp);
}

void sort_population3(population3 & P){
    sort(P.begin(), P.end(), cmp3);
}

gene reproduce(int que, int seq, gene g1, gene g2)
{
    bool flag = false;
    gene gn1, gn2;
    while(!flag){
    int cut = rand() % (g1.len-2) + 1;
    int num1 = 0;
    int num2 = 0;
    for(int i = 0; i <= cut; ++i){
        if(g1.v1[i]) ++num1;
        if(g1.v2[i]) ++num2;
    }
    int count1 = cut + 1 - num1;
    int count2 = cut + 1 - num2;
    int ptr1 = 0;
    int ptr2 = 0;
    for(int i = 0; i < g2.len; ++i){
        if(g2.v1[i] == 0) {
            --count1;
            if(count1 == 0) {ptr1 = i; break;}
        }
    }
    for(int i = 0; i < g2.len; ++i){
        if(g2.v2[i] == 0) {
            --count2;
            if(count2 == 0) {ptr2 = i; break;}
        }
    }
    int L1 = cut + (g2.len - ptr1);
    int L2 = cut + (g2.len - ptr2);
    int L3 = g1.len + ptr1 - cut;
    int L4 = g1.len + ptr2 - cut;
    int L = std::max(L1, L2);
    int K = std::max(L3, L4);
    gene tmp1(L);
    gene tmp2(K);
    if(L1 == L2){
        for(int i = 0; i <= cut ; ++i) tmp1.v1[i] = g1.v1[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v1[i] = g2.v1[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v2[i] = g1.v2[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v2[i] = g2.v2[i + g2.len -L];
    }
    else if(L == L2){
        for(int i = 0; i <= cut; ++i) tmp1.v1[i] = g1.v1[i];
        for(int i = cut + 1; i <= cut + L - L1; ++i) tmp1.v1[i] = 1;
        for(int i = cut + L - L1 + 1; i < L; ++i) tmp1.v1[i] = g2.v1[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v2[i] = g1.v2[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v2[i] = g2.v2[i + g2.len -L];
    }
    else{
        for(int i = 0; i <= cut; ++i) tmp1.v2[i] = g1.v2[i];
        for(int i = cut + 1; i <= cut + L - L2; ++i) tmp1.v2[i] = 1;
        for(int i = cut + L - L2 + 1; i < L; ++i) tmp1.v2[i] = g2.v2[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v1[i] = g1.v1[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v1[i] = g2.v1[i + g2.len -L]; 
    }

    if(L3 == L4){
        for(int i = 0; i <= ptr1; ++i) tmp2.v1[i] = g2.v1[i];
        for(int i = ptr1 + 1; i < K; ++i) tmp2.v1[i] = g1.v1[i + g1.len - K];
        for(int i = 0; i <= ptr2; ++i) tmp2.v2[i] = g2.v2[i];
        for(int i = ptr2 + 1; i < K; ++i) tmp2.v2[i] = g1.v2[i + g1.len - K];
    }
    else if(K == L3){
        for(int i = 0; i <= ptr2; ++i) tmp2.v2[i] = g2.v2[i];
        for(int i = ptr2 + 1; i <= ptr2 + K - L4; ++i) tmp2.v2[i] = 1;
        for(int i = ptr2 + K - L4 + 1; i < K; ++i) tmp2.v2[i] = g1.v2[i + g1.len - K];
        for(int i = 0; i <= ptr1; ++i) tmp2.v1[i] = g2.v1[i];
        for(int i = ptr1 + 1; i < K; ++i) tmp2.v1[i] = g1.v1[i + g1.len - K];
    }
    else{
        for(int i = 0; i <= ptr1; ++i) tmp2.v1[i] = g2.v1[i];
        for(int i = ptr1 + 1; i <= ptr1 + K - L3; ++i) tmp2.v1[i] = 1;
        for(int i = ptr1 + K - L3 + 1; i < K; ++i) tmp2.v1[i] = g1.v1[i + g1.len - K];
        for(int i = 0; i <= ptr2; ++i) tmp2.v2[i] = g2.v2[i];
        for(int i = ptr2 + 1; i < K; ++i) tmp2.v2[i] = g1.v2[i + g1.len - K];
    }

    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    int cnt4 = 0;
    for(int i = 0; i < tmp1.len; ++i){
        if(tmp1.v1[i]) ++cnt1;
        if(tmp1.v2[i]) ++cnt2;
    }
    for(int i = 0; i < tmp2.len; ++i){
        if(tmp2.v1[i]) ++cnt3;
        if(tmp2.v2[i]) ++cnt4;
    }
    if(tmp1.len-cnt1 == Query_2[que].size() && tmp1.len-cnt2 == Database[seq].size() && tmp2.len-cnt3 == Query_2[que].size() && tmp2.len-cnt4 == Database[seq].size()){
        flag = true;
        gn1 = tmp1;
        gn2 = tmp2;
    }
    }

    gn1.cost = evaluate_2_gene(que, seq, gn1);
    gn2.cost = evaluate_2_gene(que, seq, gn2);

    if(gn1.cost > gn2.cost) {
        if(gn1.cost / gn2.cost > 2) return gn1;
        else 
        return gn2;
    }
    else{
        if(gn2.cost / gn1.cost > 2) return gn2;
        else 
        return gn1;
    }
}

gene3 reproduce3(int que, int seq1, int seq2, gene3 g1, gene3 g2)
{
    bool flag = false;
    gene3 gn1, gn2;
    while(!flag){
    int cut = rand() % (g1.len-2) + 1;
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    for(int i = 0; i <= cut; ++i){
        if(g1.v1[i]) ++num1;
        if(g1.v2[i]) ++num2;
        if(g1.v3[i]) ++num3;
    } 
    int count1 = cut + 1 - num1;
    int count2 = cut + 1 - num2;
    int count3 = cut + 1 - num3;
    int ptr1 = 0;
    int ptr2 = 0;
    int ptr3 = 0;
    for(int i = 0; i < g2.len; ++i){
        if(g2.v1[i] == 0) {
            --count1;
            if(count1 == 0) {ptr1 = i; break;}
        }
    }
    for(int i = 0; i < g2.len; ++i){
        if(g2.v2[i] == 0) {
            --count2;
            if(count2 == 0) {ptr2 = i; break;}
        }
    }for(int i = 0; i < g2.len; ++i){
        if(g2.v3[i] == 0){
            --count3;
            if(count3 == 0) {ptr3 = i; break;}
        }
    }
    int L1 = cut + (g2.len - ptr1);
    int L2 = cut + (g2.len - ptr2);
    int L3 = cut + (g2.len - ptr3);
    int L4 = g1.len + ptr1 - cut;
    int L5 = g1.len + ptr2 - cut;
    int L6 = g1.len + ptr3 - cut;
    int L = std::max(std::max(L1, L2), L3);
    int K = std::max(std::max(L4, L5), L6);
    gene3 tmp1(L);
    gene3 tmp2(K);
    if(L == L1){
        for(int i = 0; i <= cut; ++i) tmp1.v2[i] = g1.v2[i];
        for(int i = cut + 1; i <= cut + L - L2; ++i) tmp1.v2[i] = 1;
        for(int i = cut + L - L2 + 1; i < L; ++i) tmp1.v2[i] = g2.v2[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v3[i] = g1.v3[i];
        for(int i = cut + 1; i <= cut + L - L3; ++i) tmp1.v3[i] = 1;
        for(int i = cut + L - L3 + 1; i < L; ++i) tmp1.v3[i] = g2.v3[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v1[i] = g1.v1[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v1[i] = g2.v1[i + g2.len -L]; 
    }
    else if(L == L2){
        for(int i = 0; i <= cut; ++i) tmp1.v1[i] = g1.v1[i];
        for(int i = cut + 1; i <= cut + L - L1; ++i) tmp1.v1[i] = 1;
        for(int i = cut + L - L1 + 1; i < L; ++i) tmp1.v1[i] = g2.v1[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v3[i] = g1.v3[i];
        for(int i = cut + 1; i <= cut + L - L3; ++i) tmp1.v3[i] = 1;
        for(int i = cut + L - L3 + 1; i < L; ++i) tmp1.v3[i] = g2.v3[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v2[i] = g1.v2[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v2[i] = g2.v2[i + g2.len -L];
    }
    else if(L == L3){
        for(int i = 0; i <= cut; ++i) tmp1.v1[i] = g1.v1[i];
        for(int i = cut + 1; i <= cut + L - L1; ++i) tmp1.v1[i] = 1;
        for(int i = cut + L - L1 + 1; i < L; ++i) tmp1.v1[i] = g2.v1[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v2[i] = g1.v2[i];
        for(int i = cut + 1; i <= cut + L - L2; ++i) tmp1.v2[i] = 1;
        for(int i = cut + L - L2 + 1; i < L; ++i) tmp1.v2[i] = g2.v2[i + g2.len - L];
        for(int i = 0; i <= cut; ++i) tmp1.v3[i] = g1.v3[i];
        for(int i = cut + 1; i < L; ++i) tmp1.v3[i] = g2.v3[i + g2.len -L];
    }

    if(K == L4){
        for(int i = 0; i <= ptr2; ++i) tmp2.v2[i] = g2.v2[i];
        for(int i = ptr2 + 1; i <= ptr2 + K - L5; ++i) tmp2.v2[i] = 1;
        for(int i = ptr2 + K - L5 + 1; i < K; ++i) tmp2.v2[i] = g1.v2[i + g1.len - K];
        for(int i = 0; i <= ptr3; ++i) tmp2.v3[i] = g2.v3[i];
        for(int i = ptr3 + 1; i <= ptr3 + K - L6; ++i) tmp2.v3[i] = 1;
        for(int i = ptr3 + K - L6 + 1; i < K; ++i) tmp2.v3[i] = g1.v3[i + g1.len - K];
        for(int i = 0; i <= ptr1; ++i) tmp2.v1[i] = g2.v1[i];
        for(int i = ptr1 + 1; i < K; ++i) tmp2.v1[i] = g1.v1[i + g1.len - K];
    }
    else if(K == L5){
        for(int i = 0; i <= ptr1; ++i) tmp2.v1[i] = g2.v1[i];
        for(int i = ptr1 + 1; i <= ptr1 + K - L4; ++i) tmp2.v1[i] = 1;
        for(int i = ptr1 + K - L4 + 1; i < K; ++i) tmp2.v1[i] = g1.v1[i + g1.len - K];
        for(int i = 0; i <= ptr3; ++i) tmp2.v3[i] = g2.v3[i];
        for(int i = ptr3 + 1; i <= ptr3 + K - L6; ++i) tmp2.v3[i] = 1;
        for(int i = ptr3 + K - L6 + 1; i < K; ++i) tmp2.v3[i] = g1.v3[i + g1.len - K];
        for(int i = 0; i <= ptr2; ++i) tmp2.v2[i] = g2.v2[i];
        for(int i = ptr2 + 1; i < K; ++i) tmp2.v2[i] = g1.v2[i + g1.len - K];
    }
    else if(K == L6){
        for(int i = 0; i <= ptr2; ++i) tmp2.v2[i] = g2.v2[i];
        for(int i = ptr2 + 1; i <= ptr2 + K - L5; ++i) tmp2.v2[i] = 1;
        for(int i = ptr2 + K - L5 + 1; i < K; ++i) tmp2.v2[i] = g1.v2[i + g1.len - K];
        for(int i = 0; i <= ptr1; ++i) tmp2.v1[i] = g2.v1[i];
        for(int i = ptr1 + 1; i <= ptr1 + K - L4; ++i) tmp2.v1[i] = 1;
        for(int i = ptr1 + K - L4 + 1; i < K; ++i) tmp2.v1[i] = g1.v1[i + g1.len - K];
        for(int i = 0; i <= ptr3; ++i) tmp2.v3[i] = g2.v3[i];
        for(int i = ptr3 + 1; i < K; ++i) tmp2.v3[i] = g1.v3[i + g1.len - K];
    }
    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    int cnt4 = 0;
    int cnt5 = 0;
    int cnt6 = 0;
    for(int i = 0; i < tmp1.len; ++i){
        if(tmp1.v1[i]) ++cnt1;
        if(tmp1.v2[i]) ++cnt2;
        if(tmp1.v3[i]) ++cnt3;
    }
    for(int i = 0; i < tmp2.len; ++i){
        if(tmp2.v1[i]) ++cnt4;
        if(tmp2.v2[i]) ++cnt5;
        if(tmp2.v3[i]) ++cnt6;
    }
    if(tmp1.len - cnt1 == Query_3[que].size() && tmp1.len - cnt2 == Database[seq1].size() && tmp1.len - cnt3 == Database[seq2].size() && 
        tmp2.len - cnt4 == Query_3[que].size() && tmp2.len - cnt5 == Database[seq1].size() && tmp2.len - cnt6 == Database[seq2].size())
    {
        flag = true;
        gn1 = tmp1;
        gn2 = tmp2;
    }
    }
    gn1.cost = evaluate_3_gene(que, seq1, seq2, gn1);
    gn2.cost = evaluate_3_gene(que, seq1, seq2, gn2);
    if(gn1.cost > gn2.cost) {
        if(gn1.cost / gn2.cost > 2) return gn1;
        else 
        return gn2;
    }
    else{
        if(gn2.cost / gn1.cost > 2) return gn2;
        else 
        return gn1;
    }
}

void mutate(int que, int seq, population & P)
{
    int cnt = P.size() * mutation_percent1;
    while(cnt--){
        int ind = rand() % P.size();
        int bound = P[ind].len * mutation_rate1;
        std::vector<bool> temp1 = P[ind].v1;
        std::vector<bool> temp2 = P[ind].v2;
        int C = P[ind].cost;
        int L = P[ind].len;
        while(bound--){
            int pos = rand() % P[ind].len;
            if(P[ind].v1[pos] == 0) {
                P[ind].v1.emplace(P[ind].v1.begin()+pos, 1);
                P[ind].v2.push_back(1);
                P[ind].len += 1;
            }
            int temp = evaluate_2_gene(que, seq, P[ind]);
            P[ind].cost = temp;
            if(C > temp * 2){
                P[ind].v1 = temp1; P[ind].v2 = temp2; P[ind].len = L; P[ind].cost = C;
            }
        }
    }
    //mutation-2
    /*cnt = P.size() * mutation_percent2;
    while(cnt--){
        int ind;
        while(1){
            ind = rand() % P.size();
            if(P[ind].len > Query_2[que].size() + 1 && P[ind].len > Database[seq].size() + 1) break;
        }
        int bound = 1;
        std::vector<bool> temp1 = P[ind].v1;
        std::vector<bool> temp2 = P[ind].v2;
        int C = P[ind].cost;
        int L = P[ind].len;
        while(bound--){
            for(int i = P[ind].v1.size() - 1; i >= 0; --i) {
                if(P[ind].v1[i] == 1) {P[ind].v1.erase(P[ind].v1.begin()+i); break;}
            }
            for(int i = P[ind].v2.size() - 1; i >= 0; --i) {
                if(P[ind].v2[i] == 1) {P[ind].v2.erase(P[ind].v2.begin()+i); break;}
            }
            int temp = evaluate_2_gene(que, seq, P[ind]);
            P[ind].cost = temp;
            if(C > temp * 2){
                P[ind].v1 = temp1; P[ind].v2 = temp2; P[ind].len = L; P[ind].cost = C;
            }
        }
    }
    //mutation-3
    cnt = P.size() * mutation_percent3;
    while(cnt--){
        int ind;
        while(1){
            ind = rand() % P.size();
            if(P[ind].len > Query_2[que].size()) break;
        }
        std::vector<bool> temp1 = P[ind].v1;
        std::vector<bool> temp2 = P[ind].v2;
        int C = P[ind].cost;
        int L = P[ind].len;
        int ptr1, ptr2;
        while(1){
            int tmp = rand() % P[ind].v1.size();
            if(P[ind].v1[tmp] == 0) {ptr1 = tmp; break;}
        }
        while(1){
            int tmp = rand() % P[ind].v1.size();
            if(P[ind].v1[tmp] == 1) {ptr2 = tmp; break;}
        }
        swap(P[ind].v1[ptr1], P[ind].v1[ptr2]);
        int temp = evaluate_2_gene(que, seq, P[ind]);
        P[ind].cost = temp;
        if(C > temp * 2){
            P[ind].v1 = temp1; P[ind].v2 = temp2; P[ind].len = L; P[ind].cost = C;
        }
    }*/
}

void select(population & P)
{
    population temp;
    int upper = P.size() - 1;
    int lower = population_size * inherit_rate;
    for(int i = 0; i < lower; ++i)
        temp.push_back(P[i]);
    for(int i = lower; i < population_size; ++i){
        int n = rand() % (population_size - lower) + lower;
        int m;
        while(1){
            m = rand() % (population_size - lower) + lower;
            if(m != n) break;
        }
        if(P[n].cost < P[m].cost) temp.push_back(P[n]);
        else temp.push_back(P[m]);
    }
    P = temp;
}

void select3(population3 & P)
{
    population3 temp;
    int upper = P.size() - 1;
    int lower = population_size * inherit_rate;
    for(int i = 0; i < lower; ++i)
        temp.push_back(P[i]);
    for(int i = lower; i < population_size; ++i){
        int n = rand() % (population_size - lower) + lower;
        int m;
        while(1){
            m = rand() % (population_size - lower) + lower;
            if(m != n) break;
        }
        if(P[n].cost < P[m].cost) temp.push_back(P[n]);
        else temp.push_back(P[m]);
    }
    P = temp;
}

int update(int que, int seq, population & P)
{
    //mutate(que, seq, P);
    for(int i = 0; i < population_size * breed_rate; ++i){
        int n = rand() % population_size;
        bool flag = true;
        int m;
        while(flag){
            m = rand() % population_size;
            if(m != n) flag = false;
        }
        P.push_back(reproduce(que, seq, P[n], P[m]));
    }
    sort_population(P);
    select(P);
    return P[0].cost;
}

int update3(int que, int seq1, int seq2, population3 & P)
{
    for(int i = 0; i < population_size * breed_rate; ++i){
        int n = rand() % population_size;
        bool flag = true;
        int m;
        while(flag){
            m = rand() % population_size;
            if(m != n) flag = false;
        }
        P.push_back(reproduce3(que, seq1, seq2, P[n], P[m]));
    }
    sort_population3(P);
    select3(P);
    return P[0].cost;
}

void search_2(int que)
{
    int MIN = 1e6;
    gene g_min;
    int min_ind = 0;
    for(int i = 0; i < Database.size(); ++i){
        population P(init_2_pop(que, i, population_size));
        int temp = update(que, i, P);
        int cyc = Cycle;
        while(cyc){
            int t = update(que, i, P);
            if(temp - t < 4) --cyc;
            temp = t;
        }
        if(MIN > P[0].cost){
            MIN = P[0].cost;
            g_min = P[0];
            min_ind = i;
        }
        log << "Test (Query2: " << que+1 << " and Database: " << i+1 << ") cost = " << P[0].cost <<std::endl;
    }
    std::string s1 = "";
    std::string s2 = "";
    int ptr1 = 0;
    int ptr2 = 0;
    for(int i = 0; i < g_min.len; ++i){
        if(g_min.v1[i] && g_min.v2[i]) continue;
        if(g_min.v1[i]) s1 += "-";
        else s1 += Query_2[que][ptr1++];
        if(g_min.v2[i]) s2 += "-";
        else s2 += Database[min_ind][ptr2++];
    }
    outfile << "Query " << que+1 << " : " << "cost = " << MIN << std::endl;
    outfile << "          alignment with: " << "number (" << min_ind + 1 << ") " << "in Database" << std::endl;;
    outfile << "          alignment result: " << s1 << std::endl;
    outfile << "                            " << s2 << std::endl;
}

void search_3(int que)
{
    int MIN = 1e6;
    gene3 g_min;
    int min_ind1 = 0;
    int min_ind2 = 0;
    for(int i = 0; i < Database.size(); ++i){
        for(int j = i + 1; j < Database.size(); ++j){
            population3 P(init_3_pop(que, i, j, population_size));
            int temp = update3(que, i, j, P);
            int cyc = Cycle;
            while(cyc){
                int t = update3(que, i, j, P);
                if(temp - t < 5) --cyc;
                temp = t;
            }
            if(MIN > P[0].cost){
                MIN = P[0].cost;
                g_min = P[0];
                min_ind1 = i;
                min_ind2 = j;
            }
            log3 << "Test (Query3: " << que+1 << " , Database: " << i+1 << " and Database: " << j+1 << ") cost = " << P[0].cost <<std::endl;
        }
    }
    std::string s1 = "";
    std::string s2 = "";
    std::string s3 = "";
    int ptr1 = 0;
    int ptr2 = 0;
    int ptr3 = 0;
    for(int i = 0; i < g_min.len; ++i){
        if(g_min.v1[i] && g_min.v2[i] && g_min.v3[i]) continue;
        if(g_min.v1[i]) s1 += "-";
        else s1 += Query_3[que][ptr1++];
        if(g_min.v2[i]) s2 += "-";
        else s2 += Database[min_ind1][ptr2++];
        if(g_min.v3[i]) s3 += "-";
        else s3 += Database[min_ind2][ptr3++];
    }
    outfile << "Query " << que+1 << " : " << "cost = " << MIN << std::endl;
    outfile << "          alignment with: " << "number (" << min_ind1 + 1 << ") " << "and number (" << min_ind2 + 1 << ")" << " in Database" << std::endl;
    outfile << "          alignment result: " << s1 << std::endl;
    outfile << "                            " << s2 << std::endl;
    outfile << "                            " << s3 << std::endl;
}

void Search_2()
{
    outfile << "Pairwise Alignment: " << std::endl;
    clock_t start_time, end_time;
    for(int i = 0; i < Query_2.size(); ++i) {
        start_time = clock();
        search_2(i);
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
        search_3(i);
        end_time = clock();
        outfile << "Running Time is " << (double)(end_time - start_time)/CLOCKS_PER_SEC << "s" << std::endl << std::endl;
    }
}

/*void test()
{
    population3 P(init_3_pop(0,0,1,population_size));
    std::cout << "finish test init" << '\n';
    reproduce3(0,0,1,P[0],P[1]);
}*/


int main()
{
    log.open("../logfiles/log_GA_2.txt");
    log3.open("../logfiles/log_GA_3.txt");
    outfile.open("../output/output_GA.txt");
    load();
    Search_2();
    Search_3();
    log.close();
    log3.close();
    outfile.close();
    return 0;
}