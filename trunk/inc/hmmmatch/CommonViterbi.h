#ifndef __COMMON_VITERBI_H__
#define __COMMON_VITERBI_H__

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

namespace limap {
namespace hmmmatch {

typedef vector<double>     RealVector;
typedef vector<RealVector> RealMatrix;
typedef vector<size_t>     IntVector;
typedef vector<IntVector>  IntMatrix;

template<typename OBSERVE, typename STATE>
class CommonViterbi {
    friend class ViterbiTest;
public:
    enum Module {
        DEBUG_MODULE = 0,
        RELEASE_MODULE
    };
    CommonViterbi(Module module):module_(module) {}
    CommonViterbi() = default;
    vector<STATE> operator()(const vector<OBSERVE> &obs,
                             const vector<STATE>   &stats,
                             const RealVector      &start_prob,
                             const RealMatrix      &trans_prob,
                             const RealMatrix      &emit_prob) {
        size_t rows = stats.size();
        size_t cols = obs.size();
        // input validation
        if (start_prob.size() != rows) {
            throw string("start_prob error");
        }
        if (trans_prob.size() != rows) {
            throw string("trans_prob error");
        } else {
            for (auto elem : trans_prob) {
                if (elem.size() != rows) {
                    throw string("trnas_prob error");
                }
            }
        }
        if (emit_prob.size() != rows) {
            throw string("emit_prob error");
        } else {
            for (auto elem : emit_prob) {
                if (elem.size() != cols) {
                    throw string("emit_prob error");
                }
            }
        }

        RealMatrix vtb(rows, RealVector(cols));//viterbi matrix
        IntMatrix  bkp(rows, IntVector(cols)); //back-pointer matrix

        for (size_t i = 0; i < rows; ++i) {
            vtb[i][0] = start_prob[i] * emit_prob[i][0];
            bkp[i][0] = 0;
        }

        for (size_t j = 1; j < cols; ++j) {
            for (size_t i = 0; i < rows; ++i) {
                double max_prob = vtb[0][j-1] * trans_prob[0][i];
                size_t best_stat = 0;
                for (size_t k = 1; k < rows; ++k) {
                    double loop_prob = vtb[k][j-1] * trans_prob[k][i];
                    if (max_prob < loop_prob) {
                        max_prob = loop_prob;
                        best_stat = k;
                    }
                }
                vtb[i][j] = max_prob * emit_prob[i][j];
                bkp[i][j] = best_stat;
            }
        }

        if (module_ == DEBUG_MODULE) {
            cout << "begin to print viterbi matrix---\n";
            print_matrix(vtb);
            cout << "---end of print viterbi matrix\n";
            cout << "begin to print back-pointer matrix---\n";
            print_matrix(bkp);
            cout << "---end of print back-pointer matrix\n";
        }

        vector<STATE> rets(cols);
        double max_prob = .0;
        size_t best_stat = size_t(-1);
        for (size_t i = 0; i < rows; ++i) {
            if (max_prob < vtb[i][cols-1]) {
                max_prob = vtb[i][cols-1];
                best_stat = i;
            }
        }
        rets[cols-1] = stats.at(best_stat);
        size_t previous = bkp[best_stat][cols-1];

        for (size_t i = cols-2; i != size_t(-1); --i) {
            rets[i] = stats.at(previous);
            previous = bkp[previous+1][i];
        }

        if (module_ == DEBUG_MODULE) {
            cout << "The steps of states are ";
            for (auto state : rets) {
                cout << state << " ";
            }
            cout << "with highest probability of " << max_prob << "\n";
        }

        return rets;
    }

private:
    void print_matrix(const RealMatrix &m) {
        for (auto row : m) {
            for (auto col : row) {
                cout << col << "\t";
            }
            cout << "\n";
        }
    }
    void print_matrix(const IntMatrix &m) {
        for (auto row : m) {
            for (auto col : row) {
                cout << col << "\t";
            }
            cout << "\n";
        }
    }

private:
    Module module_;
};

}
}

#endif
