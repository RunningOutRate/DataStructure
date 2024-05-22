#pragma once

#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

template<typename T>
void SA_IS_SetTypeSS(vector<int>& LMS_List, vector<T>& S, map<T, int> Tb_Idx_FromBack, map<T, vector<int>>& Bins) {
    for (int i = LMS_List.size() - 1; i >= 0; i--) {
        int idx = LMS_List[i];
        T initial = S[idx];
        Bins[initial][Tb_Idx_FromBack[initial] - 1] = idx;
        Tb_Idx_FromBack[initial]--;
    }
}

template<typename T>
void SA_IS_SetTypeL(vector<T>& S, vector<string>& SuffixType, map<T, int> Tb_Idx_FromFront, map<T, vector<int>>& Bins) {
    for (const auto& BinElement : Bins) {
        for (int idx : BinElement.second) {
            if (idx <= 0) {
                continue;
            }
            idx--;
            T initial = S[idx];
            if (SuffixType[idx] == "L") {
                Bins[initial][Tb_Idx_FromFront[initial]] = idx;
                Tb_Idx_FromFront[initial]++;
            }
        }
    }
}

template<typename T>
void SA_IS_SetTypeS(vector<T>& S, vector<string>& SuffixType, map<T, int> Tb_Idx_FromBack, map<T, vector<int>>& Bins) {
    for (auto itr = Bins.rbegin(); itr != Bins.rend(); itr++) {
        auto& BinElement = *itr;

        for (int i = BinElement.second.size() - 1; i >= 0; i--) {
            int idx = BinElement.second[i];
            idx--;
            if (idx <= -1 || SuffixType[idx] == "L") {
                continue;
            }
            T initial = S[idx];
            Bins[initial][Tb_Idx_FromBack[initial] - 1] = idx;
            Tb_Idx_FromBack[initial]--;
        }
    }
}

template<typename T>
bool operator>(vector<T>& a, vector<T>& b) {
    for (int i = 0; i < min(a.size(), b.size()); i++) {
        if (a[i] > b[i]) {
            return true;
        }
        if (a[i] < b[i]) {
            return false;
        }
    }
    if (a.size() > b.size()) {
        return true;
    }
    else {
        return false;
    }
}

template<typename T>
vector<int> SuffixArray_InducedSorting(vector<T> S) {
    S.push_back(0);
    size_t L = S.size();

    // SuffixのTypeを求める
    vector<string> SuffixType(L);
    SuffixType[L - 1] = "SS";
    for (int i = L - 2; i >= 0; i--) {
        if (S[i] < S[i + 1]) {
            SuffixType[i] = "S";
        }
        else if (S[i] > S[i + 1]) {
            SuffixType[i] = "L";
            if (SuffixType[i + 1] == "S") {
                SuffixType[i + 1] = "SS";
            }
        }
        else {
            SuffixType[i] = SuffixType[i + 1];
        }
    }
    // LMSのindexリストを作る
    vector<int> LMS_Idx_List;
    for (int i = 0; i < L; i++) {
        if (SuffixType[i] == "SS") {
            LMS_Idx_List.push_back(i);
        }
    }

    // 使用する文字の種類と数を求める
    map<T, int> Table_nElements, IndexFromFront;
    map<T, vector<int>> Bins_empty;
    for (T elem : S) {
        Table_nElements[elem]++;
    }
    for (auto data : Table_nElements) {
        Bins_empty[data.first] = vector<int>(data.second, -1);
        IndexFromFront[data.first] = 0;
    }

    // Set TypeSS
    map<T, vector<int>> Bins = Bins_empty;

    SA_IS_SetTypeSS(LMS_Idx_List, S, Table_nElements, Bins);

    // Set TypeL
    SA_IS_SetTypeL(S, SuffixType, IndexFromFront, Bins);

    // Set TypeS
    SA_IS_SetTypeS(S, SuffixType, Table_nElements, Bins);

    // LMS-Sorting
    vector<int> LMS_SA;
    // LMS-substringの構築
    vector<pair<int, vector<T>>> LMS_SS;
    {
        for (const auto& BinElement : Bins) {
            for (int idx : BinElement.second) {
                if (SuffixType[idx] == "SS") {
                    vector<T> SS;
                    for (int pos = idx; pos < L; pos++) {
                        SS.push_back(S[pos]);
                        if (SS.size() > 1 && SuffixType[pos] == "SS") {
                            break;
                        }
                    }
                    LMS_SS.push_back({ idx, SS });
                }
            }
        }
    }

    // Substringを数値列に変換する
    bool Exist_MultipleElementPerBin = false;
    for (int i = 0; i < LMS_SS.size() - 1; i++) {
        if (LMS_SS[i].second[0] == LMS_SS[i + 1].second[0]) {
            Exist_MultipleElementPerBin = true;
        }
    }

    vector<int> LMS_SS_to_Num;
    {
        int idx = 1;
        LMS_SS_to_Num.push_back(idx);
        for (int i = 0; i < LMS_SS.size() - 1; i++) {
            if (LMS_SS[i].second != LMS_SS[i + 1].second) {
                idx++;
            }
            LMS_SS_to_Num.push_back(idx);
        }
    }

    if (Exist_MultipleElementPerBin) {
        vector<pair<int, int>> LMS_SS_Num(LMS_SS.size());
        for (int i = 0; i < LMS_SS.size(); i++) {
            LMS_SS_Num[i] = { LMS_SS[i].first, LMS_SS_to_Num[i] };
        }
        // 実際に配列Sにて登場する順番に並べる
        sort(LMS_SS_Num.begin(), LMS_SS_Num.end());
        vector<int> LMS_SS_OnlyNum(LMS_SS.size());
        for (int i = 0; i < LMS_SS.size(); i++) {
            LMS_SS_OnlyNum[i] = LMS_SS_Num[i].second;
        }
        LMS_SA = SuffixArray_InducedSorting(LMS_SS_OnlyNum);

        // 正しい順位に並び替える
        vector<int> LMS_Idx_List_Ordered(LMS_Idx_List.size());
        for (int i = 0; i < LMS_Idx_List_Ordered.size(); i++) {
            LMS_Idx_List_Ordered[i] = LMS_Idx_List[LMS_SA[i]];
        }
        // Type-SSの正しい順位が求まったので再びInduced Sortを行う
        Bins = Bins_empty;
        SA_IS_SetTypeSS(LMS_Idx_List_Ordered, S, Table_nElements, Bins);

        // Set TypeL
        SA_IS_SetTypeL(S, SuffixType, IndexFromFront, Bins);

        // Set TypeS
        SA_IS_SetTypeS(S, SuffixType, Table_nElements, Bins);
    }

    // 完成したので返す
    vector<int> SuffixArray;
    for (const auto& BinElement : Bins) {
        for (int idx : BinElement.second) {
            if (idx == L - 1) {
                continue;
            }
            SuffixArray.push_back(idx);
        }
    }

    return SuffixArray;
}

vector<int> SuffixArray_InducedSorting(string S) {
    vector<char> Vec;
    for (char c : S) {
        Vec.push_back(c);
    }
    return SuffixArray_InducedSorting(Vec);
}
